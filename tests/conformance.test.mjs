/**
 * conformance.test.mjs — the wclap-rack conformance harness (SPEC.md §4-5) on the bundled testfx
 * bundle (tests/rack-bundle). L1: validate + render serially with bypass and per-device latency
 * compensation. L2: determinism (two renders bit-identical). L3: latency reported per device.
 *
 * Run: node --test tests/conformance.test.mjs   (build the test bundle first:
 *   EMSDK=~/emsdk bash tests/bundles/testfx/scripts/build.sh)
 */
import { test } from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import zlib from 'node:zlib';
import {
  untar, loadBundleBytesAsync, validateRackBundle, validateRackDoc, renderRack, fingerprint, sha256Hex,
} from '../reference-host/index.mjs';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const BUNDLE_DIR = path.join(HERE, 'rack-bundle');

function loadTestBundle() {
	const rack = JSON.parse(fs.readFileSync(path.join(BUNDLE_DIR, 'rack.json'), 'utf8'));
	const manifest = JSON.parse(fs.readFileSync(path.join(BUNDLE_DIR, 'MANIFEST.json'), 'utf8'));
	const bundles = {};
	for (const [key, entry] of Object.entries(manifest)) {
		bundles[key] = new Uint8Array(fs.readFileSync(path.join(BUNDLE_DIR, entry.path)));
	}
	return { rack, manifest, bundles };
}

const loadRack = (name) => JSON.parse(fs.readFileSync(path.join(BUNDLE_DIR, 'racks', name), 'utf8'));

const signal = (n) => {
	const a = new Float32Array(n);
	for (let i = 0; i < n; i++) a[i] = Math.fround(Math.sin(i * 0.01) * 0.8 + Math.sin(i * 0.0031) * 0.2);
	return a;
};

test('L1: the test rack bundle validates (schema + MANIFEST sha256)', async () => {
	const { rack, manifest, bundles } = loadTestBundle();
	assert.deepEqual(validateRackDoc(rack), []);
	const { errors } = await validateRackBundle(rack, manifest, bundles);
	assert.deepEqual(errors, []);
});

test('L1: a MANIFEST sha mismatch is refused', async () => {
	const { rack, bundles } = loadTestBundle();
	const badManifest = { testfx: { path: 'testfx.wclap.tar.gz', sha256: '0'.repeat(64) } };
	const { errors } = await validateRackBundle(rack, badManifest, bundles);
	assert.ok(errors.some((e) => e.includes('sha mismatch')));
});

test('L1: gain 0 dB rack passes audio bit-exact', async () => {
	const { bundles } = loadTestBundle();
	const s = signal(5000);
	const r = await renderRack({ rack: loadRack('gain0.rack.json'), bundles, input: { left: s, right: s }, preRoll: 128 });
	assert.deepEqual(r.left, s);
	assert.deepEqual(r.right, s);
});

test('L1: bypassed device is bit-transparent', async () => {
	const { bundles } = loadTestBundle();
	const s = signal(5000);
	const r = await renderRack({ rack: loadRack('bypass.rack.json'), bundles, input: { left: s, right: s }, preRoll: 128 });
	assert.deepEqual(r.left, s);
});

test('L1+L3: -6 dB halves the amplitude; delay device reports latency 64 and stays aligned', async () => {
	const { bundles } = loadTestBundle();
	const s = signal(5000);
	const r = await renderRack({ rack: loadRack('gain-m6-delay.rack.json'), bundles, input: { left: s, right: s }, preRoll: 128 });
	const factor = Math.fround(Math.pow(10, -6 / 20));
	const expected = s.map((x) => Math.fround(x * factor));
	// the second device's 64-frame delay is compensated at that device: output lines up with input
	assert.deepEqual(r.left.subarray(0, s.length - 64), expected.subarray(0, s.length - 64));
	assert.equal(r.devices[1].latency, 64);
	assert.equal(r.latency, 64); // summed, reported upward
});

test('L2: two renders of the same rack are bit-identical', async () => {
	const { rack, bundles } = loadTestBundle();
	const s = signal(4000);
	const a = await renderRack({ rack, bundles, input: { left: s, right: s }, preRoll: 256 });
	const b = await renderRack({ rack, bundles, input: { left: s, right: s }, preRoll: 256 });
	assert.equal(a.fingerprint, b.fingerprint);
	assert.equal(fingerprint([a.left, a.right]), fingerprint([b.left, b.right]));
});

test('sha256Hex matches a known vector', async () => {
	assert.equal(await sha256Hex(new TextEncoder().encode('abc')),
		'ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad');
});
