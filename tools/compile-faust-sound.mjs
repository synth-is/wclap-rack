#!/usr/bin/env node
// compile-faust-sound.mjs — compile a Synth.is free-form Faust genome (sound id) to the
// faust2wasm pair (genome.wasm + genome.json) that the faust-runner WCLAP runs
// (engines/faust-runner, wclap.files/1 delivery). This is the piece the browser cannot
// do: the vendored libfaust compiles to its own worklet, not to an exportable module, so
// the pair is compiled here with the Faust compiler ($FAUST or ~/faust/bin/faust) — the
// same step extract/takeout uses when packing a sound's delivered wclap-rack.
//
// Usage: node tools/compile-faust-sound.mjs <soundId> <outDir>
// Env:   RECOMMEND_API (default https://api.synth.is), FAUST, SYNTH_IS (default
//        ~/Developer/apps/synth.is — for kromosynth/faust-genome).
import { execFileSync } from 'node:child_process';
import fs from 'node:fs';
import os from 'node:os';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const soundId = process.argv[2];
const outDir = process.argv[3];
if (!soundId || !outDir) { console.error('usage: compile-faust-sound.mjs <soundId> <outDir>'); process.exit(2); }

const RECOMMEND_API = (process.env.RECOMMEND_API || 'https://api.synth.is').replace(/\/+$/, '');
const FAUST = process.env.FAUST || path.join(os.homedir(), 'faust', 'bin', 'faust');
const SYNTH_IS = process.env.SYNTH_IS || path.join(os.homedir(), 'Developer', 'apps', 'synth.is');

// 1. the genome, exactly as the API returns it
const r = await fetch(`${RECOMMEND_API}/api/exploration/genome/${encodeURIComponent(soundId)}?format=raw`, {
  // the API 500s requests carrying a non-allowlisted Origin — send the production one
  headers: { Origin: 'https://synth.is' },
});
if (!r.ok) { console.error(`genome fetch: HTTP ${r.status}`); process.exit(3); }
const raw = await r.json();
const genome = raw.genome ?? raw;
if ((genome.substrate ?? raw.substrate) !== 'faust') { console.error('not a faust genome'); process.exit(3); }

// 2. tree -> .dsp via the platform's own printer (evolved params baked in)
const { deserialiseFaustGenome, faustGenomeToDSP } = await import(
  path.join(SYNTH_IS, 'kromosynth', 'faust-genome', 'genome.js'));
const live = deserialiseFaustGenome(genome);
const dsp = faustGenomeToDSP(live);
fs.mkdirSync(outDir, { recursive: true });
const dspFile = path.join(outDir, 'genome.dsp');
fs.writeFileSync(dspFile, dsp);

// 3. faust2wasm — same target/options the synth-is-faust example rack was built with
//    (-lang wasm-i: the module is meant to be run by an interpreter, here wasm3)
const outWasm = path.join(outDir, 'genome.wasm');
execFileSync(FAUST, [
  '-lang', 'wasm-i', '-fpga-mem-th', '4', '-ct', '1', '-es', '1', '-mcd', '16',
  '-mdd', '1024', '-mdy', '33', '-single', '-ftz', '0',
  '-o', outWasm, dspFile,
], { stdio: 'inherit' });
const json = outWasm.replace(/\.wasm$/, '.json');
if (fs.existsSync(json)) fs.renameSync(json, path.join(outDir, 'genome.json'));
else { console.error('faust produced no metadata json'); process.exit(4); }
console.log(`wrote ${outWasm} (${fs.statSync(outWasm).size} B) + genome.json (${fs.statSync(path.join(outDir, 'genome.json')).size} B) for ${soundId}`);
