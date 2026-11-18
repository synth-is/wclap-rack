// Effect entries for CDP's `envnu` program.
// Entry shape and arg-token grammar: see the header comment in ../effects.js.

export default [
  {
    id: 'envnu.expdecay', label: 'Exponential decay', category: 'Envelope',
    program: 'envnu', domain: 'sound', mono: false,
    args: ['expdecay', '$IN', '$OUT', { p: 'starttime' }, { p: 'endtime' }],
    // An endtime past the file end is legal (it means "decay to the end"), but
    // the decay must start before it ends: envnu.c clamps endtime to the file,
    // then rejects start >= end.
    params: [
      { name: 'starttime', label: 'Start (s)', min: 0, max: 2, default: 0.2, step: 0.05,
        srcMax: (s, v) => Math.max(0, Math.min(Number(v.endtime) || s.dur, s.dur) - 0.01),
        srcDefault: (s) => Math.min(0.2, s.dur / 2),
        help: 'When the decay begins, in seconds from the start of the file.' },
      { name: 'endtime', label: 'End (s)', min: 0.5, max: 10, default: 10, step: 0.5,
        srcMin: (s) => Math.min(0.5, s.dur / 2),
        srcMax: (s) => s.dur,
        srcDefault: (s) => s.dur,           // CDP's own default: decay to the file end
        help: 'When the decay reaches silence. If earlier than the file end the output is cut there; at or beyond it, the decay lasts to the end of the file.' },
    ],
    blurb: 'Impose an exponential fade-to-silence from a chosen start time.',
  },
  {
    id: 'envnu.peakchop', label: 'Peak chop', category: 'Envelope',
    program: 'envnu', domain: 'sound', mono: false, prefersGrains: true,
    args: ['peakchop', '1', '$IN', '$OUT', { p: 'wsize' }, { p: 'pkwidth' }, { p: 'risetime' }, { p: 'tempo' }, { p: 'gain' }],
    // CDP's table caps the window at the file length (ap->hi = min(dur, 1 s)),
    // but a window that spans the whole file leaves a one-point envelope and the
    // peak search reports "No peaks found" — so the usable ceiling is half the
    // file (at least two windows), and under 20 ms the curated span inverts.
    minSrcDur: 0.02,
    params: [
      { name: 'wsize', label: 'Window (ms)', min: 10, max: 64, default: 50, step: 1,
        hardMin: 1,
        srcMax: (s) => Math.min(64, Math.max(1, Math.floor(Math.min(s.dur, 1) * 500))),
        help: 'Window size used to find loudness peaks. Smaller windows find more, smaller peaks.' },
      { name: 'pkwidth', label: 'Peak width (ms)', min: 5, max: 100, default: 20, step: 1, help: 'Length of sound kept around each peak. Must stay shorter than the gap between peaks in the source.' },
      { name: 'risetime', label: 'Rise (ms)', min: 1, max: 50, default: 10, step: 1, help: 'Fade-in time from silence up to each peak, in milliseconds.' },
      { name: 'tempo', label: 'Tempo (per min)', min: 60, max: 600, default: 120, step: 5, help: 'Playback rate of the extracted peaks, in events per minute.' },
      { name: 'gain', label: 'Gain', min: 0.1, max: 1, default: 1, step: 0.05, help: 'Lower this if a fast tempo makes the replayed peaks overlap and clip.' },
    ],
    blurb: 'Isolate the attack peaks and replay them at a set tempo. Needs rhythmic input.',
  },
];
