#!/usr/bin/env node
// rack-genome.mjs — a wclap-rack document as a QD genome, NEAT-flavoured.
//
// The genome IS the rack document (schema/rack.schema.json) plus one extra field per device
// gene: `innovation` — a NEAT-style historical marking. Innovations let crossover align two
// chains' genes by ORIGIN (same innovation = same structural mutation) instead of by index,
// so matching genes combine, and disjoint/excess genes are inherited from the fitter parent.
// A serial chain is the "network topology"; add/remove/swap-device are the structural
// mutations (NEAT's add-node/add-connection), param perturbation is the weight mutation.
//
// Pure logic only — no I/O, no wasm — so it is unit-testable; tools/qd-rack.mjs drives it with
// renderRack() as the fitness evaluator.
import { validateRackDoc } from '../reference-host/index.mjs';

export const GENOME_FORMAT = 'wclap-rack-genome';
export const GENOME_VERSION = 1;

/** Deterministic RNG (mulberry32) so a run dir can be reproduced from its seed. */
export function mulberry32(seed) {
	let a = seed >>> 0;
	return function () {
		a |= 0; a = (a + 0x6D2B79F5) | 0;
		let t = Math.imul(a ^ (a >>> 15), 1 | a);
		t = (t + Math.imul(t ^ (t >>> 7), 61 | t)) ^ t;
		return ((t ^ (t >>> 14)) >>> 0) / 4294967296;
	};
}

const clone = (o) => JSON.parse(JSON.stringify(o));

/**
 * Rack document -> genome. Devices get innovation ids from `ctx.nextInnovation()` (a shared
 * counter, so a device introduced twice in a lineage shares its marking — NEAT semantics).
 * Existing genomes pass through unchanged.
 */
export function genomeFromRack(rack, ctx = {}) {
	if (rack?.format === GENOME_FORMAT) return clone(rack);
	const next = ctx.nextInnovation ?? (() => 0);
	const genome = {
		format: GENOME_FORMAT,
		version: GENOME_VERSION,
		name: rack.name,
		source: rack.source ? { ...clone(rack.source) } : undefined,
		devices: (rack.devices ?? []).map((d) => ({ ...clone(d), innovation: next() })),
	};
	return genome;
}

/** Genome -> plain rack document (drops genome-only fields); validates against the schema. */
export function rackFromGenome(genome) {
	const rack = {
		format: 'wclap-rack',
		version: 1,
		...(genome.name ? { name: genome.name } : {}),
		...(genome.source ? { source: clone(genome.source) } : {}),
		devices: (genome.devices ?? []).map((d) => {
			const { innovation, ...rest } = d;
			return clone(rest);
		}),
	};
	// source keeps its own non-gene fields; strip nothing there beyond what the schema allows
	if (rack.source) delete rack.source.innovation;
	const errors = validateRackDoc(rack);
	if (errors.length) throw new Error(`genome -> rack invalid: ${errors.join('; ')}`);
	return rack;
}

/**
 * Param range lookup context. ctx.paramRange(bundle, plugin, key) -> [min, max] | null.
 * qd-rack.mjs builds this by instantiating each pool plugin once and reading clap.params.
 */
const clampParam = (value, range) => {
	if (!range) return value;
	const [min, max] = range;
	return Math.min(max, Math.max(min, value));
};

const perturbParams = (device, ctx, rng, sigma = 0.1) => {
	const params = { ...(device.params ?? {}) };
	for (const [k, v] of Object.entries(params)) {
		if (typeof v !== 'number') continue;
		const range = ctx.paramRange?.(device.bundle, device.plugin, k) ?? null;
		const span = range ? range[1] - range[0] : Math.abs(v) || 1;
		let nv = v + (rng() * 2 - 1) * sigma * span;
		nv = clampParam(nv, range);
		params[k] = range ? nv : clampParam(nv, [v - 2 * Math.abs(v) - 1, v + 2 * Math.abs(v) + 1]);
	}
	device.params = params;
};

/**
 * One structural or parametric mutation. ctx = {
 *   nextInnovation(), pool: [{ bundle, plugin }], paramRange?, weights?, maxDevices?
 * }. Mutates and returns `genome`.
 */
export function mutateGenome(genome, ctx, rng, weights = {}) {
	const w = {
		param: 0.45, addDevice: 0.15, removeDevice: 0.08, swapDevice: 0.08,
		toggleOn: 0.05, reorder: 0.05, sourceParam: 0.09, note: 0.05,
		...weights,
	};
	const devices = genome.devices ?? [];
	const pick = () => {
		let r = rng() * Object.values(w).reduce((a, b) => a + b, 0);
		for (const [k, v] of Object.entries(w)) { r -= v; if (r <= 0) return k; }
		return 'param';
	};
	const op = pick();
	const maxDevices = ctx.maxDevices ?? 8;

	if (op === 'param' && devices.length) {
		perturbParams(devices[Math.floor(rng() * devices.length)], ctx, rng);
	} else if (op === 'addDevice' && ctx.pool?.length && devices.length < maxDevices) {
		const p = ctx.pool[Math.floor(rng() * ctx.pool.length)];
		const gene = { bundle: p.bundle, plugin: p.plugin, on: true, params: clone(p.params ?? {}), innovation: ctx.nextInnovation() };
		devices.splice(Math.floor(rng() * (devices.length + 1)), 0, gene);
	} else if (op === 'removeDevice' && devices.length > 1) {
		devices.splice(Math.floor(rng() * devices.length), 1);
	} else if (op === 'swapDevice' && ctx.pool?.length && devices.length) {
		const i = Math.floor(rng() * devices.length);
		const p = ctx.pool[Math.floor(rng() * ctx.pool.length)];
		devices[i] = { bundle: p.bundle, plugin: p.plugin, on: devices[i].on ?? true, params: clone(p.params ?? {}), innovation: ctx.nextInnovation() };
	} else if (op === 'toggleOn' && devices.length) {
		const i = Math.floor(rng() * devices.length);
		devices[i].on = !(devices[i].on ?? true);
	} else if (op === 'reorder' && devices.length > 1) {
		const i = Math.floor(rng() * (devices.length - 1));
		[devices[i], devices[i + 1]] = [devices[i + 1], devices[i]];
	} else if (op === 'sourceParam' && genome.source) {
		perturbParams(genome.source, ctx, rng);
	} else if (op === 'note' && genome.source?.notes?.length) {
		const n = genome.source.notes[Math.floor(rng() * genome.source.notes.length)];
		if (rng() < 0.7) n.key = Math.max(0, Math.min(127, n.key + (rng() < 0.5 ? -1 : 1) * [1, 2, 3, 5, 7, 12][Math.floor(rng() * 6)]));
		else n.velocity = Math.max(0.05, Math.min(1, (n.velocity ?? 0.8) + (rng() * 2 - 1) * 0.2));
	} else {
		// fall back to a param mutation so every call mutates something
		if (devices.length) perturbParams(devices[Math.floor(rng() * devices.length)], ctx, rng);
		else if (genome.source) perturbParams(genome.source, ctx, rng);
	}
	genome.devices = devices;
	return genome;
}

/**
 * A random rack over the base genome's source (the random-rack baseline arm): the source is
 * kept as-is, and 1..maxDevices devices are drawn uniformly from ctx.pool, each with every
 * param drawn uniformly in its known clap.params range (params with no known range keep the
 * pool default). Fresh innovation ids for every device.
 * Reason: a same-budget random arm shows what selection buys over blind sampling of the pool
 * (autonomous-chain lesson 7: random draws matched the model's reasoned proposals).
 * @param {object} base genome whose source (and name) is kept
 * @param {{pool: Array, nextInnovation: Function, paramRange?: Function}} ctx
 * @param {() => number} rng
 * @param {{minDevices?: number, maxDevices?: number}} [opts]
 * @returns {object} a new genome
 */
export function randomGenome(base, ctx, rng, { minDevices = 1, maxDevices = 6 } = {}) {
	if (!ctx.pool?.length) throw new Error('randomGenome: empty pool');
	const k = minDevices + Math.floor(rng() * (maxDevices - minDevices + 1));
	const devices = [];
	for (let i = 0; i < k; i++) {
		const p = ctx.pool[Math.floor(rng() * ctx.pool.length)];
		const params = {};
		for (const [key, def] of Object.entries(p.params ?? {})) {
			const range = ctx.paramRange?.(p.bundle, p.plugin, key) ?? null;
			params[key] = range && typeof def === 'number' ? range[0] + rng() * (range[1] - range[0]) : def;
		}
		devices.push({ bundle: p.bundle, plugin: p.plugin, on: true, params, innovation: ctx.nextInnovation() });
	}
	const g = { format: GENOME_FORMAT, version: GENOME_VERSION, devices };
	if (base?.name) g.name = base.name;
	if (base?.source) g.source = clone(base.source);
	return g;
}

/**
 * NEAT-style crossover. Device genes align by innovation id: matching genes take params from
 * the fitter parent (or a 50/50 blend when equal); disjoint/excess genes come from the fitter
 * parent only — classic NEAT. Chain order follows parent A, then B's extra genes appended.
 * The source comes from the fitter parent with params blended 50/50.
 */
export function crossoverGenomes(a, b, rng, { fitnessA = 0, fitnessB = 0, blendParams = true } = {}) {
	const byInnovA = new Map((a.devices ?? []).map((d) => [d.innovation, d]));
	const byInnovB = new Map((b.devices ?? []).map((d) => [d.innovation, d]));
	const fitterFirst = fitnessA >= fitnessB;
	const devices = [];
	const used = new Set();
	// A's order first: matching genes are chosen by fitness, A-only genes only if A is fitter
	for (const da of a.devices ?? []) {
		const db = byInnovB.get(da.innovation);
		if (db) {
			used.add(da.innovation);
			devices.push(blendGene(da, db, rng, blendParams, fitterFirst ? 'a' : 'b'));
		} else if (fitterFirst) {
			devices.push(clone(da));
		}
	}
	// then B's disjoint genes, only if B is fitter
	if (!fitterFirst) {
		for (const db of b.devices ?? []) {
			if (!byInnovA.has(db.innovation)) devices.push(clone(db));
		}
	}
	const source = blendSource(a.source, b.source, rng, fitterFirst);
	const child = { format: GENOME_FORMAT, version: GENOME_VERSION, devices };
	if (source) child.source = source;
	if (a.name || b.name) child.name = `${a.name ?? 'a'} x ${b.name ?? 'b'}`;
	return child;
}

function blendGene(da, db, rng, blendParams, fitter) {
	const base = clone(fitter === 'a' ? da : db);
	if (blendParams) {
		const params = { ...(da.params ?? {}) };
		for (const [k, vb] of Object.entries(db.params ?? {})) {
			const va = da.params?.[k];
			params[k] = typeof va === 'number' && typeof vb === 'number' ? (rng() < 0.5 ? va : vb) : vb;
		}
		base.params = params;
	}
	return base;
}

function blendSource(sa, sb, rng, fitterFirst) {
	if (!sa) return sb ? clone(sb) : undefined;
	if (!sb) return clone(sa);
	const base = clone(fitterFirst ? sa : sb);
	const params = { ...(sa.params ?? {}) };
	for (const [k, vb] of Object.entries(sb.params ?? {})) {
		const va = sa.params?.[k];
		params[k] = typeof va === 'number' && typeof vb === 'number' ? (va + vb) / 2 : vb;
	}
	base.params = params;
	if (rng() < 0.5 && sb.notes) base.notes = clone(sb.notes);
	return base;
}

// ── behavioural descriptors (the CV of a MAP-Elites-style map) ─────────────

/** Spectral centroid (Hz), rms, peak, zero-crossing rate, crest factor of a render. */
export function describeBuffer(left, right, sampleRate = 48000) {
	const n = left.length;
	let sum = 0, sumSq = 0, peak = 0, zc = 0, prev = 0;
	for (let i = 0; i < n; i++) {
		const v = (left[i] + (right?.[i] ?? left[i])) / 2;
		const a = Math.abs(v);
		sum += v; sumSq += v * v; if (a > peak) peak = a;
		if (i > 0 && ((prev < 0) !== (v < 0))) zc++;
		prev = v;
	}
	const rms = Math.sqrt(sumSq / n);
	// magnitude spectrum of the MID signal: Hann-windowed 4096-point FFTs, averaged over up to 8
	// windows spread across the whole buffer (so a 4-note phrase is described by more than the
	// attack of its first note). Centroid = magnitude-weighted mean frequency; flatness =
	// geometric / arithmetic mean of the power spectrum (0 = tonal, 1 = white noise).
	const N = 4096;
	const half = N / 2;
	const windows = Math.max(1, Math.min(8, Math.floor(n / N)));
	const starts = Array.from({ length: windows }, (_, k) => windows === 1 ? 0 : Math.floor(k * (n - N) / (windows - 1)));
	const mags = new Float64Array(half);
	const re = new Float64Array(N), im = new Float64Array(N);
	for (const st of starts) {
		for (let i = 0; i < N; i++) {
			const j = st + i;
			const v = j < n ? (left[j] + (right?.[j] ?? left[j])) / 2 : 0;
			re[i] = v * (0.5 - 0.5 * Math.cos((2 * Math.PI * i) / (N - 1)));
			im[i] = 0;
		}
		fft(re, im);
		for (let b = 0; b < half; b++) mags[b] += Math.hypot(re[b], im[b]) / windows;
	}
	let num = 0, den = 0, logSum = 0, powSum = 0;
	for (let b = 1; b < half; b++) {
		const f = (b * sampleRate) / N;
		num += f * mags[b]; den += mags[b];
		const p = mags[b] * mags[b] + 1e-20;
		logSum += Math.log(p); powSum += p;
	}
	const centroid = den > 1e-12 ? num / den : 0;
	const flatness = powSum > 0 ? Math.exp(logSum / (half - 1)) / (powSum / (half - 1)) : 0;
	return {
		spectralCentroid: centroid,
		flatness,
		rms,
		peak,
		zcr: n > 1 ? zc / (n - 1) : 0,
		crest: rms > 1e-9 ? peak / rms : 0,
		finite: Number.isFinite(centroid) && Number.isFinite(flatness) && Number.isFinite(rms) && Number.isFinite(peak),
	};
}

/** In-place iterative radix-2 FFT (length must be a power of two). */
function fft(re, im) {
	const n = re.length;
	for (let i = 1, j = 0; i < n; i++) {
		let bit = n >> 1;
		for (; j & bit; bit >>= 1) j ^= bit;
		j ^= bit;
		if (i < j) { [re[i], re[j]] = [re[j], re[i]]; [im[i], im[j]] = [im[j], im[i]]; }
	}
	for (let len = 2; len <= n; len <<= 1) {
		const ang = (-2 * Math.PI) / len;
		const wr = Math.cos(ang), wi = Math.sin(ang);
		for (let i = 0; i < n; i += len) {
			let cr = 1, ci = 0;
			for (let k = 0; k < len / 2; k++) {
				const ar = re[i + k + len / 2] * cr - im[i + k + len / 2] * ci;
				const ai = re[i + k + len / 2] * ci + im[i + k + len / 2] * cr;
				re[i + k + len / 2] = re[i + k] - ar; im[i + k + len / 2] = im[i + k] - ai;
				re[i + k] += ar; im[i + k] += ai;
				const t = cr * wr - ci * wi; ci = cr * wi + ci * wr; cr = t;
			}
		}
	}
}

/** Map a descriptor vector to a grid cell key. dims = [{key, min, max, cells, log?}] (log needs min > 0). */
export function cellOf(cv, dims) {
	return dims.map((d) => {
		const v = cv[d.key] ?? 0;
		// d.log: log-scaled axis (flatness spans ~8 decades, centroid is perceptually log)
		const t = d.log
			? (Math.log(Math.max(v, d.min)) - Math.log(d.min)) / (Math.log(d.max) - Math.log(d.min))
			: d.max > d.min ? (v - d.min) / (d.max - d.min) : 0;
		return Math.max(0, Math.min(d.cells - 1, Math.floor(t * d.cells)));
	}).join(',');
}
