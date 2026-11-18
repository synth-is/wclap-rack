// Effect entry for CDP's `verges` program.
// Entry shape and arg-token grammar: see the header comment in ../effects.js.

// Input the engine integrates for one gliss (find_contraction_sampcount): the
// gliss duration, stretched by the speed-up ramp when the verge starts above
// pitch. Every gap — verge to verge, and the last kept verge to the end of the
// file — must strictly exceed this, or verges.c errors out.
const glissNeed = (v) =>
  (Number(v.dur) / 1000) * (1 + (2 ** (Math.max(0, Number(v.transp)) / 12) - 1) / (Number(v.exp) + 1));

export default [
  {
    id: 'verges.verges', label: 'Verge glisses', category: 'Pitch & time',
    program: 'verges', domain: 'sound', mono: false,
    // The datafile lists verge-attack times, one per line. A time past the end
    // of the file is dropped with only a warning — the hard errors are the gap
    // checks (see glissNeed above), including the one from the last surviving
    // verge to the end of the file. The bounds below keep every requested verge
    // inside the file with its gliss tail intact.
    args: ['verges', '$IN', '$OUT', '$DATA', { p: 'transp', flag: '-t' }, { p: 'exp', flag: '-e' }, { p: 'dur', flag: '-d' }],
    data: (v) => Array.from({ length: Math.round(v.count) }, (_, i) => ((i + 1) * v.spacing).toFixed(3)).join('\n') + '\n',
    // Even a single verge at the tightest spacing needs its gliss tail before
    // the file end; below ~0.26 s the defaults' span is empty.
    minSrcDur: 0.26,
    params: [
      { name: 'count', label: 'Verges', min: 1, max: 3, default: 3, step: 1,
        srcMax: (s, v) => Math.max(1, Math.min(3, Math.floor((s.dur - glissNeed(v) - 0.01) / (glissNeed(v) + 0.01)))),
        help: 'How many gliss attacks are placed in the sound.' },
      { name: 'spacing', label: 'Every (s)', min: 0.2, max: 0.6, default: 0.25, step: 0.05,
        srcMin: (s, v) => glissNeed(v) + 0.01,
        srcMax: (s, v) => Math.min(0.6, (s.dur - glissNeed(v) - 0.01) / Math.max(1, Math.round(Number(v.count)))),
        srcDefault: (s) => Math.min(0.25, s.dur / 4),
        help: 'Spacing of the verge attacks, in seconds. All attacks must fall inside the sound, with room for the gliss before the end.' },
      { name: 'transp', label: 'Gliss from (st)', min: -12, max: 12, default: 5, step: 1, help: 'Transposition at the start of each verge, in semitones. Positive glisses down onto the pitch from above; negative swoops up from below.' },
      { name: 'exp', label: 'Slope', min: 1, max: 8, default: 1, step: 0.5, help: 'Curve of the gliss. Higher values gliss faster at the start.' },
      { name: 'dur', label: 'Gliss (ms)', min: 30, max: 150, default: 100, step: 10, help: 'Duration of each gliss, in milliseconds. Must stay well under the spacing between verges.' },
    ],
    blurb: 'Play the source with brief pitch-gliss "verges" swooping onto chosen moments.',
  },
];
