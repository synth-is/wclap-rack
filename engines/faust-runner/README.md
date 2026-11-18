# Faust Runner (`engines/faust-runner`)

A **generic Faust-graph runner** as a WCLAP instrument — the other reference engine plugin for
wclap-rack SPEC §8 (`wclap.files/1` file delivery). Instead of baking one WCLAP per Faust graph
(the faust-aether/faust-elements ports in open-webclap are that, for static distribution), a rack
delivers ANY free-form Faust genome — a faust2wasm module plus its metadata JSON — to this engine,
and the engine runs it. See `examples/faust-rack/` for a complete rack (demo genome → WetChorus)
and `tests/faust-runner.test.mjs` for the end-to-end tests.

- **Plugin**: `is.synth.kromosynth.faust-runner`, features `instrument` + `synthesizer`. No audio
  input, one stereo output; one note input port (CLAP dialect, raw MIDI note on/off too).
- **File delivery**: `clap.webview/3` `receive()` implements `wclap.files/1` (SPEC §8) — type 240
  write / 241 clear, FNV-1a digest logging on stdout (same format as testfx / sfz-engine). On
  activate the engine picks the first delivered `*.json` + `*.wasm`.
- **Execution**: the genome wasm runs inside this module through **wasm3** (vendored in
  `third_party/wasm3`, MIT, tag v0.5.0). Its `env` math imports (`_sin`/`_sinf`/…) are linked to
  libm. Memory layout is the faust2wasm one (same as faust-wasm-dsp.js): DSP state at address 0
  (`json.size` bytes), then the u32 input/output pointer tables, then channel buffers; the genome
  memory is grown when needed. Exports used: `init`, `compute`, `getNumInputs`, `getNumOutputs`.
  **Widget writes bypass `setParamValue`**: `m3_Call`'s f32 argument packing is unreliable when
  wasm3 itself runs on wasm32, so parameters are written directly to the genome's state
  (`dsp + index`), which is exactly what the genome's own `setParamValue` does.
- **Dynamic parameters** (the Synth.is slider case): the JSON's ui leaves become the plugin's
  `clap.params` — count/info are reported after delivery, so a host can build one slider per
  parameter of whatever genome arrived. `freq`/`gain`/`gate` are note-driven and excluded.
  The reference host rescans params after `activate()` (`_scanParams`).
- **Notes**: monophonic; note on → `freq = 440·2^((key−69)/12)`, `gain = velocity`, `gate = 1`;
  note off/choke → `gate = 0`. The process block is split at event boundaries before each
  `compute` call (block-granular, ≤ 4096 frames per call).
- **State**: version + param values + delivered file paths. `clap.latency` = 0, `clap.tail` = 0.

## Build

```sh
EMSDK=~/emsdk bash engines/faust-runner/scripts/build.sh    # -> build/module.wasm + faust-runner.wclap.tar.gz
```

Emscripten 3.1.51, standalone wasm, WASI-only imports, 64-bit interpreter slots
(`-Dd_m3Use32BitSlots=0` — `m3_Call` packs host-call args into 8-byte slots).

## Demo genome

```sh
node engines/faust-runner/scripts/make-demo-genome.mjs examples/faust-rack/files
```

compiles `scripts/demo.dsp` with the Faust compiler (`$FAUST` or `~/faust/bin/faust`):
`faust -lang wasm -o genome.wasm demo.dsp` produces `genome.wasm` + `genome.json` (the JSON with
per-widget byte offsets).

## Test

```sh
node --test tests/faust-runner.test.mjs
node --test tests/
```

## Known limitation (2026-11-10)

wasm3 compiled to wasm32 (emsdk 3.1.51 / LLVM 18) mis-packs host→guest call arguments in
`m3_Call`: on wasm hosts the genome's `init(dsp, sampleRate)` receives the sample rate scaled 4×,
so pitch and envelope timing render 4× low in Node/Chrome (the genome, memory layout and param
plumbing are verified correct natively and in plain JS — see WCLAP/SESSION-LOG.md for the full
investigation). Fix upstream in wasm3 (or switch the embedded interpreter) to lift this.

After rebuilding the engine, refresh the example's copy and re-pin (MANIFEST sha256 = sha256 of
`module.wasm` inside the bundle tar.gz), then re-pack:

```sh
cp engines/faust-runner/faust-runner.wclap.tar.gz examples/faust-rack/
rm -f examples/faust-rack/faust-rack.wclap-rack.tar.gz
node tools/pack-rack.mjs examples/faust-rack --out examples/faust-rack/faust-rack.wclap-rack.tar.gz
```
