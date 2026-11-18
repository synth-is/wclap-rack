#!/usr/bin/env node
// qd-rack.mjs — a MAP-Elites-style QD loop over wclap-rack GENOMES (tools/rack-genome.mjs).
//
//   node tools/qd-rack.mjs --seed <rack dir | .wclap-rack.tar.gz> \
//       [--pool <dir with *.wclap.tar.gz>] [--generations 10] [--population 24] \
//       [--seconds 1] [--rng-seed 1] [--cells 8] [--cv spectralCentroid,zcr] \
//       [--out <run dir>]
//
// Each individual is a rack document (source + ordered device chain + params) with NEAT-style
// innovation markings. Fitness = renderRack() (reference host, deterministic) -> behavioural
// descriptors (tools/rack-genome.mjs describeBuffer) mapped onto a CV grid; the archive keeps
// the best-quality elite per cell. Quality for now is a placeholder objective (finite render,
// peak near 0.5 — NOT silence, NOT clipping); the real objective (CLAP/latent descriptors,
// human liking via the recommend service) plugs into evaluate() later.
//
// The run dir gets run.json, gen-XXX.json per generation and archive.json + one rack.json per
// surviving elite — the same shape the web app saves to the Sound Garden as
// kind 'wclap-rack-combination', so QD outputs are directly loadable on /wclap and in DAWs.
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { loadRackBundle, writeWav } from './rack-bundle.mjs';
import { renderRack, describeBundle } from '../reference-host/index.mjs';
import {
	GENOME_FORMAT, genomeFromRack, rackFromGenome, mutateGenome, crossoverGenomes,
	describeBuffer, cellOf, mulberry32, randomGenome,
} from './rack-genome.mjs';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const args = process.argv.slice(2);
const opt = (name, dflt) => { const i = args.indexOf(`--${name}`); return i >= 0 ? args[i + 1] : dflt; };
const has = (name) => args.includes(`--${name}`);

const seedBundle = opt('seed');
if (!seedBundle) {
	console.error('usage: qd-rack.mjs --seed <rack dir|tar.gz> [--pool dir] [--generations 10] [--population 24] [--seconds 1] [--rng-seed 1] [--cells 8] [--cv spectralCentroid,zcr] [--out runDir]');
	process.exit(2);
}
const poolDir = opt('pool');
const generations = Number(opt('generations', 10));
const population = Number(opt('population', 24));
const seconds = Number(opt('seconds', 1));
// RNG seed. NOTE: `--seed` is the seed RACK; reading the RNG seed from `--seed` too (as this did
// until 2026-11-12) gave Number('<path>') = NaN -> mulberry32 state 0, so every run shared one
// RNG stream regardless of what was passed. Use --rng-seed.
const seed = Number(opt('rng-seed', 1));
if (!Number.isFinite(seed)) { console.error('--rng-seed must be a number'); process.exit(2); }
const cells = Number(opt('cells', 8));
const cvKeys = String(opt('cv', 'spectralCentroid,zcr')).split(',').map((s) => s.trim());
// --quality peak (original placeholder: peak near 0.5 — rewards LEVEL, so a gain change wins a
// cell) | gate (pass/fail: finite, audible, not clipped, no DC; ties keep the incumbent, i.e.
// first-come-kept like the burst campaign's --fitness none).
const qualityMode = String(opt('quality', 'peak'));
// --cv-range spectralCentroid:100:8000:log,flatness:1e-9:0.1:log — fixed grid ranges (optionally
// log-scaled) instead of seed ±25%, so archives from different seeds/runs share one map.
const cvRangeArg = opt('cv-range');
const cvFixed = new Map(cvRangeArg ? String(cvRangeArg).split(',').map((s) => { const [k, lo, hi, scale] = s.split(':'); return [k, [Number(lo), Number(hi), scale === 'log']]; }) : []);
// --fixed-source: the source genome and its notes never mutate; only the rack evolves (the
// 2026-10-31 effect-racks design's stage 1, "measure first").
const fixedSource = has('fixed-source');
const mutWeights = fixedSource ? { sourceParam: 0, note: 0 } : {};
// --max-minutes N: stop breeding after N minutes (checked between generations), then write out.
const maxMinutes = Number(opt('max-minutes', 0));
// --render-elites: also write a peak-normalised render.wav next to every elite's rack.json.
const renderElites = has('render-elites');
// --max-evals N: an evaluation budget (stops mid-generation when reached). Budgets, not time
// boxes, make arms comparable on a shared, loaded machine (autonomous-chain lesson 6).
const maxEvals = Number(opt('max-evals', 0));
// --random: the random-rack baseline arm. Every individual is randomGenome() over the seed's
// source (no selection, crossover or mutation); the archive is filled the same way.
const randomArm = has('random');
let evals = 0;
const t0 = Date.now();
const outDir = path.resolve(opt('out', path.join(HERE, '..', 'runs', `qd-${Date.now()}`)));
const sampleRate = 48000;
const rng = mulberry32(seed);

// CV dimension ranges: measured once from the seed render, padded, so the grid adapts to the
// seed's behaviour space. (A fixed global range would need a corpus; this is the per-run map.)
const CV_RANGE_PAD = 0.25;

// ── seed + pool ──────────────────────────────────────────────────────────────
const seedPack = loadRackBundle(seedBundle); // { rack, manifest, bundles, files }
const bundles = { ...seedPack.bundles };   // bundleKey -> archive/wasm bytes
let pool = [];                               // [{ bundle, plugin, params: {key: def} }]
const paramRanges = new Map();               // `${bundle}|${plugin}|${key}` -> [min,max]

if (poolDir) {
	for (const f of fs.readdirSync(poolDir).filter((f) => f.endsWith('.wclap.tar.gz'))) {
		const key = f.replace(/\.wclap\.tar\.gz$/, '');
		const bytes = new Uint8Array(fs.readFileSync(path.join(poolDir, f)));
		bundles[key] = bytes;
		try {
			const desc = await describeBundle(bytes);
			for (const p of desc.plugins) {
				if (!p.features?.includes('audio-effect')) continue; // source-only plugins stay out of the fx pool
				pool.push({ bundle: key, plugin: p.id, params: Object.fromEntries(p.params.map((pp) => [String(pp.id), pp.def])) });
				for (const pp of p.params) paramRanges.set(`${key}|${p.id}|${pp.id}`, [pp.min, pp.max]);
			}
		} catch (e) {
			console.error(`pool: skipping ${f}: ${e.message}`);
		}
	}
}
console.log(`seed rack "${seedPack.rack.name ?? '(unnamed)'}" devices=${seedPack.rack.devices?.length ?? 0} · pool=${pool.length} fx plugins`);

// innovation counter shared by the whole run (NEAT historical markings)
let innovationCounter = (seedPack.rack.devices ?? []).length + 1;
const ctx = {
	nextInnovation: () => innovationCounter++,
	pool,
	paramRange: (bundle, plugin, key) => paramRanges.get(`${bundle}|${plugin}|${key}`)
		?? paramRanges.get(`${bundle}|${plugin}|${Number(key)}`) ?? null,
};

// ── evaluation ───────────────────────────────────────────────────────────────
// deterministic reference input when the genome has no source (same recipe as render.mjs)
function referenceInput(n) {
	const left = new Float32Array(n);
	for (let i = 0; i < n; i++) {
		const saw = 2 * ((i * 110 / sampleRate) % 1) - 1;
		const noise = ((i * 2654435761 >>> 0) % 4294967296) / 4294967296 * 2 - 1;
		left[i] = Math.fround((saw * 0.5 + noise * 0.3) * 0.8);
		if (i === 0) left[i] = 1;
	}
	return { left, right: left };
}

async function evaluate(genome) {
	const rack = rackFromGenome(genome);
	const n = Math.round(seconds * sampleRate);
	const input = rack.source ? undefined : referenceInput(n);
	try {
		const r = await renderRack({ rack, bundles, files: seedPack.files ?? {}, input, sampleRate });
		const cv = describeBuffer(r.left, r.right, sampleRate);
		// placeholder objective: finite render with peak in a sane window (not silence, not clipping)
		let quality;
		if (qualityMode === 'gate') {
			// level-blind: the output is judged as if through a fixed peak-normalising master (the
			// Faust sources render well above 0 dBFS, so "not clipped" on raw float output rejected
			// the dry seed itself). Fails only non-finite, silent, DC-dominated or blown-up renders.
			const mean = r.left.reduce((a, v) => a + v, 0) / r.left.length;
			const pass = cv.finite && cv.rms > 1e-4 && cv.peak < 1000 && Math.abs(mean) < 0.05 * cv.peak && cv.crest > 1.05;
			quality = pass ? 1 : 0;
		} else {
			quality = cv.finite ? Math.max(0, 1 - Math.abs(cv.peak - 0.5) * 2) : 0;
		}
		return { cv, quality, fingerprint: r.fingerprint, ok: true };
	} catch (e) {
		return { cv: null, quality: 0, ok: false, error: e.message };
	}
}

// ── CV grid ranges from the seed ─────────────────────────────────────────────
const seedGenome = genomeFromRack(seedPack.rack, ctx);
const seedEval = await evaluate(seedGenome);
const cvDims = cvKeys.map((k) => {
	if (cvFixed.has(k)) { const [min, max, log] = cvFixed.get(k); return { key: k, min, max, cells, ...(log ? { log: true } : {}) }; }
	const v = seedEval.cv?.[k] ?? 0;
	const pad = Math.abs(v) * CV_RANGE_PAD + 1e-6;
	return { key: k, min: Math.max(0, v - pad), max: v + pad, cells };
});
console.log(`cv dims: ${cvDims.map((d) => `${d.key} [${d.min.toFixed(3)}, ${d.max.toFixed(3)}] x ${d.cells}`).join(' · ')}`);

// ── MAP-Elites loop ──────────────────────────────────────────────────────────
fs.mkdirSync(outDir, { recursive: true });
const archive = new Map(); // cellKey -> { genome, cv, quality, fingerprint }
let current = [];
for (let i = 0; i < population; i++) {
	if (randomArm) { current.push(randomGenome(seedGenome, ctx, rng)); continue; }
	const g = genomeFromRack(seedPack.rack, ctx);
	for (let m = 0; m < 1 + Math.floor(rng() * 3); m++) mutateGenome(g, ctx, rng, mutWeights);
	current.push(g);
}

const runDoc = {
	format: 'wclap-rack-qd-run', version: 1, seed, generations, population, seconds, sampleRate,
	cvDims, quality: qualityMode, fixedSource, mode: randomArm ? 'random' : 'evolve', maxEvals, node: process.version, seedRack: seedPack.rack.name ?? null, pool: pool.map((p) => `${p.bundle}/${p.plugin}`),
	createdAt: new Date().toISOString(),
};
fs.writeFileSync(path.join(outDir, 'run.json'), JSON.stringify(runDoc, null, 2));

for (let gen = 0; gen < generations; gen++) {
	const scored = [];
	for (const g of current) {
		if (maxEvals > 0 && evals >= maxEvals) break;
		const ev = await evaluate(g);
		const evalIndex = evals++;
		scored.push({ genome: g, ...ev });
		if (ev.ok && (qualityMode !== 'gate' || ev.quality > 0)) {
			const key = cellOf(ev.cv, cvDims);
			const prev = archive.get(key);
			if (!prev || ev.quality > prev.quality) archive.set(key, { genome: g, cv: ev.cv, quality: ev.quality, fingerprint: ev.fingerprint, gen, evalIndex });
		}
	}
	const genDoc = {
		generation: gen,
		evals,
		archiveSize: archive.size,
		best: [...archive.values()].sort((a, b) => b.quality - a.quality).slice(0, 5)
			.map((e) => ({ quality: +e.quality.toFixed(4), cell: cellOf(e.cv, cvDims), fingerprint: e.fingerprint, devices: e.genome.devices.map((d) => d.plugin) })),
		individuals: scored.map((s) => ({ ok: s.ok, quality: +s.quality.toFixed(4), fingerprint: s.fingerprint, cv: s.cv ? Object.fromEntries(Object.entries(s.cv).map(([k, v]) => [k, typeof v === 'number' ? +v.toPrecision(5) : v])) : null, error: s.error, devices: s.genome.devices.map((d) => `${d.bundle}/${d.plugin}${d.on === false ? ' (off)' : ''}`) })),
	};
	fs.writeFileSync(path.join(outDir, `gen-${String(gen).padStart(3, '0')}.json`), JSON.stringify(genDoc, null, 2));
	writeArchive();
	const best = genDoc.best[0];
	console.log(`gen ${gen}: evals=${evals} archive=${archive.size} best=${best ? `q=${best.quality} cell=${best.cell} fp=${best.fingerprint}` : 'none'}`);

	// breed: elites + crossover + mutation
	if (maxEvals > 0 && evals >= maxEvals) { console.log(`eval budget reached: ${evals} evaluations`); break; }
	const elites = [...archive.values()];
	const next = [];
	while (next.length < population) {
		if (randomArm) {
			next.push(randomGenome(seedGenome, ctx, rng));
		} else if (elites.length && rng() < 0.85) {
			const a = elites[Math.floor(rng() * elites.length)];
			const b = elites[Math.floor(rng() * elites.length)];
			const child = crossoverGenomes(a.genome, b.genome, rng, { fitnessA: a.quality, fitnessB: b.quality });
			for (let m = 0; m < 1 + Math.floor(rng() * 2); m++) mutateGenome(child, ctx, rng, mutWeights);
			next.push(child);
		} else {
			const g = genomeFromRack(seedPack.rack, ctx);
			for (let m = 0; m < 2 + Math.floor(rng() * 3); m++) mutateGenome(g, ctx, rng, mutWeights);
			next.push(g);
		}
	}
	current = next;
	if (maxMinutes > 0 && (Date.now() - t0) / 60000 >= maxMinutes) {
		console.log(`time box reached after generation ${gen} (${maxMinutes} min)`);
		break;
	}
}

// ── write the archive as rack documents (loadable on /wclap and in DAWs) ──────
// Called after every generation too, so a run killed overnight keeps what it found.
function writeArchive() {
	const doc = [...archive.entries()].map(([cell, e]) => ({
		cell, quality: e.quality, fingerprint: e.fingerprint, generation: e.gen, evalIndex: e.evalIndex, cv: e.cv, rack: rackFromGenome(e.genome),
	})).sort((a, b) => b.quality - a.quality);
	fs.writeFileSync(path.join(outDir, 'archive.json.tmp'), JSON.stringify(doc, null, 2));
	fs.renameSync(path.join(outDir, 'archive.json.tmp'), path.join(outDir, 'archive.json'));
	return doc;
}
const archiveDoc = writeArchive();
const elitesDir = path.join(outDir, 'elites');
fs.mkdirSync(elitesDir, { recursive: true });
for (const [i, e] of archiveDoc.entries()) {
	const dir = path.join(elitesDir, `elite-${String(i).padStart(3, '0')}-q${e.quality.toFixed(3)}-c${e.cell.replace(/,/g, '_')}`);
	fs.mkdirSync(dir, { recursive: true });
	fs.writeFileSync(path.join(dir, 'rack.json'), JSON.stringify({ $schema: '../../../schema/rack.schema.json', ...e.rack }, null, 2));
	if (renderElites) {
		try {
			const r = await renderRack({ rack: e.rack, bundles, files: seedPack.files ?? {}, input: e.rack.source ? undefined : referenceInput(Math.round(seconds * sampleRate)), sampleRate });
			writeNormalisedWav(path.join(dir, 'render.wav'), r);
		} catch (err) { console.error(`render ${dir}: ${err.message}`); }
	}
}
function writeNormalisedWav(file, r) {
	let pk = 0; for (let i = 0; i < r.left.length; i++) pk = Math.max(pk, Math.abs(r.left[i]), Math.abs(r.right[i]));
	const g = pk > 0 ? 0.891 / pk : 1;
	writeWav(file, r.left.map((v) => v * g), r.right.map((v) => v * g), sampleRate);
}
// also render the single best elite to wav for a quick listen
const best = archiveDoc[0];
if (best) {
	const r = await renderRack({ rack: best.rack, bundles, files: seedPack.files ?? {}, input: best.rack.source ? undefined : referenceInput(Math.round(seconds * sampleRate)), sampleRate });
	// fixed master for listening: peak-normalise to -1 dBFS (renders can be far above 0 dBFS)
	writeNormalisedWav(path.join(outDir, 'best.wav'), r);
}
console.log(`run written to ${outDir} — archive=${archive.size} elites, best q=${best?.quality.toFixed(4)} (${best?.rack.name ?? ''})`);
