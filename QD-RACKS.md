# QD over wclap-racks — design & review notes

Status: **working scaffold, verified end-to-end** (2026-11-12). Session narrative in
`../SESSION-LOG.md` cont. 30 (and cont. 25–29 for the WCLAP delivery work this builds on).
This document is the review entry point for the implementation and its decisions.

## The question

Can wclap-rack documents serve as the genome/preset in QD (quality-diversity) runs — the same
role CPPN+DSP and free-form Faust genomes play today in Synth.is evolution runs — with NEAT as
the evolutionary blueprint (connecting to the NEAT-evolved CPPN networks)? Earlier questions to
answer: fixed vs free-form set of plugins? What can be cross-bred? Single rack type or not?
Are there meta-parameters?

## Decisions

- **The genome IS the wclap-rack v1 document** (`schema/rack.schema.json`), plus one extra field
  per device gene: `innovation` (a NEAT-style historical marking). Format tag
  `wclap-rack-genome`, version 1. `rackFromGenome()` strips the markings and validates against
  the schema, so every individual is a loadable rack (on `/wclap`, in a DAW via wclap-bridge).
- **The serial device chain is the topology.** The source device (engine bundle + delivered
  files + notes, e.g. faust-runner with a genome pair) rides as `source` — so source-bearing and
  source-less racks are both genomes.
- **Crossover = NEAT gene alignment by innovation id**: matching genes (same structural origin)
  take params from the fitter parent or a 50/50 per-param pick; disjoint/excess genes are
  inherited from the fitter parent only. Chain order follows parent A, then B's extra genes.
  The source comes from the fitter parent with params blended 50/50 (and notes picked at random).
- **Mutations**: param perturbation (gaussian, σ = 10% of the param's clap.params [min,max],
  clamped), add device from the pool (new innovation id), remove device, swap device, toggle
  bypass, adjacent reorder, source-param perturbation, note key/velocity mutation. Weights are
  configurable.
- So: **not a fixed single-type rack** — a free-form chain over a pool of built fx ports, and
  the "meta-parameters" are exactly device params + on/off + order + source choice.
- **Seeds**: any rack bundle directory/archive; in the web app, combinations saved to the Sound
  Garden on `/wclap` (metadata kind `wclap-rack-combination`) are seed candidates directly.
- **Evaluation = the reference host's `renderRack()`** (deterministic offline render over wasm
  hosts, `reference-host/rack.mjs`), descriptors from `describeBuffer()` (spectral centroid via
  Goertzel bins, rms, peak, zero-crossing rate, crest factor) mapped onto a **per-run CV grid**
  (ranges measured from the seed render ±25%, `--cells` per dim). MAP-Elites: keep the
  best-quality elite per cell; breed = random elite + crossover with another elite + 1–3
  mutations (85%), else a re-mutated seed (15%). Fully seeded (`mulberry32`, `--seed`), so a run
  directory reproduces.
- **Quality is a PLACEHOLDER**: finite render with peak near 0.5 (not silence, not clipping).
  The real objective — server CLAP/latent descriptors (the `clap-*-2d` CVs of the evo runs) or
  human liking via the recommend service — plugs into `evaluate()` in `tools/qd-rack.mjs`.

## File map

```
wclap-rack/
├── tools/
│   ├── rack-genome.mjs     # pure genome logic: genomeFromRack, rackFromGenome (validates),
│   │                       #   mutateGenome, crossoverGenomes, describeBuffer, cellOf, mulberry32
│   └── qd-rack.mjs         # the QD loop CLI (see below)
├── tests/
│   └── rack-genome.test.mjs  # 7 tests: round-trip validity, rng/mutation determinism, param
│                             #   clamping, crossover innovation alignment, descriptors, cellOf
└── reference-host/host.mjs # bug fix: clap host callbacks are now arrow functions (see below)
```

## Running it

```sh
# pool = a directory of built WCLAP fx bundles (*.wclap.tar.gz, e.g. from open-webclap/build)
node tools/qd-rack.mjs --seed examples/effects-rack --pool /path/to/pool \
  --generations 10 --population 24 --seconds 1 --rng-seed 1 --cells 8 \
  --cv spectralCentroid,zcr --out runs/my-run

# as run overnight 2026-11-12 (see "Review and first overnight run" below)
node tools/qd-rack.mjs --seed examples/synth-is-faust --pool /path/to/pool \
  --generations 100000 --max-minutes 420 --population 32 --rng-seed 1 --cells 16 \
  --quality gate --fixed-source --render-elites \
  --cv spectralCentroid,flatness \
  --cv-range spectralCentroid:100:8000:log,flatness:1e-9:0.1:log --out runs/faust
```

Smoke run verified 2026-11-12 (3 generations, population 6, 0.25 s, seed 7, pool of 5 bundles →
10 fx plugins): archive filled 3→4→6 cells, `best q=0.8023`.

Output run directory:

```
runs/my-run/
├── run.json            # config: seed, generations, cv dims, pool, seed rack name
├── gen-000.json …      # per-generation: archive size, top-5 elites, all individuals (devices, fp)
├── archive.json        # cell -> { rack, cv, quality, fingerprint, generation }
├── elites/elite-XXX-qY/rack.json   # schema-valid rack docs, sorted by quality
└── best.wav            # render of the single best elite (quick listen)
```

## Bug found while wiring this up (fixed)

`reference-host/host.mjs` implemented the clap host callbacks (`requestCallback`,
`requestRestart`, `paramsRescan`, …) as object-literal shorthand methods. They are exported
through a wasm table and invoked by the guest as raw function pointers, where `this` is
`undefined` in ESM strict mode — so any plugin calling `host.request_callback` crashed the
render (`TypeError: Cannot set properties of undefined (setting 'callbackWanted')`); the
effects-rack's limiter does exactly that. Fixed by making them arrow functions. This is why
`renderRack` on `examples/effects-rack` failed before and works now (fp `70968a6cf9949fdb`).

## Verification

- `node --test tests/` — **53/53 pass** (45 pre-existing + 7 new rack-genome + 1 from the fix's
  coverage of previously unexercised paths).
- Smoke QD run above; every written `elites/*/rack.json` re-validates with `validateRackDoc`.
- Determinism: same `--seed` → same mutations (tested); `renderRack` itself is bit-deterministic
  (the conformance renderer asserts two renders fingerprint-equal).

## Open questions / review checklist for the next session

1. **Real objective & CV** — replace the placeholder quality in `evaluate()` with server CLAP
   descriptors (how the evo runs do `clap-acoustic-2d` etc.) or liking from the recommend
   service; consider multi-note renders (chords) not just the seed's notes.
2. **Source genomes** — today the source device is inherited/blended as a block. The deeper NEAT
   move: cross-breed the *inner* genome (faust-runner's genome.wasm/json, cppn-engine's CPPN) —
   that is where NEAT proper already lives in Synth.is; decide whether rack-level QD should
   compose with it or stay at chain level.
3. **Scale** — population sizes, parallel renders (renderRack is per-process; worker pool),
   seconds per eval, preRoll.
4. **Persistence/integration** — write runs into the `synth.is/qd/evoruns` layout, expose archive
   entries on `/wclap` ("used with" data) and as downloadable `.wclap-rack.tar.gz` (pack via
   `tools/pack-rack.mjs`), feed the evolution manager on the mac studio.
5. **Pool governance** — licence per pool entry (the /wclap picker already surfaces it); decide
   whether GPL-3.0-only ports may enter QD populations that get published.
6. **CV grid ranges** — currently per-run from the seed ±25%; a corpus-wide fixed range would make
   archives comparable across runs.

## Reproducibility

- Everything is seeded: `node tools/qd-rack.mjs … --seed N` reproduces mutations exactly;
  rendering is deterministic per the reference host. Re-run the smoke command above to confirm.
- Review order for code: `tools/rack-genome.mjs` (pure logic, ~250 lines) →
  `tests/rack-genome.test.mjs` → `tools/qd-rack.mjs` (loop + I/O) → the one-line host fix.

## Review and first overnight run (2026-11-12, later the same night)

### Fixed before the run (tools/qd-rack.mjs, tools/rack-genome.mjs)

1. **The RNG seed was never applied.** `--seed` is the seed *rack* and the RNG seed was read from
   `--seed` too, so it was `Number('<path>')` = NaN, which mulberry32 turns into state 0. Every
   earlier run shared one RNG stream and recorded `"seed": null` in run.json. The RNG seed is now
   `--rng-seed N`. Checked: the same seed gives identical fingerprints, a different seed gives
   different ones.
2. **The placeholder quality rewarded level.** "Peak near 0.5" lets a gain change win a cell. The
   Faust example sources also render about +10 dB over full scale (seed peak 3.4), so a "not
   clipped" check on raw float output rejected the dry seed itself. New `--quality gate` is
   level-blind: it fails only non-finite, silent, DC-dominated or blown-up renders, and is
   otherwise pass/fail, so a cell keeps its first occupant (like the burst campaign's
   `--fitness none`). The old mode is still the default.
3. **The descriptors only heard the first 85 ms, and the centroid was compressed.**
   `describeBuffer` took 24 Goertzel probes over one 4096-sample window of the left channel. A
   reverb, a chorus and a dry note all read 180–370 Hz. It is now a Hann-windowed 4096-point FFT
   of the mid signal, averaged over up to 8 windows spread across the buffer. A sine reads
   440.1 Hz, and white noise reads 12 kHz. `flatness` (geometric/arithmetic mean of power) is a
   new descriptor.
4. **The grid range was the seed ±25%,** so most offspring clamped into edge cells.
   `--cv-range key:min:max[:log]` sets fixed ranges, optionally log-scaled; flatness spans about
   8 decades. One range is shared by all sources, so their archives are comparable (open question
   6).
5. **archive.json was written only at the end;** it is now written atomically every generation.
   `--max-minutes` time-boxes a run, `--render-elites` writes a −1 dBFS normalised `render.wav`
   next to every elite, and `best.wav` is normalised too.
6. **`--fixed-source`** sets the source-param and note mutation weights to 0, so only the rack
   evolves (the 2026-10-31 effect-racks design's stage 1). gen-XXX.json now records every
   individual's descriptors and errors.

`node --test tests/rack-genome.test.mjs`: 7/7. On 2026-11-12 at 01:16, three cppn-engine tests
started failing because another session was rebuilding that engine in this checkout; the example
racks carry their own engine copy, so they are unaffected.

### Not fixed: for review

- **Innovation ids never repeat.** Every add or swap mints a fresh id, so two lineages that add
  the same plugin never align, and crossover only ever matches the seed's own devices. NEAT reuses
  an id for the same structural innovation (a per-generation registry). For a serial chain the
  natural key is `(bundle, plugin)` first introduced, possibly with a slot context. The comment in
  `genomeFromRack` claims sharing that doesn't happen.
- **Crossover ties.** With `--quality gate` every elite has quality 1, so `fitnessA >= fitnessB`
  always holds: A is always the "fitter" parent and B's disjoint genes are never inherited. Break
  ties at random.
- **Param mutation.** `perturbParams` moves every numeric param of the device at once, with
  uniform noise (the doc says Gaussian). Stepped and enum params get continuous values (read the
  clap.params `stepped` flag). `blendSource` averages source params, which is invalid for enums.
- **Speed.** `renderRack` compiles every bundle's wasm on every evaluation. Keeping one host per
  bundle across evaluations, with the existing `snapshot()`/`restore()`, should be a large win.
  Today it's about 0.8 s/eval for the Faust source on the Studio.
- **Cross-machine determinism.** The same seed render fingerprints `d596db2b…` on the laptop
  (Node 22) and `4869be67…` on the Studio (Node 26). Deterministic per machine only, so run.json
  should record the Node version.
- **Cells are not diversity** (risks 1–2 of the effect-racks design): a centroid × flatness grid
  rewards EQ and saturation recolouring over chorus, delay and reverb. The measures that answer
  "new sound or recoloured?" are CLAP `dSeed` (distance to the dry source) and `N_new`
  (distinguishable sounds beyond the sources), plus blind listening.

### Relation to the 2026-10-31 effect-racks design (Beads epic "Effect racks over any substrate: measure first")

That design (`synth.is/kromosynth-projects/Evolutionary Sound Discovery Platform/effect-racks/`)
builds racks inside kromosynth-burst: genome v1, a loudness-matching master and the Faust units
are done; the WebCLAP unit backend and host audio input are still open. The two efforts overlap:

- **They agree:** the rack works after any source; dry-source-fixed first (`--fixed-source`
  here).
- **They differ:** free-form chain + NEAT crossover (here) vs **fixed slots + bypass** (decision
  D4 there, chosen for clean comparison and easy export). Here there is no loudness-matched master
  (D4's "wet never louder than dry") and no `C0` control arm.
- **Complement:** this repo's reference host already does audio input, file delivery and
  latency, which are the open items there. The rack document is the natural export format for
  burst's rack genomes too.

### Overnight run (Mac Studio, started 2026-11-12 01:23 GMT, 7 h time box)

- Code snapshot: `~/qd-racks/code` (this repo, rsynced, uncommitted state).
- Pool: `~/qd-racks/pool`, 15 open-webclap effect bundles → 18 fx plugins. Internal only: the
  pool includes GPL ZL/SC-Dark builds, so nothing from it is published.
- Three processes, one per source, rng-seed 1, population 32, a 16×16 log grid, centroid
  100–8000 Hz × flatness 1e-9–0.1:
  `synth-is-faust`, `synth-is-cppn`, `synth-fx-rack` (Aether).
- Output: `~/qd-racks/run-2026-11-12/<source>/` (archive.json, gen-*.json,
  elites/*/{rack.json,render.wav}).
- Guard: `rss.log` samples RSS every 2 min and kills a run above 2.5 GB.
  **Stop everything: `touch ~/qd-racks/STOP`.** Relaunch: `~/qd-racks/launch.sh` (MIN=minutes).
- What it tests: does it stay up overnight on a loaded Studio, is memory flat, how does the
  archive fill, and do the elites sound like new sounds or recoloured ones. It is a pipeline soak
  and a listening set, NOT the pre-registered measurement (no control arm, one rng seed, no
  CLAP).

### Added the evening of 2026-11-12

- `--max-evals N`: an evaluation budget, so arms on a loaded machine compare by work done rather
  than wall time.
- `--random`: the random-rack baseline arm, via `randomGenome()` in `tools/rack-genome.mjs` (1–6
  pool devices with uniform params over the fixed source; 2 new tests, 9/9).
- Every archive entry now records `evalIndex`, and run.json records `mode`, `maxEvals` and the
  Node version.

The runs and their analysis are described in `synth.is/kromosynth-projects/Evolutionary Sound
Discovery Platform/effect-racks/RACKS_AS_BIOMES_FIRST_RESULTS_2026-11-12.md` §7.
