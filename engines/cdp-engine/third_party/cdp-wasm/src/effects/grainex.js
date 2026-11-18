// Effect entry for CDP's `grainex` program.
// Entry shape and arg-token grammar: see the header comment in ../effects.js.

import { srcRate } from './helpers.js';

export default [
  {
    // grainex extend: finds grains in the stt..end region and repeats a random
    // re-selection of them to add `plus` seconds.
    id: 'grainex.extend', label: 'Extend grain region', category: 'Granular',
    program: 'grainex', domain: 'sound', mono: true, prefersGrains: true,
    parityExempt: 'random grain reselection (rand)',
    args: ['extend', '$IN', '$OUT', { p: 'wsiz' }, { p: 'trof' }, { p: 'plus' }, { p: 'stt' }, { p: 'end' }],
    // grainex.c derives its limits from the infile: the search window may span at
    // most a third of the file, and the region must lie inside it (with start and
    // end kept 50 ms apart — CDP rejects a zero-length region).
    params: [
      { name: 'wsiz', label: 'Window (ms)', min: 10, max: 200, default: 50, step: 5,
        srcMin: (s) => Math.max(8000 / srcRate(s), Math.min(10, s.dur / 3 * 1000)),
        srcMax: (s) => Math.min(200, s.dur / 3 * 1000),   // ap->hi = (dur/3) in ms
        help: 'Size of the search window, in milliseconds — sets the size of the grains the process looks for.' },
      { name: 'trof', label: 'Trough depth', min: 0.05, max: 0.95, default: 0.5, step: 0.05, help: 'How deep a dip must be, relative to its neighbouring peaks, to count as a gap between grains. Lower keeps only clearly-separated grains; higher splits more readily.' },
      { name: 'plus', label: 'Add (s)', min: 0.5, max: 30, default: 2, step: 0.5, help: 'How many seconds of material to add to the source by repeating grains from the region.' },
      { name: 'stt', label: 'Region start (s)', min: 0, max: 10, default: 0, step: 0.1,
        srcMax: (s, v) => Math.max(0, Math.min(Number(v.end) || s.dur, s.dur) - 0.05),
        help: 'Start of the grain region within the source, in seconds. Must lie within the file.' },
      { name: 'end', label: 'Region end (s)', min: 0.2, max: 30, default: 1, step: 0.1,
        srcMin: (s, v) => Math.min((Number(v.stt) || 0) + 0.05, s.dur),
        srcMax: (s) => s.dur,                             // ap->hi = duration
        srcDefault: (s) => s.dur,                         // CDP's own default: the whole file
        help: 'End of the grain region, in seconds. Must be no later than the end of the file.' },
    ],
    blurb: 'Find grains in a region of the sound and extend it by repeating them. Needs rhythmic/grainy input.',
  },
];
