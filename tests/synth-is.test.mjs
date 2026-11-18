/**
 * synth-is.test.mjs — the two Synth.is sound renditions (from ~/Downloads zips) as rack bundles:
 *   examples/synth-is-sfz    CPPN+DSP sound 01KR0R5T4S7SQF4W8M1G99CGS9 (instrument.sfz + samples,
 *                            trimmed to keys C3/C4/C5 for size; the full 111-region rendition was
 *                            verified locally) played by engines/sfz-engine
 *   examples/synth-is-faust  Faust sound 01M6ZRAYNZG2KXPCJBWFGRZ8KF (instrument.wasm + instrument.json
 *                            straight from the Synth.is takeout) played by engines/faust-runner
 * Both validate, deliver source.files, render non-silent and deterministic.
 */
import { test } from 'node:test';
import assert from 'node:assert/strict';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { renderRack, validateRackBundle } from '../reference-host/index.mjs';
import { loadRackBundleDir } from '../tools/rack-bundle.mjs';

const HERE = path.dirname(fileURLToPath(import.meta.url));

function peak(chans) {
	let p = 0;
	for (const c of chans) for (const v of c) p = Math.max(p, Math.abs(v));
	return p;
}

test('synth-is-sfz: validates, 9 regions load, renders non-silent and deterministic', async () => {
	const dir = path.join(HERE, '..', 'examples', 'synth-is-sfz');
	const { rack, manifest, bundles, files } = loadRackBundleDir(dir);
	const { errors } = await validateRackBundle(rack, manifest, bundles);
	assert.deepEqual(errors, []);
	assert.equal(rack.source.kind, 'sampler');
	assert.equal(rack.source.files.length, 10); // sfz + 9 samples
	const a = await renderRack({ rack, bundles, files, preRoll: 4096 });
	const b = await renderRack({ rack, bundles, files, preRoll: 4096 });
	assert.equal(a.devices[0].source, true);
	assert.equal(a.devices[0].plugin, 'is.synth.kromosynth.sfz-engine');
	assert.equal(a.fingerprint, b.fingerprint);
	assert.ok(peak([a.left, a.right]) > 0.001, `audible, peak=${peak([a.left, a.right])}`);
	const stdout = a.hosts.get('sfz-engine').stdout;
	assert.match(stdout, /sfz-engine: loaded sfz=\/instruments\/instrument\.sfz regions=9 samples=9/);
});

test('synth-is-faust: validates, genome params exposed, renders non-silent and deterministic', async () => {
	const dir = path.join(HERE, '..', 'examples', 'synth-is-faust');
	const { rack, manifest, bundles, files } = loadRackBundleDir(dir);
	const { errors } = await validateRackBundle(rack, manifest, bundles);
	assert.deepEqual(errors, []);
	assert.equal(rack.source.kind, 'synth');
	const a = await renderRack({ rack, bundles, files, preRoll: 4096 });
	const b = await renderRack({ rack, bundles, files, preRoll: 4096 });
	assert.equal(a.devices[0].source, true);
	assert.equal(a.devices[0].plugin, 'is.synth.kromosynth.faust-runner');
	assert.equal(a.fingerprint, b.fingerprint);
	assert.ok(peak([a.left, a.right]) > 0.001, `audible, peak=${peak([a.left, a.right])}`);
	const stdout = a.hosts.get('faust-runner').stdout;
	assert.match(stdout, /faust-runner: loaded json=\/genome\/instrument\.json wasm=\/genome\/instrument\.wasm params=6/);
});
