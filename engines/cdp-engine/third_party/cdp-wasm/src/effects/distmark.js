// Effect entry for CDP's `distmark` program.
// Entry shape and arg-token grammar: see the header comment in ../effects.js.

export default [
  {
    id: 'distmark.interp', label: 'Waveset interpolate (marks)', category: 'Waveset distortion',
    program: 'distmark', domain: 'sound', mono: true,
    // Mode 1. The datafile lists times at which waveset-groups are found; marks
    // past the end of a short source are tolerated (ignored). The trailing '-t'
    // keeps the remaining tail of the source so the output spans the full
    // duration. unitlen must stay under half the min gap between marks — its
    // max (45ms) is pinned under half the spacing minimum (0.1s). rand > 0
    // consumes rand() (platform-dependent) but the default 0 is bit-exact.
    args: ['distmark', '1', '$IN', '$OUT', '$DATA', { p: 'unitlen' }, { p: 'stretch', flag: '-s' }, { p: 'rand', flag: '-r' }, '-t'],
    // One mark at the tightest spacing (0.1 s) still needs unitlen headroom to
    // the end of the file.
    minSrcDur: 0.12,
    data: (v) => Array.from({ length: Math.round(v.marks) }, (_, i) => ((i + 1) * v.spacing).toFixed(3)).join('\n') + '\n',
    params: [
      { name: 'marks', label: 'Marks', min: 2, max: 6, default: 3, step: 1, help: 'How many marked points in the source the interpolation moves between. Marks past the end of a short sound are ignored.' },
      // distmark.c requires unitlen under half the smallest step between marks
      // — INCLUDING the step from the last surviving mark to the end of the
      // file — so the spacing cap leaves a 0.1 s tail and unitlen's cap is
      // computed from the marks that actually land inside the file.
      { name: 'spacing', label: 'Every (s)', min: 0.1, max: 0.5, default: 0.25, step: 0.05,
        srcMax: (s, v) => Math.min(0.5, Math.max(0.1, (s.dur - 0.1) / Math.max(1, Math.round(Number(v.marks))))),
        help: 'Spacing of the marks in the source, in seconds.' },
      { name: 'unitlen', label: 'Unit (ms)', min: 5, max: 45, default: 20, step: 1,
        srcMax: (s, v) => {
          const sp = Number(v.spacing), marks = Math.round(Number(v.marks));
          let last = 0;
          for (let k = 1; k <= marks; k++) if (k * sp < s.dur) last = k * sp;
          const minstep = Math.min(sp, s.dur - last);
          return Math.max(5, Math.min(45, Math.floor(minstep * 500) - 1));  // strict '<' in CDP
        },
        help: 'Approximate size of the waveset group found at each mark, in milliseconds. Must stay under half the mark spacing.' },
      { name: 'stretch', label: 'Time stretch', min: 1, max: 4, default: 1, step: 0.25, help: 'Stretches the distances between marks in the output. 1 keeps the original timing.' },
      { name: 'rand', label: 'Randomise', min: 0, max: 1, default: 0, step: 0.05, help: 'Randomly shortens the interpolated wavesets (0–1), making the heard pitch rise unevenly.' },
    ],
    blurb: 'Rebuild the sound by interpolating between waveset-groups found at marked times (glassy waveset morphing).',
  },
];
