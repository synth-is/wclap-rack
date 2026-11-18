// Effect entries for CDP's `waveform` program.
// Entry shape and arg-token grammar: see the header comment in ../effects.js.

export default [
  // Build a sound *from* an existing one. (Pure no-input generators — wave,
  // noise, additive, etc. — live in generators.js, not the effect catalog.)
  {
    id: 'waveform.make', label: 'Make wavetable', category: 'Synthesis',
    program: 'waveform', domain: 'sound', input: 'mono',
    // Mode 2: cut a short segment (at TIME, DUR in mS) at a zero-crossing and abut
    // it to its inversion, forming a symmetric wavecycle (for use as a wavetable).
    args: ['make', '2', '$IN', '$OUT', { p: 'time' }, { p: 'dur' }],
    // waveform.c rejects a cut running past the file ("Cannot cut duration ...
    // after time ..."), and the zero-crossing hunt needs a few ms of headroom,
    // hence the 5 ms margins.
    minSrcDur: 0.01,
    params: [
      { name: 'time', label: 'Cut at (s)', min: 0, max: 60, default: 0, step: 0.01,
        srcMax: (s, v) => Math.max(0, s.dur - (Number(v.dur) || 0) / 1000 - 0.005),
        help: 'Time in the source where the segment is extracted, in seconds. The cut must fit inside the file.' },
      { name: 'dur', label: 'Segment (ms)', min: 1, max: 500, default: 50, step: 1,
        srcMax: (s, v) => Math.min(500, Math.max(1, (s.dur - (Number(v.time) || 0)) * 1000 - 5)),
        help: 'Length of the extracted clip, in milliseconds. The result is this clip abutted to its inversion (twice as long).' },
    ],
    blurb: 'Extract a short segment and mirror it into a symmetric wavecycle — a wavetable cut from the sound.',
  },
  {
    id: 'waveform.halfwaves', label: 'Wavetable (half-wavesets)', category: 'Synthesis',
    program: 'waveform', domain: 'sound', input: 'mono',
    // Mode 1: extract N half-wavesets at TIME, abutted to their inversion.
    args: ['make', '1', '$IN', '$OUT', { p: 'time' }, { p: 'cnt' }],
    params: [
      // ap->hi = duration, and the half-waveset hunt runs off the end at the
      // very last samples, so stay just inside it.
      { name: 'time', label: 'Cut at (s)', min: 0, max: 60, default: 0, step: 0.01,
        srcMax: (s) => Math.max(0, s.dur - 0.01),
        help: 'Time in the source where the half-wavesets are extracted, in seconds. Must lie within the file.' },
      { name: 'cnt', label: 'Half-wavesets', min: 1, max: 64, default: 4, step: 1, help: 'How many half-wave-segments to take. The result is abutted to its inversion (a symmetric wavecycle).' },
    ],
    blurb: 'Build a wavetable from a group of half-wavesets at a point in the sound.',
  },
  {
    id: 'waveform.sinemix', label: 'Wavetable + sine', category: 'Synthesis',
    program: 'waveform', domain: 'sound', input: 'mono',
    // Mode 3: a segment crossed with a sinusoid of wavelength dur*2.
    args: ['make', '3', '$IN', '$OUT', { p: 'time' }, { p: 'dur' }, { p: 'balance' }],
    // Same cut-must-fit rule as waveform.make (mode 2), same margins.
    minSrcDur: 0.01,
    params: [
      { name: 'time', label: 'Cut at (s)', min: 0, max: 60, default: 0, step: 0.01,
        srcMax: (s, v) => Math.max(0, s.dur - (Number(v.dur) || 0) / 1000 - 0.005),
        help: 'Time in the source where the segment is extracted, in seconds. The cut must fit inside the file.' },
      { name: 'dur', label: 'Segment (ms)', min: 1, max: 500, default: 50, step: 1,
        srcMax: (s, v) => Math.min(500, Math.max(1, (s.dur - (Number(v.time) || 0)) * 1000 - 5)),
        help: 'Length of the extracted clip, in milliseconds.' },
      { name: 'balance', label: 'Source/sine', min: 0.05, max: 1, default: 0.5, step: 0.05, help: 'Level of the extracted material relative to the sine it is superimposed on (low = mostly sine, 1 = mostly source).' },
    ],
    blurb: 'Make a wavetable by crossing a sound segment with a sine wave.',
  },
];
