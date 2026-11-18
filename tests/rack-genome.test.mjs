/**
 * rack-genome.test.mjs — the QD genome layer (tools/rack-genome.mjs): a wclap-rack document plus
 * NEAT-style innovation markings, with mutate/crossover and behavioural descriptors.
 * Pure logic: no wasm, deterministic via the seeded rng.
 */
import { test } from 'node:test';
import assert from 'node:assert/strict';
import {
	GENOME_FORMAT, mulberry32, genomeFromRack, rackFromGenome,
	mutateGenome, crossoverGenomes, describeBuffer, cellOf, randomGenome,
} from '../tools/rack-genome.mjs';
import { validateRackDoc } from '../reference-host/index.mjs';

const seedRack = {
	format: 'wclap-rack',
	version: 1,
	name: 'test rack',
	source: { bundle: 'faust-aether', plugin: 'org.faust.aether', length: 48000, notes: [{ key: 60, velocity: 0.8, frame: 0 }], params: { '0': 0.18 } },
	devices: [
		{ bundle: 'wet-chorus', plugin: 'studio.opendaw.commons.wet-chorus', on: true, params: { '0': 5, '1': 1 } },
		{ bundle: 'pearls', plugin: 'studio.opendaw.pearls', on: true, params: { '914662914': 0.4 } },
	],
};

const makeCtx = () => {
	let innov = 100;
	return {
		nextInnovation: () => innov++,
		pool: [
			{ bundle: 'basics', plugin: 'uk.co.signalsmith.basics.reverb', params: { '3538153984': 0.5 } },
			{ bundle: 'zl-limiter', plugin: 'org.zlcreator.zl-limiter', params: {} },
		],
		paramRange: (b, p, k) => (b === 'wet-chorus' && k === '0' ? [0, 20] : null),
	};
};

test('genomeFromRack adds innovation ids; rackFromGenome round-trips to a valid rack doc', () => {
	const g = genomeFromRack(seedRack, makeCtx());
	assert.equal(g.format, GENOME_FORMAT);
	assert.deepEqual(g.devices.map((d) => d.innovation), [100, 101]);
	const rack = rackFromGenome(g);
	assert.equal(rack.format, 'wclap-rack');
	assert.equal(rack.devices.length, 2);
	assert.ok(!('innovation' in rack.devices[0]));
	assert.deepEqual(validateRackDoc(rack), []);
});

test('mulberry32 is deterministic per seed', () => {
	const a = mulberry32(42); const b = mulberry32(42); const c = mulberry32(43);
	const seqA = [a(), a(), a()]; const seqB = [b(), b(), b()];
	assert.deepEqual(seqA, seqB);
	assert.notDeepEqual(seqA, [c(), c(), c()]);
});

test('mutateGenome keeps the rack doc valid and mutates something, deterministically', () => {
	const run = (seed) => {
		const ctx = makeCtx(); // fresh innovation counter per run, like a fresh run dir
		const rng = mulberry32(seed);
		const g = genomeFromRack(seedRack, ctx);
		for (let i = 0; i < 5; i++) mutateGenome(g, ctx, rng);
		return g;
	};
	const g1 = run(7); const g2 = run(7);
	assert.deepEqual(g1, g2, 'same seed -> same mutations');
	assert.deepEqual(validateRackDoc(rackFromGenome(g1)), []);
	assert.notDeepEqual(g1, genomeFromRack(seedRack, makeCtx()), 'mutations changed the genome');
});

test('mutateGenome clamps params to the known range', () => {
	const ctx = makeCtx();
	const rng = mulberry32(1);
	const g = genomeFromRack(seedRack, ctx);
	// force many param mutations onto the wet-chorus device's param "0" (range [0,20], init 5)
	for (let i = 0; i < 50; i++) {
		const gg = genomeFromRack(seedRack, makeCtx());
		mutateGenome(gg, { ...ctx, pool: [] }, rng, { param: 1 });
		const dev = gg.devices.find((d) => d.bundle === 'wet-chorus');
		if (dev) assert.ok(dev.params['0'] >= 0 && dev.params['0'] <= 20, `param in range, got ${dev.params['0']}`);
	}
});

test('crossoverGenomes aligns by innovation: matching genes blend, disjoint come from the fitter parent', () => {
	const ctx = makeCtx();
	const a = genomeFromRack(seedRack, ctx); // innovations 100 (chorus), 101 (pearls)
	// b shares innovation 100 (same structural origin) but has a disjoint gene 999
	const b = {
		format: GENOME_FORMAT, version: 1,
		source: { ...seedRack.source, params: { '0': 0.9 } },
		devices: [
			{ bundle: 'wet-chorus', plugin: 'studio.opendaw.commons.wet-chorus', on: true, params: { '0': 11, '1': 2 }, innovation: 100 },
			{ bundle: 'basics', plugin: 'uk.co.signalsmith.basics.reverb', on: true, params: { '3538153984': 0.7 }, innovation: 999 },
		],
	};
	const rng = mulberry32(3);
	// A is fitter: child must contain A-only gene 101 and NOT B-only gene 999
	const childA = crossoverGenomes(a, b, rng, { fitnessA: 1, fitnessB: 0 });
	const innovsA = childA.devices.map((d) => d.innovation).sort();
	assert.deepEqual(innovsA, [100, 101]);
	// B is fitter: child must contain B-only gene 999 and NOT A-only gene 101
	const childB = crossoverGenomes(a, b, mulberry32(3), { fitnessA: 0, fitnessB: 1 });
	const innovsB = childB.devices.map((d) => d.innovation).sort();
	assert.deepEqual(innovsB, [100, 999]);
	// matching gene 100: params come from the fitter parent when blending picks them
	assert.ok(childA.devices.find((d) => d.innovation === 100).params['0'] !== undefined);
	// source params blend 50/50
	assert.equal(childA.source.params['0'], (0.18 + 0.9) / 2);
	assert.deepEqual(validateRackDoc(rackFromGenome(childA)), []);
	assert.deepEqual(validateRackDoc(rackFromGenome(childB)), []);
});

test('describeBuffer: a 440 Hz sine has centroid near 440, peak ~amplitude, finite flags set', () => {
	const sr = 48000; const n = 48000;
	const left = new Float32Array(n);
	for (let i = 0; i < n; i++) left[i] = 0.5 * Math.sin(2 * Math.PI * 440 * i / sr);
	const d = describeBuffer(left, left, sr);
	assert.ok(d.finite);
	assert.ok(Math.abs(d.spectralCentroid - 440) < 60, `centroid ${d.spectralCentroid} near 440`);
	assert.ok(Math.abs(d.peak - 0.5) < 0.01);
	assert.ok(d.rms > 0.3 && d.rms < 0.4);
	assert.ok(d.zcr > 0 && d.zcr < 0.1);
	const silence = describeBuffer(new Float32Array(n), new Float32Array(n), sr);
	assert.equal(silence.peak, 0);
});

test('cellOf maps a cv vector into the grid, clamped', () => {
	const dims = [
		{ key: 'spectralCentroid', min: 0, max: 8000, cells: 8 },
		{ key: 'zcr', min: 0, max: 0.2, cells: 4 },
	];
	assert.equal(cellOf({ spectralCentroid: 0, zcr: 0 }, dims), '0,0');
	assert.equal(cellOf({ spectralCentroid: 4000, zcr: 0.1 }, dims), '4,2');
	assert.equal(cellOf({ spectralCentroid: 99999, zcr: 99 }, dims), '7,3', 'clamped to last cell');
});

test('randomGenome: keeps the source, draws 1..max pool devices with params inside known ranges, deterministically', () => {
	const ctx = makeCtx();
	ctx.pool.push({ bundle: 'wet-chorus', plugin: 'studio.opendaw.commons.wet-chorus', params: { '0': 5, '1': 1 } });
	const base = genomeFromRack(seedRack, makeCtx());
	for (let s = 1; s <= 20; s++) {
		const g = randomGenome(base, ctx, mulberry32(s), { minDevices: 1, maxDevices: 4 });
		assert.ok(g.devices.length >= 1 && g.devices.length <= 4);
		assert.deepEqual(g.source, base.source);
		for (const d of g.devices) {
			assert.ok(ctx.pool.some((p) => p.plugin === d.plugin));
			if (d.bundle === 'wet-chorus') {
				assert.ok(d.params['0'] >= 0 && d.params['0'] <= 20, 'ranged param inside [0, 20]');
				assert.equal(d.params['1'], 1, 'unranged param keeps the pool default');
			}
		}
		assert.equal(validateRackDoc(rackFromGenome(g)).length, 0);
	}
	const a = randomGenome(base, makeCtx(), mulberry32(7));
	const b = randomGenome(base, makeCtx(), mulberry32(7));
	assert.deepEqual(a, b);
});

test('randomGenome: an empty pool is an error, not an empty rack', () => {
	const ctx = makeCtx(); ctx.pool = [];
	assert.throws(() => randomGenome(genomeFromRack(seedRack, makeCtx()), ctx, mulberry32(1)), /empty pool/);
});
