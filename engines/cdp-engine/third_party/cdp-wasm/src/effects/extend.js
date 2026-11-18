// Effect entries for CDP's `extend` program.
// Entry shape and arg-token grammar: see the header comment in ../effects.js.

export default [
  // ==========================================================================
  // Phase B — new programs added to the build (extend, pitch, repitch, glisten).
  // ==========================================================================
  {
    id: 'extend.loop', label: 'Loop segment', category: 'Extend & segment',
    program: 'extend', domain: 'sound', mono: false,
    args: ['loop', '3', '$IN', '$OUT', { p: 'cnt' }, '0', { p: 'len' }],
    // The loop must fit between two 15 ms splices inside the file (tklib1.c,
    // case LOOP: hi = dur·1000 − 30 ms) and be strictly longer than the two
    // splices itself (extprepro.c). The hardcoded start of 0 additionally needs
    // the file to exceed 30 ms; below 61 ms no loop length is valid at all.
    minSrcDur: 0.061,
    params: [
      { name: 'cnt', label: 'Repeats', min: 1, max: 32, default: 6, step: 1, help: 'How many times the looped segment plays.' },
      { name: 'len', label: 'Loop length (ms)', min: 40, max: 1000, default: 200, step: 10,
        hardMin: 31,                                       // > 2 × 15 ms splice, strict
        srcMin: (s) => Math.max(31, Math.min(40, s.dur * 1000 - 30)),
        srcMax: (s) => Math.min(1000, s.dur * 1000 - 30),  // ap->hi = dur·1000 − 2 splices
        help: 'Length of the segment taken from the start of the file to loop, in milliseconds. Must fit inside the file with room for its splices.' },
    ],
    blurb: 'Loop a segment from the start of the file a number of times.',
  },
  {
    id: 'extend.iterate', label: 'Iterate (fluid)', category: 'Extend & segment',
    program: 'extend', domain: 'sound', mono: false,
    parityExempt: 'random pitch/time scatter (rand)',
    args: ['iterate', '2', '$IN', '$OUT', { p: 'reps' }, { p: 'pshift', flag: '-p' }, { p: 'rand', flag: '-r' }],
    params: [
      { name: 'reps', label: 'Repetitions', min: 1, max: 32, default: 6, step: 1, help: 'How many times the sound is re-sounded.' },
      { name: 'pshift', label: 'Pitch scatter', min: 0, max: 12, default: 2, step: 0.5, help: 'Maximum random pitch variation between repeats, in semitones. 0 keeps every repeat at the same pitch.' },
      { name: 'rand', label: 'Time scatter', min: 0, max: 1, default: 0.2, step: 0.05, help: 'Maximum random variation in the timing of repeats (0–1). Higher gives a looser, more fluid feel.' },
    ],
    blurb: 'Iterate the whole sound in a fluid manner with random pitch/time scatter.',
  },
  {
    id: 'extend.doublets', label: 'Doublets', category: 'Extend & segment',
    program: 'extend', domain: 'sound', mono: false,
    args: ['doublets', '$IN', '$OUT', { p: 'segdur' }, { p: 'repets' }],
    // The file must hold two 5 ms splices and leave a tail after the first
    // complete segment: at exact EOF do_doubling exits before writing it.
    minSrcDur: 0.016,
    params: [
      { name: 'segdur', label: 'Segment (s)', min: 0.02, max: 1, default: 0.15, step: 0.01,
        hardMin: 0.01,                                    // 2 × 5 ms splice
        srcMin: (s) => Math.max(0.01, Math.min(0.02, s.dur - 0.005)),
        srcMax: (s) => Math.min(1, s.dur - 0.005),        // reserve the final 5 ms down-splice/tail
        help: 'Length of each chunk the source is cut into, in seconds.' },
      { name: 'repets', label: 'Repeats', min: 2, max: 8, default: 2, step: 1, help: 'How many times each chunk is repeated before moving on, giving a stutter.' },
    ],
    blurb: 'Cut the source into segments and repeat each one (stuttering).',
  },
  {
    id: 'extend.zigzag', label: 'Zigzag', category: 'Extend & segment',
    program: 'extend', domain: 'sound', mono: false,
    parityExempt: 'random zigzag points (rand)',
    args: ['zigzag', '1', '$IN', '$OUT', '0', { p: 'end' }, { p: 'dur' }, { p: 'minzig' }],
    // Below 0.1 s no parameter combination is valid at all: `end` is capped at
    // the source duration, and minzig's floor (0.04) only fits under its
    // end/2 - 0.01 ceiling once the source reaches 0.1 s. Shorter sources must
    // be rejected up front — paramRange would collapse the inverted span to a
    // minzig that looks usable but sits inside the infinite-loop condition
    // documented on the parameter.
    minSrcDur: 0.1,
    // Every bound here is CDP's own, read off dev/cdp2k/tklib1.c `set_param_ranges`
    // (case ZIGZAG) — which the program fills from the infile's duration, so the
    // three source-relative ones are engine limits, not taste. The exception is
    // the output ceiling: CDP's is BIG_TIME (32767 s), no use as slider travel.
    params: [
      { name: 'end', label: 'Window end (s)', min: 0.15, max: 2, default: 0.5, step: 0.05,
        hardMin: 0.031,                                   // (2 * splice) + 1ms unspliced
        // A source shorter than the curated floor drops the floor toward the
        // engine's, so the span stays inside the file rather than collapsing to
        // an end time past it.
        srcMin: (s) => Math.max(0.031, Math.min(0.15, s.dur)),
        srcMax: (s) => s.dur,                             // ap->hi = the infile duration
        srcDefault: (s) => s.dur,
        help: 'End of the region (from the start of the file) that is read back and forth, in seconds. Cannot run past the end of the source.' },
      { name: 'dur', label: 'Output (s)', min: 0.5, max: 8, default: 2, step: 0.5,
        hardMax: 32767,                                   // BIG_TIME
        srcMin: (s) => s.dur + 0.01,                      // ap->lo = duration + FLTERR
        srcMax: (s) => Math.min(32767, Math.max(8, s.dur * 8)), // never past BIG_TIME
        srcDefault: (s) => s.dur * 2,
        help: 'Length of the output, in seconds. The zigzagging is followed by the rest of the source, so this has to be longer than the source itself.' },
      { name: 'minzig', label: 'Min zig (s)', min: 0.04, max: 0.45, default: 0.1, step: 0.01,
        hardMin: 0.031,
        // Below 2s because maxzig, which we leave at its default of
        // min(2, dur - 30ms), must stay above minzig (check_zigzag_consistency).
        srcMax: (s) => Math.min(1.99, s.dur - 0.03, Math.max(0.45, s.dur / 4)),
        // The window must hold a zig in BOTH directions: at end <= 2 * minzig
        // generate_zigzag_table() spins forever (extprepro.c) — its own guard
        // only catches half that. Bounding minzig alone is enough: lowering the
        // window re-bounds it here (refreshAll iterates to a fixed point), and
        // leaving `end` uncoupled keeps its slider on its own step.
        maxOf: (v) => (Number(v.end) / 2) - 0.01,
        help: 'Shortest single back-or-forth sweep, in seconds. Smaller allows faster, more jittery zigzagging; it has to fit twice inside the window.' },
    ],
    blurb: 'Read back and forth (zigzag) inside the first part of the file.',
  },
  {
    id: 'extend.freeze', label: 'Freeze / sustain', category: 'Extend & segment',
    program: 'extend', domain: 'sound', mono: false,
    parityExempt: 'randomised iteration (rand)',
    args: ['freeze', '1', '$IN', '$OUT', { p: 'outdur' }, { p: 'delay' }, { p: 'rand' }, { p: 'pshift' }, { p: 'ampcut' }, { p: 'start' }, { p: 'end' }, { p: 'gain' }],
    // tklib1.c (ITERATE_EXTEND): the output must be at least the whole source
    // long (ap->lo = duration — zigzag's twin), start/end lie within the file,
    // and extprepro.c wants the frozen chunk over 512 samples per channel
    // (~12 ms at 44.1 kHz — the 13 ms margins below) and no shorter than the
    // delay between re-soundings.
    minSrcDur: 0.02,
    params: [
      { name: 'outdur', label: 'Output (s)', min: 0.5, max: 20, default: 4, step: 0.5,
        hardMax: 32767,
        srcMin: (s) => s.dur,                             // ap->lo = the infile duration
        srcMax: (s) => Math.min(32767, Math.max(20, s.dur * 4)),
        srcDefault: (s) => s.dur * 2,                     // CDP's own default
        help: 'Total length of the stretched-out (frozen) result, in seconds. Cannot be shorter than the source: the freeze is followed by the rest of the file.' },
      { name: 'start', label: 'Freeze from (s)', min: 0, max: 60, default: 0, step: 0.05,
        srcMax: (s) => Math.max(0, s.dur - 0.013),
        help: 'Start of the segment in the source that gets frozen, in seconds.' },
      { name: 'end', label: 'Freeze to (s)', min: 0.05, max: 60, default: 0.5, step: 0.05,
        minOf: (v) => Number(v.start) + 0.013,            // chunk > 512 samples/chan
        srcMin: (s) => Math.min(0.05, s.dur),             // short sources must not collapse above EOF
        srcMax: (s) => s.dur,                             // ap->hi = the infile duration
        srcDefault: (s) => Math.min(0.5, s.dur),
        help: 'End of the frozen segment in the source, in seconds. Must lie within the file.' },
      { name: 'delay', label: 'Iter delay (s)', min: 0.01, max: 1, default: 0.1, step: 0.01,
        maxOf: (v) => Number(v.end) - Number(v.start),    // chunk must hold one delay
        help: 'Average gap between re-soundings of the frozen segment (must be ≤ the segment length).' },
      { name: 'rand', label: 'Delay scatter', min: 0, max: 1, default: 0.2, step: 0.05, help: 'Randomisation of the delay between iterations (0–1).' },
      { name: 'pshift', label: 'Pitch scatter', min: 0, max: 12, default: 1, step: 0.5, help: 'Maximum random pitch shift per iteration, in semitones (up or down).' },
      { name: 'ampcut', label: 'Amp scatter', min: 0, max: 1, default: 0.2, step: 0.05, help: 'Maximum random amplitude reduction per iteration (0–1).' },
      { name: 'gain', label: 'Gain', min: 0.25, max: 4, default: 1, step: 0.05, help: 'Level adjustment of the frozen segment.' },
    ],
    blurb: 'Freeze a segment and re-sound it to fill a longer duration (granular sustain).',
  },
  {
    id: 'extend.drunk', label: 'Drunk walk', category: 'Extend & segment',
    program: 'extend', domain: 'sound', mono: false,
    parityExempt: 'drunken-walk randomisation (rand)',
    args: ['drunk', '1', '$IN', '$OUT', { p: 'outdur' }, { p: 'locus' }, { p: 'ambitus' }, { p: 'step' }, { p: 'clock' }],
    // tklib1.c (DRUNKWALK): locus/ambitus/step/clock are all capped at the
    // infile duration, and the segment clock must strictly exceed two 15 ms
    // splices (ap_extend.c). drunk.c also needs a further 15 ms tail when
    // reading each segment, so below 46 ms no clock value is valid at all.
    minSrcDur: 0.048,
    params: [
      { name: 'outdur', label: 'Output (s)', min: 0.5, max: 20, default: 4, step: 0.5, help: 'Total length of the output, in seconds.' },
      { name: 'locus', label: 'Centre (s)', min: 0, max: 60, default: 0.5, step: 0.05,
        srcMax: (s) => s.dur,                             // ap->hi = the infile duration
        srcDefault: (s) => Math.min(0.5, s.dur / 2),
        help: 'Time in the source the drunken walk wanders around, in seconds. Can move through the source.' },
      { name: 'ambitus', label: 'Wander (s)', min: 0.02, max: 10, default: 0.4, step: 0.02,
        srcMax: (s) => Math.min(10, s.dur),
        srcDefault: (s) => Math.min(0.4, s.dur),
        help: 'Half-width of the region around the centre that segments are read from, in seconds.' },
      { name: 'step', label: 'Step (s)', min: 0.01, max: 2, default: 0.1, step: 0.01,
        srcMax: (s) => Math.min(2, s.dur),
        help: 'Maximum random step between successive segment reads, in seconds (kept within the wander region).' },
      { name: 'clock', label: 'Segment (s)', min: 0.04, max: 1, default: 0.1, step: 0.01,
        hardMin: 0.031,                                   // > 2 × 15 ms splice, strict
        // One extra millisecond avoids sample-rounding pushing clock + the
        // 15 ms splice one frame past EOF (e.g. 35 ms at 44.1 kHz).
        srcMin: (s) => Math.max(0.031, Math.min(0.04, s.dur - 0.016)),
        srcMax: (s) => Math.min(1, s.dur - 0.016),
        help: 'Time between reads, i.e. the duration of each segment, in seconds.' },
    ],
    blurb: 'Splice segments read by a drunken walk around a moving point in the source.',
  },
  {
    id: 'extend.baktobak', label: 'Back-to-back', category: 'Extend & segment',
    program: 'extend', domain: 'sound', mono: false,
    args: ['baktobak', '$IN', '$OUT', { p: 'join' }, { p: 'splice' }],
    // tklib1.c (BAKTOBAK): the join lies within the file, the splice within
    // min(5 s, the file). ap_extend.c additionally rejects a join at or under
    // half the splice — so a join of 0, though inside the table's range, never
    // works; the minOf below encodes that, and the curated floor keeps off it.
    // hardMin records that the table itself starts at 0, so unlocking reaches
    // the short joins (down to half a splice) CDP does accept.
    minSrcDur: 0.02,
    params: [
      { name: 'join', label: 'Join at (s)', min: 0.01, max: 60, default: 0.5, step: 0.05,
        hardMin: 0.001,
        minOf: (v) => Number(v.splice) / 2000 + 0.001,    // strict: cut > splice/2
        srcMax: (s) => s.dur,                             // ap->hi = the infile duration
        srcDefault: (s) => Math.min(0.5, s.dur / 2),
        help: 'Time in the source where it is cut and abutted to its reverse, in seconds. Must lie within the file, past half a splice.' },
      { name: 'splice', label: 'Splice (ms)', min: 1, max: 200, default: 15, step: 1,
        hardMax: 5000,
        srcMax: (s) => Math.min(200, s.dur * 1000),       // ap->hi = min(5 s, duration) in ms
        srcHardMax: (s) => Math.min(5000, s.dur * 1000), // source cap survives unlocking
        help: 'Length of the crossfade at the join, in milliseconds.' },
    ],
    blurb: 'Abut the sound to a reversed copy of itself at the join point (palindrome).',
  },
  {
    id: 'extend.repetitions', label: 'Repeat at times', category: 'Extend & segment',
    program: 'extend', domain: 'sound', mono: false,
    args: ['repetitions', '$IN', '$OUT', '$DATA', { p: 'level' }],
    // timesfile: one start-time (seconds) per line at which the whole source plays.
    data: (v) => Array.from({ length: v.count }, (_, i) => (i * v.gap).toFixed(3)).join('\n') + '\n',
    params: [
      { name: 'count', label: 'Repeats', min: 2, max: 16, default: 4, step: 1, help: 'How many times the whole source is replayed.' },
      { name: 'gap', label: 'Spacing (s)', min: 0.1, max: 3, default: 1.2, step: 0.1, help: 'Time between the start of one replay and the next, in seconds. Spacings shorter than the source make the copies overlap.' },
      { name: 'level', label: 'Level', min: 0, max: 1, default: 0.8, step: 0.05, help: 'Output level (0–1), lowered to leave headroom when the copies overlap.' },
    ],
    blurb: 'Replay the whole source at a series of evenly-spaced times (echo/overlay).',
  },
];
