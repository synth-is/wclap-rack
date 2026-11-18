/**
 * linked-rack.test.mjs — conformance for linked racks (SPEC.md §1.1) and the ustar prefix rule
 * (SPEC.md §1), on the bundled testfx rack (tests/rack-bundle). The network is a fake fetch that
 * serves bundle bytes by URL and counts requests, so no server is needed.
 *
 * Run: node --test tests/linked-rack.test.mjs   (part of `npm test`)
 */
import { test } from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import {
	untar, renderRack, validateRackBundle, loadRackLink, validateRackLink, rackBundleKeys,
	RACK_LINK_FORMAT, RACK_LINK_VERSION,
} from '../reference-host/index.mjs';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const BUNDLE_DIR = path.join(HERE, 'rack-bundle');
const DOC_URL = 'https://racks.example/r/demo.json';
const BUNDLE_URL = 'https://racks.example/r/testfx.wclap.tar.gz';

const rack = JSON.parse(fs.readFileSync(path.join(BUNDLE_DIR, 'rack.json'), 'utf8'));
const archiveManifest = JSON.parse(fs.readFileSync(path.join(BUNDLE_DIR, 'MANIFEST.json'), 'utf8'));
const testfxBytes = new Uint8Array(fs.readFileSync(path.join(BUNDLE_DIR, archiveManifest.testfx.path)));
const SHA = archiveManifest.testfx.sha256;

/** A linked form of the test rack whose one bundle sits next to the document (relative url). */
const linkedDoc = (entry = { url: 'testfx.wclap.tar.gz', sha256: SHA }, overrides = {}) => ({
	format: RACK_LINK_FORMAT, version: RACK_LINK_VERSION, rack, manifest: { testfx: entry }, ...overrides,
});

/**
 * A fetch that answers from a URL -> bytes map (404 otherwise) and records every request.
 * @param {Object<string, Uint8Array>} served
 */
function fakeFetch(served) {
	const calls = [];
	const fetchImpl = async (url) => {
		calls.push(String(url));
		const bytes = served[String(url)];
		return bytes
			? { ok: true, status: 200, arrayBuffer: async () => bytes.slice().buffer }
			: { ok: false, status: 404, arrayBuffer: async () => new ArrayBuffer(0) };
	};
	return { fetchImpl, calls };
}

const signal = (n) => {
	const a = new Float32Array(n);
	for (let i = 0; i < n; i++) a[i] = Math.fround(Math.sin(i * 0.01) * 0.8 + Math.sin(i * 0.0031) * 0.2);
	return a;
};

test('linked rack: loads, validates and renders bit-identically to the archive form', async () => {
	const { fetchImpl, calls } = fakeFetch({ [BUNDLE_URL]: testfxBytes });
	const linked = await loadRackLink(linkedDoc(), { baseUrl: DOC_URL, fetch: fetchImpl });
	assert.deepEqual(calls, [BUNDLE_URL]); // relative url resolved against the document's url
	assert.deepEqual(linked.manifest, { testfx: { url: BUNDLE_URL, sha256: SHA } });
	assert.deepEqual((await validateRackBundle(linked.rack, linked.manifest, linked.bundles)).errors, []);

	const s = signal(4000);
	const input = { left: s, right: s };
	const fromLink = await renderRack({ rack: linked.rack, bundles: linked.bundles, input, preRoll: 128 });
	const fromArchive = await renderRack({ rack, bundles: { testfx: testfxBytes }, input, preRoll: 128 });
	assert.equal(fromLink.fingerprint, fromArchive.fingerprint);
});

test('linked rack: an absolute url needs no baseUrl; a relative one without baseUrl is an error', async () => {
	const { fetchImpl } = fakeFetch({ [BUNDLE_URL]: testfxBytes });
	const ok = await loadRackLink(linkedDoc({ url: BUNDLE_URL, sha256: SHA }), { fetch: fetchImpl });
	assert.ok(ok.bundles.testfx.length > 0);
	await assert.rejects(loadRackLink(linkedDoc(), { fetch: fetchImpl }), /relative and no baseUrl/);
});

test('linked rack: racks sharing a cache download a shared bundle once', async () => {
	const { fetchImpl, calls } = fakeFetch({ [BUNDLE_URL]: testfxBytes });
	const cache = new Map();
	await loadRackLink(linkedDoc(), { baseUrl: DOC_URL, fetch: fetchImpl, cache });
	await loadRackLink(linkedDoc(), { baseUrl: 'https://racks.example/r/other.json', fetch: fetchImpl, cache });
	assert.equal(calls.length, 1);
	await loadRackLink(linkedDoc(), { baseUrl: DOC_URL, fetch: fetchImpl }); // own cache: fetched again
	assert.equal(calls.length, 2);
});

test('linked rack: a module.wasm that does not match the pinned sha256 is refused', async () => {
	const { fetchImpl } = fakeFetch({ [BUNDLE_URL]: testfxBytes });
	await assert.rejects(
		loadRackLink(linkedDoc({ url: 'testfx.wclap.tar.gz', sha256: '0'.repeat(64) }), { baseUrl: DOC_URL, fetch: fetchImpl }),
		/does not match the pinned/,
	);
});

test('linked rack: a missing or malformed sha256 is invalid, and nothing is fetched', async () => {
	const { fetchImpl, calls } = fakeFetch({ [BUNDLE_URL]: testfxBytes });
	for (const entry of [{ url: 'testfx.wclap.tar.gz' }, { url: 'testfx.wclap.tar.gz', sha256: SHA.toUpperCase() }]) {
		assert.ok(validateRackLink(linkedDoc(entry)).some((e) => e.includes('needs a sha256')));
		await assert.rejects(loadRackLink(linkedDoc(entry), { baseUrl: DOC_URL, fetch: fetchImpl }), /invalid rack link/);
	}
	assert.equal(calls.length, 0);
});

test('linked rack: structural errors are reported', () => {
	assert.deepEqual(validateRackLink(linkedDoc()), []);
	assert.deepEqual(validateRackLink(null), ['rack link: not an object']);
	assert.ok(validateRackLink(linkedDoc(undefined, { format: 'wclap-rack' })).some((e) => e.startsWith('format')));
	assert.ok(validateRackLink(linkedDoc(undefined, { version: 2 })).some((e) => e.startsWith('version')));
	assert.ok(validateRackLink(linkedDoc(undefined, { manifest: [] })).includes('manifest must be an object'));
	assert.ok(validateRackLink({ ...linkedDoc(), manifest: {} }).some((e) => e.includes('"testfx" is missing')));
	assert.ok(validateRackLink(linkedDoc(undefined, { rack: { format: 'wclap-rack', version: 1 } })).some((e) => e.startsWith('rack: ')));
	// unknown top-level members are allowed (SPEC §1.1)
	assert.deepEqual(validateRackLink(linkedDoc(undefined, { credit: 'a rack by someone', page: 'https://example.org' })), []);
});

test('linked rack: a source that lists files is refused (version 1)', () => {
	const withFiles = {
		...rack,
		source: { bundle: 'testfx', plugin: 'is.synth.kromosynth.testfx', files: [{ path: '/x.sfz', archive: 'files/x.sfz' }] },
	};
	assert.ok(validateRackLink(linkedDoc(undefined, { rack: withFiles })).some((e) => e.includes('source.files')));
	assert.deepEqual(rackBundleKeys(withFiles), ['testfx']);
});

test('linked rack: an HTTP error is reported and not cached, so a later load can retry', async () => {
	const cache = new Map();
	const down = fakeFetch({});
	await assert.rejects(loadRackLink(linkedDoc(), { baseUrl: DOC_URL, fetch: down.fetchImpl, cache }), /HTTP 404/);
	assert.equal(cache.size, 0);
	const up = fakeFetch({ [BUNDLE_URL]: testfxBytes });
	const linked = await loadRackLink(linkedDoc(), { baseUrl: DOC_URL, fetch: up.fetchImpl, cache });
	assert.ok(linked.bundles.testfx.length > 0);
});

test('ustar: a path longer than 100 bytes is joined from the prefix field (SPEC §1)', () => {
	const long = `resource/${'deep/'.repeat(25)}x.wav`;
	const cut = long.lastIndexOf('/', 100);
	const enc = new TextEncoder();
	const header = new Uint8Array(512);
	header.set(enc.encode(long.slice(cut + 1)), 0); // name: the last part
	header.set(enc.encode('0000644\0'), 100);
	header.set(enc.encode('00000000004\0'), 124); // size 4
	header[156] = 0x30; // regular file
	header.set(enc.encode('ustar\0' + '00'), 257);
	header.set(enc.encode(long.slice(0, cut)), 345); // prefix: the leading directories
	const tar = new Uint8Array(512 + 512 + 1024);
	tar.set(header, 0);
	tar.set(enc.encode('RIFF'), 512);
	const files = untar(tar);
	assert.deepEqual(Object.keys(files), [long]);
	assert.equal(new TextDecoder().decode(files[long]), 'RIFF');
});
