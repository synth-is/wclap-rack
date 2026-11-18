/**
 * cdp-engine.test.mjs — the CDP-WASM-SUITE as one consolidated WCLAP instrument
 * (engines/cdp-engine): the module asks the host (wclap.cdp/1 bridge, cdp-bridge.mjs) to
 * render the selected catalog generator offline per note, then plays the buffer back.
 *
 *   1. plugin level: note-on 69 -> bridge renders synth 'wave' at 440 Hz -> audible output
 *      at ~440 Hz, deterministic across runs; without a bridge the note stays silent
 *   2. offline effect path: applyEffectToWav('blur.blur') over a rendered generator output
 *   3. rack level: examples/cdp-rack validates and renders non-silent, deterministic,
 *      via renderRack's block-wise async source path (extensions option)
 */
import { test } from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath, pathToFileURL } from 'node:url';
import { renderRack, validateRackBundle, WclapHost, CDPBridge, loadBundleBytesAsync } from '../reference-host/index.mjs';
import { loadRackBundleDir, loadRackBundleArchive } from '../tools/rack-bundle.mjs';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const ENGINE = path.join(HERE, '..', 'engines', 'cdp-engine');
const ENGINE_BUNDLE = path.join(ENGINE, 'cdp-engine.wclap.tar.gz');
const PKG_URL = pathToFileURL(path.join(ENGINE, 'third_party', 'cdp-wasm') + '/').href;
const EXAMPLE = path.join(HERE, '..', 'examples', 'cdp-rack');
const PLUGIN_ID = 'is.synth.kromosynth.cdp-engine';

async function makeBridge(sampleRate = 48000) {
	const bridge = await new CDPBridge({ pkgUrl: PKG_URL, sampleRate }).init();
	return bridge;
}

async function loadEngine({ withBridge = true } = {}) {
	const { wasm } = await loadBundleBytesAsync(new Uint8Array(fs.readFileSync(ENGINE_BUNDLE)));
	const bridge = withBridge ? await makeBridge() : null;
	const host = await WclapHost.load(wasm, bridge
		? { extensions: bridge.imports(), onInstantiated: (h) => bridge.attach(h) }
		: {});
	const plugin = host.createPlugin(PLUGIN_ID);
	assert.ok(plugin, 'factory must create is.synth.kromosynth.cdp-engine');
	return { host, plugin, bridge };
}

/** Render in small blocks, yielding between blocks so async bridge deliveries can land.
 *  `drain` is awaited after every block so deliveries land at deterministic block boundaries. */
async function renderAsync(plugin, totalFrames, events, { blockSize = 256, drain = null } = {}) {
	const out = [new Float32Array(totalFrames), new Float32Array(totalFrames)];
	const sorted = [...events].sort((a, b) => a.frame - b.frame);
	let done = 0, ei = 0;
	while (done < totalFrames) {
		const n = Math.min(blockSize, totalFrames - done);
		const last = done + n >= totalFrames;
		const evs = [];
		while (ei < sorted.length && (last || sorted[ei].frame < done + n)) {
			const e = sorted[ei++];
			evs.push({ ...e, frame: Math.max(0, e.frame - done) });
		}
		const chunk = plugin.render(n, evs);
		out[0].set(chunk[0], done);
		out[1].set(chunk[1] ?? chunk[0], done);
		done += n;
		await new Promise((r) => setTimeout(r, 0));
		if (drain) await drain();
	}
	return out;
}

function peak(chans) {
	let p = 0;
	for (const c of chans) for (const v of c) p = Math.max(p, Math.abs(v));
	return p;
}

function approxFreq(ch, sampleRate) {
	let crossings = 0;
	for (let i = 1; i < ch.length; i++) if ((ch[i - 1] < 0) !== (ch[i] < 0)) crossings++;
	return crossings / (ch.length / sampleRate) / 2;
}

// program 0 = catalog generator 'wave' (synth wave: shape, freq, dur, amp); key 69 = 440 Hz
const PARAMS = [
	{ frame: 0, type: 'param', id: 1, value: 0 },   // Program = 0 ('wave')
	{ frame: 0, type: 'param', id: 2, value: 0.5 }, // Duration 0.5 s
	{ frame: 0, type: 'param', id: 3, value: 60 },  // Root key
	{ frame: 0, type: 'param', id: 4, value: 1 },   // P1 shape = 1 (sine)
	{ frame: 0, type: 'param', id: 7, value: 0.8 }, // P4 amp = 0.8
];

test('cdp-engine: note-on renders the catalog generator through the bridge and sounds at the note frequency', async () => {
	async function renderOnce() {
		const { host, plugin, bridge } = await loadEngine();
		plugin.activate({ sampleRate: 48000, blockSize: 512 });
		const events = [
			...PARAMS,
			{ frame: 0, type: 'noteOn', key: 69, velocity: 0.9, channel: 0, noteId: 1 },
			{ frame: 36000, type: 'noteOff', key: 69, velocity: 0, channel: 0, noteId: 1 },
		];
		const out = await renderAsync(plugin, 48000, events, { drain: () => bridge.drain() });
		plugin.destroy();
		return { out, stdout: host.stdout };
	}
	const a = await renderOnce();
	const b = await renderOnce();
	assert.ok(/cdp-engine: delivered req=\d+ note=69/.test(a.stdout), `delivery logged:\n${a.stdout}`);
	assert.ok(peak(a.out) > 0.3, `expected audible output, peak=${peak(a.out)}`);
	const f = approxFreq(a.out[0].slice(2000, 20000), 48000);
	assert.ok(Math.abs(f - 440) < 15, `key 69 should render ~440 Hz, got ${f.toFixed(1)}`);
	// velocity 0.9 -> gain 0.81, amp 0.8 -> peak ~0.65
	assert.ok(Math.abs(peak(a.out) - 0.8 * 0.81) < 0.15, `peak ~0.65, got ${peak(a.out).toFixed(3)}`);
	assert.deepEqual([...a.out[0]], [...b.out[0]], 'same notes + params -> bit-identical output');
});

test('cdp-engine: without a wclap.cdp/1 bridge the note stays silent (ENOSYS), no crash', async () => {
	const { plugin } = await loadEngine({ withBridge: false });
	plugin.activate({ sampleRate: 48000, blockSize: 512 });
	const out = plugin.render(24000, [
		...PARAMS,
		{ frame: 0, type: 'noteOn', key: 69, velocity: 0.9, channel: 0, noteId: 1 },
	]);
	plugin.destroy();
	assert.ok(peak(out) === 0, `expected silence without bridge, peak=${peak(out)}`);
});

test('cdp-engine: state save/load round-trips the params', async () => {
	const { plugin } = await loadEngine({ withBridge: false });
	plugin.activate({ sampleRate: 48000, blockSize: 512 });
	plugin.render(1, [
		{ frame: 0, type: 'param', id: 1, value: 5 },
		{ frame: 0, type: 'param', id: 2, value: 2.5 },
		{ frame: 0, type: 'param', id: 7, value: 0.3 },
	]);
	const saved = plugin.saveState();
	plugin.destroy();
	const { plugin: p2 } = await loadEngine({ withBridge: false });
	p2.activate({ sampleRate: 48000, blockSize: 512 });
	p2.loadState(saved);
	const info = p2.params.find((p) => p.id === 1);
	assert.equal(p2.getParam(1), 5);
	assert.equal(p2.getParam(2), 2.5);
	assert.equal(p2.getParam(7), 0.3);
	assert.ok(info);
	p2.destroy();
});

test('cdp-engine: switching the Program param resets P1..P12 to the sentinel (catalog defaults)', async () => {
	const { plugin } = await loadEngine({ withBridge: false });
	plugin.activate({ sampleRate: 48000, blockSize: 512 });
	plugin.render(1, [
		{ frame: 0, type: 'param', id: 1, value: 0 },   // Program 0
		{ frame: 0, type: 'param', id: 4, value: 440 }, // P1 = freq for 'wave'
		{ frame: 0, type: 'param', id: 7, value: 0.8 }, // P4 = amp
	]);
	assert.equal(plugin.getParam(4), 440);
	// switch to program 4 (Chord): its positional params are kind/dur/amp — a stale 440 in
	// P1 would be dur=440 s and the CDP CLI would reject it; the plugin must reset P1..P12
	plugin.render(1, [{ frame: 0, type: 'param', id: 1, value: 4 }]);
	assert.equal(plugin.getParam(1), 4);
	for (let id = 4; id < 16; id++) assert.equal(plugin.getParam(id), -1e9, `P${id - 3} reset`);
	assert.equal(plugin.paramText(7, -1e9), 'default', 'sentinel renders as "default"');
	plugin.destroy();
});

test('cdp-bridge: stale/out-of-range param values are clamped to the catalog range (impulse train plays)', async () => {
	const bridge = await makeBridge();
	const delivered = [];
	const statuses = [];
	bridge.deliverFn = (reqId, planar, frames, chans, rate, baseKey) => delivered.push({ reqId, frames, baseKey, peak: Math.max(...planar.map(Math.abs)) });
	bridge.onStatus = (s) => statuses.push(s);
	// program 6 = impulse train; P5 (index 4) valid range is 1..20 — a stale 0.8 (an amp
	// value left over from the previous program) must be clamped, not rejected by the CLI
	await bridge.renderRequest(1, { i: 6, p: [0, 0, 0, 0, 0.8], dur: 0.5, root: 60 }, 69, 100);
	assert.equal(delivered.length, 1, `expected a delivery, statuses=${JSON.stringify(statuses)}`);
	assert.ok(delivered[0].peak > 0.3, `clamped render audible, peak=${delivered[0].peak}`);
	assert.ok(statuses.some((s) => s.state === 'ready'));
});

test('cdp-bridge: fxChain applies an offline CDP effect between render and delivery; failures route to cdp_fail', async () => {
	const bridge = await makeBridge();
	const { EFFECTS } = bridge.mod;
	const blurIdx = EFFECTS.findIndex((e) => e.id === 'blur.blur');
	assert.ok(blurIdx >= 0, 'blur.blur in the catalog');
	const delivered = [];
	bridge.deliverFn = (reqId, planar, frames) => delivered.push({ reqId, frames, peak: Math.max(...planar.map(Math.abs)) });
	// without the chain
	await bridge.renderRequest(1, { i: 0, p: [1, 440, 0.5, 0.8], dur: 0.5, root: 60 }, 69, 100);
	// with a blur effect chained (param windows=20 at position 0)
	bridge.setFxChain([{ i: blurIdx, p: [20] }]);
	await bridge.renderRequest(2, { i: 0, p: [1, 440, 0.5, 0.8], dur: 0.5, root: 60 }, 69, 100);
	assert.equal(delivered.length, 2);
	assert.notEqual(delivered[0].peak, delivered[1].peak, 'effect changed the rendered buffer');
	assert.ok(delivered[1].peak > 0.05, 'effected render still audible');
	// a failing render (no generator at index 999) routes to the fail callback
	const fails = [];
	bridge.failFn = (reqId, message) => fails.push({ reqId, message });
	const statuses = [];
	bridge.onStatus = (s) => statuses.push(s);
	await bridge.renderRequest(3, { i: 999, p: [], dur: 0.5, root: 60 }, 69, 100);
	assert.equal(fails.length, 1);
	assert.equal(fails[0].reqId, 3);
	assert.ok(/no generator at index 999/.test(fails[0].message));
	assert.ok(statuses.some((s) => s.state === 'error' && s.reqId === 3));
});

test('cdp-bridge: input-dependent CDP ranges self-correct via the CLI-reported range (focus.step on a 0.5 s note)', async () => {
	const bridge = await makeBridge();
	const { EFFECTS } = bridge.mod;
	const stepIdx = EFFECTS.findIndex((e) => e.id === 'focus.step'); // "Step-freeze (focus)"
	assert.ok(stepIdx >= 0);
	// a 0.5 s note (the panel default Duration): the raw max step the catalog clamps to (1)
	// exceeds focus's input-dependent limit (~0.52) — the bridge must parse the CLI's
	// "out of range (lo to hi)" message, clamp, and retry instead of failing
	const wav = await bridge.renderGeneratorToWav('wave', { shape: '4', freq: 220, dur: 0.5, amp: 0.8 });
	const out = await bridge.applyEffectIndexToWav(stepIdx, wav, { step: 1 });
	assert.ok(out instanceof Uint8Array && out.length > 1000, 'effect rendered after self-correction');
	// and through the rack fxChain path with sentinel P values (what the UI sends on switch)
	const delivered = [];
	bridge.deliverFn = (reqId, planar, frames) => delivered.push({ frames, peak: Math.max(...planar.map(Math.abs)) });
	bridge.setFxChain([{ i: stepIdx, p: Array(12).fill(-1e9) }]);
	await bridge.renderRequest(1, { i: 0, p: [1, 440, 0.5, 0.8], dur: 0.5, root: 60 }, 69, 100);
	assert.equal(delivered.length, 1, 'fxChain render delivered, no INCORRECT USE');
	assert.ok(delivered[0].peak > 0.1, `audible through the effect, peak=${delivered[0].peak}`);
});

test('cdp-bridge: source-aware ranges — extend.zigzag with End above the source length self-clamps', async () => {
	const bridge = await makeBridge();
	const { EFFECTS } = bridge.mod;
	const zigIdx = EFFECTS.findIndex((e) => e.id === 'extend.zigzag'); // "Zigzag (extended)"
	assert.ok(zigIdx >= 0);
	// 0.5 s source: zigzag's `end` is capped at the source duration by CDP, so an End of 1
	// (within the STATIC catalog max of 2) must be clamped to the source range, not rejected
	const wav = await bridge.renderGeneratorToWav('wave', { shape: '4', freq: 220, dur: 0.5, amp: 0.8 });
	for (const end of [0.5, 1, 2]) {
		const out = await bridge.applyEffectIndexToWav(zigIdx, wav, { end });
		assert.ok(out instanceof Uint8Array && out.length > 1000, `end=${end} rendered`);
	}
	// and via the fxChain rack path with sentinel P values
	const delivered = [];
	bridge.deliverFn = (reqId, planar, frames) => delivered.push({ frames, peak: Math.max(...planar.map(Math.abs)) });
	bridge.setFxChain([{ i: zigIdx, p: Array(12).fill(-1e9) }]);
	await bridge.renderRequest(1, { i: 0, p: [1, 440, 0.5, 0.8], dur: 0.5, root: 60 }, 69, 100);
	assert.equal(delivered.length, 1);
	assert.ok(delivered[0].peak > 0.1, `audible through zigzag, peak=${delivered[0].peak}`);
});

test('cdp-bridge: offline effect path — blur.blur over a rendered generator output', async () => {
	const bridge = await makeBridge();
	const wav = await bridge.renderGeneratorToWav('wave', { shape: '4', freq: 220, dur: 1.0, amp: 0.8 });
	assert.ok(wav.length > 1000);
	const out = await bridge.applyEffectToWav('blur.blur', wav, { windows: 20 });
	assert.ok(out instanceof Uint8Array && out.length > 1000, 'effect produced audio');
	const { decodeWav } = bridge.mod;
	const d = decodeWav(out);
	let pk = 0;
	for (const ch of d.channelData) for (const s of ch) pk = Math.max(pk, Math.abs(s));
	assert.ok(pk > 0.05, `effect output audible, peak=${pk}`);
});

test('cdp-rack example: validates and renders non-silent, deterministic through renderRack (async source)', async () => {
	const { rack, manifest, bundles, files } = loadRackBundleDir(EXAMPLE);
	const { errors } = await validateRackBundle(rack, manifest, bundles);
	assert.deepEqual(errors, []);
	async function run() {
		const bridge = await makeBridge();
		const extensions = { 'cdp-engine': { imports: bridge.imports(), onInstantiated: (h) => bridge.attach(h), drain: () => bridge.drain() } };
		const r = await renderRack({ rack, bundles, files, preRoll: 4096, extensions });
		return r;
	}
	const a = await run();
	const b = await run();
	assert.equal(a.devices[0].source, true);
	assert.equal(a.devices[0].plugin, PLUGIN_ID);
	assert.equal(a.fingerprint, b.fingerprint, 'deterministic across renders');
	assert.ok(peak([a.left, a.right]) > 0.3, `expected audible output, peak=${peak([a.left, a.right])}`);
	assert.equal(a.left.length, 96000);
	// four notes, four deliveries
	const deliveries = (a.hosts.get('cdp-engine').stdout.match(/cdp-engine: delivered/g) || []).length;
	assert.equal(deliveries, 4, `expected 4 delivered notes, got ${deliveries}:\n${a.hosts.get('cdp-engine').stdout}`);
});

test('cdp-rack example: same render from the packed .wclap-rack.tar.gz archive', async () => {
	const { rack, manifest, bundles, files } = loadRackBundleArchive(path.join(EXAMPLE, 'cdp-rack.wclap-rack.tar.gz'));
	const { errors } = await validateRackBundle(rack, manifest, bundles);
	assert.deepEqual(errors, []);
	const bridge = await makeBridge();
	const extensions = { 'cdp-engine': { imports: bridge.imports(), onInstantiated: (h) => bridge.attach(h), drain: () => bridge.drain() } };
	const r = await renderRack({ rack, bundles, files, preRoll: 4096, extensions });
	assert.ok(peak([r.left, r.right]) > 0.3, `expected audible output, peak=${peak([r.left, r.right])}`);
});
