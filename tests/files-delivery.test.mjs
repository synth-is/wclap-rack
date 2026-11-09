/**
 * files-delivery.test.mjs — wclap.files/1 (SPEC §8): a rack source's `files` are delivered to the
 * module before activate over the clap.webview/3 port channel (binary frames, types 240 write /
 * 241 clear). The testfx fixture logs every frame to stdout with an FNV-1a digest; the reference
 * host collects stdout, so the test can assert exact delivery. Uses tests/files-rack (source =
 * testfx standing in as an engine plugin with two files, plus one effect device).
 */
import { test } from 'node:test';
import assert from 'node:assert/strict';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { renderRack, validateRackBundle, WclapHost, loadBundleBytesAsync } from '../reference-host/index.mjs';
import { loadRackBundleDir, loadRackBundleArchive } from '../tools/rack-bundle.mjs';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const DIR = path.join(HERE, 'files-rack');

/** The digest testfx computes: FNV-1a over (path bytes, 0, data bytes) per write, reset to the offset basis on clear. */
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

const enc = new TextEncoder();

test('wclap.files/1: source.files are delivered before activate (directory bundle)', async () => {
	const { rack, manifest, bundles, files } = loadRackBundleDir(DIR);
	const { errors } = await validateRackBundle(rack, manifest, bundles);
	assert.deepEqual(errors, []);

	const entries = rack.source.files.map((f) => ({ path: f.path, data: files[f.archive] }));
	const want = expectedDigest(entries);

	const r = await renderRack({ rack, bundles, files });
	const stdout = r.hosts.get('testfx').stdout;
	assert.match(stdout, /wclap\.files\/1 clear files=0 digest=811c9dc5/);
	assert.match(stdout, /wclap\.files\/1 write path=\/instruments\/hello\.txt bytes=23 files=1/);
	assert.match(stdout, /wclap\.files\/1 write path=\/instruments\/nested\/params\.json bytes=13 files=2/);
	assert.ok(stdout.includes(`digest=${want}`), `expected final digest ${want} in:\n${stdout}`);
	assert.equal(r.devices[0].source, true);
	assert.equal(r.devices.length, 2);
});

test('wclap.files/1: same delivery from the packed .wclap-rack.tar.gz archive', async () => {
	const { rack, manifest, bundles, files } = loadRackBundleArchive(path.join(HERE, 'files-rack.wclap-rack.tar.gz'));
	const { errors } = await validateRackBundle(rack, manifest, bundles);
	assert.deepEqual(errors, []);
	const entries = rack.source.files.map((f) => ({ path: f.path, data: files[f.archive] }));
	const want = expectedDigest(entries);
	const r = await renderRack({ rack, bundles, files });
	const stdout = r.hosts.get('testfx').stdout;
	assert.ok(stdout.includes(`digest=${want}`), `expected final digest ${want} in:\n${stdout}`);
});

test('wclap.files/1: a missing archive file is a clear error', async () => {
	const { rack, bundles } = loadRackBundleDir(DIR);
	await assert.rejects(() => renderRack({ rack, bundles, files: {} }), /archive file "files\/hello\.txt" not provided/);
});

test('wclap.files/1: plugin-level deliverFiles on a bare plugin (no rack)', async () => {
	const { bundles } = loadRackBundleDir(DIR);
	const { wasm } = await loadBundleBytesAsync(bundles.testfx);
	const host = await WclapHost.load(wasm);
	host.snapshot();
	const plugin = host.createPlugin('is.synth.kromosynth.testfx');
	const entries = [{ path: '/x/a.bin', data: enc.encode('abc') }, { path: '/x/b.bin', data: enc.encode('de') }];
	assert.equal(plugin.deliverFiles(entries), true);
	const want = expectedDigest(entries);
	assert.ok(host.stdout.includes(`digest=${want}`), `expected ${want} in:\n${host.stdout}`);
	plugin.destroy();
});

test('validateRackDoc rejects malformed source.files', async () => {
	const { validateRackDoc } = await import('../reference-host/rack.mjs');
	assert.ok(validateRackDoc({ format: 'wclap-rack', version: 1, devices: [], source: { bundle: 'b', plugin: 'p', files: [{ path: '/x' }] } }).some((e) => e.includes('source.files[0]')));
	assert.ok(validateRackDoc({ format: 'wclap-rack', version: 1, devices: [], source: { plugin: 'p' } }).some((e) => e.includes('source.bundle')));
});
