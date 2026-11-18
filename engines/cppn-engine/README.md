# cppn-engine — real-time CPPN+DSP substrate engine (WCLAP)

`is.synth.kromosynth.cppn-engine` — a CLAP instrument that runs a Synth.is **CPPN+DSP genome live,
per sample, inside a WCLAP plugin**, the real-time counterpart of kromosynth's sample-based (SFZ)
delivery of the same sounds, built like `engines/sfz-engine`. **Licence:** our source is MIT (LICENSE); the
built engine (`cppn-engine.wclap.tar.gz`) is GPL-3.0-only (COPYING) because it links the Faust
`vital_rev` reverb. NOTICE has the details.

In this substrate a genome is two networks: a `waveNetwork` (a CPPN whose outputs are signals —
audio-rate waves and/or control-rate envelopes/LFOs, each rendered at its own frequency) and an
`asNEATPatch` (the DSP graph those signals feed, which kromosynth realizes as a Web Audio /
virtual-audio-graph patch). This engine evaluates **both** in real time:

- `src/cppn_core.h` — a dependency-free C++ port of neatjs `networkDecode` + cppnjs
  `recursiveActivation` + kromosynth's `CpuBackend` input schemes and activation functions.
  **Bit-exact vs neatjs/cppnjs**: maxAbsDiff = 0.0 over 1,728,000 samples on the real genome
  01KR0R5T4S7SQF4W8M1G99CGS9 in `WCLAP/poc-cppn-wasm`, and 0.0 end-to-end through this plugin's
  CLAP path (default graph) over 24,000 samples. ~0.06 ms per 128-frame block @ 48 kHz for the whole
  21-node/18-output network — the full genome with its 13-signal, 9-node patch renders 1 s of audio
  in ~12 ms of wasm (~1% of one audio block per voice).
- `src/cppn_graph.h` — the asNEAT patch, parsed and compiled once into an index-based plan and
  rendered per sample (no strings, maps or allocation while sounding). Its specification is the
  **production renderer** (`renderWithWorkletOffline` in kromosynth-render, `network-rendering.js` in
  'streaming' mode, `patchFromAsNEATnetwork`) and the Rust `web-audio-api` crate's node maths, not the
  legacy batch renderer the first version copied. The rules are listed at the top of that file.
  Implemented: `gain`, `biquadFilter`, `oscillator`, `bufferSource`, `wavetable`, `additive`,
  `waveShaper`, `delay`, `dynamicsCompressor`, `convolver`, and `faust`: every Faust template of the
  platform (kromosynth/faust-templates, 39) is compiled to C++ and linked in (src/faust_nodes.h,
  src/faust_templates/, regenerated with scripts/gen-faust-templates.sh). `feedbackDelay` is silent, as
  it is in the production renderer. Not yet: genomes with one CPPN per frequency. A Faust template
  this build does not have is silent and named as `unsupported=faust:<name>` in the load log.
  Measured against the production renderer with `WCLAP/poc-cppn-wasm/compare.mjs`: sample-exact on the
  deterministic test genomes (residual −92 to −139 dB, level 0.0 dB); see `WCLAP/SESSION-LOG.md`,
  "cppn-engine parity review".
- The genome's own pitch is at **key 60**, as in kromosynth-vi's sample instruments. Parameters 7
  "DC Filter" and 8 "Auto Level" (both on by default) apply the renderer's delivery rule: the root
  note is rendered once at load to get its mean and peak; the mean is subtracted and the level divided
  by the peak when it exceeds 1.

## Pre-rendered playing mode (for genomes too heavy to compute live)

Some genomes (40-75 patch nodes, convolvers, the bell and marimba Faust templates, jpverb) run at only
1-4x real time for one voice, so 8 voices cannot be computed live. Parameter 15 **Pre-render**
(0 Auto, 1 Live, 2 Pre-rendered; Auto by default, kept in the state from version 6):

- **Auto** decides at load from the speed of one voice: below 12x real time the genome is pre-rendered.
  The speed is estimated from the compiled plan (`cgraph::costFeatures`, weights fitted on 49 genomes and
  the 39 Faust templates by `WCLAP/poc-cppn-wasm/cost-calibrate.mjs`), because our hosts give the plugin
  a virtual clock; if the clock does move during the load-time level measurement, the measured speed is
  used instead.
- **Pre-rendered:** keys 24-96 are rendered into buffers (48 kHz mono float, the note plus its release),
  the root first, then every third key outward, then the keys between, within 64 MB (the grid widens to
  every 4th/6th/12th key for long notes). Rendering runs in slices at the end of each `process()` call,
  using half of the block's time minus the live voices sounding (no thread is available: the browser's
  AudioWorklet is one thread, the reference host has no thread pool). A note plays its own key's buffer
  (equal to the live note sample for sample), else a neighbour a semitone away resampled (cubic
  Hermite), else the nearest planned key, else a live voice if one fits (at 1.6x real time or more).
  Otherwise its key is **rendered at once and heard as it renders**: the note waits until the render is
  far enough ahead that it will never be caught (checked at the start, where a convolver's block
  boundaries stop, and at the end, with two process blocks plus two convolver blocks of margin), then
  plays from the buffer while it is still being filled. While a note waits for or plays from that render,
  the render gets 60% of the block. A 30 s note starts after about 2.5 s on the slowest genomes measured
  (2 s at 1.65x, 0.2 s for a 2 s note). Keys rendered with older settings are never played. One velocity
  layer (velocity 1; a note's velocity is then a gain, as live, but the CPPN's velocity input is not
  followed), one noise seed per key.
- Moving a pad, Duration, Pitch, CPPN X/Y, Limiter, DC Filter or a longer Release makes every rendered
  key out of date; re-rendering starts 0.3 s after the last change, or at once for a key that is played.
  A played key also goes before the new level measurement (redone afterwards; with the DC Filter on, a
  key rendered before it is redone if the measured offset moved).
- **Convolvers:** a convolver's level needs one CPPN pass over the whole note before its first sample.
  Live, that pass runs in slices at the end of the following blocks (60% of the block while nothing else
  sounds) and the note waits for it, instead of stalling the audio thread (it took 0.76 s at one go for a
  30 s note). Its block products are done all at once every 512 samples and grow along the note; the
  render budget counts them (`kConvPairUs`).
- **Offline (clap.render):** a host rendering offline gets everything live and the convolver pass done at
  note on, so no note waits.
- The page shows the mode in a status line (frame 83), and "Preparing a note … about N s" while a note
  waits (frame 83 bytes 24-31: notes waiting, seconds left as a float).

## Files (wclap.files/1, SPEC §8 — before activate, type 240/241 frames, FNV-1a digest on stdout)

- `*.json` with an `offspring` (raw Synth.is API envelope `{genome:{waveNetwork:{offspring,...}}}`
  accepted, as are a bare waveNetwork or a bare offspring) — the CPPN genome. The `asNEATPatch` may
  be embedded (string or char-split object) or omitted.
- optionally a second `*.json` with `{audioGraph, networkOutputs}` — a pre-converted synthIsPatch
  (what kromosynth's `patchFromAsNEATnetwork` produces; the web app can deliver it directly).
- with no patch at all, a default graph plays CPPN output 0 at the note frequency.

## Parameters (clap.params)

`0` Volume (dB, -60..12), `1` Duration (s, 0.05..30 — the render length of one note),
`2` Attack (s), `3` Release (s), `6` Pitch (semitones, -36..+36, added to the note like the web app's
noteDelta), `4`/`5` "CPPN X"/"CPPN Y" (0..1 — hybrid-phase navigation coords, marked
CLAP_PARAM_IS_HIDDEN unless the genome is hybrid-phase; the web app's geometric weight pads instead
re-deliver a weight-adjusted genome via `wclap.files/1`), `11`-`14` Weights X/Y and Frequencies X/Y,
`15` Pre-render (above). Notes drive pitch (buffer outputs
transpose) and the CPPN velocity input. 8 voices, deterministic, no allocation in `process()`.
No editor page.

## Build / re-pin

`engines/cppn-engine/scripts/build.sh` (emcc 3.1.51, same discipline as sfz-engine) produces
`build/module.wasm` and packs `cppn-engine.wclap.tar.gz`. After a rebuild, re-pin the
`module.wasm` sha256 in `build/BUILD-INFO.txt` into any example `MANIFEST.json` that references the
bundle (see `examples/synth-is-cppn/MANIFEST.json`) and re-pack the example rack:

```
node tools/pack-rack.mjs examples/synth-is-cppn --out examples/synth-is-cppn/synth-is-cppn.wclap-rack.tar.gz
```

Tests: `tests/cppn-engine.test.mjs` (delivery digest, params, audible + deterministic + silent
tail, exact 440/220 Hz pitch on a synthetic genome, example rack from dir and packed archive).
Run `node --test tests/` from `wclap-rack/`.
