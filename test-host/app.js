// app.js — the browser test host page: load a rack bundle (.wclap-rack.tar.gz, or a directory via
// the directory picker), verify it, and play a test signal through the rack in realtime.
import { loadBundleBytesAsync, validateRackBundle, untar } from '../reference-host/index.mjs';

const $ = (sel) => document.querySelector(sel);
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
	const { files } = await loadBundleBytesAsync(raw); // untar (gzip via DecompressionStream)
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
	else return;
	const { rack, manifest, bundles } = loaded;
	const { errors } = await validateRackBundle(rack, manifest, bundles);
	if (errors.length) { $('#status').textContent = 'INVALID: ' + errors.join('; '); return; }

	ctx ??= new AudioContext();
	await ctx.audioWorklet.addModule(new URL('./rack-worklet.js', import.meta.url), { type: 'module' });
	node?.disconnect();
	node = new AudioWorkletNode(ctx, 'wclap-rack', { numberOfInputs: 1, numberOfOutputs: 1, outputChannelCount: [2] });
	node.connect(ctx.destination);
	const bundlesAB = {};
	for (const [k, v] of Object.entries(bundles)) bundlesAB[k] = v.buffer.slice(v.byteOffset, v.byteOffset + v.byteLength);
	node.port.postMessage({ type: 'load', rack, bundles: bundlesAB });
	node.port.onmessage = (e) => {
		if (e.data.type === 'ready') renderPanels(e.data, rack);
	};
	// test signal: 110 Hz saw through the rack
	const osc = ctx.createOscillator();
	osc.type = 'sawtooth';
	osc.frequency.value = 110;
	const gain = ctx.createGain();
	gain.gain.value = 0.5;
	osc.connect(gain).connect(node);
	osc.start();
	$('#status').textContent = `playing "${rack.name ?? 'rack'}" — ${rack.devices.length} device(s), summed latency ${'?'} frames`;
	window.__node = node;
}

function renderPanels(ready, rack) {
	$('#status').textContent = `playing "${rack.name ?? 'rack'}" — ${rack.devices.length} device(s), summed latency ${ready.latency} frames`;
	const host = $('#devices');
	host.innerHTML = '';
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
