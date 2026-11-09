#!/usr/bin/env node
// unpack-rack.mjs — turn a .wclap-rack.tar.gz into the on-disk layout the Synth.is plugin
// loads as one instrument/preset:
//   <out>/rack.json, <out>/MANIFEST.json, <out>/bundles/<key>/module.wasm (+ ui/, LICENSE, …)
//   node tools/unpack-rack.mjs <bundle.wclap-rack.tar.gz> <out-dir>
// e.g. into the plugin's instrument library on macOS:
//   node tools/unpack-rack.mjs synth-fx-rack.wclap-rack.tar.gz "$HOME/Music/Synth.is/Instruments/my-rack"
import fs from 'node:fs';
import path from 'node:path';
import zlib from 'node:zlib';
import { untar, loadBundleBytesAsync, validateRackBundle, sha256Hex } from '../reference-host/index.mjs';

const [archive, outDir] = process.argv.slice(2);
if (!archive || !outDir) { console.error('usage: unpack-rack.mjs <bundle.wclap-rack.tar.gz> <out-dir>'); process.exit(2); }

const raw = new Uint8Array(fs.readFileSync(archive));
const tar = raw[0] === 0x1f && raw[1] === 0x8b ? zlib.gunzipSync(Buffer.from(raw)) : Buffer.from(raw);
let files = untar(tar);
if (!files['rack.json']) {
	const prefix = Object.keys(files).find((n) => /^[^/]+\/rack\.json$/.test(n))?.slice(0, -'rack.json'.length);
	if (prefix) files = Object.fromEntries(Object.entries(files).filter(([n]) => n.startsWith(prefix)).map(([n, v]) => [n.slice(prefix.length), v]));
}
if (!files['rack.json']) { console.error('no rack.json in archive'); process.exit(1); }

const rack = JSON.parse(Buffer.from(files['rack.json']).toString('utf8'));
const manifest = JSON.parse(Buffer.from(files['MANIFEST.json']).toString('utf8'));
const bundles = {};
for (const [key, entry] of Object.entries(manifest)) bundles[key] = files[entry.path];
const { errors } = await validateRackBundle(rack, manifest, bundles);
if (errors.length) { for (const e of errors) console.error('ERROR: ' + e); process.exit(1); }

fs.mkdirSync(outDir, { recursive: true });
fs.writeFileSync(path.join(outDir, 'rack.json'), JSON.stringify(rack, null, 2) + '\n');
fs.writeFileSync(path.join(outDir, 'MANIFEST.json'), JSON.stringify(manifest, null, 2) + '\n');
for (const [key, bytes] of Object.entries(bundles)) {
	const { files: inner } = await loadBundleBytesAsync(bytes);
	const dir = path.join(outDir, 'bundles', key);
	for (const [name, content] of Object.entries(inner)) {
		const p = path.join(dir, name);
		fs.mkdirSync(path.dirname(p), { recursive: true });
		fs.writeFileSync(p, content);
	}
	const sha = await sha256Hex(inner['module.wasm']);
	console.log(`bundles/${key}/  module.wasm sha256=${sha}`);
}
console.log(`unpacked "${rack.name ?? archive}" -> ${outDir} (${rack.devices?.length ?? 0} devices${rack.source ? ' + source' : ''})`);
