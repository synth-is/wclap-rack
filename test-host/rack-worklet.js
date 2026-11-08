// rack-worklet.js — realtime wclap-rack host inside an AudioWorklet (loaded as a module worklet:
// ctx.audioWorklet.addModule('rack-worklet.js', { type: 'module' })).
//
// One WclapHost per bundle; one plugin per device; devices run serially in process() in rack order.
// The page sends `bundles` as { key: module.wasm bytes } — decompression happens page-side (worklets
// have no DecompressionStream/import()).
// Realtime semantics per SPEC.md §3: no frames are dropped; the summed latency is reported to the
// page (a DAW host would compensate it upward).
import { WclapHost } from '../reference-host/index.mjs';

const B = 128; // RenderQuantum

class RackProcessor extends AudioWorkletProcessor {
	constructor() {
		super();
		this.hosts = new Map();
		this.devices = [];
		this.ready = false;
		this.port.onmessage = (e) => { void this.onmsg(e.data); };
	}

	async onmsg(m) {
		if (m.type === 'load') {
		 try {
			const { rack, bundles } = m;
			for (const [i, d] of rack.devices.entries()) {
				if (d.on === false) { this.devices.push({ index: i, on: false }); continue; }
				let host = this.hosts.get(d.bundle);
				if (!host) {
					// the page sends module.wasm bytes per bundle key (it can gunzip; worklets cannot import())
					host = await WclapHost.load(new Uint8Array(bundles[d.bundle]));
					host.snapshot();
					this.hosts.set(d.bundle, host);
				}
				host.restore();
				const plugin = host.createPlugin(d.plugin);
				plugin.activate({ sampleRate, blockSize: B });
				if (d.state) plugin.loadState(Uint8Array.from(atob(d.state), (c) => c.charCodeAt(0)));
				const events = Object.entries(d.params ?? {}).map(([k, v]) => ({
					frame: 0, type: 'param', id: plugin.param(isNaN(Number(k)) ? k : Number(k))?.id ?? k, value: v,
				}));
				this.devices.push({
					index: i, on: true, plugin, events, first: true, pending: null,
					latency: 0,
				});
			}
			let latency = 0;
			for (const d of this.devices) if (d.on) { d.latency = d.plugin.latency(); latency += d.latency; }
			this.port.postMessage({
				type: 'ready',
				latency,
				devices: this.devices.map((d) => d.on
					? { index: d.index, plugin: rack.devices[d.index].plugin, params: d.plugin.params.map((p) => ({ id: p.id, name: p.name, min: p.min, max: p.max, def: p.def, stepped: p.stepped })) }
					: { index: d.index, bypassed: true, plugin: rack.devices[d.index].plugin }),
			});
			this.ready = true;
		 } catch (e) {
			this.port.postMessage({ type: 'error', message: String(e && e.stack || e) });
		 }
		}
		if (m.type === 'param') {
			const d = this.devices[m.index];
			if (d?.on) d.pending = [...(d.pending ?? []), { frame: 0, type: 'param', id: m.id, value: m.value }];
		}
		if (m.type === 'setOn') {
			const d = this.devices[m.index];
			if (d) d.on = m.on;
		}
	}

	process(inputs, outputs) {
		const input = inputs[0], output = outputs[0];
		if (!this.ready || !input || input.length === 0) return true;
		let l = input[0], r = input[1] ?? input[0];
		for (const d of this.devices) {
			if (!d.on) continue;
			const events = d.first ? d.events : (d.pending ?? []);
			d.first = false;
			d.pending = null;
			const out = d.plugin.render(B, events, { input: [l, r] });
			l = out[0];
			r = out[1] ?? out[0];
			if (events === d.events) { // first block: params applied now, latency may have changed
				const lat = d.plugin.latency();
				if (lat !== d.latency) {
					d.latency = lat;
					this.port.postMessage({ type: 'latencyUpdate', index: d.index, latency: lat });
				}
			}
		}
		output[0].set(l);
		if (output[1]) output[1].set(r);
		return true;
	}
}

registerProcessor('wclap-rack', RackProcessor);
