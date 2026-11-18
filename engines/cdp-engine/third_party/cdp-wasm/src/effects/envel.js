// Effect entries for CDP's `envel` program.
// Entry shape and arg-token grammar: see the header comment in ../effects.js.

export default [
  {
    id: 'envel.tremolo', label: 'Tremolo (envelope)', category: 'Envelope',
    program: 'envel', domain: 'sound', mono: false,
    args: ['tremolo', '1', '$IN', '$OUT', { p: 'frq' }, { p: 'depth' }, '1'],
    params: [
      { name: 'frq', label: 'Rate (Hz)', min: 0.1, max: 30, default: 5, step: 0.1, help: 'Speed of the volume pulsing, in pulses per second.' },
      { name: 'depth', label: 'Depth', min: 0, max: 1, default: 0.8, step: 0.05, help: 'How deep the pulsing cuts: 0 is none, 1 dips to silence.' },
    ],
    blurb: 'Impose a tremolo (amplitude) envelope on the sound.',
  },
  {
    id: 'envel.swell', label: 'Swell', category: 'Envelope',
    program: 'envel', domain: 'sound', mono: false,
    args: ['swell', '$IN', '$OUT', { p: 'peaktime' }, '1'],
    // The peak must sit at least one 5 ms envelope window inside the file
    // (ENV_SWELL in dev/cdp2k/tklib1.c: lo = 5 ms, hi = duration - 5 ms).
    minSrcDur: 0.01,
    params: [{ name: 'peaktime', label: 'Peak time (s)', min: 0.05, max: 3, default: 0.4, step: 0.05,
      hardMin: 0.005,
      srcMin: (s) => Math.max(0.005, Math.min(0.05, s.dur / 2)),
      srcMax: (s) => s.dur - 0.005,       // ap->hi = duration - ENV_MIN_WSIZE
      srcDefault: (s) => s.dur / 2,       // CDP's own default: the middle of the file
      help: 'When the loudness peak falls, in seconds from the start. The sound swells up to it and fades away after. Must lie within the file.' }],
    blurb: 'Fade in to and out from a peak moment (peak must be within the file).',
  },
  {
    id: 'envel.attack', label: 'Emphasise attack', category: 'Envelope',
    program: 'envel', domain: 'sound', mono: false,
    args: ['attack', '4', '$IN', '$OUT', { p: 'gain' }, { p: 'onset' }, { p: 'decay' }],
    minSrcDur: 0.01,
    params: [
      { name: 'gain', label: 'Gain', min: 1, max: 8, default: 2, step: 0.5, help: 'How much the loudest moment is boosted to make it pop as an attack.' },
      { name: 'onset', label: 'Onset (ms)', min: 5, max: 1000, default: 50, step: 5, help: 'Length of the rising edge leading into the peak (milliseconds).' },
      { name: 'decay', label: 'Decay (ms)', min: 5, max: 2000, default: 200, step: 5,
        hardMin: 5,
        srcMax: (s) => Math.min(2000, s.dur * 1000 - 5),  // ap->hi = dur in ms - ENV_MIN_ATK_ONSET
        help: 'Length of the falling edge after the peak (milliseconds).' },
    ],
    blurb: 'Boost the loudest point as an attack transient.',
  },
  {
    id: 'envel.dovetail', label: 'Fade in/out (dovetail)', category: 'Envelope',
    program: 'envel', domain: 'sound', mono: false,
    args: ['dovetail', '2', '$IN', '$OUT', { p: 'infade' }, { p: 'outfade' }],
    // The two fades must fit inside the file together (envprepro.c: "Start and
    // End Trims overlap"), so each is capped by the file minus the other. CDP
    // also rejects BOTH fades at zero ("no change in file") — not expressible
    // here; the defaults keep clear of it.
    params: [
      { name: 'infade', label: 'Fade-in (s)', min: 0, max: 2, default: 0.1, step: 0.05,
        srcMax: (s, v) => Math.min(2, Math.max(0, s.dur - (Number(v.outfade) || 0))),
        srcDefault: (s) => Math.min(0.1, s.dur / 3),
        help: 'Length of the fade up from silence at the start, in seconds. Together with the fade-out it must fit inside the file.' },
      { name: 'outfade', label: 'Fade-out (s)', min: 0, max: 2, default: 0.2, step: 0.05,
        srcMax: (s, v) => Math.min(2, Math.max(0, s.dur - (Number(v.infade) || 0))),
        srcDefault: (s) => Math.min(0.2, s.dur / 3),
        help: 'Length of the fade down to silence at the end, in seconds. Together with the fade-in it must fit inside the file.' },
    ],
    blurb: 'Dovetail the file by fading the start in and the end out.',
  },
  {
    id: 'envel.curtail', label: 'Fade to silence', category: 'Envelope',
    program: 'envel', domain: 'sound', mono: false,
    args: ['curtail', '2', '$IN', '$OUT', { p: 'start' }, { p: 'dur' }, '1'],
    params: [
      // envprepro.c rejects start >= duration outright; a fade running past the
      // file end is only clamped with a warning, but the parse-time table still
      // caps the fade length at the file duration.
      { name: 'start', label: 'Fade start (s)', min: 0, max: 5, default: 0.5, step: 0.05,
        srcMax: (s) => Math.max(0, s.dur - 0.01),
        srcDefault: (s) => Math.min(0.5, s.dur / 2),
        help: 'When the fade to silence begins, in seconds from the start. Must lie within the file.' },
      { name: 'dur', label: 'Fade length (s)', min: 0.05, max: 3, default: 0.3, step: 0.05,
        srcMin: (s) => Math.min(0.05, s.dur),
        srcMax: (s) => Math.min(3, s.dur),  // ap->hi = duration
        help: 'How long the fade-out takes once it begins, in seconds.' },
    ],
    blurb: 'Fade the sound to silence starting at a given time within it.',
  },
  {
    id: 'envel.scaled', label: 'Amplitude ramp', category: 'Envelope',
    program: 'envel', domain: 'sound', mono: false,
    // The brkpnt envelope (two time/level pairs, times arbitrary) is stretched to
    // the file's duration, so 0→1 spans the whole sound as a linear level ramp.
    args: ['scaled', '$IN', '$DATA', '$OUT'],
    data: (v) => `0 ${v.start}\n1 ${v.end}\n`,
    params: [
      { name: 'start', label: 'Start level', min: 0, max: 1, default: 0.2, step: 0.05, help: 'Loudness at the start of the sound (0–1).' },
      { name: 'end', label: 'End level', min: 0, max: 1, default: 1, step: 0.05, help: 'Loudness at the end. Higher than the start gives a crescendo; lower a decrescendo.' },
    ],
    blurb: 'Impose a linear amplitude ramp across the whole sound (crescendo / decrescendo).',
  },
];
