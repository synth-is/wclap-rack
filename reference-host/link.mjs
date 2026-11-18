// link.mjs — linked racks (SPEC §1.1): a rack that travels as one small JSON document instead of a
// .wclap-rack.tar.gz archive, with every bundle fetched by URL and pinned by the sha256 of its
// module.wasm:
//   { "format": "wclap-rack-link", "version": 1,
//     "rack": <wclap-rack v1 document>,
//     "manifest": { "<bundle key>": { "url": "<.wclap.tar.gz archive>", "sha256": "<module.wasm>" } } }
//
// Environment-agnostic like the rest of the reference host: the network goes through an injected
// fetch (globalThis.fetch by default), so it runs in Node, a browser page and tests.

import { validateRackDoc, sha256Hex } from './rack.mjs';
import { loadBundleBytesAsync } from './bundle.mjs';

export const RACK_LINK_FORMAT = 'wclap-rack-link';
export const RACK_LINK_VERSION = 1;

const SHA256_HEX = /^[0-9a-f]{64}$/;

/**
 * The bundle keys a rack document uses: the source's first, then each device's, without repeats.
 *
 * @param {object} rack  wclap-rack v1 document
 * @returns {string[]}
 */
export function rackBundleKeys(rack) {
	return [...new Set([rack?.source?.bundle, ...(rack?.devices ?? []).map((d) => d?.bundle)].filter(Boolean))];
}

/**
 * Structural validation of a linked rack (schema/rack-link.schema.json is the normative form).
 * Beyond the schema it checks that every bundle the rack uses has a manifest entry with a url and
 * a lower-case hex sha256, and that the source asks for no files (not defined for linked racks in
 * version 1: such a rack travels as an archive).
 *
 * @param {object} doc  parsed linked-rack JSON
 * @returns {string[]}  errors; empty when valid
 */
export function validateRackLink(doc) {
	if (!doc || typeof doc !== 'object' || Array.isArray(doc)) return ['rack link: not an object'];
	const errors = [];
	if (doc.format !== RACK_LINK_FORMAT) errors.push(`format must be "${RACK_LINK_FORMAT}"`);
	if (doc.version !== RACK_LINK_VERSION) errors.push(`version must be ${RACK_LINK_VERSION}`);
	errors.push(...validateRackDoc(doc.rack).map((e) => `rack: ${e}`));
	const manifest = doc.manifest;
	if (!manifest || typeof manifest !== 'object' || Array.isArray(manifest)) {
		errors.push('manifest must be an object');
		return errors;
	}
	for (const key of rackBundleKeys(doc.rack)) {
		const entry = manifest[key];
		if (!entry || typeof entry !== 'object') { errors.push(`manifest: bundle "${key}" is missing`); continue; }
		if (typeof entry.url !== 'string' || !entry.url) errors.push(`manifest: bundle "${key}" needs a url`);
		if (typeof entry.sha256 !== 'string' || !SHA256_HEX.test(entry.sha256)) {
			errors.push(`manifest: bundle "${key}" needs a sha256 (64 lower-case hex digits of its module.wasm)`);
		}
	}
	if (Array.isArray(doc.rack?.source?.files) && doc.rack.source.files.length > 0) {
		errors.push('rack: source.files is not defined for linked racks in version 1 (send the rack as an archive)');
	}
	return errors;
}

/**
 * Fetch one bundle archive, check its module.wasm against the pinned sha256, and share the result
 * through `cache` (keyed by sha256), so racks using the same bundle download it once.
 *
 * @param {string} url  absolute URL of the bundle archive
 * @param {string} sha256  expected sha256 of its module.wasm
 * @param {typeof fetch} fetchImpl
 * @param {Map<string, Promise<Uint8Array>>} cache
 * @returns {Promise<Uint8Array>}  the archive bytes as downloaded
 */
function fetchPinnedBundle(url, sha256, fetchImpl, cache) {
	if (cache.has(sha256)) return cache.get(sha256);
	const pending = (async () => {
		const res = await fetchImpl(url);
		if (!res.ok) throw new Error(`rack link: bundle ${url}: HTTP ${res.status}`);
		const bytes = new Uint8Array(await res.arrayBuffer());
		const { wasm } = await loadBundleBytesAsync(bytes);
		const got = await sha256Hex(wasm);
		if (got !== sha256) throw new Error(`rack link: bundle ${url}: module.wasm sha256 ${got} does not match the pinned ${sha256}`);
		return bytes;
	})();
	cache.set(sha256, pending);
	// Reason: a failed download must not poison the cache; a later load may retry it.
	pending.catch(() => { if (cache.get(sha256) === pending) cache.delete(sha256); });
	return pending;
}

/**
 * Load a linked rack: validate it, resolve each bundle url (absolute, or relative to `baseUrl`, the
 * linked document's own URL), download every bundle the rack uses, and verify each module.wasm
 * against its pinned sha256. The result has the shape renderRack and validateRackBundle take.
 *
 * @param {object} doc  parsed linked-rack JSON
 * @param {{ baseUrl?: string|URL, fetch?: typeof fetch, cache?: Map<string, Promise<Uint8Array>> }} [options]
 *   baseUrl  needed when any url is relative
 *   fetch    defaults to globalThis.fetch
 *   cache    pass the same Map to several loads to download a shared bundle once (default: per call)
 * @returns {Promise<{ rack: object, manifest: Object<string, {url: string, sha256: string}>,
 *   bundles: Object<string, Uint8Array> }>}  manifest urls are returned resolved
 * @throws {Error} on an invalid document, an unresolvable url, an HTTP error or a sha256 mismatch
 */
export async function loadRackLink(doc, { baseUrl, fetch: fetchImpl = globalThis.fetch, cache = new Map() } = {}) {
	const errors = validateRackLink(doc);
	if (errors.length) throw new Error(`invalid rack link: ${errors.join('; ')}`);
	if (typeof fetchImpl !== 'function') throw new Error('rack link: no fetch available (pass options.fetch)');
	const manifest = {};
	const bundles = {};
	await Promise.all(rackBundleKeys(doc.rack).map(async (key) => {
		const { url, sha256 } = doc.manifest[key];
		let resolved;
		try {
			resolved = new URL(url, baseUrl).href;
		} catch {
			throw new Error(`rack link: bundle "${key}": url "${url}" is relative and no baseUrl was given`);
		}
		manifest[key] = { url: resolved, sha256 };
		bundles[key] = await fetchPinnedBundle(resolved, sha256, fetchImpl, cache);
	}));
	return { rack: doc.rack, manifest, bundles };
}
