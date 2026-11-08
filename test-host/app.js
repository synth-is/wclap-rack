// app.js — the wclap-rack test host / assembler.
//
// Two modes:
//  - load an existing rack bundle (?bundle=<url>, file picker: .wclap-rack.tar.gz or a bundle directory)
//  - assemble one: "Add bundles…" loads .wclap.tar.gz archives into the catalog; each plugin is a card
//    with a kind badge — instruments can be "set as source", audio effects "add to rack". Device panels
//    get bypass, reorder (up/down), remove and per-parameter sliders. "Download rack.json" /
//    "Download .wclap-rack.tar.gz" export the assembled rack (the archive includes a generated
//    MANIFEST.json with sha256 of every module.wasm plus the bundle archives).
//
// UI patterns borrowed from open-webclap's site (plugin cards with kind + licence) and openDAW's
// DeviceCatalog/DeviceSlot (kind-tagged entries applied at a slot).
import { untarArchiveAsync, loadBundleBytesAsync, validateRackBundle, describeBundle } from '../reference-host/index.mjs';
import { download, packRackBundle, buildRackBundle } from './pack.js';

const $ = (sel) => document.querySelector(sel);
window.__errors = [];
window.addEventListener('error', (e) => window.__errors.push(String(e.message)));
window.addEventListener('unhandledrejection', (e) => window.__errors.push('rejection: ' + String(e.reason)));

const kindOf = (features) =>
	features.includes('instrument') ? 'instrument'
		: features.includes('note-effect') ? 'note-fx'
			: features.includes('audio-effect') ? 'audio-fx' : 'other';
const KIND_LABEL = { instrument: 'Instrument', 'audio-fx': 'Audio effect', 'note-fx': 'Note effect', other: 'Other' };

/** The assembler model. bundles: Map key -> { bytes, plugins }. rack: the rack.json document. */
const state = {
	bundles: new Map(),
	rack: { format: 'wclap-rack', version: 1, name: 'untitled rack', devices: [] },
	ctx: null,
	node: null,
	autoPlay: null,
};

// ---------------------------------------------------------------------------
// loading an existing rack bundle
// ---------------------------------------------------------------------------

const td = new TextDecoder();
const getFile = (files, n) => files[n] ?? files[Object.keys(files).find((k) => k.endsWith('/' + n))];

function rackFromFiles(files) {
	const rack = JSON.parse(td.decode(getFile(files, 'rack.json')));
	const manifest = JSON.parse(td.decode(getFile(files, 'MANIFEST.json')));
	const bundles = new Map();
	for (const [key, entry] of Object.entries(manifest)) bundles.set(key, { bytes: getFile(files, entry.path), plugins: null });
	return { rack, bundles };
}

async function loadRackArchive(file) {
	const files = await untarArchiveAsync(new Uint8Array(await file.arrayBuffer()));
	return rackFromFiles(files);
}

async function loadRackDir(fileList) {
	const files = {};
	for (const f of fileList) files[f.webkitRelativePath.split('/').slice(1).join('/')] = new Uint8Array(await f.arrayBuffer());
	return rackFromFiles(files);
}

// ---------------------------------------------------------------------------
// catalog: adding bundles
// ---------------------------------------------------------------------------

async function addBundleFiles(fileList) {
	for (const f of fileList) {
		const key = f.name.replace(/\.wclap\.tar\.gz$/, '').replace(/\.wclap$/, '');
		const bytes = new Uint8Array(await f.arrayBuffer());
		const { plugins } = await describeBundle(bytes);
		state.bundles.set(key, { bytes, plugins });
	}
	renderCatalog();
}

function renderCatalog() {
	const host = $('#catalog');
	host.innerHTML = '';
	for (const [key, { plugins }] of state.bundles) {
		for (const p of plugins ?? []) {
			const kind = kindOf(p.features ?? []);
			const card = document.createElement('div');
			card.className = 'card';
			const head = document.createElement('div');
			head.className = 'card-head';
			const badge = document.createElement('span');
			badge.className = `kind ${kind}`;
			badge.textContent = KIND_LABEL[kind];
			head.append(document.createTextNode(`${p.name} `), badge, document.createTextNode(` (${key}, ${p.params.length} params)`));
			card.append(head);
			const actions = document.createElement('div');
			actions.className = 'card-actions';
			if (kind === 'instrument') {
				const b = document.createElement('button');
				b.textContent = state.rack.source?.plugin === p.id ? 'source ✓' : 'set as source';
				b.onclick = () => { state.rack.source = { bundle: key, plugin: p.id, length: 48000, notes: [{ key: 60, velocity: 0.8, frames: 48000 }], params: {} }; state.autoPlay = state.rack.source.notes[0]; rebuild(); };
				actions.append(b);
			} else if (kind === 'audio-fx') {
				const b = document.createElement('button');
				b.textContent = '+ add to rack';
				b.onclick = () => { state.rack.devices.push({ bundle: key, plugin: p.id, on: true, params: {} }); rebuild(); };
				actions.append(b);
			} else {
				const s = document.createElement('span');
				s.className = 'muted';
				s.textContent = 'note effects need a MIDI path — not in v1 rack chains';
				s.title = 'v1 racks are a serial audio chain; note effects are out of scope';
				actions.append(s);
			}
			card.append(actions);
			host.append(card);
		}
	}
	if (state.bundles.size === 0) host.innerHTML = '<p class="muted">Add .wclap.tar.gz bundles to populate the catalog, or load a rack bundle.</p>';
}

// ---------------------------------------------------------------------------
// audio: (re)build the worklet from the model
// ---------------------------------------------------------------------------

async function rebuild() {
	const { rack, bundles } = state;
	// for the in-memory model, build the MANIFEST (sha256 of each module.wasm) on the fly
	const { sha256Hex } = await import('../reference-host/index.mjs');
	const manifest = {};
	const bundlesAB = {};
	for (const [k, v] of bundles) {
		const { wasm } = await loadBundleBytesAsync(v.bytes);
		manifest[k] = { path: k, sha256: await sha256Hex(wasm) };
		bundlesAB[k] = wasm.buffer.slice(wasm.byteOffset, wasm.byteOffset + wasm.byteLength);
	}
	const v = await validateRackBundle(rack, manifest, Object.fromEntries([...bundles].map(([k, vv]) => [k, vv.bytes])));
	if (v.errors.length) { $('#status').textContent = 'INVALID: ' + v.errors.join('; '); return; }

	const ctx = (state.ctx ??= new AudioContext());
	document.addEventListener('click', () => ctx.resume().catch(() => {}), { once: true });
	await ctx.audioWorklet.addModule(new URL('./rack-worklet.js', import.meta.url), { type: 'module' });
	state.node?.disconnect();
	const node = new AudioWorkletNode(ctx, 'wclap-rack', { numberOfInputs: 1, numberOfOutputs: 1, outputChannelCount: [2] });
	node.connect(ctx.destination);
	let autoPlay = state.autoPlay;
	node.port.onmessage = (e) => {
		if (e.data.type === 'ready') {
			window.__latency = e.data.latency;
			renderPanels(e.data);
			if (autoPlay) { autoPlay = null; setTimeout(() => node.port.postMessage({ type: 'note', on: true, key: state.rack.source?.notes?.[0]?.key ?? 60, velocity: 0.8 }), 100); }
		}
		if (e.data.type === 'error') { $('#status').textContent = 'worklet error: ' + e.data.message; window.__errors.push('worklet: ' + e.data.message); }
		if (e.data.type === 'latencyUpdate') {
			$('#status').textContent = $('#status').textContent.replace(/summed latency \S+ frames/, `summed latency ${e.data.latencyTotal ?? window.__latency} frames`);
		}
	};
	state.node = node;
	node.port.postMessage({ type: 'load', rack, bundles: bundlesAB });

	if (rack.source) {
		// source racks play their own notes; no saw
	} else {
		const osc = ctx.createOscillator();
		osc.type = 'sawtooth';
		osc.frequency.value = 110;
		const gain = ctx.createGain();
		gain.gain.value = 0.5;
		osc.connect(gain).connect(node);
		osc.start();
		window.__src = { osc, gain };
	}
	ctx.resume().catch(() => {});
	$('#status').textContent = `"${rack.name}" — ${rack.devices.length} device(s)${rack.source ? ' + source' : ''}, loading…`;
	window.__node = node;
	renderCatalog();
}

// ---------------------------------------------------------------------------
// panels
// ---------------------------------------------------------------------------

function paramRow(target, p, onInput) {
	const row = document.createElement('label');
	row.className = 'param';
	const slider = document.createElement('input');
	slider.type = 'range';
	slider.min = p.min; slider.max = p.max; slider.step = p.stepped ? 1 : (p.max - p.min) / 200; slider.value = p.def;
	const val = document.createElement('span');
	val.textContent = p.def;
	slider.oninput = () => { val.textContent = slider.value; onInput(Number(slider.value)); };
	row.append(document.createTextNode(p.name), slider, val);
	return row;
}

function renderPanels(ready) {
	const { rack } = state;
	$('#status').textContent = `playing "${rack.name}" — ${rack.devices.length} device(s)${ready.source ? ' + source' : ''}, summed latency ${ready.latency} frames`;
	const host = $('#devices');
	host.innerHTML = '';
	const node = state.node;

	if (ready.source) {
		const div = document.createElement('div');
		div.className = 'device source';
		const head = document.createElement('div');
		head.className = 'device-head';
		head.append(document.createTextNode(`[source] ${ready.source.plugin} — notes: `));
		for (const key of [48, 52, 55, 60, 64]) {
			const b = document.createElement('button');
			b.textContent = String(key);
			b.style.margin = '0 2px';
			b.onmousedown = () => node.port.postMessage({ type: 'note', on: true, key, velocity: 0.8 });
			b.onmouseup = () => node.port.postMessage({ type: 'note', on: false, key });
			head.append(b);
		}
		const rm = document.createElement('button');
		rm.textContent = '✕ remove source';
		rm.onclick = () => { delete state.rack.source; state.autoPlay = null; rebuild(); };
		head.append(rm);
		div.append(head);
		for (const p of ready.source.params.slice(0, 12)) {
			div.append(paramRow(null, p, (v) => {
				(state.rack.source.params ??= {})[String(p.id)] = v;
				node.port.postMessage({ type: 'param', index: -1, id: p.id, value: v });
			}));
		}
		host.append(div);
	}

	ready.devices.forEach((d, i) => {
		const div = document.createElement('div');
		div.className = 'device';
		const head = document.createElement('div');
		head.className = 'device-head';
		const bypass = document.createElement('input');
		bypass.type = 'checkbox';
		bypass.checked = rack.devices[i].on !== false;
		bypass.title = 'bypass';
		bypass.onchange = () => { rack.devices[i].on = bypass.checked; node.port.postMessage({ type: 'setOn', index: i, on: bypass.checked }); };
		const up = document.createElement('button'); up.textContent = '↑'; up.title = 'move up';
		up.onclick = () => { if (i > 0) { [rack.devices[i - 1], rack.devices[i]] = [rack.devices[i], rack.devices[i - 1]]; rebuild(); } };
		const down = document.createElement('button'); down.textContent = '↓'; down.title = 'move down';
		down.onclick = () => { if (i < rack.devices.length - 1) { [rack.devices[i], rack.devices[i + 1]] = [rack.devices[i + 1], rack.devices[i]]; rebuild(); } };
		const rm = document.createElement('button'); rm.textContent = '✕'; rm.title = 'remove from rack';
		rm.onclick = () => { rack.devices.splice(i, 1); rebuild(); };
		head.append(bypass, document.createTextNode(` [${i}] ${d.plugin} `), up, down, rm);
		div.append(head);
		for (const p of d.params ?? []) {
			div.append(paramRow(null, p, (v) => {
				(rack.devices[i].params ??= {})[String(p.id)] = v;
				node.port.postMessage({ type: 'param', index: i, id: p.id, value: v });
			}));
		}
		host.append(div);
	});
}

// ---------------------------------------------------------------------------
// toolbar
// ---------------------------------------------------------------------------

async function initialLoad() {
	const fileInput = $('#bundle');
	const dirInput = $('#bundleDir');
	let loaded = null;
	if (dirInput.files?.length) loaded = await loadRackDir(dirInput.files);
	else if (fileInput.files?.length) loaded = await loadRackArchive(fileInput.files[0]);
	else {
		const url = new URLSearchParams(location.search).get('bundle');
		if (url) loaded = await loadRackArchive(new File([await (await fetch(url)).blob()], 'bundle.wclap-rack.tar.gz'));
	}
	if (loaded) {
		state.rack = loaded.rack;
		state.bundles = loaded.bundles;
		if (state.rack.source) state.autoPlay = state.rack.source.notes?.[0] ?? null;
		$('#rackName').value = state.rack.name ?? 'untitled rack';
	}
	await rebuild();
}

$('#load').onclick = initialLoad;
$('#addBundles').onchange = (e) => addBundleFiles(e.target.files);
$('#rackName').oninput = (e) => { state.rack.name = e.target.value; };
$('#exportJson').onclick = () => {
	state.rack.name = $('#rackName').value;
	download(`${state.rack.name || 'rack'}.rack.json`, new TextEncoder().encode(JSON.stringify(state.rack, null, 2) + '\n'), 'application/json');
};
$('#exportPack').onclick = async () => {
	state.rack.name = $('#rackName').value;
	$('#status').textContent = 'packing…';
	const bundles = Object.fromEntries([...state.bundles].map(([k, v]) => [k, v.bytes]));
	const { bytes } = await packRackBundle(state.rack.name || 'rack', state.rack, bundles);
	$('#status').textContent = `packed ${bytes} bytes — downloaded`;
};

// debug/test hook: drive the assembler programmatically (used by Playwright checks)
window.__assemble = {
	state,
	addBundleBytes: async (key, bytes) => {
		const { plugins } = await describeBundle(bytes);
		state.bundles.set(key, { bytes, plugins });
		renderCatalog();
	},
	addDevice: (bundle, plugin) => { state.rack.devices.push({ bundle, plugin, on: true, params: {} }); return rebuild(); },
	setSource: (bundle, plugin) => { state.rack.source = { bundle, plugin, length: 48000, notes: [{ key: 60, velocity: 0.8, frames: 48000 }], params: {} }; state.autoPlay = state.rack.source.notes[0]; return rebuild(); },
	buildPack: () => buildRackBundle(state.rack, Object.fromEntries([...state.bundles].map(([k, v]) => [k, v.bytes]))),
};

if (new URLSearchParams(location.search).has('bundle')) initialLoad();
else renderCatalog();
