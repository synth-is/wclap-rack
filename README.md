# wclap-rack

A proposed standard for **racks of WCLAP plugins**: a portable, hash-verified JSON document that any
host with a CLAP runtime can render — in a browser, in Node, or natively inside a plugin via
[`wclap-bridge`](https://github.com/WebCLAP/wclap-bridge). Read [SPEC.md](SPEC.md).

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

# 4. browser test host: serve this repo and open test-host/ (module worklets needed)
npx serve .   # then http://localhost:3000/test-host/
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
