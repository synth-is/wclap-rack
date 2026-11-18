/**
 * sfz-engine.test.mjs — the reference engine plugin for wclap-rack SPEC §8:
 * engines/sfz-engine (a minimal SFZ sampler WCLAP) receives its instrument files
 * (demo.sfz + samples/c4.wav) via wclap.files/1 before activate, then plays as a
 * rack source. Two levels:
 *   1. plugin level: deliverFiles -> activate -> note render (digest asserted on stdout,
 *      output non-silent, release ends in silence, deterministic across hosts)
 *   2. rack level: examples/sfz-rack (source.files -> WetChorus) validates and renders
 *      non-silent and deterministic through renderRack.
 */
import { test } from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { renderRack, validateRackBundle, WclapHost, loadBundleBytesAsync } from '../reference-host/index.mjs';
import { loadRackBundleDir, loadRackBundleArchive } from '../tools/rack-bundle.mjs';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const ENGINE_BUNDLE = path.join(HERE, '..', 'engines', 'sfz-engine', 'sfz-engine.wclap.tar.gz');
const EXAMPLE = path.join(HERE, '..', 'examples', 'sfz-rack');
const FILES = {
	'/instruments/demo.sfz': new Uint8Array(fs.readFileSync(path.join(EXAMPLE, 'files', 'demo.sfz'))),
	'/instruments/samples/c4.wav': new Uint8Array(fs.readFileSync(path.join(EXAMPLE, 'files', 'samples', 'c4.wav'))),
};
const PLUGIN_ID = 'is.synth.kromosynth.sfz-engine';

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
	assert.ok(plugin, 'factory must create is.synth.kromosynth.sfz-engine');
	return { host, plugin };
}

function peak(chans) {
	let p = 0;
	for (const c of chans) for (const v of c) p = Math.max(p, Math.abs(v));
	return p;
}

test('sfz-engine: wclap.files/1 delivery before activate, digest on stdout', async () => {
	const { host, plugin } = await loadEngine();
	const entries = Object.entries(FILES).map(([p, data]) => ({ path: p, data }));
	assert.ok(plugin.deliverFiles(entries), 'deliverFiles must succeed (clap.webview/3 present)');
	const want = expectedDigest(entries);
	assert.ok(host.stdout.includes(`digest=${want}`), `expected digest ${want} in:\n${host.stdout}`);
	assert.match(host.stdout, /wclap\.files\/1 write path=\/instruments\/demo\.sfz bytes=\d+ files=1/);
	assert.match(host.stdout, /wclap\.files\/1 write path=\/instruments\/samples\/c4\.wav bytes=96044 files=2/);
	plugin.activate({ sampleRate: 48000, blockSize: 512 });
	assert.match(host.stdout, /sfz-engine: sample \/instruments\/samples\/c4\.wav frames=48000 ch=1 sr=48000/);
	assert.match(host.stdout, /sfz-engine: loaded sfz=\/instruments\/demo\.sfz regions=1 samples=1 tail=12000/);
	plugin.destroy();
});

test('sfz-engine: a note sounds, release ends in silence, render is deterministic', async () => {
	async function renderOnce() {
		const { host, plugin } = await loadEngine();
		plugin.deliverFiles(Object.entries(FILES).map(([p, data]) => ({ path: p, data })));
		plugin.activate({ sampleRate: 48000, blockSize: 512 });
		const out = plugin.render(48000, [
			{ frame: 0, type: 'noteOn', key: 60, velocity: 0.9, channel: 0, noteId: 1 },
			{ frame: 24000, type: 'noteOff', key: 60, velocity: 0, channel: 0, noteId: 1 },
		]);
		plugin.destroy();
		return out;
	}
	const a = await renderOnce();
	const b = await renderOnce();
	assert.equal(a.length, 2, 'stereo output');
	assert.ok(peak(a) > 0.01, `expected audible output, peak=${peak(a)}`);
	assert.deepEqual([...a[0].slice(0, 1000)], [...b[0].slice(0, 1000)]);
	assert.deepEqual([...a[1]], [...b[1]], 'same input + files -> bit-identical output');
	// after note off (frame 24000) + release (0.25 s = 12000 frames), the tail must be silent
	let tailPeak = 0;
	for (let i = 40000; i < 48000; i++) tailPeak = Math.max(tailPeak, Math.abs(a[0][i]), Math.abs(a[1][i]));
	assert.ok(tailPeak < 1e-4, `expected silence after release, tail peak=${tailPeak}`);
});

test('sfz-engine: pitched playback — key 72 is an octave above key 60', async () => {
	async function fundamental(key) {
		const { host, plugin } = await loadEngine();
		plugin.deliverFiles(Object.entries(FILES).map(([p, data]) => ({ path: p, data })));
		plugin.activate({ sampleRate: 48000, blockSize: 512 });
		const out = plugin.render(24000, [{ frame: 0, type: 'noteOn', key, velocity: 1, channel: 0, noteId: 1 }]);
		plugin.destroy();
		// crude zero-crossing count over the first 24000 frames
		let crossings = 0;
		for (let i = 1; i < out[0].length; i++)
			if ((out[0][i - 1] < 0) !== (out[0][i] < 0)) crossings++;
		return crossings / (out[0].length / 48000) / 2; // approx frequency in Hz
	}
	const f60 = await fundamental(60);
	const f72 = await fundamental(72);
	assert.ok(Math.abs(f60 - 261.63) < 15, `key 60 should be ~261.6 Hz, got ${f60.toFixed(1)}`);
	assert.ok(Math.abs(f72 / f60 - 2) < 0.1, `key 72 should be 2x key 60, got ratio ${(f72 / f60).toFixed(3)}`);
});

test('sfz-rack example: validates, source.files delivered, renders non-silent and deterministic', async () => {
	const { rack, manifest, bundles, files } = loadRackBundleDir(EXAMPLE);
	const { errors } = await validateRackBundle(rack, manifest, bundles);
	assert.deepEqual(errors, []);
	assert.equal(rack.source.kind, 'sampler');
	const a = await renderRack({ rack, bundles, files, preRoll: 4096 });
	const b = await renderRack({ rack, bundles, files, preRoll: 4096 });
	assert.equal(a.devices[0].source, true);
	assert.equal(a.devices[0].plugin, PLUGIN_ID);
	assert.equal(a.devices.length, 2);
	assert.equal(a.fingerprint, b.fingerprint, 'deterministic across renders');
	assert.ok(peak([a.left, a.right]) > 0.001, `expected audible output, peak=${peak([a.left, a.right])}`);
	assert.equal(a.left.length, 48000);
	const stdout = a.hosts.get('sfz-engine').stdout;
	assert.match(stdout, /sfz-engine: loaded sfz=\/instruments\/demo\.sfz regions=1 samples=1/);
});

test('sfz-rack example: same render from the packed .wclap-rack.tar.gz archive', async () => {
	const { rack, manifest, bundles, files } = loadRackBundleArchive(path.join(EXAMPLE, 'sfz-rack.wclap-rack.tar.gz'));
	const { errors } = await validateRackBundle(rack, manifest, bundles);
	assert.deepEqual(errors, []);
	const r = await renderRack({ rack, bundles, files, preRoll: 4096 });
	assert.equal(r.devices[0].source, true);
	assert.ok(peak([r.left, r.right]) > 0.001, `expected audible output, peak=${peak([r.left, r.right])}`);
	const stdout = r.hosts.get('sfz-engine').stdout;
	const entries = rack.source.files.map((f) => ({ path: f.path, data: files[f.archive] }));
	assert.ok(stdout.includes(`digest=${expectedDigest(entries)}`), `expected delivery digest in:\n${stdout}`);
});
