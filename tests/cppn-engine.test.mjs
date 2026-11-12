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
  assert.equal(plugin.params.length, 15, 'the eleven before, plus Weights X/Y and Frequencies X/Y');
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
  assert.equal(desc.params.length, 15);
  assert.deepEqual(desc.params.slice(11).map((p) => p.name), ['Weights X', 'Weights Y', 'Frequencies X', 'Frequencies Y']);
  assert.equal(byType(81).length, 1, 'the genome is passed to the page');
  assert.equal(byType(81)[0].length - 4, GENOME.length);
  assert.ok(byType(82).length >= 1, 'values frame');
  // page sets Weights X; a poll reports it back
  got.length = 0;
  plugin.guiReceive(frame(65, 12, (dv, o) => { dv.setUint32(o, 11, true); dv.setFloat64(o + 4, 0.8, true); }));
  plugin.guiReceive(frame(69));
  const vals = new DataView(byType(82).at(-1).buffer, 4);
  assert.equal(vals.getUint32(12, true), 15);
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
