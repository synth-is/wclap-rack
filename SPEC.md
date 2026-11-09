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
- `source` (optional): an **instrument device** at the head of the rack — a bundle/plugin with the
  `instrument` feature. It is rendered from `notes` (MIDI note on/off events; `frame`/`frames` are in
  frames relative to the start of the audio) with no audio input, and its output becomes the stream the
  `devices` process. Without a `source`, the host supplies the input audio (a DAW track, a rendered
  substrate, a test signal). `source.length` sets the rendered length in frames when no input audio is
  given (default one second). This is what makes a rack bundle fully self-contained: "Aether + chorus +
  reverb" is one portable document. `source.kind` (optional hint: `"sampler"`, `"synth"`, …) and
  `source.files` (instrument files the plugin needs, e.g. an SFZ and its samples, a Faust genome wasm) are
  defined in §8 — engine plugins are how a host delivers *any* instrument through the same rack vessel.
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
2. If the rack has a `source`, render it first: one host per bundle, `snapshot()` after load,
   `restore()` before its render, create the plugin, activate at the host sample rate, load `state`,
   apply `params` at frame 0, send the `notes` as note events (shifted by `preRoll`), and render
   `preRoll + length` frames with no audio input. Its stereo output is the stream.
3. For each device in order: same host discipline (`restore()` before every device render, so devices
   share a bundle deterministically), load `state`, apply `params` at frame 0, render the whole stream
   through the device's audio input port 0.
3. Each device's reported `clap.latency` is compensated **at that device** (its output is realigned),
   so the rack output is sample-aligned with the rack input and has the input's length. The tail
   beyond the input length is the render buffer's business, not the rack's.
4. A bypassed device is skipped entirely (bit-transparent).

**Realtime rendering** (AudioWorklet, native plugin): devices run in `process()` in order. Do NOT drop
frames; instead report the **sum of active devices' `clap.latency`** (and the max `clap.tail`, plus the
source's latency) upward — CLAP convention lets the host/DAW compensate. Note events route to the
`source` device; its audio output feeds the first effect. One host per bundle, plugins created once and
kept alive — do NOT call `restore()` between device creations in realtime, it rewinds the whole module
memory and invalidates live plugins sharing that bundle (`restore()`-per-render is an offline-only
discipline). See `test-host/rack-worklet.js`.

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

## 7. Reserved for v2: parallel sends / multiple chains

v1 is deliberately serial (§2: `devices` run in array order over one stereo stream). The Reason-style
"parallel sends" shape — one source feeding several chains that are summed, with per-send gain — is a
v2 routing extension, reserved here so v1 hosts and UIs stay compatible:

- `devices[i].out` (optional, default `"main"`): the bus this device's output goes to — `"main"`, or
  a bus name declared in a top-level `buses: [{ "name": "fx", "gain": 1.0 }]`.
- `devices[i].sends` (optional): `[ { "to": "fx" | <device index> | "main", "gain": 0.5 } ]` — the
  device's output is ALSO copied to the target bus/device input at `gain`, in addition to its `out`.
- `source.sends` may appear with the same shape (the source can feed several chains directly).

Semantics when present: each bus sums its inputs; `main` is the rack's output; a device whose `out`
is a bus feeds that bus instead of the serial stream. v1 hosts MUST ignore unknown fields only if
they also ignore the routing effect — i.e. a v1 host rendering a v2 document may produce a different
(musically wrong) result, so v1 hosts SHOULD refuse `version: 2` documents rather than misrender
them, and `version` stays `1` for purely serial racks.

UI note (2026-11-09): the device-line UIs built so far (Synth.is plugin shell, Strudel rack viz panel,
kromosynth-desktop sound cards) list devices with per-device view/params/bypass and are
topology-agnostic — a v2 rack renders as the same list, with send controls added per line later. No
UI work needs to change for v2 beyond that.

## 8. Engine (source) plugins and file delivery

A `source` plugin is often an **engine plugin**: a general player (SFZ sampler, Faust-graph runner)
that needs *files* at runtime — an SFZ plus its samples, a Faust genome's wasm + JSON. A WCLAP module
gets no file system from its host by default, so the rack document carries the files and the host
delivers them:

- `source.files` (optional): an array of `{ "path": "<path inside the module>", "archive": "<path inside
  the rack bundle archive>" }`. Every `archive` file is part of the rack bundle (and so covered by the
  bundle's licence texts). The host MUST make each file's bytes available to the source plugin at `path`
  **before activate** (before the first render / before note events).
- Delivery mechanism **`wclap.files/1`**: over the `clap.webview/3` port channel, before activation, the
  host sends JSON frames `{"type":"wclap.files/1","op":"write","path":"<path>","data":"<base64>"}` (and
  `{"op":"clear"}` on unload). The module writes them into its own writable file system (e.g. Emscripten
  WasmFS). This one mechanism works identically in a browser worklet, Node, and a native host.
  A native host MAY instead preopen a directory (`wclap_open_with_dirs`) and place `path`s under the
  preopen mount, letting the module read them through WASI directly — but it MUST do so only in addition
  to being able to serve `wclap.files/1`, since browser hosts have no preopens.
- `source.kind` (optional, non-normative hint): `"sampler"` when the source is a sample-based instrument
  whose files include an `.sfz`, `"synth"` for a generated-DSP instrument, etc. Hosts that cannot run wasm
  at all (e.g. iOS, where JIT is forbidden) MAY use the hint + `files` to fall back to a native equivalent
  player instead of running the WCLAP; a host that can run wasm MUST run the WCLAP as specified.

Rationale: this is what makes the rack bundle the **single extraction/distribution vessel** — a CPPN+DSP
genome rendered to SFZ, a free-form Faust genome, and a ported synth (Aether, …) all ship as
`source` = engine plugin (or the ported plugin itself) + `devices` = effects, and load through one path
in every host. The engine plugins' own licences ride in their WCLAP bundles as usual (sfizz is
BSD-2-Clause; Faust-generated DSP code is the instrument author's own), so engine plugins do not tighten
the licence posture of a public palette.
