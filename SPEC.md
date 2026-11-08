# WCLAP Rack — a standard for serial racks of WCLAP plugins

Status: **proposal** (developed in this repo; intended for the [WebCLAP](https://github.com/WebCLAP)
org alongside `wclap-bridge`, `wclap-host-js`, …). Licence of this spec and the reference code: MIT.

A WCLAP is a single CLAP plugin compiled to wasm32 (see the WebCLAP org profile). This document defines
how **several WCLAPs form a rack**: a portable, hash-verified document that any host with a CLAP runtime
can render — in a browser, in Node, or natively (via `wclap-bridge`) inside a plugin such as Synth.is.

## 0. Motivation

Why racks of WCLAPs, and why a standard for them:

- **Portability.** One wasm binary runs in a browser page, an AudioWorklet, Node, and — through
  `wclap-bridge` — as a native CLAP in DAWs and inside other plugins (e.g. the Synth.is plugin
  hosting engine WCLAPs). The plugin author builds once; every host is a new distribution channel for
  free.
- **Embeddability.** Hosts can embed third-party DSP without taking the process down: wasm is sandboxed
  (memory-limited, WASI-only imports, `thread-spawn` refused), so a platform can safely host plugins it
  does not control — in Synth.is's web app, in a Strudel live-coding session, in openDAW, in the plugin.
- **Shareability.** A bundle is a self-contained, content-addressed archive (module.wasm + webview UI +
  LICENSE/NOTICES + source link). This spec adds the missing piece: a **rack document** makes whole
  *combinations* shareable and re-loadable, not just single plugins — a discovered sound is a bundle
  plus a state blob; a rack of effects is a `rack.json` anyone can render.
- **Determinism, hence measurability.** With pinned WASI clock/RNG and one host per bundle, rendering is
  bit-identical across processes, machines and hosts (the reference host proves Node == Chrome by
  fingerprint). That is what makes racks scientifically evaluable — do effects add distinguishable new
  sounds or only recolour old ones — and what makes rescoring, blind listening and evolution experiments
  trustworthy.
- **Licence compliance by construction.** Bundles carry their licence texts and notices; rack bundles pin
  every module by sha256 in the MANIFEST, so licence provenance travels with the artefact and a host can
  verify it before loading. Serving a GPL/AGPL bundle is conveyance, and the source offer is mechanical
  (pinned commits + patches), not an afterthought.
- **UI portability.** The CLAP webview draft extension means a plugin's *own* editor (not a generic
  parameter list) runs anywhere: openDAW's device panel, our test host, the Synth.is plugin editor via
  the bridge's translation to native `clap.gui`.
- **Client-side DSP.** Audio is processed in the user's browser/device, not on a render farm — lower
  cost, and no audio leaves the machine unless the user sends it.
- **Racks as data.** A rack is a small JSON document, so it can be a preset, a genome in an evolutionary
  loop, a pattern value in a live-coding language, or a track chain in a DAW — the same artefact moving
  between discovery (evolution/measurement tooling), live performance (Strudel), and production (the
  Synth.is plugin, openDAW).

## 1. The rack bundle

A **rack bundle** is a directory (packed as `<name>.wclap-rack.tar.gz` with `tools/pack-rack.mjs`):

```
my-rack.wclap-rack/
├── rack.json                 # the rack document (schema/rack.schema.json)
├── MANIFEST.json             # { "<key>": { "path": "<archive>", "sha256": "<sha256 of module.wasm>" } }
├── testfx.wclap.tar.gz       # ordinary WCLAP bundle(s), as defined by WebCLAP
└── ...
```

`MANIFEST.json` pins every referenced bundle by content hash (`schema/manifest.schema.json`). A host
MUST verify the sha256 of each `module.wasm` against the manifest and MUST refuse a mismatch.

## 2. The rack document (`rack.json`)

```json
{
  "format": "wclap-rack",
  "version": 1,
  "name": "demo",
  "devices": [
    { "bundle": "testfx", "plugin": "is.synth.kromosynth.testfx", "on": true, "params": { "0": -6 } }
  ]
}
```

- `devices`: an **ordered array**. Devices run **serially** in array order over a stereo stream.
- `bundle`: key into `MANIFEST.json`. `plugin`: the clap plugin id inside that bundle's factory.
- `on: false`: **bypass** — the device is skipped and the stream passes through bit-transparent.
- `params`: `{ "<param id as string>" | "<param name>": <raw CLAP value within [min, max]> }`, applied as
  parameter events at frame 0 of the device's render.
- `state`: optional base64 `clap.state` blob, loaded after plugin init and before `params`.

Profiles (non-normative) may layer conventions on top — e.g. the Synth.is profile fixes four named
slots (`colour`, `dynamics`, `modulation`, `space`) with gene bounds for evolution. The core format
stays generic: an ordered device list.

## 3. Render semantics

**Offline rendering** (what `reference-host/rack.mjs` `renderRack` implements):

1. Prepend `preRoll` frames of silence (default 4096) once, so parameter ramps and type re-inits
   settle before the audio.
2. For each device in order: create the plugin (one host per bundle; `snapshot()` after load,
   `restore()` before every device render, so devices share a bundle deterministically), activate at
   the host sample rate, load `state`, apply `params` at frame 0, render the whole stream through the
   device's audio input port 0.
3. Each device's reported `clap.latency` is compensated **at that device** (its output is realigned),
   so the rack output is sample-aligned with the rack input and has the input's length. The tail
   beyond the input length is the render buffer's business, not the rack's.
4. A bypassed device is skipped entirely (bit-transparent).

**Realtime rendering** (AudioWorklet, native plugin): devices run in `process()` in order. Do NOT drop
frames; instead report the **sum of active devices' `clap.latency`** (and the max `clap.tail`) upward —
CLAP convention lets the host/DAW compensate. See `test-host/rack-worklet.js`.

Mono/stereo adaptation is the host's: a mono device gets the mid channel; a single-channel output is
duplicated to stereo.

## 4. Host obligations and conformance levels

A host claims a level by passing `tests/conformance.test.mjs` (or its own equivalent):

- **L1 — load & render.** Parse and validate `rack.json` and `MANIFEST.json`, verify sha256, create each
  plugin from its bundle's factory, activate, apply state/params, render serially, honour bypass.
- **L2 — determinism.** Same rack + same input bytes + same events → bit-identical output, across
  repeats and across two separate processes (fingerprint = the reference host's FNV-1a lanes over raw
  float bits).
- **L3 — latency & tail.** Report per-device `clap.latency`/`clap.tail` (0 when the extension is
  absent; `UINT32_MAX` = Infinity for tail) and compensate/sum as in §3.
- **L4 — GUI (optional).** Show a device's webview UI (`clap.webview` draft ext, translated to
  `clap.gui` natively by `wclap-bridge`) — e.g. per-device panels in `test-host/`.

## 5. Testing capabilities

Like the open-webclap kit's `test-host/`, this repo ships a **browser test host** (`test-host/`) that
loads a rack bundle and plays it in realtime through an AudioWorklet — plus a **conformance harness**
(`tools/validate.mjs`, `tools/render.mjs`, `tests/conformance.test.mjs`) that any host can run against
its own implementation. The test bundle (`tests/bundles/testfx`, MIT) is a tiny gain+delay CLAP effect
used by the harness.

These live in this repo on purpose: the test host and the harness share the reference host, and a spec
repo that cannot demonstrate itself is not a standard. If the browser host outgrows the spec (bundler,
UI framework), split it into `wclap-rack-test-host` later — same pattern as WebCLAP's
`browser-test-host`.

## 6. Relation to other projects

- **WebCLAP org**: this proposal extends the WCLAP bundle format with a rack document; hosts listed
  there (`wclap-host-js`, `wclap-bridge`, `browser-test-host`) can adopt it without changes to the
  bundle format itself.
- **open-webclap**: ports WCLAPs and hosts single plugins; racks of its ports are valid rack bundles
  (its `ports/` + a `MANIFEST.json` + `rack.json`). A stock-rack demo on its site can come later.
- **openDAW**: its per-track device chain is the same shape as a rack document; import/export of rack
  bundles as track chains is a natural mapping.
- **Synth.is**: consumes rack bundles as presets in the plugin (native via `wclap-bridge`, or as a
  WCLAP itself), and evolves them in `kromosynth-burst` under the Synth.is profile (§2).
