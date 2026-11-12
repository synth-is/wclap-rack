/**
 * cppn-engine.test.mjs — the CPPN+DSP substrate engine as a WCLAP plugin (engines/cppn-engine):
 * receives a Synth.is CPPN+DSP genome JSON via wclap.files/1 before activate, evaluates the CPPN
 * per sample (bit-exact port of neatjs/cppnjs, see WCLAP/poc-cppn-wasm) and runs the genome's
 * asNEAT DSP patch as a per-sample node graph. Two levels:
 *   1. plugin level: deliverFiles -> activate -> note render (digest asserted on stdout, output
 *      non-silent, deterministic, release ends in silence, params present, exact pitch on a
 *      synthetic genome+patch fixture)
 *   2. rack level: examples/synth-is-cppn (real Synth.is genome 01KR0R5T... -> WetChorus)
 *      validates and renders non-silent and deterministic through renderRack, from dir and archive.
 */
import { test } from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { renderRack, validateRackBundle, WclapHost, loadBundleBytesAsync } from '../reference-host/index.mjs';
import { loadRackBundleDir, loadRackBundleArchive } from '../tools/rack-bundle.mjs';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const ENGINE_BUNDLE = path.join(HERE, '..', 'engines', 'cppn-engine', 'cppn-engine.wclap.tar.gz');
const EXAMPLE = path.join(HERE, '..', 'examples', 'synth-is-cppn');
const GENOME = new Uint8Array(fs.readFileSync(path.join(EXAMPLE, 'files', 'genome.json')));
const FILES = { '/genomes/01KR0R5T4S7SQF4W8M1G99CGS9.json': GENOME };
const PLUGIN_ID = 'is.synth.kromosynth.cppn-engine';

/** A minimal synthetic genome + patch for exact pitch assertions: hybrid-phase CPPN (6 inputs),
 *  output 0 = Sine2(pi * phase) = sin(2*pi*phase) — a pure wave at the networkOutput's frequency,
 *  wired as the buffer of a bufferSource -> output. Buffer-connected outputs are transposed by the
 *  note. The genome's own pitch sits at key 60 (as in kromosynth-vi's sample instruments), so key 60
 *  renders exactly 440 Hz and key 48 exactly 220 Hz. */
const SYNTH_GENOME = JSON.stringify({
  nodes: [
    { gid: '0', nodeType: 'Bias', activationFunction: 'NullFn', layer: 0, bias: 0 },
    ...[1, 2, 3, 4, 5, 6].map((i) => ({ gid: String(i), nodeType: 'Input', activationFunction: 'NullFn', layer: 0, bias: 0 })),
    { gid: '7', nodeType: 'Output', activationFunction: 'Sine2', layer: 1, bias: 0 },
  ],
  connections: [
    { sourceID: '0', targetID: '7', weight: 0, enabled: true },
    { sourceID: '1', targetID: '7', weight: Math.PI, enabled: true },  // input 0 = phase
  ],
  inputNodeCount: 6,
  outputNodeCount: 1,
});
const SYNTH_PATCH = JSON.stringify({
  audioGraph: { buf: ['bufferSource', ['output'], {}] },
  networkOutputs: [
    { id: 'no0', networkOutput: 0, frequency: 440, audioGraphNodes: { buf: [{ paramName: 'buffer', range: [-1, 1], weight: 1 }] } },
  ],
});
const SYNTH_FILES = {
  '/genomes/synth.json': new Uint8Array(Buffer.from(SYNTH_GENOME)),
  '/genomes/patch.json': new Uint8Array(Buffer.from(SYNTH_PATCH)),
};

/** The digest the engine computes: FNV-1a over (path bytes, 0, data bytes) per write, reset on clear. */
function expectedDigest(entries) {
  let h = 0x811c9dc5;
  const mix = (b) => { h ^= b; h = Math.imul(h, 0x01000193) >>> 0; };
  for (const { path: p, data } of entries) {
    for (let i = 0; i < p.length; i++) mix(p.charCodeAt(i) & 0xff);
    mix(0);
    for (const b of data) mix(b);
  }
  return h.toString(16).padStart(8, '0');
}

async function loadEngine() {
  const { wasm } = await loadBundleBytesAsync(new Uint8Array(fs.readFileSync(ENGINE_BUNDLE)));
  const host = await WclapHost.load(wasm);
  const plugin = host.createPlugin(PLUGIN_ID);
  assert.ok(plugin, `factory must create ${PLUGIN_ID}`);
  return { host, plugin };
}

function peak(chans) {
  let p = 0;
  for (const c of chans) for (const v of c) p = Math.max(p, Math.abs(v));
  return p;
}

function deliver(plugin, files) {
  return plugin.deliverFiles(Object.entries(files).map(([p, data]) => ({ path: p, data })));
}

test('cppn-engine: wclap.files/1 delivery before activate, digest on stdout, genome parsed', async () => {
  const { host, plugin } = await loadEngine();
  const entries = Object.entries(FILES).map(([p, data]) => ({ path: p, data }));
  assert.ok(deliver(plugin, FILES), 'deliverFiles must succeed (clap.webview/3 present)');
  const want = expectedDigest(entries);
  assert.ok(host.stdout.includes(`digest=${want}`), `expected digest ${want} in:\n${host.stdout}`);
  plugin.activate({ sampleRate: 48000, blockSize: 512 });
  assert.match(host.stdout, /cppn-engine: loaded genome=\/genomes\/01KR0R5T4S7SQF4W8M1G99CGS9\.json cppnNodes=21 cppnConns=51 outputs=18 scheme=legacy-2input patchNodes=\d+ patchEdges=\d+ netOuts=13/);
  assert.equal(plugin.params.length, 16, 'the eleven before, plus Weights X/Y, Frequencies X/Y and Pre-render');
  const byId = new Map(plugin.params.map((p) => [p.id, p]));
  assert.equal(byId.get(1).name, 'Duration');
  assert.equal(byId.get(1).min, 0.05);
  assert.equal(byId.get(1).max, 30);
  assert.equal(byId.get(6).name, 'Pitch');
  assert.equal(byId.get(6).min, -36);
  assert.equal(byId.get(6).max, 36);
  // this genome is legacy-2input, so the hybrid-phase-only CPPN X/Y are hidden
  assert.equal(byId.get(4).hidden, true, 'CPPN X hidden on legacy-2input genome');
  assert.equal(byId.get(5).hidden, true, 'CPPN Y hidden on legacy-2input genome');
  plugin.destroy();
});

test('cppn-engine: a note sounds, render is deterministic, release ends in silence', async () => {
  async function renderOnce() {
    const { host, plugin } = await loadEngine();
    deliver(plugin, FILES);
    plugin.activate({ sampleRate: 48000, blockSize: 512 });
    const out = plugin.render(48000, [
      { frame: 0, type: 'noteOn', key: 69, velocity: 1, channel: 0, noteId: 1 },
      { frame: 24000, type: 'noteOff', key: 69, velocity: 0, channel: 0, noteId: 1 },
    ]);
    plugin.destroy();
    return out;
  }
  const a = await renderOnce();
  const b = await renderOnce();
  assert.equal(a.length, 2, 'stereo output');
  assert.ok(peak(a) > 0.01, `expected audible output, peak=${peak(a)}`);
  assert.deepEqual([...a[0]], [...b[0]], 'same input + files -> bit-identical output');
  assert.deepEqual([...a[1]], [...b[1]], 'same input + files -> bit-identical output');
  let tailPeak = 0;
  for (let i = 40000; i < 48000; i++) tailPeak = Math.max(tailPeak, Math.abs(a[0][i]), Math.abs(a[1][i]));
  assert.ok(tailPeak < 1e-4, `expected silence after release, tail peak=${tailPeak}`);
});

test('cppn-engine: params take effect (Duration changes the render)', async () => {
  async function renderWith(duration) {
    const { plugin } = await loadEngine();
    deliver(plugin, FILES);
    plugin.activate({ sampleRate: 48000, blockSize: 512 });
    const out = plugin.render(48000, [
      { frame: 0, type: 'param', id: 1, value: duration },
      { frame: 0, type: 'noteOn', key: 69, velocity: 1, channel: 0, noteId: 1 },
    ]);
    plugin.destroy();
    return out[0];
  }
  const short = await renderWith(0.25);
  const long = await renderWith(2);
  let diff = 0;
  for (let i = 0; i < 48000; i++) diff = Math.max(diff, Math.abs(short[i] - long[i]));
  assert.ok(diff > 1e-6, 'Duration param must change the rendered audio');
});

test('cppn-engine: exact pitch — synthetic genome renders 440 Hz at root key 60, 220 Hz at key 48; Pitch param transposes', async () => {
  async function fundamental(key, pitch = 0) {
    const { plugin } = await loadEngine();
    deliver(plugin, SYNTH_FILES);
    plugin.activate({ sampleRate: 48000, blockSize: 512 });
    const out = plugin.render(24000, [
      { frame: 0, type: 'param', id: 1, value: 0.5 },  // Duration 0.5 s
      { frame: 0, type: 'param', id: 2, value: 0 },    // Attack 0
      { frame: 0, type: 'param', id: 6, value: pitch }, // Pitch (semitones)
      { frame: 0, type: 'param', id: 9, value: 0 },    // Limiter off: its 384-frame delay would shorten the counted window
      { frame: 0, type: 'noteOn', key, velocity: 1, channel: 0, noteId: 1 },
    ]);
    plugin.destroy();
    let crossings = 0;
    for (let i = 1; i < out[0].length; i++)
      if ((out[0][i - 1] < 0) !== (out[0][i] < 0)) crossings++;
    return crossings / (out[0].length / 48000) / 2;
  }
  const fRoot = await fundamental(60);
  const fOctDown = await fundamental(48);
  const f51 = await fundamental(51);
  const f51up = await fundamental(51, 12);  // key 51 + Pitch 12 st = key 63, three semitones above the root
  assert.ok(Math.abs(fRoot - 440) < 5, `root key 60 should be ~440 Hz, got ${fRoot.toFixed(1)}`);
  assert.ok(Math.abs(fOctDown - 220) < 5, `key 48 should be ~220 Hz, got ${fOctDown.toFixed(1)}`);
  assert.ok(Math.abs(fRoot / fOctDown - 2) < 0.05, `octave ratio, got ${(fRoot / fOctDown).toFixed(3)}`);
  assert.ok(Math.abs(f51up - 523.25) < 10, `key 51 + Pitch 12 st should be ~523 Hz, got ${f51up.toFixed(1)}`);
  assert.ok(Math.abs(f51up / f51 - 2) < 0.05, `Pitch +12 doubles key 51, got ratio ${(f51up / f51).toFixed(3)}`);
});

test('cppn-engine: Pitch param bends a sounding note in real time', async () => {
  const { plugin } = await loadEngine();
  deliver(plugin, SYNTH_FILES);
  plugin.activate({ sampleRate: 48000, blockSize: 512 });
  const out = plugin.render(24000, [
    { frame: 0, type: 'param', id: 1, value: 0.5 },
    { frame: 0, type: 'param', id: 2, value: 0 },
    { frame: 0, type: 'param', id: 9, value: 0 },
    { frame: 0, type: 'noteOn', key: 60, velocity: 1, channel: 0, noteId: 1 },
    { frame: 12000, type: 'param', id: 6, value: 12 },  // +12 st halfway through the note
  ]);
  plugin.destroy();
  const zc = (a, b) => {
    let c = 0;
    for (let i = a + 1; i < b; i++) if ((out[0][i - 1] < 0) !== (out[0][i] < 0)) c++;
    return c / ((b - a) / 48000) / 2;
  };
  const fFirst = zc(0, 12000);
  const fSecond = zc(12000, 24000);
  assert.ok(Math.abs(fFirst - 440) < 10, `first half ~440 Hz, got ${fFirst.toFixed(1)}`);
  assert.ok(Math.abs(fSecond / fFirst - 2) < 0.1, `second half should be an octave up, got ${fSecond.toFixed(1)} vs ${fFirst.toFixed(1)}`);
});

test('cppn-engine: pre-converted patch.json delivery selects the right CPPN output (regression: dangling-parser bug played output 0)', async () => {
  const { host, plugin } = await loadEngine();
  deliver(plugin, {
    ...FILES,
    // a synthIsPatch wiring CPPN output 13 @ 619.326 Hz alone into a bufferSource -> output
    '/genomes/patch.json': new Uint8Array(Buffer.from(JSON.stringify({
      audioGraph: { buf: ['bufferSource', ['output'], {}] },
      networkOutputs: [{ id: 'no13', networkOutput: 13, frequency: 619.3261557946394, audioGraphNodes: { buf: [{ paramName: 'buffer', range: [-1, 1], weight: 1 }] } }],
    }))),
  });
  plugin.activate({ sampleRate: 48000, blockSize: 512 });
  const out = plugin.render(48000, [
    { frame: 0, type: 'param', id: 1, value: 4 },
    { frame: 0, type: 'param', id: 2, value: 0 },
    { frame: 0, type: 'param', id: 7, value: 0 },  // DC Filter off: the raw signal is asserted
    { frame: 0, type: 'param', id: 8, value: 0 },  // Auto Level off
    { frame: 0, type: 'param', id: 9, value: 0 },  // Limiter off (it delays by 384 frames)
    { frame: 0, type: 'noteOn', key: 60, velocity: 1, channel: 0, noteId: 1 },
  ]);
  plugin.destroy();
  // golden: CPPN output 13 @ 619.326 Hz over a 4 s render at samples 40..45, verified bit-exact vs
  // neatjs, times the renderer's 128-sample raised-cosine fade-in that the engine applies
  const golden = [-0.699512, -0.675926, -0.650333, -0.622625, -0.592700, -0.560472];
  for (let i = 0; i < golden.length; i++) {
    const want = golden[i] * 0.5 * (1 - Math.cos(Math.PI * (40 + i) / 128));
    assert.ok(Math.abs(out[0][40 + i] - want) < 2e-3, `sample ${40 + i}: got ${out[0][40 + i]}, want ~${want}`);
  }
  let mn = 1e9, mx = -1e9;
  for (let i = 0; i < 48000; i++) { mn = Math.min(mn, out[0][i]); mx = Math.max(mx, out[0][i]); }
  assert.ok(mx - mn > 0.5, `output must vary (bug signature was a constant 1.0), range=${mn}..${mx}`);
});

test('synth-is-cppn example: validates, genome delivered, renders non-silent and deterministic', async () => {
  const { rack, manifest, bundles, files } = loadRackBundleDir(EXAMPLE);
  const { errors } = await validateRackBundle(rack, manifest, bundles);
  assert.deepEqual(errors, []);
  assert.equal(rack.source.kind, 'synth');
  const a = await renderRack({ rack, bundles, files, preRoll: 4096 });
  const b = await renderRack({ rack, bundles, files, preRoll: 4096 });
  assert.equal(a.devices[0].source, true);
  assert.equal(a.devices[0].plugin, PLUGIN_ID);
  assert.equal(a.devices.length, 2);
  assert.equal(a.fingerprint, b.fingerprint, 'deterministic across renders');
  assert.ok(peak([a.left, a.right]) > 0.001, `expected audible output, peak=${peak([a.left, a.right])}`);
  const stdout = a.hosts.get('cppn-engine').stdout;
  assert.match(stdout, /cppn-engine: loaded genome=\/genomes\/01KR0R5T4S7SQF4W8M1G99CGS9\.json/);
  const entries = rack.source.files.map((f) => ({ path: f.path, data: files[f.archive] }));
  assert.ok(stdout.includes(`digest=${expectedDigest(entries)}`), `expected delivery digest in:\n${stdout}`);
});

test('synth-is-cppn example: same render from the packed .wclap-rack.tar.gz archive', async () => {
  const { rack, manifest, bundles, files } = loadRackBundleArchive(path.join(EXAMPLE, 'synth-is-cppn.wclap-rack.tar.gz'));
  const { errors } = await validateRackBundle(rack, manifest, bundles);
  assert.deepEqual(errors, []);
  const r = await renderRack({ rack, bundles, files, preRoll: 4096 });
  assert.equal(r.devices[0].plugin, PLUGIN_ID);
  assert.ok(peak([r.left, r.right]) > 0.001, `expected audible output, peak=${peak([r.left, r.right])}`);
});

// ── the editor page: clap.gui "webview" + the page protocol (types 64..69 in, 80..82 out) ──────────────────
const frame = (type, size = 0, fill = () => {}) => { const b = new Uint8Array(4 + size); b[0] = type; fill(new DataView(b.buffer), 4); return b; };

test('cppn-engine: editor page link, description, genome and values over the webview relay', async () => {
  const { host, plugin } = await loadEngine();
  deliver(plugin, FILES);
  plugin.activate({ sampleRate: 48000, blockSize: 512 });
  const info = plugin.guiOpenWebview();
  assert.ok(info, 'clap.gui webview api + clap.webview must open');
  assert.equal(info.uri, 'ui/index.html');
  assert.ok(fs.existsSync(path.join(HERE, '..', 'engines', 'cppn-engine', 'ui', 'index.html')), 'the page ships with the engine');
  const got = [];
  host.webviewSink = (bytes) => got.push(Uint8Array.from(bytes));
  assert.ok(plugin.guiReceive(frame(64)), 'hello accepted');
  const byType = (t) => got.filter((f) => f[0] === t);
  assert.equal(byType(80).length, 1, 'one description frame');
  const desc = JSON.parse(Buffer.from(byType(80)[0].slice(4)).toString());
  assert.equal(desc.rootKey, 60);
  assert.equal(desc.params.length, 16);
  assert.deepEqual(desc.params.slice(11).map((p) => p.name), ['Weights X', 'Weights Y', 'Frequencies X', 'Frequencies Y', 'Pre-render']);
  assert.equal(byType(81).length, 1, 'the genome is passed to the page');
  assert.equal(byType(81)[0].length - 4, GENOME.length);
  assert.ok(byType(82).length >= 1, 'values frame');
  // page sets Weights X; a poll reports it back
  got.length = 0;
  plugin.guiReceive(frame(65, 12, (dv, o) => { dv.setUint32(o, 11, true); dv.setFloat64(o + 4, 0.8, true); }));
  plugin.guiReceive(frame(69));
  const vals = new DataView(byType(82).at(-1).buffer, 4);
  assert.equal(vals.getUint32(12, true), 16);
  assert.ok(Math.abs(vals.getFloat64(16 + 11 * 12, true) - 0.8) < 1e-12, 'Weights X = 0.8 after the page set it');
  plugin.guiCloseWebview();
  plugin.destroy();
});

test('cppn-engine: MIDI learn binds a controller to a parameter and the controller then moves it', async () => {
  const { host, plugin } = await loadEngine();
  deliver(plugin, FILES);
  plugin.activate({ sampleRate: 48000, blockSize: 512 });
  plugin.guiOpenWebview();
  const got = [];
  host.webviewSink = (bytes) => got.push(Uint8Array.from(bytes));
  plugin.guiReceive(frame(67, 4, (dv, o) => dv.setInt32(o, 12, true)));       // arm learn for Weights Y
  plugin.render(512, [{ frame: 0, type: 'midi', data: [0xb0, 74, 127] }]);     // controller 74 at full
  got.length = 0;
  plugin.guiReceive(frame(69));
  let v = new DataView(got.at(-1).buffer, 4);
  assert.equal(v.getInt32(0, true), -1, 'learn is finished');
  assert.equal(v.getInt32(16 + 12 * 12 + 8, true), 74, 'Weights Y is bound to controller 74');
  assert.ok(Math.abs(v.getFloat64(16 + 12 * 12, true) - 1) < 1e-9, 'and already follows it');
  plugin.render(512, [{ frame: 0, type: 'midi', data: [0xb0, 74, 0] }]);
  got.length = 0;
  plugin.guiReceive(frame(69));
  v = new DataView(got.at(-1).buffer, 4);
  assert.ok(Math.abs(v.getFloat64(16 + 12 * 12, true)) < 1e-9, 'controller at 0 puts Weights Y at 0');
  plugin.guiReceive(frame(68, 4, (dv, o) => dv.setUint32(o, 12, true)));       // free it
  got.length = 0;
  plugin.guiReceive(frame(69));
  assert.equal(new DataView(got.at(-1).buffer, 4).getInt32(16 + 12 * 12 + 8, true), -1, 'binding cleared');
  plugin.destroy();
});

test('cppn-engine: the Weights pad parameters change the sound, and the centre is the genome as it is', async () => {
  const run = async (wx, wy) => {
    const { plugin } = await loadEngine();
    deliver(plugin, FILES);
    plugin.activate({ sampleRate: 48000, blockSize: 512 });
    const out = plugin.render(24000, [
      { frame: 0, type: 'param', id: 1, value: 0.5 }, { frame: 0, type: 'param', id: 11, value: wx }, { frame: 0, type: 'param', id: 12, value: wy },
      { frame: 0, type: 'noteOn', key: 60, velocity: 1, channel: 0, noteId: 1 },
    ]);
    plugin.destroy();
    return out[0];
  };
  const centre = await run(0.5, 0.5), nearCentre = await run(0.504, 0.5), moved = await run(0.8, 0.3);
  assert.deepEqual([...nearCentre], [...centre], 'within 0.01 of the centre nothing changes');
  let diff = 0;
  for (let i = 0; i < centre.length; i++) diff += Math.abs(centre[i] - moved[i]);
  assert.ok(diff > 1, 'a moved pad must change the audio');
});

// ── Faust nodes in the patch (engines/cppn-engine/src/faust_nodes.h) ───────────────────────────────

async function renderSynthWithPatch(patch, genomeText = SYNTH_GENOME) {
  const { host, plugin } = await loadEngine();
  deliver(plugin, {
    '/genomes/synth.json': new Uint8Array(Buffer.from(genomeText)),
    '/genomes/patch.json': new Uint8Array(Buffer.from(JSON.stringify(patch))),
  });
  plugin.activate({ sampleRate: 48000, blockSize: 512 });
  const out = plugin.render(24000, [
    { frame: 0, type: 'param', id: 1, value: 0.5 }, { frame: 0, type: 'param', id: 2, value: 0 },
    { frame: 0, type: 'param', id: 7, value: 0 }, { frame: 0, type: 'param', id: 8, value: 0 },
    { frame: 0, type: 'param', id: 9, value: 0 }, { frame: 0, type: 'param', id: 10, value: 0 },
    { frame: 0, type: 'noteOn', key: 60, velocity: 1, channel: 0, noteId: 1 },
  ]);
  const stdout = host.stdout;
  plugin.destroy();
  return { samples: out[0], stdout };
}
const sineInto = (node) => ({
  audioGraph: { buf: ['bufferSource', node ? ['fx'] : ['output'], {}], ...(node ? { fx: ['faust', ['output'], node] } : {}) },
  networkOutputs: [{ id: 'no0', networkOutput: 0, frequency: 440, audioGraphNodes: { buf: [{ paramName: 'buffer', range: [-1, 1], weight: 1 }] } }],
});
const rms = (x, a = 4800, b = 19200) => { let s = 0; for (let i = a; i < b; i++) s += x[i] * x[i]; return Math.sqrt(s / (b - a)); };

test('cppn-engine: a Faust node runs its template: raw passes the signal, a low-pass removes it, an unknown template is silent', async () => {
  const plain = await renderSynthWithPatch(sineInto(null));
  const raw = await renderSynthWithPatch(sineInto({ templateName: 'raw', paramValues: {} }));
  let maxDiff = 0;
  for (let i = 0; i < 24000; i++) maxDiff = Math.max(maxDiff, Math.abs(plain.samples[i] - raw.samples[i]));
  assert.ok(rms(plain.samples) > 0.5, 'the 440 Hz test tone sounds');
  assert.ok(maxDiff < 1e-6, `the raw template is a wire (largest difference ${maxDiff})`);
  assert.match(raw.stdout, /unsupported=none/);
  // subtractive = resonant low-pass; its stored cutoff (50 Hz, far below the tone) must be applied
  const low = await renderSynthWithPatch(sineInto({ templateName: 'subtractive', paramValues: { '/subtractive/cutoff': 50, '/subtractive/resonance': 1 } }));
  const open = await renderSynthWithPatch(sineInto({ templateName: 'subtractive', paramValues: { '/subtractive/cutoff': 18000, '/subtractive/resonance': 1 } }));
  assert.ok(rms(low.samples) < 0.1 * rms(open.samples), `cutoff 50 Hz attenuates 440 Hz (rms ${rms(low.samples)} against ${rms(open.samples)})`);
  const unknown = await renderSynthWithPatch(sineInto({ templateName: 'no-such-template', paramValues: {} }));
  assert.equal(peak([unknown.samples]), 0, 'a template this build does not have is silent');
  assert.match(unknown.stdout, /unsupported=faust:no-such-template/);
});

test('cppn-engine: a genome delivered without the {genome: …} envelope loads', async () => {
  const inner = JSON.parse(Buffer.from(GENOME).toString()).genome;
  const { host, plugin } = await loadEngine();
  deliver(plugin, { '/genomes/bare.json': new Uint8Array(Buffer.from(JSON.stringify(inner))) });
  plugin.activate({ sampleRate: 48000, blockSize: 512 });
  assert.match(host.stdout, /cppn-engine: loaded genome=\/genomes\/bare\.json cppnNodes=21/);
  const out = plugin.render(4800, [{ frame: 0, type: 'noteOn', key: 60, velocity: 1, channel: 0, noteId: 1 }]);
  assert.ok(peak(out) > 0.01, 'and sounds');
  plugin.destroy();
});

test('cppn-engine: asNEATPatch as an object (not a JSON string) plays the same as the string form', async () => {
  const env = JSON.parse(Buffer.from(GENOME).toString());
  const asString = JSON.parse(JSON.stringify(env));
  if (typeof asString.genome.asNEATPatch !== 'string') asString.genome.asNEATPatch = JSON.stringify(asString.genome.asNEATPatch);
  const asObject = JSON.parse(JSON.stringify(asString));
  asObject.genome.asNEATPatch = JSON.parse(asObject.genome.asNEATPatch);
  const render = async (g) => {
    const { host, plugin } = await loadEngine();
    deliver(plugin, { '/genomes/g.json': new Uint8Array(Buffer.from(JSON.stringify(g))) });
    plugin.activate({ sampleRate: 48000, blockSize: 512 });
    const out = plugin.render(24000, [{ frame: 0, type: 'noteOn', key: 60, velocity: 1, channel: 0, noteId: 1 }]);
    const m = host.stdout.match(/patchNodes=(\d+)/);
    plugin.destroy();
    return { out: out[0], patchNodes: Number(m?.[1] ?? -1) };
  };
  const a = await render(asString), b = await render(asObject);
  assert.ok(a.patchNodes > 0 && b.patchNodes === a.patchNodes, `both forms give the patch (${a.patchNodes} / ${b.patchNodes} nodes)`);
  let maxDiff = 0;
  for (let i = 0; i < a.out.length; i++) maxDiff = Math.max(maxDiff, Math.abs(a.out[i] - b.out[i]));
  assert.equal(maxDiff, 0, 'identical samples');
});

// ── pre-rendered playing mode (parameter 15: 0 Auto, 1 Live, 2 Pre-rendered; page frame 83) ────────────
// The heavy case is synthetic: the 440 Hz tone into a Faust jpverb, the slowest template (about 13 µs
// per sample on the calibration machine), so the cost estimate puts it far below 12x real time.
const HEAVY_PATCH = sineInto({ templateName: 'jpverb', paramValues: {} });

async function openWithStatus(files, events = []) {
  const { host, plugin } = await loadEngine();
  deliver(plugin, files);
  plugin.activate({ sampleRate: 48000, blockSize: 512 });
  plugin.guiOpenWebview();
  let st = null;
  host.webviewSink = (f) => {
    if (f[0] !== 83) return;
    const v = new DataView(f.buffer, f.byteOffset + 4);
    st = { pre: f[4], reason: f[5], fresh: v.getUint16(2, true), planned: v.getUint16(4, true), stale: v.getUint16(6, true), speed: v.getFloat32(8, true), bytes: v.getUint32(12, true), step: v.getUint16(18, true), sampled: v.getUint16(20, true), live: v.getUint16(22, true), waiting: v.getUint16(24, true), waitSec: v.getFloat32(28, true) };
  };
  plugin.render(512, events);
  const status = () => { plugin.guiReceive(frame(69)); return st; };
  return { host, plugin, status };
}
const heavyFiles = () => ({ '/genomes/synth.json': new Uint8Array(Buffer.from(SYNTH_GENOME)), '/genomes/patch.json': new Uint8Array(Buffer.from(JSON.stringify(HEAVY_PATCH))) });

test('cppn-engine pre-render: Auto keeps a light genome live and pre-renders a heavy one', async () => {
  const light = await openWithStatus(FILES);
  let s = light.status();
  assert.equal(s.pre, 0, 'the example genome plays live');
  assert.equal(s.reason, 0, 'decided from the estimate (the reference host clock is virtual)');
  assert.ok(s.speed >= 12, `estimated ${s.speed}x real time`);
  assert.match(light.host.stdout, /playing mode=live reason=estimated/);
  light.plugin.destroy();
  const heavy = await openWithStatus(heavyFiles());
  s = heavy.status();
  assert.equal(s.pre, 1, 'a jpverb patch is pre-rendered');
  assert.ok(s.speed < 12, `estimated ${s.speed}x real time`);
  assert.equal(s.planned, 73, 'every key from 24 to 96 fits in memory at 2 s');
  assert.equal(s.step, 3);
  heavy.plugin.destroy();
});

test('cppn-engine pre-render: parameter 15 forces Live or Pre-rendered either way', async () => {
  const heavy = await openWithStatus(heavyFiles(), [{ frame: 0, type: 'param', id: 15, value: 1 }]);
  let s = heavy.status();
  assert.deepEqual([s.pre, s.reason], [0, 2], 'Live forced on the heavy genome');
  heavy.plugin.render(512, [{ frame: 0, type: 'param', id: 15, value: 0 }]);
  s = heavy.status();
  assert.deepEqual([s.pre, s.reason], [1, 0], 'back to Auto: pre-rendered again');
  heavy.plugin.destroy();
  const light = await openWithStatus(FILES, [{ frame: 0, type: 'param', id: 15, value: 2 }]);
  s = light.status();
  assert.deepEqual([s.pre, s.reason], [1, 2], 'Pre-rendered forced on the light genome');
  const info = light.plugin.params.find((p) => p.id === 15);
  assert.equal(info.name, 'Pre-render');
  light.plugin.destroy();
});

test('cppn-engine pre-render: parameter 15 is kept in the state (version 6); a version 5 state loads as Auto', async () => {
  const a = await openWithStatus(FILES, [{ frame: 0, type: 'param', id: 15, value: 2 }, { frame: 0, type: 'param', id: 1, value: 1.25 }]);
  const saved = a.plugin.saveState();
  assert.equal(saved[0], 6, 'state version 6');
  a.plugin.destroy();
  const b = await openWithStatus(FILES);
  b.plugin.loadState(saved);
  b.plugin.render(512);
  const s = b.status();
  assert.equal(s.pre, 1, 'restored state plays pre-rendered');
  assert.equal(b.plugin.getParam(15), 2);
  assert.equal(b.plugin.getParam(1), 1.25, 'and the other parameters');
  b.plugin.destroy();
  // the same state written as version 5: 15 values, 15 controller slots, then the genome path
  const v5 = new Uint8Array(1 + 8 * 15 + 2 * 15 + (saved.length - (1 + 8 * 16 + 2 * 16)));
  v5[0] = 5;
  v5.set(saved.subarray(1, 1 + 8 * 15), 1);
  v5.set(saved.subarray(1 + 8 * 16, 1 + 8 * 16 + 2 * 15), 1 + 8 * 15);
  v5.set(saved.subarray(1 + 8 * 16 + 2 * 16), 1 + 8 * 15 + 2 * 15);
  const c = await openWithStatus(FILES, [{ frame: 0, type: 'param', id: 15, value: 1 }]);
  c.plugin.loadState(v5);   // throws if refused
  assert.equal(c.plugin.getParam(1), 1.25, 'a version 5 state loads its values');
  assert.equal(c.plugin.getParam(15), 0);
  c.plugin.render(512);
  const sc = c.status();
  assert.equal(sc.pre, 0, 'Pre-render is Auto after a version 5 state, and the light genome plays live');
  assert.equal(sc.reason, 0);
  c.plugin.destroy();
});

test('cppn-engine pre-render: a rendered key at its own pitch is the live note, sample for sample', async () => {
  const setup = [{ frame: 0, type: 'param', id: 1, value: 0.5 }];
  const pre = await openWithStatus(FILES, [...setup, { frame: 0, type: 'param', id: 15, value: 2 }]);
  // keep processing until the root and the first grid key (63) are rendered
  for (let i = 0; i < 2000 && pre.status().fresh < 2; i++) pre.plugin.render(512);
  assert.ok(pre.status().fresh >= 2, 'keys 60 and 63 rendered');
  const live = await openWithStatus(FILES, [...setup, { frame: 0, type: 'param', id: 15, value: 1 }]);
  for (const key of [60, 63]) {   // 60 comes from the level measurement, 63 from a key render
    const ev = [{ frame: 0, type: 'noteOn', key, velocity: 1, channel: 0, noteId: key }];
    const head = pre.plugin.render(512, ev)[0];
    const st = pre.status();
    assert.deepEqual([st.sampled, st.live], [1, 0], `key ${key} is played from its rendered buffer, not live`);
    const a = new Float32Array(48000);
    a.set(head);
    a.set(pre.plugin.render(48000 - 512)[0], 512);
    const b = live.plugin.render(48000, ev)[0];
    let diff = 0, p = 0;
    for (let i = 0; i < a.length; i++) { diff = Math.max(diff, Math.abs(a[i] - b[i])); p = Math.max(p, Math.abs(b[i])); }
    assert.ok(p > 0.01, `key ${key} sounds`);
    assert.equal(diff, 0, `key ${key}: largest difference ${diff}`);
  }
  // a key not rendered yet, next to a rendered one, is that neighbour resampled: close, not equal
  const ev = [{ frame: 0, type: 'noteOn', key: 61, velocity: 1, channel: 0, noteId: 61 }];
  pre.plugin.render(512, ev);
  assert.deepEqual([pre.status().sampled, pre.status().live], [1, 0], 'key 61 plays key 60 resampled');
  pre.plugin.destroy();
  live.plugin.destroy();
});

test('cppn-engine pre-render: after a Duration change a played key is rendered at once and heard as it renders (not the old length)', async () => {
  // DC Filter and Auto Level off: the level measurement is redone after the played key, so they would differ
  const setup = [{ frame: 0, type: 'param', id: 15, value: 2 }, { frame: 0, type: 'param', id: 1, value: 0.5 }, { frame: 0, type: 'param', id: 7, value: 0 }, { frame: 0, type: 'param', id: 8, value: 0 }];
  const pre = await openWithStatus(heavyFiles(), setup);
  for (let i = 0; i < 4000 && pre.status().fresh < 1; i++) pre.plugin.render(512);
  assert.ok(pre.status().fresh >= 1, 'key 60 rendered at 0.5 s');
  pre.plugin.render(512, [{ frame: 0, type: 'param', id: 1, value: 1.0 }]);
  assert.equal(pre.status().fresh, 0, 'every key is out of date after the change');
  const ev = [{ frame: 0, type: 'noteOn', key: 60, velocity: 1, channel: 0, noteId: 1 }];
  const chunks = [pre.plugin.render(512, ev)[0]];
  const s0 = pre.status();
  assert.equal(s0.waiting, 1, 'the note waits for its key');
  assert.ok(s0.waitSec >= 0, `an estimate of the wait is reported (${s0.waitSec} s)`);
  let waited = 1;
  while (pre.status().waiting && waited < 4000) { chunks.push(pre.plugin.render(512)[0]); waited++; }
  assert.equal(pre.status().waiting, 0, 'the note started');
  for (let i = 0; i < 150; i++) chunks.push(pre.plugin.render(512)[0]);
  const a = new Float32Array(chunks.reduce((n, c) => n + c.length, 0));
  { let o = 0; for (const c of chunks) { a.set(c, o); o += c.length; } }
  const live = await openWithStatus(heavyFiles(), [...setup.slice(1), { frame: 0, type: 'param', id: 1, value: 1.0 }, { frame: 0, type: 'param', id: 15, value: 1 }]);
  const b = live.plugin.render(48000 * 1.4, ev)[0];
  const onset = (x) => x.findIndex((v) => v !== 0);
  const oa = onset(a), ob = onset(b);
  assert.ok(oa > 0 && ob >= 0, `both sound (onsets ${oa}, ${ob})`);
  const rms = (x, o, t0, t1) => { let e = 0; for (let i = o + Math.round(t0 * 48000); i < o + Math.round(t1 * 48000); i++) e += x[i] * x[i]; return Math.sqrt(e / ((t1 - t0) * 48000)); };
  assert.ok(rms(a, oa, 0.7, 0.95) > 0.01, `still sounding at 0.7-0.95 s: the new 1 s length, not the old 0.5 s (rms ${rms(a, oa, 0.7, 0.95)})`);
  let diff = 0;
  for (let i = 0; i < 48000 * 1.15; i++) diff = Math.max(diff, Math.abs(a[oa + i] - b[ob + i]));
  assert.equal(diff, 0, `the streamed note is the live note, sample for sample (largest difference ${diff}); waited ${waited} blocks`);
  pre.plugin.destroy();
  live.plugin.destroy();
});

test('cppn-engine: Live, a convolver note prepares over several blocks instead of stalling one, and then plays as a prepared one', async () => {
  const conv = JSON.stringify({
    audioGraph: { buf: ['bufferSource', ['conv'], {}], conv: ['convolver', ['output'], {}] },
    networkOutputs: [{ id: 'no0', networkOutput: 0, frequency: 440, audioGraphNodes: { buf: [{ paramName: 'buffer', range: [-1, 1], weight: 1 }], conv: [{ paramName: 'buffer', range: [-1, 1], weight: 1 }] } }],
  });
  const files = { '/genomes/synth.json': new Uint8Array(Buffer.from(SYNTH_GENOME)), '/genomes/patch.json': new Uint8Array(Buffer.from(conv)) };
  const p = await openWithStatus(files, [{ frame: 0, type: 'param', id: 15, value: 1 }, { frame: 0, type: 'param', id: 1, value: 30 }]);
  const play = (noteId) => {
    const out = [p.plugin.render(512, [{ frame: 0, type: 'noteOn', key: 60, velocity: 1, channel: 0, noteId }])[0]];
    const st = p.status();
    let silent = 0;
    while (out.at(-1).every((v) => v === 0) && silent < 2000) { silent++; out.push(p.plugin.render(512)[0]); }
    for (let i = 0; i < 40; i++) out.push(p.plugin.render(512)[0]);
    const a = new Float32Array(out.length * 512);
    out.forEach((c, i) => a.set(c, i * 512));
    return { a, st, silent };
  };
  const first = play(1);
  assert.equal(first.st.waiting, 1, 'the note is waiting after its first block');
  assert.ok(first.st.waitSec > 0, `with an estimate (${first.st.waitSec} s)`);
  assert.ok(first.silent >= 2, `prepared over several blocks (${first.silent + 1} silent)`);
  p.plugin.render(512, [{ frame: 0, type: 'noteOff', key: 60, velocity: 0, channel: 0, noteId: 1 }]);
  for (let i = 0; i < 20; i++) p.plugin.render(512);   // the release ends it
  const second = play(2);
  assert.equal(second.st.waiting, 0, 'the same note again starts at once (its pre-pass is remembered)');
  const onset = (x) => x.findIndex((v) => v !== 0);
  const oa = onset(first.a), ob = onset(second.a);
  let diff = 0, pk = 0;
  for (let i = 0; i < 48000 * 0.3; i++) { diff = Math.max(diff, Math.abs(first.a[oa + i] - second.a[ob + i])); pk = Math.max(pk, Math.abs(second.a[ob + i])); }
  assert.ok(pk > 0.001, `it sounds (peak ${pk})`);
  assert.equal(diff, 0, `the waited note is the prepared note (largest difference ${diff})`);
  p.plugin.destroy();
  // clap.render offline (a bounce): the pre-pass is done at note on, so the note starts in its first block
  const off = await openWithStatus(files, [{ frame: 0, type: 'param', id: 15, value: 1 }, { frame: 0, type: 'param', id: 1, value: 30 }]);
  const r = off.plugin.ext('clap.render');
  assert.ok(r, 'clap.render offered');
  assert.equal(off.plugin.h.fn(off.plugin.h.u32(r + 4))(off.plugin.ptr, 1), 1, 'offline mode accepted');
  const c = off.plugin.render(48000 * 0.4, [{ frame: 0, type: 'noteOn', key: 60, velocity: 1, channel: 0, noteId: 1 }])[0];
  const oc = onset(c);
  assert.ok(oc >= 0 && oc < 512, `offline: sounds from the start (onset ${oc})`);
  let d2 = 0;
  for (let i = 0; i < 48000 * 0.3; i++) d2 = Math.max(d2, Math.abs(first.a[oa + i] - c[oc + i]));
  assert.equal(d2, 0, `offline note = the waited note (largest difference ${d2})`);
  off.plugin.destroy();
});

test('cppn-engine pre-render: offline rendering (clap.render) plays a heavy genome live', async () => {
  const h = await openWithStatus(heavyFiles());
  assert.equal(h.status().pre, 1, 'pre-rendered in real time');
  const r = h.plugin.ext('clap.render');
  h.plugin.h.fn(h.plugin.h.u32(r + 4))(h.plugin.ptr, 1);
  h.plugin.render(512);
  assert.equal(h.status().pre, 0, 'live when the host renders offline');
  h.plugin.h.fn(h.plugin.h.u32(r + 4))(h.plugin.ptr, 0);
  h.plugin.render(512);
  assert.equal(h.status().pre, 1, 'pre-rendered again in real time');
  h.plugin.destroy();
});
