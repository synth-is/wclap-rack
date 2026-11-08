/**
 * source-device.test.mjs — the optional rack.source (instrument at the head of the rack),
 * on examples/synth-fx-rack (Aether -> WetChorus -> Pearls). Offline: the source renders from
 * notes and its output flows through the effect chain; the render is non-silent and
 * deterministic. (The effects-only cases are in conformance.test.mjs.)
 */
import { test } from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { renderRack, validateRackBundle } from '../reference-host/index.mjs';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const DIR = path.join(HERE, '..', 'examples', 'synth-fx-rack');

function load() {
	const rack = JSON.parse(fs.readFileSync(path.join(DIR, 'rack.json'), 'utf8'));
	const manifest = JSON.parse(fs.readFileSync(path.join(DIR, 'MANIFEST.json'), 'utf8'));
	const bundles = {};
	for (const [key, entry] of Object.entries(manifest)) bundles[key] = new Uint8Array(fs.readFileSync(path.join(DIR, entry.path)));
	return { rack, manifest, bundles };
}

test('synth-fx rack validates and the source device is reported first', async () => {
	const { rack, manifest, bundles } = load();
	const { errors } = await validateRackBundle(rack, manifest, bundles);
	assert.deepEqual(errors, []);
	const r = await renderRack({ rack, bundles, preRoll: 4096 });
	assert.equal(r.devices[0].source, true);
	assert.equal(r.devices[0].plugin, 'org.faust.aether');
	assert.equal(r.devices.length, 3);
});

test('source renders audio from notes (non-silent) and is deterministic', async () => {
	const { rack, bundles } = load();
	const a = await renderRack({ rack, bundles, preRoll: 4096 });
	const b = await renderRack({ rack, bundles, preRoll: 4096 });
	assert.equal(a.fingerprint, b.fingerprint);
	let peak = 0;
	for (const v of a.left) peak = Math.max(peak, Math.abs(v));
	assert.ok(peak > 0.001, `expected audible output, peak=${peak}`);
	assert.equal(a.left.length, 48000);
});

test('without a source, input audio is required', async () => {
	const { rack, bundles } = load();
	await assert.rejects(() => renderRack({ rack: { ...rack, source: undefined }, bundles }), /no rack\.source and no input/);
});
