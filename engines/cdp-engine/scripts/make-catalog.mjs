#!/usr/bin/env node
// make-catalog.mjs — generate engines/cdp-engine/catalog.json from the vendored cdp-wasm
// typed catalog (third_party/cdp-wasm/src): the list of CDP generator programs playable
// by the cdp-engine WCLAP instrument and the curated effects usable as offline rack
// stages, with each entry's parameter schema (name, label, min/max/default/choices).
//
// The shim maps its positional numeric params P1..P12 onto entry.params by index; the
// host bridge (reference-host/cdp-bridge.mjs) maps the same positions onto the catalog
// entry's params by name. Keep both in sync with this file's output.
//
// Usage: node engines/cdp-engine/scripts/make-catalog.mjs
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath, pathToFileURL } from 'node:url';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const PKG = path.join(HERE, '..', 'third_party', 'cdp-wasm');
const OUT = path.join(HERE, '..', 'catalog.json');

const mod = await import(pathToFileURL(path.join(PKG, 'src', 'index.js')).href);
const { GENERATORS, EFFECTS } = mod;

const slim = (p) => ({
	name: p.name,
	label: p.label,
	...(p.choices ? { choices: p.choices } : { min: p.min, max: p.max, default: p.default, ...(p.step ? { step: p.step } : {}), ...(p.env ? { env: true } : {}) }),
	...(p.default !== undefined && !p.choices ? {} : {}),
});
// Effects that cannot work as a single-input offline stage (verified by sweep against a
// rendered generator, 2026-11-12): the multimix/panorama "mixfile" processes need several
// input files; rejoin needs a segment file. They stay in the catalog but are flagged so
// UIs can grey them out. Input-dependent effects (envspeak/grain on a pure sine, …) are NOT
// flagged — they work on suitable input, and the bridge retries out-of-range params.
const UNSUPPORTED = {
	'multimix.spread': 'needs multiple input files (mixfile), not a single-input stage',
	'multimix.channels': 'needs multiple input files (mixfile), not a single-input stage',
	'panorama.spatial': 'needs multiple input files (mixfile), not a single-input stage',
	'rejoin.rejoin': 'needs a segment file, not plain audio',
};

const entry = (e) => ({
	id: e.id,
	label: e.label,
	category: e.category,
	program: e.program,
	...(e.domain ? { domain: e.domain } : {}),
	...(e.mode ? { mode: e.mode } : {}),
	...(UNSUPPORTED[e.id] ? { unsupported: UNSUPPORTED[e.id] } : {}),
	params: (e.params || []).map(slim),
	...(e.data ? { data: e.data } : {}),
});

const catalog = {
	format: 'wclap-cdp-catalog',
	version: 1,
	source: 'cdp-wasm@' + JSON.parse(fs.readFileSync(path.join(PKG, 'package.json'), 'utf8')).version,
	generators: GENERATORS.map(entry),
	effects: EFFECTS.map(entry),
};
fs.writeFileSync(OUT, JSON.stringify(catalog, null, 1) + '\n');
console.log(`catalog.json: ${catalog.generators.length} generators, ${catalog.effects.length} effects -> ${OUT} (${fs.statSync(OUT).size} bytes)`);
