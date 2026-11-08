// Node-side loading of a rack bundle (directory or .wclap-rack.tar.gz) for the tools.
import fs from 'node:fs';
import path from 'node:path';
import zlib from 'node:zlib';
import { untar, loadBundleBytesAsync } from '../reference-host/index.mjs';

/** Read a rack bundle directory into { rack, manifest, bundles: { key: archive bytes } }. */
export function loadRackBundleDir(dir) {
	const rack = JSON.parse(fs.readFileSync(path.join(dir, 'rack.json'), 'utf8'));
	const manifest = JSON.parse(fs.readFileSync(path.join(dir, 'MANIFEST.json'), 'utf8'));
	const bundles = {};
	for (const [key, entry] of Object.entries(manifest)) {
		bundles[key] = new Uint8Array(fs.readFileSync(path.join(dir, entry.path)));
	}
	return { rack, manifest, bundles };
}

/** Read a packed rack bundle archive (.wclap-rack.tar.gz). */
export function loadRackBundleArchive(file) {
	const raw = new Uint8Array(fs.readFileSync(file));
	const tar = raw[0] === 0x1f && raw[1] === 0x8b ? zlib.gunzipSync(Buffer.from(raw)) : Buffer.from(raw);
	let files = untar(tar);
	if (!files['rack.json']) {
		const prefix = Object.keys(files).find((n) => /^[^/]+\/rack\.json$/.test(n))?.slice(0, -'rack.json'.length);
		if (prefix) files = Object.fromEntries(Object.entries(files).filter(([n]) => n.startsWith(prefix)).map(([n, v]) => [n.slice(prefix.length), v]));
	}
	if (!files['rack.json']) throw new Error('no rack.json in archive');
	const rack = JSON.parse(Buffer.from(files['rack.json']).toString('utf8'));
	const manifest = JSON.parse(Buffer.from(files['MANIFEST.json']).toString('utf8'));
	const bundles = {};
	for (const [key, entry] of Object.entries(manifest)) bundles[key] = files[entry.path];
	return { rack, manifest, bundles };
}

export function loadRackBundle(source) {
	return fs.statSync(source).isDirectory() ? loadRackBundleDir(source) : loadRackBundleArchive(source);
}

/** Minimal WAV writer (PCM16 stereo). */
export function writeWav(file, left, right, sampleRate = 48000) {
	const n = left.length;
	const data = Buffer.alloc(n * 4);
	for (let i = 0; i < n; i++) {
		data.writeInt16LE(Math.max(-1, Math.min(1, left[i])) * 32767 | 0, i * 4);
		data.writeInt16LE(Math.max(-1, Math.min(1, right[i])) * 32767 | 0, i * 4 + 2);
	}
	const header = Buffer.alloc(44);
	header.write('RIFF', 0); header.writeUInt32LE(36 + data.length, 4); header.write('WAVE', 8);
	header.write('fmt ', 12); header.writeUInt32LE(16, 16); header.writeUInt16LE(1, 20); header.writeUInt16LE(2, 22);
	header.writeUInt32LE(sampleRate, 24); header.writeUInt32LE(sampleRate * 4, 28); header.writeUInt16LE(4, 32); header.writeUInt16LE(16, 34);
	header.write('data', 36); header.writeUInt32LE(data.length, 40);
	fs.writeFileSync(file, Buffer.concat([header, data]));
}

/** Minimal tar writer (ustar, regular files only). Offsets: name@0, mode@100, uid@108, gid@116, size@124, mtime@136, chksum@148, typeflag@156, magic@257. */
export function tarFiles(entries) {
	const chunks = [];
	for (const [name, content] of entries) {
		const header = Buffer.alloc(512);
		header.write(name.slice(0, 100), 0);
		header.write('0000644\0', 100);  // mode
		header.write('0000000\0', 108);  // uid
		header.write('0000000\0', 116);  // gid
		header.write(content.length.toString(8).padStart(11, '0') + '\0', 124); // size
		header.write('00000000000\0', 136); // mtime
		header.fill(0x20, 148, 156);        // chksum field is spaces while computing
		header.write('0', 156);             // typeflag: regular file
		header.write('ustar\0', 257);
		header.write('00', 263);
		let sum = 0;
		for (const b of header) sum += b;
		header.write(sum.toString(8).padStart(6, '0') + '\0 ', 148);
		chunks.push(header, content, Buffer.alloc((512 - (content.length % 512)) % 512));
	}
	chunks.push(Buffer.alloc(1024)); // end-of-archive
	return Buffer.concat(chunks);
}
