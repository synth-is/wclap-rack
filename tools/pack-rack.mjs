#!/usr/bin/env node
// pack-rack.mjs — pack a rack bundle directory into <name>.wclap-rack.tar.gz
//   node tools/pack-rack.mjs <rack-bundle-dir> [--out <file>]
import fs from 'node:fs';
import path from 'node:path';
import zlib from 'node:zlib';
import { tarFiles } from './rack-bundle.mjs';

const args = process.argv.slice(2);
const dir = args[0];
const outI = args.indexOf('--out');
const out = outI >= 0 ? args[outI + 1] : `${path.basename(path.resolve(dir))}.wclap-rack.tar.gz`;
if (!dir) { console.error('usage: pack-rack.mjs <rack-bundle-dir> [--out <file>]'); process.exit(2); }

const entries = [];
const walk = (d, base) => {
	for (const e of fs.readdirSync(d, { withFileTypes: true })) {
		const full = path.join(d, e.name);
		if (e.isDirectory()) walk(full, base);
		else entries.push([path.relative(base, full).split(path.sep).join('/'), fs.readFileSync(full)]);
	}
};
walk(dir, dir);
const tar = tarFiles(entries);
fs.writeFileSync(out, zlib.gzipSync(tar));
console.log(`packed ${entries.length} file(s) from ${dir} -> ${out} (${fs.statSync(out).size} bytes)`);
