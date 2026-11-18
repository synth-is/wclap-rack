// cdp-bridge.mjs — host side of the cdp-engine's "wclap.cdp/1" bridge (engines/cdp-engine).
//
// The cdp-engine module.wasm cannot run the CDP programs itself (they are Emscripten side
// modules needing their JS glue), so on note-on it calls the imported render_request; this
// bridge renders the selected catalog generator offline with the vendored cdp-wasm engine
// (third_party/cdp-wasm, loaded lazily per program), then hands the planar float32 buffer
// back through the module's exported cdp_deliver (which takes ownership of the malloc'd
// memory). Renders are serialized (the CDP engine stages files in one MEMFS) and cached by
// (program, params, note), so repeated notes are instant.
//
// Environment-agnostic (no node: imports at module top level): in Node pass a file: URL as
// pkgUrl; in the browser pass the http(s) URL of third_party/cdp-wasm (serve.mjs serves the
// repo root, so e.g. /wclap/engines/cdp-engine/third_party/cdp-wasm/).
//
//   const bridge = new CDPBridge({ pkgUrl, sampleRate: 48000 });
//   await bridge.init();
//   const host = await WclapHost.load(wasm, {
//     extensions: { 'wclap.cdp/1': bridge.imports() },
//     onInstantiated: (h) => bridge.attach(h),
//   });
//   ... await bridge.drain();  // wait for all pending renders to be delivered
//
// Also exposes the offline effect path (applyEffectToWav) — CDP effects are offline
// programs, so in a wclap-rack they run at extract/QD time over the rendered source audio,
// not as realtime CLAP process().

const sleep = (ms) => new Promise((r) => setTimeout(r, ms));

export class CDPBridge {
	/**
	 * @param {{ pkgUrl: string|URL, sampleRate?: number, log?: (s:string)=>void,
	 *           deliver?: (reqId:number, planar:Float32Array, frames:number, chans:number, rate:number, baseKey:number) => void,
	 *           onStatus?: (s:{state:string, message?:string, reqId?:number}) => void }} opts
	 *   pkgUrl: URL of the vendored cdp-wasm package root (contains src/ and wasm/);
	 *           relative URLs resolve against location (browser) — absolute file: URLs in Node.
	 *   deliver: where rendered buffers go. When omitted, the Node host path is used:
	 *           attach(host) + imports() wire render_request straight into the module's
	 *           memory and call its exported cdp_deliver (the wclap.cdp/1 bridge).
	 *   fail: (reqId, message) => void — where render failures go (the page-side host posts
	 *           {type:'cdpFail'} to the worklet, which calls the module's cdp_fail export).
	 *           When omitted and attached to a host, cdp_fail is called in module memory.
	 *   fxChain: [{ i: effectCatalogIndex, p: number[12] }] — CDP effects are offline
	 *           programs, so they are applied here, between the generator render and the
	 *           delivery (a chained cdp-fx rack device). setFxChain() updates it live.
	 */
	constructor({ pkgUrl, sampleRate = 48000, log = () => {}, deliver = null, onStatus = null, fail = null, fxChain = [] }) {
		this.pkgUrl = new URL(String(pkgUrl).replace(/\/?$/, '/'), globalThis.location?.href ?? 'file:///').href;
		this.sampleRate = sampleRate;
		this.log = log;
		this.deliverFn = deliver;
		this.onStatus = onStatus;
		this.failFn = fail;
		this.fxChain = fxChain;
		this.host = null;
		this.cdp = null;
		this._queue = Promise.resolve();
		this._pending = 0;
		this._cache = new Map(); // renderKey -> Promise<{planar:Float32Array, frames, chans, rate, baseKey}>
	}

	async init() {
		const mod = await import(this.pkgUrl + 'src/index.js');
		this.mod = mod;
		this.cdp = new mod.CDP({ baseUrl: new URL('wasm/', this.pkgUrl) });
		await this.cdp.programs(); // warm manifest (fails fast if wasm/ is not served)
		return this;
	}

	attach(host) {
		this.host = host;
		if (host.virtualRate) this.sampleRate = host.virtualRate;
	}

	/** The wasm import object for WclapHost's `extensions`. */
	imports() {
		return {
			'wclap.cdp/1': {
				render_request: (reqId, specPtr, specLen, note, vel7) => this._renderRequest(reqId, specPtr, specLen, note, vel7),
			},
		};
	}

	pendingCount() { return this._pending; }

	/** Wait until every requested render has been delivered (or failed). */
	async drain(timeoutMs = 30000) {
		const deadline = Date.now() + timeoutMs;
		while (this._pending > 0 && Date.now() < deadline) await sleep(5);
		if (this._pending > 0) throw new Error(`cdp-bridge: ${this._pending} render(s) still pending after ${timeoutMs} ms`);
	}

	_renderRequest(reqId, specPtr, specLen, note, vel7) {
		if (!this.host) return 0;
		let spec;
		try {
			const bytes = new Uint8Array(this.host.memory.buffer, specPtr, specLen);
			spec = JSON.parse(new TextDecoder().decode(bytes));
		} catch (e) {
			this.log(`cdp-bridge: bad spec for req ${reqId}: ${e.message}`);
			return 0;
		}
		this.renderRequest(reqId, spec, note, vel7);
		return 1;
	}

	/** Queue one render; resolves when the buffer has been delivered (or the render failed). */
	renderRequest(reqId, spec, note, vel7) {
		this._pending++;
		this.onStatus?.({ state: 'rendering', reqId });
		return this._queue
			.then(() => this._renderAndDeliver(reqId, spec, note, vel7))
			.then(() => this.onStatus?.({ state: 'ready', reqId }))
			.catch((e) => {
				this.log(`cdp-bridge: render req ${reqId} failed: ${e.message}`);
				this.onStatus?.({ state: 'error', reqId, message: e.message });
				this._fail(reqId, e.message);
			})
			.finally(() => { this._pending--; });
	}

	/** Route a render failure to the module (cdp_fail export -> editor page frame 84). */
	_fail(reqId, message) {
		if (this.failFn) { try { this.failFn(reqId, message); } catch { /* page gone */ } return; }
		const host = this.host;
		if (!host || typeof host.exports?.cdp_fail !== 'function') return;
		const bytes = new TextEncoder().encode(String(message ?? 'render failed').slice(0, 4000));
		const ptr = host.malloc(bytes.length + 1);
		new Uint8Array(host.memory.buffer, ptr, bytes.length + 1).set(bytes);
		host.exports.cdp_fail(reqId, ptr, bytes.length);
		host.exports.free(ptr);
	}

	/** Live-update the offline effect chain applied between generator render and delivery. */
	setFxChain(stages) {
		this.fxChain = Array.isArray(stages) ? stages : [];
		this._cache.clear(); // cached renders were made without (or with another) chain
	}

	/** Run a CDP program (generator or effect) with source-aware values + retry.
	 *
	 *  CDP's valid ranges can depend on the INPUT (focus.step's max step and extend.zigzag's
	 *  `end` both scale with the source duration), and the vendored catalog encodes that:
	 *  params carry srcMin/srcMax/maxOf and entries carry minSrcDur. So instead of clamping
	 *  against the static catalog ranges, `given` holds only the explicitly-set values and
	 *  `_resolve` fills the rest via the vendored defaultsFor(entry, src, given) (which picks
	 *  source-valid defaults at a fixed point) and clamps the givens into paramRange(p, vals,
	 *  {src}). On failure the CLI's structured "Parameter[N] Value (x) out of range (lo to hi)"
	 *  message is parsed (Parameter[N] = the Nth arg after $OUT in the entry's args template)
	 *  and the offending value clamped into the reported range, then retried; as a last resort
	 *  one retry with pure defaults. Throws the last error otherwise. */
	async _runCdp(entry, given, inputWav = null) {
		const apply = inputWav
			? (v) => this.mod.applyEffect(this.cdp, entry, v, inputWav, { sampleRate: this.sampleRate })
			: (v) => this.mod.applyGenerator(this.cdp, entry, v, { sampleRate: this.sampleRate });
		const src = inputWav ? this._srcContext(inputWav) : null;
		let vals = this._resolve(entry, given, src);
		let lastErr = null;
		let triedDefaults = false;
		for (let attempt = 0; attempt < 5; attempt++) {
			try {
				const out = await apply(vals);
				return out instanceof Uint8Array ? out : out.outputs[0];
			} catch (e) {
				lastErr = e;
				const fixed = this._fixOutOfRange(entry, vals, String(e.message || e));
				if (fixed) { vals = fixed; continue; }
				if (!triedDefaults) { triedDefaults = true; vals = this._resolve(entry, {}, src); continue; }
				break;
			}
		}
		throw lastErr;
	}

	/** { dur, srate, chans } of an input wav, for the catalog's source-relative ranges. */
	_srcContext(wavBytes) {
		const d = this.mod.decodeWav(wavBytes);
		return { dur: d.length / d.sampleRate, srate: d.sampleRate, chans: d.numChannels };
	}

	/** Resolve given values against the catalog's source-aware ranges: defaults via the
	 *  vendored defaultsFor (source-valid), givens clamped into paramRange (which applies
	 *  srcMin/srcMax/minOf/maxOf). Falls back to static clamping without a source. */
	_resolve(entry, given, src) {
		const { paramRange, defaultsFor } = this.mod;
		if (typeof defaultsFor !== 'function') {
			// older vendored package: static defaults + static clamp
			const vals = {};
			for (const p of entry.params || []) {
				const g = given ? given[p.name] : undefined;
				vals[p.name] = g !== undefined ? g : p.default;
				if (typeof vals[p.name] === 'number' && !p.choices && typeof p.min === 'number' && typeof p.max === 'number')
					vals[p.name] = Math.min(p.max, Math.max(p.min, vals[p.name]));
			}
			return vals;
		}
		const vals = defaultsFor(entry, src, given || {});
		if (typeof paramRange === 'function') {
			for (const p of entry.params || []) {
				if (p.choices || !(p.name in (given || {}))) continue; // defaults are already range-checked
				if (typeof vals[p.name] !== 'number') continue;
				const { min, max } = paramRange(p, vals, { src });
				if (isFinite(min) && isFinite(max)) vals[p.name] = Math.min(max, Math.max(min, vals[p.name]));
			}
		}
		return vals;
	}

	/** Only the explicitly-set, choice-validated values from a positional raw array
	 *  (sentinel -1e9 = "not given" -> the source-aware default is used). */
	_given(entry, rawArr) {
		const out = {};
		(entry.params || []).forEach((p, k) => {
			const raw = Array.isArray(rawArr) ? rawArr[k] : undefined;
			if (raw === undefined || raw === null || raw <= -1e9 + 1) return;
			if (p.choices) {
				if (p.choices.some((c) => Number(c[1]) === raw)) out[p.name] = raw;
			} else {
				out[p.name] = raw;
			}
		});
		return out;
	}

	/** Parameter[N] in CDP error messages = the Nth argument AFTER $OUT in the entry's
	 *  args template (literals included), e.g. extend.zigzag ['zigzag','1','$IN','$OUT',
	 *  '0',{p:end},…] reports `end` as Parameter[2]; focus.step [...,'$OUT',{p:step}] as
	 *  Parameter[1]. Returns the param name, or null for literals/unknown. */
	_argParamName(entry, n) {
		const args = entry.args || [];
		let out = args.indexOf('$OUT');
		if (out < 0) out = args.indexOf('$IN');
		if (out >= 0) {
			const a = args[out + n];
			if (a && typeof a === 'object' && a.p != null) return a.p;
			return null;
		}
		return (entry.params || [])[n - 1]?.name ?? null;
	}

	/** Clamp parameters reported as out of range into the CLI-reported range; returns a
	 *  new values object when something changed, else null. */
	_fixOutOfRange(entry, vals, message) {
		const re = /Parameter\[(\d+)\] Value \(([^)]*)\) out of range \(([^)]*) to ([^)]*)\)/g;
		let m, changed = null;
		while ((m = re.exec(message))) {
			const name = this._argParamName(entry, Number(m[1]));
			if (!name) continue;
			const lo = Number(m[3]), hi = Number(m[4]);
			if (!isFinite(lo) || !isFinite(hi)) continue;
			const cur = vals[name];
			const nv = Math.min(hi, Math.max(lo, typeof cur === 'number' ? cur : lo));
			if (nv !== cur) { changed ??= { ...vals }; changed[name] = nv; }
		}
		return changed;
	}

	async _renderAndDeliver(reqId, spec, note, vel7) {
		const { GENERATORS, EFFECTS, decodeWav } = this.mod;
		const entry = GENERATORS[spec.i | 0];
		if (!entry) throw new Error(`no generator at index ${spec.i}`);
		// only explicitly-set P values are "given"; the rest take source-aware defaults
		const given = this._given(entry, spec.p);
		if (typeof spec.dur === 'number' && (entry.params || []).some((p) => p.name === 'dur')) given.dur = spec.dur;
		// Pitch: generators with a freq param are rendered at the note's own frequency
		// (baseKey = note -> playback at native rate); others render at the root and the
		// engine pitch-shifts by 2^((key-baseKey)/12).
		let baseKey = typeof spec.root === 'number' ? spec.root : 60;
		if ((entry.params || []).some((p) => p.name === 'freq') && note != null && note >= 0) {
			given.freq = 440 * Math.pow(2, (note - 69) / 12) // MIDI: key 69 = A4 = 440 Hz;
			baseKey = note;
		}
		const key = JSON.stringify([spec.i, given, this.sampleRate, baseKey, JSON.stringify(this.fxChain)]);
		let job = this._cache.get(key);
		if (!job) {
			job = (async () => {
				// generator render, then the offline effect chain (chained cdp-fx devices);
				// both go through _runCdp so input-dependent CDP ranges self-correct
				let wav = await this._runCdp(entry, given);
				for (const stage of this.fxChain) {
					const fx = EFFECTS[stage.i | 0];
					if (!fx) throw new Error(`no effect at index ${stage.i}`);
					wav = await this._runCdp(fx, this._given(fx, stage.p), wav);
				}
				const d = decodeWav(wav);
				const chans = d.numChannels;
				const frames = d.length;
				const planar = new Float32Array(frames * chans);
				for (let c = 0; c < chans; c++) planar.set(d.channelData[c], c * frames);
				return { planar, frames, chans, rate: d.sampleRate, baseKey };
			})();
			this._cache.set(key, job);
			if (this._cache.size > 256) this._cache.delete(this._cache.keys().next().value);
		}
		const r = await job;
		if (this.deliverFn) {
			this.deliverFn(reqId, r.planar, r.frames, r.chans, r.rate, r.baseKey);
			return;
		}
		const bytes = r.frames * r.chans * 4;
		const ptr = this.host.malloc(bytes);
		new Float32Array(this.host.memory.buffer, ptr, r.frames * r.chans).set(r.planar);
		this.host.exports.cdp_deliver(reqId, ptr, r.frames, r.chans, r.rate, r.baseKey);
	}

	// ── offline paths (extract time, QD runs): CDP effects over rendered audio ──────────────

	/** Render a catalog generator to a WAV byte array (no plugin involved). */
	async renderGeneratorToWav(genIdOrIndex, values = {}, { sampleRate = this.sampleRate, dur, note = null } = {}) {
		const { GENERATORS } = this.mod;
		const entry = typeof genIdOrIndex === 'number' ? GENERATORS[genIdOrIndex] : GENERATORS.find((g) => g.id === genIdOrIndex);
		if (!entry) throw new Error(`unknown generator ${genIdOrIndex}`);
		const given = { ...values };
		if (dur != null && (entry.params || []).some((p) => p.name === 'dur')) given.dur = dur;
		if (note != null && (entry.params || []).some((p) => p.name === 'freq')) given.freq = 440 * Math.pow(2, (note - 69) / 12) // MIDI: key 69 = A4 = 440 Hz;
		return this._runCdp(entry, given);
	}

	/** Apply a catalog effect to a WAV byte array. values maps param NAME -> value. */
	async applyEffectToWav(fxId, wavBytes, values = {}, { sampleRate = this.sampleRate } = {}) {
		const { EFFECTS } = this.mod;
		const fx = EFFECTS.find((e) => e.id === fxId);
		if (!fx) throw new Error(`unknown effect ${fxId}`);
		// only explicitly-provided values are given; the rest take source-aware defaults
		const given = {};
		for (const p of fx.params || []) if (p.name in values) given[p.name] = values[p.name];
		return this._runCdp(fx, given, wavBytes);
	}

	/** Same, by catalog index (what a chained cdp-fx rack device's param 0 holds). */
	async applyEffectIndexToWav(i, wavBytes, values = {}, { sampleRate = this.sampleRate } = {}) {
		const { EFFECTS } = this.mod;
		const fx = EFFECTS[i | 0];
		if (!fx) throw new Error(`no effect at index ${i}`);
		const given = {};
		for (const p of fx.params || []) if (p.name in values) given[p.name] = values[p.name];
		return this._runCdp(fx, given, wavBytes);
	}

	/** Slim catalog (id, label, category, program, params) for UIs — same shape as engines/cdp-engine/catalog.json. */
	catalog() {
		const slim = (e) => ({
			id: e.id, label: e.label, category: e.category, program: e.program,
			...(e.domain ? { domain: e.domain } : {}),
			...(e.unsupported ? { unsupported: e.unsupported } : {}),
			params: (e.params || []).map((p) => ({ name: p.name, label: p.label, ...(p.choices ? { choices: p.choices } : { min: p.min, max: p.max, default: p.default }) })),
		});
		return { generators: this.mod.GENERATORS.map(slim), effects: this.mod.EFFECTS.map(slim) };
	}
}
