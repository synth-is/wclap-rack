#!/usr/bin/env node
// render.mjs — the offline conformance renderer: render a rack bundle over a reference input,
// print per-device info and the fingerprint, check determinism (two renders, bit-identical).
//   node tools/render.mjs --bundle <dir | .wclap-rack.tar.gz> [--seconds 1] [--out out.wav] [--json]
// Exits non-zero when the rack is invalid or the two renders differ.
import path from 'node:path';
import { loadRackBundle, writeWav } from './rack-bundle.mjs';
import { validateRackBundle, renderRack, fingerprint } from '../reference-host/index.mjs';

const args = process.argv.slice(2);
const opt = (name, dflt) => { const i = args.indexOf(`--${name}`); return i >= 0 ? args[i + 1] : dflt; };
const bundle = opt('bundle');
if (!bundle) { console.error('usage: render.mjs --bundle <dir | .wclap-rack.tar.gz> [--seconds 1] [--out out.wav] [--json]'); process.exit(2); }

// reference input: 1 s of impulse + 110 Hz saw + noise, peak 0.8 (deterministic)
const seconds = Number(opt('seconds', 1));
const sampleRate = 48000;
const n = Math.round(seconds * sampleRate);
const left = new Float32Array(n);
for (let i = 0; i < n; i++) {
	const saw = 2 * ((i * 110 / sampleRate) % 1) - 1;
	const noise = ((i * 2654435761 >>> 0) % 4294967296) / 4294967296 * 2 - 1; // deterministic pseudo-noise
	left[i] = Math.fround((saw * 0.5 + noise * 0.3) * 0.8);
	if (i === 0) left[i] = 1;
}

try {
	const { rack, manifest, bundles } = loadRackBundle(bundle);
	const { errors } = await validateRackBundle(rack, manifest, bundles);
	if (errors.length) { for (const e of errors) console.error(`ERROR: ${e}`); process.exit(1); }

	const input = { left, right: left };
	const r1 = await renderRack({ rack, bundles, input, sampleRate });
	const r2 = await renderRack({ rack, bundles, input, sampleRate });
	const deterministic = r1.fingerprint === r2.fingerprint && fingerprint([r1.left, r1.right]) === fingerprint([r2.left, r2.right]);

	if (args.includes('--json')) {
		console.log(JSON.stringify({ rack: rack.name, devices: r1.devices, latency: r1.latency, tail: r1.tail, fingerprint: r1.fingerprint, deterministic }));
	} else {
		console.log(`rack "${rack.name ?? '(unnamed)'}" rendered ${n} frames @ ${sampleRate} Hz`);
		r1.devices.forEach((d, i) => console.log(`  [${i}] ${d.plugin} on=${d.on} latency=${d.latency}`));
		console.log(`  total latency=${r1.latency} tail=${r1.tail} fingerprint=${r1.fingerprint} deterministic=${deterministic}`);
	}
	const out = opt('out');
	if (out) { writeWav(path.resolve(out), r1.left, r1.right, sampleRate); console.log(`wrote ${out}`); }
	process.exit(deterministic ? 0 : 1);
} catch (e) {
	console.error(`render: ${e.message}`);
	process.exit(1);
}
