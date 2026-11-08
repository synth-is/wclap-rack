// app.js — the browser test host page: load a rack bundle (.wclap-rack.tar.gz, or a directory via
// the directory picker), verify it, and play a test signal through the rack in realtime.
import { untarArchiveAsync, loadBundleBytesAsync, validateRackBundle } from '../reference-host/index.mjs';

const $ = (sel) => document.querySelector(sel);
window.__errors = [];
window.addEventListener('error', (e) => window.__errors.push(String(e.message)));
window.addEventListener('unhandledrejection', (e) => window.__errors.push('rejection: ' + String(e.reason)));
let ctx = null, node = null;

const td = new TextDecoder();
const get = (files, n) => files[n] ?? files[Object.keys(files).find((k) => k.endsWith('/' + n))];

function assemble(files) {
	const rack = JSON.parse(td.decode(get(files, 'rack.json')));
	const manifest = JSON.parse(td.decode(get(files, 'MANIFEST.json')));
	const bundles = {};
	for (const [key, entry] of Object.entries(manifest)) bundles[key] = get(files, entry.path);
	return { rack, manifest, bundles };
}

async function readRackBundle(file) {
	const raw = new Uint8Array(await file.arrayBuffer());
	const files = await untarArchiveAsync(raw); // untar (gzip via DecompressionStream)
	return assemble(files);
}

async function readRackBundleDirFiles(fileList) {
	const files = {};
	for (const f of fileList) files[f.webkitRelativePath.split('/').slice(1).join('/')] = new Uint8Array(await f.arrayBuffer());
	return assemble(files);
}

async function load() {
	const fileInput = $('#bundle');
	const dirInput = $('#bundleDir');
	let loaded = null;
	if (dirInput.files?.length) loaded = await readRackBundleDirFiles(dirInput.files);
	else if (fileInput.files?.length) loaded = await readRackBundle(fileInput.files[0]);
	else {
		const url = new URLSearchParams(location.search).get('bundle');
		if (!url) return;
		loaded = await readRackBundle(new File([await (await fetch(url)).blob()], 'bundle.wclap-rack.tar.gz'));
	}
	await start(loaded);
}

async function start(loaded) {
	const { rack, manifest, bundles } = loaded;
	const { errors } = await validateRackBundle(rack, manifest, bundles);
	if (errors.length) { $('#status').textContent = 'INVALID: ' + errors.join('; '); return; }

	ctx ??= new AudioContext();
	// autoplay policy: resume on the first real click; don't await resume() here (it can stay pending)
	document.addEventListener('click', () => ctx.resume().catch(() => {}), { once: true });
	await ctx.audioWorklet.addModule(new URL('./rack-worklet.js', import.meta.url), { type: 'module' });
	node?.disconnect();
	node = new AudioWorkletNode(ctx, 'wclap-rack', { numberOfInputs: 1, numberOfOutputs: 1, outputChannelCount: [2] });
	node.connect(ctx.destination);
	let autoPlay = null;
	node.port.onmessage = (e) => {
		if (e.data.type === 'ready') {
			window.__latency = e.data.latency; window.__latByIndex = {};
			for (const d of e.data.devices) if (d.params) window.__latByIndex[d.index] = 0;
			renderPanels(e.data, rack);
			if (autoPlay) { const n = autoPlay; setTimeout(() => { node.port.postMessage({ type: 'note', on: true, key: n.key, velocity: n.velocity ?? 0.8 }); }, 100); }
		}
		if (e.data.type === 'error') { $('#status').textContent = 'worklet error: ' + e.data.message; window.__errors.push('worklet: ' + e.data.message); }
		if (e.data.type === 'latencyUpdate') {
			window.__latency = (window.__latency ?? 0) - (window.__latByIndex?.[e.data.index] ?? 0) + e.data.latency;
			window.__latByIndex = { ...(window.__latByIndex ?? {}), [e.data.index]: e.data.latency };
			$('#status').textContent = $('#status').textContent.replace(/summed latency [^ ]+ frames/, `summed latency ${window.__latency} frames`);
		}
	};
	// decompress page-side and send module.wasm bytes per bundle key (worklets cannot gunzip)
	const bundlesAB = {};
	for (const [k, v] of Object.entries(bundles)) {
		const { wasm } = await loadBundleBytesAsync(v);
		bundlesAB[k] = wasm.buffer.slice(wasm.byteOffset, wasm.byteOffset + wasm.byteLength);
	}
	node.port.postMessage({ type: 'load', rack, bundles: bundlesAB });
	if (rack.source) {
		// the rack has its own instrument source: auto-play its first note (or C4) once loaded
		autoPlay = rack.source.notes?.[0] ?? { key: 60, velocity: 0.8, frames: 48000 };
	} else {
		// test signal: 110 Hz saw through the rack
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
	$('#status').textContent = `playing "${rack.name ?? 'rack'}" — ${rack.devices.length} device(s), summed latency ${'?'} frames`;
	window.__node = node;
}

function renderPanels(ready, rack) {
	$('#status').textContent = `playing "${rack.name ?? 'rack'}" — ${rack.devices.length} device(s), summed latency ${ready.latency} frames`;
	const host = $('#devices');
	host.innerHTML = '';
	if (ready.source) {
		const div = document.createElement('div');
		div.className = 'device';
		const head = document.createElement('div');
		head.className = 'device-head';
		head.append(document.createTextNode(`[source] ${ready.source.plugin} — click a note:`));
		for (const key of [48, 52, 55, 60, 64]) {
			const b = document.createElement('button');
			b.textContent = String(key);
			b.style.margin = '0 2px';
			b.onmousedown = () => node.port.postMessage({ type: 'note', on: true, key, velocity: 0.8 });
			b.onmouseup = () => node.port.postMessage({ type: 'note', on: false, key });
			head.append(b);
		}
		div.append(head);
		for (const p of ready.source.params.slice(0, 12)) {
			const row = document.createElement('label');
			row.className = 'param';
			const slider = document.createElement('input');
			slider.type = 'range';
			slider.min = p.min; slider.max = p.max; slider.step = p.stepped ? 1 : (p.max - p.min) / 200; slider.value = p.def;
			slider.oninput = () => { node.port.postMessage({ type: 'param', index: -1, id: p.id, value: Number(slider.value) }); val.textContent = slider.value; };
			const val = document.createElement('span');
			val.textContent = p.def;
			row.append(document.createTextNode(p.name), slider, val);
			div.append(row);
		}
		host.append(div);
	}
	for (const d of ready.devices) {
		const div = document.createElement('div');
		div.className = 'device';
		const head = document.createElement('div');
		head.className = 'device-head';
		const bypass = document.createElement('input');
		bypass.type = 'checkbox';
		bypass.checked = !d.bypassed;
		bypass.onchange = () => node.port.postMessage({ type: 'setOn', index: d.index, on: bypass.checked });
		head.append(bypass, document.createTextNode(` [${d.index}] ${d.plugin}`));
		div.append(head);
		for (const p of d.params ?? []) {
			const row = document.createElement('label');
			row.className = 'param';
			const slider = document.createElement('input');
			slider.type = 'range';
			slider.min = p.min; slider.max = p.max; slider.step = p.stepped ? 1 : (p.max - p.min) / 200; slider.value = p.def;
			slider.oninput = () => { node.port.postMessage({ type: 'param', index: d.index, id: p.id, value: Number(slider.value) }); val.textContent = slider.value; };
			const val = document.createElement('span');
			val.textContent = p.def;
			row.append(document.createTextNode(p.name), slider, val);
			div.append(row);
		}
		host.append(div);
	}
}

$('#load').onclick = load;

// auto-load when the page is opened as test-host/?bundle=<url to a .wclap-rack.tar.gz>
if (new URLSearchParams(location.search).has('bundle')) load();
