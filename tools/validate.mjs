#!/usr/bin/env node
// validate.mjs — validate a rack bundle (rack.json structure, MANIFEST, bundle sha256s).
//   node tools/validate.mjs --bundle <dir | .wclap-rack.tar.gz>
// Prints a summary; exits non-zero on any error.
import { loadRackBundle } from './rack-bundle.mjs';
import { validateRackBundle } from '../reference-host/index.mjs';

const args = process.argv.slice(2);
const bundle = args[args.indexOf('--bundle') + 1];
if (!bundle) { console.error('usage: validate.mjs --bundle <dir | .wclap-rack.tar.gz>'); process.exit(2); }

try {
	const { rack, manifest, bundles } = loadRackBundle(bundle);
	const { errors } = await validateRackBundle(rack, manifest, bundles);
	console.log(`rack "${rack.name ?? '(unnamed)'}" v${rack.version}: ${rack.devices.length} device(s), ${Object.keys(manifest).length} bundle(s)`);
	rack.devices.forEach((d, i) => console.log(`  [${i}] ${d.bundle} / ${d.plugin} on=${d.on !== false} params=${JSON.stringify(d.params ?? {})}`));
	if (errors.length) {
		for (const e of errors) console.error(`ERROR: ${e}`);
		process.exit(1);
	}
	console.log('valid: schema ok, all bundle sha256 match');
} catch (e) {
	console.error(`validate: ${e.message}`);
	process.exit(1);
}
