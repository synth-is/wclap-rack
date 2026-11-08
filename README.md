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
```

- `tests/rack-bundle/demo.wclap-rack.tar.gz` — the testfx demo (gain −6 dB → 64-sample delay).
- `examples/ports-rack/ports-rack.wclap-rack.tar.gz` — **real open-webclap ports**: WetChorus → Pearls.
- `examples/effects-rack/effects-rack.wclap-rack.tar.gz` — a 5-device rack mixing open-webclap ports and
  openDAW's Signalsmith Basics test bundle: WetChorus → Basics Crunch → Basics Reverb → Pearls → Basics
  Limiter (44 parameters).
  Rebuild after changing the examples: `node tools/pack-rack.mjs examples/effects-rack --out examples/effects-rack/effects-rack.wclap-rack.tar.gz`
- Or use the page's file picker: a `.wclap-rack.tar.gz`, or a rack bundle **directory**.
- Click **Load & play** (the click also unlocks audio): a 110 Hz saw plays through the rack in
  realtime; each device gets a panel with bypass checkbox and one slider per parameter; the status
  line shows the summed `clap.latency`.
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
