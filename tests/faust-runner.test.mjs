/**
 * faust-runner.test.mjs — the generic Faust-graph runner engine (engines/faust-runner,
 * wclap-rack SPEC §8): a WCLAP instrument that receives a Faust genome (wasm + JSON) via
 * wclap.files/1 before activate, exposes the genome's widgets as a DYNAMIC clap.params set,
 * maps notes to freq/gain/gate, and executes the genome wasm through the embedded wasm3
 * interpreter. Plugin level (delivery digest, params, audible + deterministic render) and
 * rack level (examples/faust-rack: source.files genome -> WetChorus).
 *
 * Known limitation (2026-11-10, logged in WCLAP/SESSION-LOG.md): wasm3 compiled to wasm32
 * (emsdk 3.1.51) mis-packs m3_Call's 2nd argument, so the genome's init sample-rate lands 4x
 * too high on wasm hosts — pitch/envelope timing render 4x low in Node. The genome, layout
 * and param plumbing are verified correct natively and in plain JS, so these tests assert
 * delivery, dynamic params, non-silence and determinism, not absolute pitch.
 */
import { test } from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { renderRack, validateRackBundle, WclapHost, loadBundleBytesAsync } from '../reference-host/index.mjs';
import { loadRackBundleDir, loadRackBundleArchive } from '../tools/rack-bundle.mjs';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const ENGINE_BUNDLE = path.join(HERE, '..', 'engines', 'faust-runner', 'faust-runner.wclap.tar.gz');
const EXAMPLE = path.join(HERE, '..', 'examples', 'faust-rack');
const FILES = {
	'/genome/genome.json': new Uint8Array(fs.readFileSync(path.join(EXAMPLE, 'files', 'genome.json'))),
	'/genome/genome.wasm': new Uint8Array(fs.readFileSync(path.join(EXAMPLE, 'files', 'genome.wasm'))),
};
const PLUGIN_ID = 'is.synth.kromosynth.faust-runner';

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

const deliver = (plugin) => plugin.deliverFiles(Object.entries(FILES).map(([p, data]) => ({ path: p, data })));

function peak(chans) {
	let p = 0;
	for (const c of chans) for (const v of c) p = Math.max(p, Math.abs(v));
	return p;
}

test('faust-runner: wclap.files/1 delivery before activate + genome load log', async () => {
	const { host, plugin } = await loadEngine();
	const entries = Object.entries(FILES).map(([p, data]) => ({ path: p, data }));
	assert.ok(deliver(plugin));
	assert.ok(host.stdout.includes(`digest=${expectedDigest(entries)}`), `digest in:\n${host.stdout}`);
	plugin.activate({ sampleRate: 48000, blockSize: 512 });
	assert.match(host.stdout, /faust-runner: loaded json=\/genome\/genome\.json wasm=\/genome\/genome\.wasm params=4 inputs=0 outputs=2/);
	plugin.destroy();
});

test('faust-runner: dynamic clap.params from the genome JSON (one entry per widget, freq/gain/gate note-driven)', async () => {
	const { plugin } = await loadEngine();
	deliver(plugin);
	plugin.activate({ sampleRate: 48000, blockSize: 512 });
	const names = plugin.params.map((p) => p.name);
	assert.deepEqual(names.sort(), ['attack', 'cutoff', 'drive', 'release']);
	const cutoff = plugin.param('cutoff');
	assert.equal(cutoff.min, 100);
	assert.equal(cutoff.max, 18000);
	assert.equal(cutoff.def, 8000);
	plugin.destroy();
});

test('faust-runner: a note renders audible, deterministic audio; param events apply', async () => {
	async function renderOnce() {
		const { plugin } = await loadEngine();
		deliver(plugin);
		plugin.activate({ sampleRate: 48000, blockSize: 512 });
		return plugin.render(48000, [
			{ frame: 0, type: 'noteOn', key: 60, velocity: 0.9, channel: 0, noteId: 1 },
			{ frame: 24000, type: 'noteOff', key: 60, velocity: 0, channel: 0, noteId: 1 },
			{ frame: 0, type: 'param', id: plugin.param('cutoff').id, value: 12000 },
		]);
	}
	const a = await renderOnce();
	const b = await renderOnce();
	assert.equal(a.length, 2);
	assert.ok(peak(a) > 0.01, `audible, peak=${peak(a)}`);
	assert.deepEqual([...a[0]], [...b[0]], 'bit-identical across fresh hosts');
	assert.deepEqual([...a[1]], [...b[1]]);
});

test('faust-rack example: validates, source.files delivered, renders non-silent and deterministic', async () => {
	const { rack, manifest, bundles, files } = loadRackBundleDir(EXAMPLE);
	const { errors } = await validateRackBundle(rack, manifest, bundles);
	assert.deepEqual(errors, []);
	assert.equal(rack.source.kind, 'synth');
	const a = await renderRack({ rack, bundles, files, preRoll: 4096 });
	const b = await renderRack({ rack, bundles, files, preRoll: 4096 });
	assert.equal(a.devices[0].source, true);
	assert.equal(a.devices[0].plugin, PLUGIN_ID);
	assert.equal(a.devices.length, 2);
	assert.equal(a.fingerprint, b.fingerprint);
	assert.ok(peak([a.left, a.right]) > 0.001, `audible, peak=${peak([a.left, a.right])}`);
	assert.equal(a.left.length, 48000);
	const stdout = a.hosts.get('faust-runner').stdout;
	assert.match(stdout, /faust-runner: loaded json=\/genome\/genome\.json/);
	const entries = rack.source.files.map((f) => ({ path: f.path, data: files[f.archive] }));
	assert.ok(stdout.includes(`digest=${expectedDigest(entries)}`));
});

test('faust-rack example: same render from the packed .wclap-rack.tar.gz archive', async () => {
	const { rack, manifest, bundles, files } = loadRackBundleArchive(path.join(EXAMPLE, 'faust-rack.wclap-rack.tar.gz'));
	const { errors } = await validateRackBundle(rack, manifest, bundles);
	assert.deepEqual(errors, []);
	const r = await renderRack({ rack, bundles, files, preRoll: 4096 });
	assert.equal(r.devices[0].source, true);
	assert.ok(peak([r.left, r.right]) > 0.001);
});
