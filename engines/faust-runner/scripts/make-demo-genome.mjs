#!/usr/bin/env node
// make-demo-genome.mjs — compile scripts/demo.dsp to a faust2wasm genome (genome.wasm + genome.json)
// for examples/faust-rack and the tests. Needs the Faust compiler: $FAUST or ~/faust/bin/faust.
// Usage: node engines/faust-runner/scripts/make-demo-genome.mjs <out-dir>
import { execFileSync } from 'node:child_process';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const outDir = process.argv[2];
if (!outDir) { console.error('usage: make-demo-genome.mjs <out-dir>'); process.exit(2); }

const faust = process.env.FAUST || path.join(process.env.HOME, 'faust', 'bin', 'faust');
const dsp = path.join(HERE, 'demo.dsp');
const outWasm = path.join(outDir, 'genome.wasm');
fs.mkdirSync(outDir, { recursive: true });
// faust2wasm: -lang wasm writes <out>.wasm and the matching metadata <out>.json (with widget byte offsets)
execFileSync(faust, ['-lang', 'wasm', '-o', outWasm, dsp], { stdio: 'inherit' });
const json = outWasm.replace(/\.wasm$/, '.json');
fs.renameSync(json, path.join(outDir, 'genome.json'));
console.log(`wrote ${outWasm} (${fs.statSync(outWasm).size} bytes) and ${path.join(outDir, 'genome.json')} (${fs.statSync(path.join(outDir, 'genome.json')).size} bytes)`);
