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
import { adjustGenomeWeights } from './geometric-map.js';

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
	files: null,  // the loaded rack bundle's raw file map { archivePath: Uint8Array } (SPEC §8 source.files)
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
	return { rack, bundles, files };
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
	const { rack, bundles, files } = state;
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
	// SPEC §8: the rack bundle's non-bundle files, for source.files delivery (wclap.files/1)
	const filesAB = {};
	for (const f of rack.source?.files ?? []) {
		const bytes = files?.[f.archive] ?? files?.[Object.keys(files ?? {}).find((k) => k.endsWith('/' + f.archive))];
		if (!bytes) { $('#status').textContent = `INVALID: source.files archive "${f.archive}" not in the bundle`; return; }
		filesAB[f.archive] = bytes.buffer.slice(bytes.byteOffset, bytes.byteOffset + bytes.byteLength);
	}
	node.port.postMessage({ type: 'load', rack, bundles: bundlesAB, files: filesAB });

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
		for (const key of [48, 52, 55, 60, 64, 69]) {
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
		const hint = document.createElement('span');
		hint.className = 'muted';
		hint.textContent = ' — note buttons trigger a key (momentary); the Pitch slider / pitch×duration pad transpose every note live, incl. held ones; Duration sets each note\'s render length';
		head.append(hint);
		div.append(head);
		window.__sliders = {};
		for (const p of ready.source.params.slice(0, 12)) {
			if (p.hidden) continue;  // e.g. CPPN X/Y on non-hybrid-phase genomes
			const row = paramRow(null, p, (v) => {
				(state.rack.source.params ??= {})[String(p.id)] = v;
				node.port.postMessage({ type: 'param', index: -1, id: p.id, value: v });
			});
			const slider = row.querySelector('input[type=range]');
			const current = state.rack.source.params?.[String(p.id)];
			if (current !== undefined) { slider.value = current; row.querySelector('span').textContent = current; }
			window.__sliders[p.id] = slider;
			div.append(row);
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
			if (p.hidden) continue;
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
		state.files = loaded.files;
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
	const { bytes } = await packRackBundle(state.rack.name || 'rack', state.rack, bundles, state.files ?? {});
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
	buildPack: () => buildRackBundle(state.rack, Object.fromEntries([...state.bundles].map(([k, v]) => [k, v.bytes])), state.files ?? {}),
};

// ---------------------------------------------------------------------------
// Synth.is CPPN+DSP sound loader: point at a sound id, fetch its raw genome from
// api.synth.is (via the dev server's /proxy/synthis/ — the API 500s on browser
// preflights from unknown origins), and assemble a rack around engines/cppn-engine
// in memory, then play it. ?sound=<id> loads directly.
// ---------------------------------------------------------------------------

const CPPN_ENGINE_BUNDLE_URL = '/engines/cppn-engine/cppn-engine.wclap.tar.gz';
const WET_CHORUS_BUNDLE_URL = '/examples/synth-is-cppn/wet-chorus.wclap.tar.gz';
const CPPN_PLUGIN_ID = 'is.synth.kromosynth.cppn-engine';

async function loadSynthIsSound(id) {
	id = (id ?? '').trim();
	if (!id) return;
	$('#status').textContent = `fetching sound ${id} from api.synth.is…`;
	const metaRes = await fetch(`/proxy/synthis/api/sounds/${encodeURIComponent(id)}`);
	if (!metaRes.ok) { $('#status').textContent = `sound ${id}: HTTP ${metaRes.status}`; return; }
	const meta = (await metaRes.json()).data?.sound ?? {};
	if (meta.substrate && meta.substrate !== 'cppn-dsp') {
		$('#status').textContent = `sound ${id} is substrate "${meta.substrate}", not cppn-dsp — this page plays CPPN+DSP genomes (faust sounds go through engines/faust-runner instead).`;
		return;
	}
	const genomeRes = await fetch(`/proxy/synthis/api/exploration/genome/${encodeURIComponent(id)}?format=raw`);
	if (!genomeRes.ok) { $('#status').textContent = `genome ${id}: HTTP ${genomeRes.status}`; return; }
	const genomeBytes = new Uint8Array(await genomeRes.arrayBuffer());
	const [engineBytes, chorusBytes] = await Promise.all([
		new Uint8Array(await (await fetch(CPPN_ENGINE_BUNDLE_URL)).arrayBuffer()),
		new Uint8Array(await (await fetch(WET_CHORUS_BUNDLE_URL)).arrayBuffer()),
	]);
	$('#status').textContent = `sound ${id}: genome ${(genomeBytes.length / 1024).toFixed(1)} KB — assembling rack…`;
	state.bundles.set('cppn-engine', { bytes: engineBytes, plugins: (await describeBundle(engineBytes)).plugins });
	state.bundles.set('wet-chorus', { bytes: chorusBytes, plugins: (await describeBundle(chorusBytes)).plugins });
	const duration = typeof meta.duration === 'number' ? meta.duration : 2;
	state.rack = {
		format: 'wclap-rack',
		version: 1,
		name: `Synth.is CPPN+DSP ${id} (live cppn-engine) -> WetChorus`,
		source: {
			bundle: 'cppn-engine',
			plugin: CPPN_PLUGIN_ID,
			kind: 'synth',
			length: 48000,
			files: [{ path: `/genomes/${id}.json`, archive: 'files/genome.json' }],
			params: { '1': duration },
			// first note = key 69 (440 Hz, noteDelta 0) at velocity 1, matching the synth.is preview render
			notes: [
				{ key: 69, velocity: 1, frame: 0, frames: 12000 },
				{ key: 72, velocity: 0.9, frame: 12000, frames: 12000 },
				{ key: 76, velocity: 0.85, frame: 24000, frames: 12000 },
				{ key: 81, velocity: 0.9, frame: 36000, frames: 12000 },
			],
		},
		devices: [{ bundle: 'wet-chorus', plugin: 'studio.opendaw.commons.wet-chorus', on: true, params: { '0': 5, '1': 1, '2': 180 } }],
	};
	state.files = { 'files/genome.json': genomeBytes };
	state.baseGenomeRaw = JSON.parse(new TextDecoder().decode(genomeBytes));  // for the weights pad
	state.autoPlay = state.rack.source.notes[0];
	$('#rackName').value = state.rack.name;
	$('#pads').style.display = 'flex';
	await rebuild();
}

// ── performance pads (sound mode) ────────────────────────────────────────────────────────────────
// padPitchDur: x -> Pitch param (semitones), y -> Duration param — mirrors the web app's
//   pitch-duration pad. padWeights: (x,y) -> GenomeGeometryMapper weights mapping, genome
//   re-delivered via source.files (rebuild) — mirrors the app's geometric (weights) pad.

function makePad(el, onMove) {
	const dot = el.querySelector('.pad-dot');
	const move = (e) => {
		const r = el.getBoundingClientRect();
		const x = Math.min(1, Math.max(0, (e.clientX - r.left) / r.width));
		const y = Math.min(1, Math.max(0, (e.clientY - r.top) / r.height));
		dot.style.left = `${x * 100}%`;
		dot.style.top = `${y * 100}%`;
		onMove(x, y);
	};
	el.addEventListener('pointerdown', (e) => { el.setPointerCapture(e.pointerId); move(e); });
	el.addEventListener('pointermove', (e) => { if (e.buttons) move(e); });
}

function setSourceParam(id, value) {
	const slider = window.__sliders?.[id];
	if (!slider) return;
	slider.value = value;
	slider.oninput();  // updates the rack doc + posts the param event to the worklet
}

makePad($('#padPitchDur'), (x, y) => {
	setSourceParam(6, (x - 0.5) * 2 * 36);          // Pitch, -36..36 st
	setSourceParam(1, 10 - y * (10 - 0.05));        // Duration, 10 s (top) .. 0.05 s (bottom)
});

let weightsTimer = null;
makePad($('#padWeights'), (x, y) => {
	if (!state.baseGenomeRaw) return;
	clearTimeout(weightsTimer);
	weightsTimer = setTimeout(async () => {
		const adjusted = adjustGenomeWeights(state.baseGenomeRaw, x, y);
		state.files['files/genome.json'] = new TextEncoder().encode(JSON.stringify(adjusted));
		state.autoPlay = state.rack.source.notes[0];
		$('#status').textContent = `weights pad (${x.toFixed(2)}, ${y.toFixed(2)}) — adjusted genome re-delivered, replaying…`;
		await rebuild();
	}, 250);
});

$('#loadSynthIs').onclick = () => loadSynthIsSound($('#soundId').value);
$('#soundId').addEventListener('keydown', (e) => { if (e.key === 'Enter') loadSynthIsSound($('#soundId').value); });
// debug/test hook
window.__loadSynthIsSound = loadSynthIsSound;

if (new URLSearchParams(location.search).has('bundle')) initialLoad();
else if (new URLSearchParams(location.search).has('sound')) loadSynthIsSound(new URLSearchParams(location.search).get('sound'));
else renderCatalog();
