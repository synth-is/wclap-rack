// Effect entries for CDP's `cantor` program.
// Entry shape and arg-token grammar: see the header comment in ../effects.js.

import { srcRate } from './helpers.js';

// cantor.c `check_cantor_param_validity_and_consistency`: the splice is turned
// into samples and then rounded UP to a multiple of ceil(1/depth) so each dig
// carves a whole number of samples — up to 19 samples longer than asked at the
// shallowest depth. Every hole must clear two of these rounded splices, so the
// hole floors below are measured against the rounded length.
const splen = (s, v) => {
  const samples = Math.round(((Number(v.splice) || 15) / 1000) * srcRate(s));
  const digsteps = Math.ceil(1 / (Number(v.depth) || 0.7));
  return Math.ceil(samples / digsteps) * digsteps;
};
const digSlack = (v) => Math.ceil(1 / (Number(v.depth) || 0.7)) - 1;   // samples the rounding may add

// Mode 1: the first hole (holesize% of the file) must exceed two splices —
// as a percentage, 200·splen/insams, plus headroom for the strict comparison.
const holeFloor = (s, v) => (200 * splen(s, v)) / (s.dur * srcRate(s)) + 0.5;
// …and the longest splice (ms) that still leaves that floor under the curated
// 90% ceiling, allowing for the rounding.
const spliceCeilPct = (s, v) => Math.floor(1000 * (0.4475 * s.dur - digSlack(v) / srcRate(s)));

// Mode 2: a fixed hole must exceed two (rounded) splices, and fit three times
// into the file.
const holeDurFloor = (s, v) => (2 * splen(s, v)) / srcRate(s) + 0.001;
const spliceCeilFixed = (s, v) => Math.floor((1000 * (((s.dur / 3 - 0.001) * srcRate(s)) / 2 - digSlack(v))) / srcRate(s));

export default [
  {
    id: 'cantor.holes', label: 'Cantor holes (proportional)', category: 'Extend & segment',
    program: 'cantor', domain: 'sound', input: 'mono', multiOut: true, parityExempt: 'multi-file output; no single-command native comparison',
    args: ['set', '1', '$IN', '$OUT', { p: 'holesize' }, { p: 'depth' }, { p: 'trigger' }, { p: 'splice' }, { p: 'maxdur' }],
    // At the 3 ms engine splice floor, an 8 ms source can still fit a hole under
    // the curated 90% ceiling at any depth. Shorter sources have no feasible
    // default pair.
    minSrcDur: 0.008,
    params: [
      // cantor.c rejects a first hole shorter than two splices, so the
      // percentage floor rises as the source shrinks (see holeFloor).
      { name: 'holesize', label: 'Hole size (%)', min: 5, max: 90, default: 40, step: 1,
        hardMax: 99,
        srcMin: (s, v) => Math.max(5, holeFloor(s, v)),
        help: 'Percentage of each segment cut away as a hole (proportional to segment length).' },
      { name: 'depth', label: 'Hole depth', min: 0.05, max: 1, default: 0.7, step: 0.05, help: 'How deep each cut goes as the hole is gradually created (0–1).' },
      { name: 'trigger', label: 'Trigger depth', min: 0.05, max: 1, default: 0.3, step: 0.05, help: 'Hole depth that triggers the next round of hole-cutting.' },
      // The splice is capped so the hole floor it implies stays under 90% —
      // otherwise holesize's span inverts and no hole fits two splices.
      { name: 'splice', label: 'Splice (ms)', min: 3, max: 50, default: 15, step: 1,
        srcMax: (s, v) => Math.min(50, Math.max(3, spliceCeilPct(s, v))),
        srcDefault: (s) => Math.min(15, Math.max(3, Math.floor(447.5 * s.dur))),
        help: 'Splice length at the hole edges, in milliseconds.' },
      // cantor.c: ap->lo[maxdur] = duration·2 — the output cap must leave room
      // for at least the first doubling. The zigzag pattern again.
      { name: 'maxdur', label: 'Max total (s)', min: 2, max: 60, default: 30, step: 1,
        hardMax: 32767,
        srcMin: (s) => s.dur * 2,
        srcMax: (s) => Math.min(32767, Math.max(60, s.dur * 8)),
        srcDefault: (s) => Math.min(32767, Math.max(30, s.dur * 4)),
        help: 'Maximum total duration across all the output files. At least twice the source.' },
    ],
    blurb: 'Cantor-set erosion: cut a hole in the middle third, then in the pieces that remain, and so on — output is the growing sequence.',
  },
  {
    id: 'cantor.holesfixed', label: 'Cantor holes (fixed)', category: 'Extend & segment',
    program: 'cantor', domain: 'sound', input: 'mono', multiOut: true, parityExempt: 'multi-file output; no single-command native comparison',
    args: ['set', '2', '$IN', '$OUT', { p: 'holedur' }, { p: 'depth' }, { p: 'trigger' }, { p: 'splice' }, { p: 'maxdur' }],
    // Even at the engine's shortest splice (3 ms), a hole must exceed two of
    // them and still fit three times into the file — ~22 ms at the shallowest
    // depth; 25 ms leaves a margin.
    minSrcDur: 0.025,
    params: [
      // cantor.c: a fixed hole is capped at a third of the file and must
      // strictly exceed two splices — so the splice default shrinks with the
      // file to keep the pair consistent (holedur's floor tracks the splice).
      { name: 'holedur', label: 'Hole length (s)', min: 0.02, max: 1, default: 0.2, step: 0.02,
        srcMin: (s, v) => holeDurFloor(s, v),
        srcMax: (s) => s.dur / 3,                         // ap->hi = duration/3
        srcDefault: (s) => Math.min(0.2, s.dur / 3),
        help: 'Fixed duration of each hole, in seconds (rather than proportional to the segment). Must exceed two splices and fit three times into the file.' },
      { name: 'depth', label: 'Hole depth', min: 0.05, max: 1, default: 0.7, step: 0.05, help: 'How deep each cut goes as the hole is gradually created (0–1).' },
      { name: 'trigger', label: 'Trigger depth', min: 0.05, max: 1, default: 0.3, step: 0.05, help: 'Hole depth that triggers the next round of hole-cutting.' },
      // cantor.c: ap->lo[splice] = 3 in this mode; the ceiling keeps two
      // splices inside the longest hole the file allows (dur/3).
      { name: 'splice', label: 'Splice (ms)', min: 3, max: 50, default: 15, step: 1,
        srcMax: (s, v) => Math.min(50, Math.max(3, spliceCeilFixed(s, v))),
        srcDefault: (s) => Math.min(15, Math.max(3, spliceCeilFixed(s, {}))),
        help: 'Splice length at the hole edges, in milliseconds.' },
      { name: 'maxdur', label: 'Max total (s)', min: 2, max: 60, default: 30, step: 1,
        hardMax: 32767,
        srcMin: (s) => s.dur * 2,                         // ap->lo = duration·2
        srcMax: (s) => Math.min(32767, Math.max(60, s.dur * 8)),
        srcDefault: (s) => Math.min(32767, Math.max(30, s.dur * 4)),
        help: 'Maximum total duration across all the output files. At least twice the source.' },
    ],
    blurb: 'Cantor-set erosion with fixed-length holes: the growing sequence of ever-more-perforated sounds.',
  },
  {
    id: 'cantor.vibrato', label: 'Cantor vibrato layers', category: 'Extend & segment',
    program: 'cantor', domain: 'sound', input: 'mono', multiOut: true, parityExempt: 'multi-file output; no single-command native comparison',
    args: ['set', '3', '$IN', '$OUT', { p: 'holelev' }, { p: 'repeats' }, { p: 'layers' }, { p: 'layerdec' }, { p: 'maxdur' }],
    params: [
      { name: 'holelev', label: 'Hole floor', min: 0, max: 0.9, default: 0.1, step: 0.05, help: 'Signal level at the base of the holes (0 = full cut).' },
      { name: 'repeats', label: 'Repeats to full', min: 2, max: 10, default: 3, step: 1, help: 'How many repeats before a hole reaches full depth.' },
      { name: 'layers', label: 'Vibrato layers', min: 1, max: 8, default: 2, step: 1, help: 'How many superimposed vibrato envelopes are used.' },
      { name: 'layerdec', label: 'Layer decay', min: 0.1, max: 1, default: 0.5, step: 0.05, help: 'Depth of each vibrato layer relative to the previous one.' },
      { name: 'maxdur', label: 'Max total (s)', min: 2, max: 60, default: 30, step: 1,
        hardMax: 32767,
        srcMin: (s) => s.dur * 2,                         // ap->lo = duration·2
        srcMax: (s) => Math.min(32767, Math.max(60, s.dur * 8)),
        srcDefault: (s) => Math.min(32767, Math.max(30, s.dur * 4)),
        help: 'Maximum total duration across all the output files. At least twice the source.' },
    ],
    blurb: 'Cantor-set erosion driven by superimposed vibrato envelopes — a shimmering sequence of perforated sounds.',
  },
];
