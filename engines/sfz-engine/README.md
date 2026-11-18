# SFZ Engine (`engines/sfz-engine`)

A minimal SFZ sampler **engine plugin** as a WCLAP instrument — the reference implementation of
wclap-rack SPEC §8's file delivery (`wclap.files/1`). It exists so a rack bundle is the single
distribution vessel for sample-based instruments: `rack.json` points `source` at this bundle and
lists the instrument's files (`source.files`); the host delivers them to the module before activate
and the engine plays them. See `examples/sfz-rack/` for a complete rack (demo `.sfz` + sample →
WetChorus) and `tests/sfz-engine.test.mjs` for the end-to-end test.

Written from scratch for this repo (MIT, like the `testfx` fixture) — deliberately **not** a sfizz
port: sfizz-to-wasm needs libsndfile and friends, and kromosynth-rendered SFZ uses a known opcode
subset, so a small in-house engine covers it licence-cleanly. sfizz remains the upgrade path (swap
the in-memory file map for WasmFS and run sfizz inside; sfizz is BSD-2-Clause, see SPEC §8).

- **Plugin**: `is.synth.kromosynth.sfz-engine` "Kromosynth SFZ Engine", features `instrument` +
  `sampler`. No audio input, one stereo output; one note input port (CLAP dialect, raw MIDI note
  on/off also understood).
- **File delivery**: `clap.webview/3` `receive()` implements `wclap.files/1` (SPEC §8) — frame
  `u8 type, u8 flags, u16 reserved, payload`; type 240 = write (`u16 pathLen, path, u32 dataLen,
  data`), type 241 = clear. Every frame is logged to stdout with an FNV-1a digest (same format as
  testfx) so hosts/tests can assert exact delivery. On activate the engine picks the first delivered
  `*.sfz`, parses it, and loads the samples it references (paths relative to the `.sfz`, `\` → `/`).
- **SFZ subset**: `<global>`/`<group>`/`<master>`/`<region>` headers, `opcode=value` pairs, `//`
  comments, `"quoted"` values, lone-`\` line continuations. Region opcodes: `sample`, `key`,
  `lokey`, `hikey`, `pitch_keycenter`, `lovel`, `hivel`, `volume`, `pan`, `tune`, `offset`,
  `loop_mode` (`no_loop`/`loop_continuous`/`loop_sustain`/`one_shot`), `loop_start`, `loop_end`,
  `ampeg_attack`/`hold`/`decay`/`sustain`/`release`. Matching regions stack (one voice each).
- **WAV subset**: RIFF/WAVE, PCM 16/24/32-bit or float 32, any channel count (first two used).
  Playback is linear-interpolated at `sampleRate * 2^((key - pitch_keycenter + tune/100)/12)`.
- **Voices**: fixed pool of 32, no allocation in `process()`; sample-accurate note on/off/choke;
  amp envelope attack → hold → decay → sustain → release; velocity → gain as `vel²`; constant-power
  pan. Deterministic (no RNG).
- **Params**: id 0 "Volume" (dB, −60…+12, default 0). **State**: version + volume + sfz path.
  `clap.latency` = 0; `clap.tail` = longest `ampeg_release`. No editor page.

## Build

```sh
EMSDK=~/emsdk bash engines/sfz-engine/scripts/build.sh        # -> build/module.wasm + sfz-engine.wclap.tar.gz
```

Emscripten 3.1.51 (same as `tests/bundles/testfx`), standalone wasm, WASI-only imports
(`env.emscripten_notify_memory_growth`, `wasi_snapshot_preview1.*` only).

## Demo instrument

```sh
node engines/sfz-engine/scripts/make-demo-instrument.mjs examples/sfz-rack/files
```

generates `demo.sfz` + `samples/c4.wav` (1 s plucked sine, 16-bit PCM mono 48 kHz) used by
`examples/sfz-rack/` and the tests.

## Test

```sh
node --test tests/sfz-engine.test.mjs     # plugin-level delivery/playback + examples/sfz-rack end-to-end
node --test tests/                        # whole suite
```

After rebuilding the engine, refresh the example's copy and pin, then re-pack:

```sh
cp engines/sfz-engine/sfz-engine.wclap.tar.gz examples/sfz-rack/
# examples/sfz-rack/MANIFEST.json: "sfz-engine".sha256 = sha256 of module.wasm INSIDE the tar.gz
rm -f examples/sfz-rack/sfz-rack.wclap-rack.tar.gz
node tools/pack-rack.mjs examples/sfz-rack --out examples/sfz-rack/sfz-rack.wclap-rack.tar.gz
```
