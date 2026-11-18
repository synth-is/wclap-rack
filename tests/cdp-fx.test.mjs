/**
 * cdp-fx.test.mjs — the cdp-fx WCLAP module (engines/cdp-fx): a bit-transparent stereo
 * passthrough audio-fx whose params select an offline CDP effect (catalog index + P1..P12).
 * The effect itself is applied host-side by the wclap.cdp/1 bridge (see cdp-engine.test.mjs
 * for the fxChain tests); here we pin the module contract: passthrough bit-exactness,
 * param layout, state round-trip.
 */
import { test } from 'node:test';
import assert from 'node:assert/strict';
import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';
import { WclapHost, loadBundleBytesAsync } from '../reference-host/index.mjs';

const HERE = path.dirname(fileURLToPath(import.meta.url));
const BUNDLE = path.join(HERE, '..', 'engines', 'cdp-fx', 'cdp-fx.wclap.tar.gz');
const PLUGIN_ID = 'is.synth.kromosynth.cdp-fx';

async function loadFx() {
	const { wasm } = await loadBundleBytesAsync(new Uint8Array(fs.readFileSync(BUNDLE)));
	const host = await WclapHost.load(wasm, {});
	const plugin = host.createPlugin(PLUGIN_ID);
	assert.ok(plugin, 'factory must create is.synth.kromosynth.cdp-fx');
	return { host, plugin };
}

test('cdp-fx: stereo passthrough is bit-exact, params are Effect + P1..P12, state round-trips', async () => {
	const { plugin } = await loadFx();
	plugin.activate({ sampleRate: 48000, blockSize: 512 });
	assert.equal(plugin.params.length, 13, 'param 0 Effect + P1..P12 (1..12)');
	assert.equal(plugin.params[0].name, 'Effect');
	assert.equal(plugin.getParam(0), 0);
	assert.equal(plugin.getParam(1), -1e9, 'P params default to the sentinel');
	assert.equal(plugin.paramText(5, -1e9), 'default');

	const n = 5000;
	const s = new Float32Array(n);
	for (let i = 0; i < n; i++) s[i] = Math.fround(Math.sin(i * 0.01) * 0.8);
	const out = plugin.render(n, [], { input: [s, s] });
	assert.deepEqual([...out[0]], [...s], 'left bit-exact');
	assert.deepEqual([...out[1]], [...s], 'right bit-exact');

	plugin.render(1, [
		{ frame: 0, type: 'param', id: 0, value: 42 },
		{ frame: 0, type: 'param', id: 3, value: 0.5 },
	]);
	const saved = plugin.saveState();
	plugin.destroy();
	const { plugin: p2 } = await loadFx();
	p2.activate({ sampleRate: 48000, blockSize: 512 });
	p2.loadState(saved);
	assert.equal(p2.getParam(0), 42);
	assert.equal(p2.getParam(3), 0.5);
	p2.destroy();
});

test('cdp-fx: webview editor protocol — hello/set/poll answer, param set from the page surfaces as an out-event', async () => {
	const { host, plugin } = await loadFx();
	plugin.activate({ sampleRate: 48000, blockSize: 512 });
	const gui = plugin.guiOpenWebview();
	assert.ok(gui && gui.uri === 'ui/index.html', `webview gui opens: ${JSON.stringify(gui)}`);
	const enc = (type, payload = new Uint8Array(0)) => {
		const f = new Uint8Array(4 + payload.length);
		f[0] = type;
		f.set(payload, 4);
		return f;
	};
	assert.equal(plugin.guiReceive(enc(64)), true, 'hello accepted');
	const p = new Uint8Array(12);
	new DataView(p.buffer).setUint32(0, 0, true);
	new DataView(p.buffer).setFloat64(4, 7, true);
	assert.equal(plugin.guiReceive(enc(65, p)), true, 'set param accepted');
	assert.equal(plugin.guiReceive(enc(69)), true, 'poll accepted');
	const s = new Float32Array(512).fill(0.1);
	plugin.render(512, [], { input: [s, s] });
	assert.equal(plugin.getParam(0), 7);
	const out = (host.outputEvents ?? []).filter((e) => e.type === 5);
	assert.deepEqual(out[0], { type: 5, paramId: 0, value: 7 }, 'paramOut out-event for the host (drives the bridge fxChain)');
	// switching the effect resets P1..P12 to the sentinel and reports it (like cdp-engine's program switch)
	const resets = out.filter((e) => e.paramId >= 1);
	assert.equal(resets.length, 12);
	assert.ok(resets.every((e) => e.value === -1e9));
	for (let id = 1; id < 13; id++) assert.equal(plugin.getParam(id), -1e9);
	plugin.guiCloseWebview();
	plugin.destroy();
});
