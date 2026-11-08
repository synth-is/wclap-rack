// pack.js — browser-side rack bundle packing and downloads for the test host's assembler:
// minimal ustar writer, gzip via CompressionStream, sha256 via crypto.subtle, anchor downloads.

/** Minimal ustar writer (regular files only) — same layout as tools/rack-bundle.mjs. */
export function tarFiles(entries) {
	const chunks = [];
	const enc = new TextEncoder();
	for (const [name, content] of entries) {
		const header = new Uint8Array(512);
		const nameBytes = enc.encode(name.slice(0, 100));
		header.set(nameBytes, 0);
		const writeOctal = (off, len, value) => {
			const s = value.toString(8).padStart(len - 1, '0') + '\0';
			header.set(enc.encode(s), off);
		};
		writeOctal(100, 8, 0o644); // mode
		writeOctal(108, 8, 0); // uid
		writeOctal(116, 8, 0); // gid
		writeOctal(124, 12, content.length); // size
		writeOctal(136, 12, 0); // mtime
		header.fill(0x20, 148, 156); // chksum spaces while computing
		header[156] = 0x30; // '0' regular file
		header.set(enc.encode('ustar\0'), 257);
		header.set(enc.encode('00'), 263);
		let sum = 0;
		for (const b of header) sum += b;
		const chk = sum.toString(8).padStart(6, '0') + '\0 ';
		header.set(enc.encode(chk), 148);
		chunks.push(header, content, new Uint8Array((512 - (content.length % 512)) % 512));
	}
	chunks.push(new Uint8Array(1024));
	const total = chunks.reduce((n, c) => n + c.length, 0);
	const out = new Uint8Array(total);
	let off = 0;
	for (const c of chunks) { out.set(c, off); off += c.length; }
	return out;
}

/** gzip via CompressionStream (all modern browsers). */
export async function gzip(bytes) {
	const cs = new CompressionStream('gzip');
	const stream = new Blob([bytes]).stream().pipeThrough(cs);
	return new Uint8Array(await new Response(stream).arrayBuffer());
}

export async function sha256Hex(bytes) {
	const digest = await crypto.subtle.digest('SHA-256', bytes);
	return [...new Uint8Array(digest)].map((b) => b.toString(16).padStart(2, '0')).join('');
}

/** Download `bytes` as `filename`. */
export function download(filename, bytes, type = 'application/gzip') {
	const a = document.createElement('a');
	a.href = URL.createObjectURL(new Blob([bytes], { type }));
	a.download = filename;
	a.click();
	setTimeout(() => URL.revokeObjectURL(a.href), 5000);
}

/**
 * Build a packed rack bundle (.wclap-rack.tar.gz bytes) from the model — no download.
 * rack: parsed rack.json (with devices/source), bundles: { key: archive-or-wasm bytes }.
 * Returns { manifest, tar, gz }.
 */
export async function buildRackBundle(rack, bundles) {
	const entries = [['rack.json', new TextEncoder().encode(JSON.stringify(rack, null, 2) + '\n')]];
	const manifest = {};
	for (const [key, bytes] of Object.entries(bundles)) {
		const file = key.replace(/\.wclap(\.tar\.gz)?$/, '') + '.wclap.tar.gz';
		entries.push([file, bytes]);
		const { wasm } = await (await import('../reference-host/index.mjs')).loadBundleBytesAsync(bytes);
		manifest[key] = { path: file, sha256: await sha256Hex(wasm) };
	}
	entries.push(['MANIFEST.json', new TextEncoder().encode(JSON.stringify(manifest, null, 2) + '\n')]);
	const tar = tarFiles(entries);
	const gz = await gzip(tar);
	return { manifest, tar, gz };
}

/** Build and download. */
export async function packRackBundle(name, rack, bundles) {
	const { manifest, gz } = await buildRackBundle(rack, bundles);
	download(`${name || 'rack'}.wclap-rack.tar.gz`, gz);
	return { manifest, bytes: gz.length };
}
