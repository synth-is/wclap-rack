# wclap-rack

A proposed standard for **racks of WCLAP plugins**: a portable, hash-verified JSON document that any
host with a CLAP runtime can render — in a browser, in Node, or natively inside a plugin via
[`wclap-bridge`](https://github.com/WebCLAP/wclap-bridge). Read [SPEC.md](SPEC.md) — including §0,
the motivation: portability, embeddability and shareability of WCLAP plugins *and their combinations*
between environments (Synth.is, Strudel, openDAW, DAWs via the bridge), plus sandboxing, bit-identical
determinism for measurement, licence compliance by construction, UI portability and client-side DSP.

A *rack bundle* is a directory (or `<name>.wclap-rack.tar.gz`):

```
my-rack.wclap-rack/
├── rack.json              # ordered serial device list (schema/rack.schema.json)
├── MANIFEST.json          # bundle key -> { path, sha256 of module.wasm }
└── testfx.wclap.tar.gz    # ordinary WCLAP bundles
```

```json
{
  "format": "wclap-rack", "version": 1, "name": "demo",
  "devices": [
    { "bundle": "testfx", "plugin": "is.synth.kromosynth.testfx", "on": true, "params": { "0": -6 } }
  ]
}
```

Devices run serially in array order; `on: false` is a bit-transparent bypass; `params` are raw CLAP
values applied at frame 0; `state` is an optional base64 `clap.state`. Offline rendering compensates
each device's `clap.latency` at that device; realtime hosts report the summed latency upward.

## Layout

```
SPEC.md                  the standard (document model, render semantics, conformance levels L1-L4)
schema/                  JSON Schemas for rack.json and MANIFEST.json
reference-host/          environment-agnostic JS host core + rack renderer (Node / browser page / AudioWorklet)
tools/                   validate.mjs (schema + sha256), render.mjs (offline conformance renderer),
                         pack-rack.mjs (pack a rack bundle dir into .wclap-rack.tar.gz)
test-host/               browser test host: loads a rack bundle, plays a saw through the rack in realtime
                         (AudioWorklet, module worklet), per-device param sliders and bypass — the same
                         testing capability the open-webclap kit's test-host offers, for racks
engines/                 engine (source) plugins for SPEC §8 file delivery: sfz-engine (a minimal SFZ
                         sampler WCLAP receiving .sfz + samples), faust-runner (a generic Faust-graph
                         runner receiving a genome wasm + JSON and executing it via embedded wasm3), and
                         cppn-engine (a real-time CPPN+DSP engine receiving a Synth.is genome JSON —
                         waveNetwork CPPN + asNEAT DSP patch — and evaluating both per sample) —
                         all delivered via wclap.files/1; see engines/<name>/README.md
tests/                   conformance harness (node --test tests/conformance.test.mjs) + the testfx
                         bundle (MIT) it runs on
```

## Quick start

```sh
# 1. build the test bundle (needs Emscripten 3.1.51; EMSDK=... overrides)
EMSDK=~/emsdk bash tests/bundles/testfx/scripts/build.sh

# 2. run the conformance harness
node --test tests/conformance.test.mjs

# 3. validate / render / pack a rack bundle
node tools/validate.mjs --bundle tests/rack-bundle
node tools/render.mjs  --bundle tests/rack-bundle --seconds 1 --out out.wav
node tools/pack-rack.mjs tests/rack-bundle --out demo.wclap-rack.tar.gz
```

## Testing — what to load where

**Browser test host** (recommended: Chrome or Edge; module AudioWorklets required — Firefox does not
support them, use the CLIs there):

```sh
cd wclap-rack && node tools/serve.mjs 8137      # no-store static server (python -m http.server works too,
                                                 # but Chrome may heuristically cache modules — hard-reload if stale)
# open one of:
http://127.0.0.1:8137/test-host/?bundle=/tests/rack-bundle/demo.wclap-rack.tar.gz
http://127.0.0.1:8137/test-host/?bundle=/examples/ports-rack/ports-rack.wclap-rack.tar.gz
http://127.0.0.1:8137/test-host/?bundle=/examples/effects-rack/effects-rack.wclap-rack.tar.gz
http://127.0.0.1:8137/test-host/?bundle=/examples/synth-fx-rack/synth-fx-rack.wclap-rack.tar.gz
http://127.0.0.1:8137/test-host/?bundle=/examples/sfz-rack/sfz-rack.wclap-rack.tar.gz
http://127.0.0.1:8137/test-host/?bundle=/examples/faust-rack/faust-rack.wclap-rack.tar.gz
http://127.0.0.1:8137/test-host/?bundle=/examples/synth-is-sfz/synth-is-sfz.wclap-rack.tar.gz
http://127.0.0.1:8137/test-host/?bundle=/examples/synth-is-faust/synth-is-faust.wclap-rack.tar.gz
http://127.0.0.1:8137/test-host/?bundle=/examples/synth-is-cppn/synth-is-cppn.wclap-rack.tar.gz
# …or point at ANY Synth.is CPPN+DSP sound id — the page fetches its raw genome from api.synth.is
# (through the dev server's /proxy/synthis/ route; the API 500s browser preflights from unknown
# origins) and assembles a live cppn-engine rack in memory. It auto-plays key 69 at velocity 1
# (the same noteDelta 0 / 440 Hz the synth.is preview renders at) and shows performance pads:
# pitch × duration (live Pitch/Duration params) and network weights (x,y) — the same geometric
# weight mapping as the main web app (test-host/geometric-map.js), re-delivering the adjusted genome:
http://127.0.0.1:8137/test-host/?sound=01KR0R5T4S7SQF4W8M1G99CGS9
# (same via the "Synth.is CPPN+DSP sound" input box on the page)
```

- `tests/rack-bundle/demo.wclap-rack.tar.gz` — the testfx demo (gain −6 dB → 64-sample delay).
- `examples/ports-rack/ports-rack.wclap-rack.tar.gz` — **real open-webclap ports**: WetChorus → Pearls.
- `examples/effects-rack/effects-rack.wclap-rack.tar.gz` — a 5-device rack mixing open-webclap ports and
  openDAW's Signalsmith Basics test bundle: WetChorus → Basics Crunch → Basics Reverb → Pearls → Basics
  Limiter (44 parameters).
- `examples/synth-fx-rack/synth-fx-rack.wclap-rack.tar.gz` — a **self-contained synth + effects rack**:
  the rack's optional `source` device (Aether, an instrument WCLAP) renders a C4 note, then WetChorus →
  Pearls process it. In the test host the source panel has note buttons; without a source the page plays
  the 110 Hz saw instead.
  Rebuild after changing the examples: `node tools/pack-rack.mjs examples/synth-fx-rack --out examples/synth-fx-rack/synth-fx-rack.wclap-rack.tar.gz`
- `examples/sfz-rack/sfz-rack.wclap-rack.tar.gz` — **file delivery (SPEC §8)**: the `source` is
  `engines/sfz-engine`, an SFZ sampler engine; the rack carries `source.files` (a `demo.sfz` + its
  `c4.wav` sample) which the host delivers to the module via `wclap.files/1` before activate. A C-major
  arpeggio renders through WetChorus. Rebuild: `node tools/pack-rack.mjs examples/sfz-rack --out examples/sfz-rack/sfz-rack.wclap-rack.tar.gz`
- `examples/faust-rack/faust-rack.wclap-rack.tar.gz` — **generic Faust genome delivery (SPEC §8)**:
  the `source` is `engines/faust-runner`, which receives a free-form Faust genome (`genome.wasm` +
  `genome.json` via `source.files`), exposes its widgets as dynamic parameters, and executes the wasm
  through an embedded interpreter. A C-major arpeggio renders through WetChorus. Rebuild:
  `node tools/pack-rack.mjs examples/faust-rack --out examples/faust-rack/faust-rack.wclap-rack.tar.gz`
- `examples/synth-is-sfz/` — **a real Synth.is CPPN+DSP sound** (01KR0R5T4S7SQF4W8M1G99CGS9, SFZ
  rendition trimmed to keys C3/C4/C5 for size) played by `sfz-engine` via `source.files`.
- `examples/synth-is-faust/` — **a real Synth.is Faust sound** (01M6ZRAYNZG2KXPCJBWFGRZ8KF,
  `instrument.wasm` + `instrument.json` from the Synth.is takeout, unmodified) played by
  `faust-runner`; its evolved parameters (cutMul, det, lfodepth, lforate, lvl, res) appear as sliders.
- `examples/synth-is-cppn/` — **a real Synth.is CPPN+DSP sound played LIVE** (01KR0R5T4S7SQF4W8M1G99CGS9,
  genome JSON delivered unmodified) by `engines/cppn-engine`, which evaluates the CPPN and its asNEAT
  DSP patch per sample inside the plugin (no SFZ rendering step); Duration/Volume/Attack/Release/X/Y
  appear as sliders. Rebuild: `node tools/pack-rack.mjs examples/synth-is-cppn --out examples/synth-is-cppn/synth-is-cppn.wclap-rack.tar.gz`
- Or use the page's file picker: a `.wclap-rack.tar.gz`, or a rack bundle **directory**.
- Click **Load & play** (the click also unlocks audio): with a `source` in the rack its note plays
  (and the source panel has clickable note buttons); without one, a 110 Hz saw plays through the rack
  in realtime. Each device gets a panel with bypass checkbox, reorder (↑/↓), remove (✕) and one slider
  per parameter; the status line shows the summed `clap.latency`.
- **Assembling a rack**: "Add bundles…" loads `.wclap.tar.gz` archives into the **catalog** (cards with
  kind badges — instrument / audio effect / note effect, borrowed from open-webclap's site and openDAW's
  DeviceCatalog/DeviceSlot). Instruments get *set as source*, audio effects *+ add to rack*; note
  effects are greyed out (v1 racks are a serial audio chain). Rename the rack, then **Download
  rack.json** or **Download .wclap-rack.tar.gz** — the archive is a complete rack bundle (rack.json +
  generated MANIFEST.json with sha256 of every module.wasm + the bundle archives), validated on load.
- The `?bundle=<url>` parameter makes any served rack bundle auto-load — handy for demos/embeds.

**Command line** (any browser, any OS):

```sh
node tools/validate.mjs --bundle <dir | .wclap-rack.tar.gz>   # schema + MANIFEST sha256, exit code
node tools/render.mjs  --bundle <dir | .wclap-rack.tar.gz> --seconds 2 --out rack.wav --json
                                                        # offline render, determinism check, WAV
node --test tests/conformance.test.mjs                  # the L1–L3 conformance harness
```

**Make your own rack bundle** from any WCLAP archives (e.g. open-webclap's `site/public/bundled/*.wclap.tar.gz`):

```sh
mkdir my-rack && cp /path/to/foo.wclap.tar.gz my-rack/
# MANIFEST.json: sha256 of each module.wasm — generate with:
node -e "import('./reference-host/index.mjs').then(async m => { const fs = await import('node:fs');
  const out = {}; for (const f of fs.readdirSync('my-rack').filter(f => f.endsWith('.tar.gz'))) {
    const { wasm } = await m.loadBundleBytesAsync(new Uint8Array(fs.readFileSync('my-rack/' + f)));
    out[f.replace(/\.wclap\.tar\.gz$/, '')] = { path: f, sha256: await m.sha256Hex(wasm) }; }
  fs.writeFileSync('my-rack/MANIFEST.json', JSON.stringify(out, null, 2)); })"
# rack.json: find plugin ids + param ids with the reference host:
node -e "import('./reference-host/index.mjs').then(async m => { const fs = await import('node:fs');
  const d = await m.describeBundle(new Uint8Array(fs.readFileSync('my-rack/foo.wclap.tar.gz')));
  console.log(JSON.stringify(d, null, 2)); })"
# then write rack.json (see examples/ports-rack/rack.json) and:
node tools/validate.mjs --bundle my-rack && node tools/pack-rack.mjs my-rack --out my-rack.wclap-rack.tar.gz
```

## Status and relation to other projects

Proposal intended for the [WebCLAP](https://github.com/WebCLAP) org (small standalone repos per
concern, like `wclap-bridge` and `wasi.wasm`). WCLAP bundles themselves are unchanged; racks of
[open-webclap](https://github.com/andremichelle/open-webclap) ports are valid rack bundles. Synth.is
consumes rack bundles as presets in its plugin and evolves them in `kromosynth-burst` (the Synth.is
profile with named slots lives there, not in this core format).

## Licence

MIT (see LICENSE) for the spec, schemas, reference host, tools and test host. Test bundles carry their
own licences (`tests/bundles/testfx/LICENSE`, NOTICE).
