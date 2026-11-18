// Effect entry for CDP's `mchshred` program.
// Entry shape and arg-token grammar: see the header comment in ../effects.js.

import { srcRate } from './helpers.js';

// How many chunks of the current length fit in the source — mchshred.c's
// MSHR_CHCNT, the ceiling it holds scatter under.
const chunkCount = (s, v) => Math.round(s.dur / (Number(v.chunklen) || 0.15));

export default [
  {
    id: 'mchshred.shred', label: 'Shred to multichannel', category: 'Spatialisation',
    program: 'mchshred', domain: 'sound', input: 'mono', setsChannels: true,
    parityExempt: 'seeded RNG (rand) differs across platforms',
    args: ['shred', '1', '$IN', '$OUT', { p: 'repeats' }, { p: 'chunklen' }, { p: 'scatter' }, { p: 'chans' }],
    // mchshred.c: a chunk needs three 256-sample splices (768/srate) and at
    // most half the file; below ~50 ms no chunk length is valid at any
    // supported rate. scatter is additionally capped at duration ÷ chunk.
    minSrcDur: 0.05,
    params: [
      { name: 'repeats', label: 'Shred passes', min: 1, max: 8, default: 2, step: 1, help: 'How many times the shredding process is applied; each pass re-shreds the previous result.' },
      { name: 'chunklen', label: 'Chunk (s)', min: 0.02, max: 0.5, default: 0.15, step: 0.01,
        srcMin: (s) => Math.max(768 / srcRate(s), Math.min(0.02, s.dur / 4)),
        srcMax: (s) => Math.min(0.5, s.dur / 2 - 0.001),  // ap->hi = dur/2 − FLTERR
        srcDefault: (s) => Math.min(0.5, Math.max(768 / srcRate(s), s.dur / 8)), // CDP's own
        help: 'Average length of the chunks the sound is cut into, in seconds. Smaller gives a finer scramble; at most half the source.' },
      // mchshred.c rejects a scatter above duration ÷ chunk length (DATA_ERROR),
      // whatever the 8 the table allows — so that cap is an engine limit and
      // must survive unlocking (srcHardMax); the curated 2 is taste (srcMax).
      { name: 'scatter', label: 'Cut scatter', min: 0, max: 2, default: 1, step: 0.25,
        hardMax: 8,
        srcMax: (s, v) => Math.min(2, chunkCount(s, v)),
        srcHardMax: (s, v) => chunkCount(s, v),
        help: 'Randomisation of the cut points. 0 reorders equal chunks without shredding; must stay below duration ÷ chunk length.' },
      { name: 'chans', label: 'Out channels', min: 2, max: 8, default: 2, step: 1, help: 'Number of output channels the shredded segments are permuted across.' },
    ],
    blurb: 'Cut the sound into random chunks and reassemble them in random order, spraying the segments across a multichannel field.',
  },
];
