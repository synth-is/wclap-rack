// Effect entry for CDP's `hover` program.
// Entry shape and arg-token grammar: see the header comment in ../effects.js.

export default [
  {
    id: 'hover.hover', label: 'Hover (zigzag read)', category: 'Granular',
    program: 'hover', domain: 'sound', mono: true,
    // The pinned read point (0.5 s) is engine-capped at the file duration
    // (hover.c: ap->hi[loc] = duration), so shorter sources are rejected
    // whatever the exposed params say. The splice literal is in milliseconds
    // and must stay under 1/(2·frq) — at the top of the frq range that is
    // half a millisecond, so it is pinned effectively to zero.
    minSrcDur: 0.5,
    args: ['hover', '$IN', '$OUT', { p: 'frq' }, '0.5', '0', '0', '0.005', { p: 'dur' }],
    params: [
      { name: 'frq', label: 'Read rate (Hz)', min: 20, max: 1000, default: 200, step: 5, help: 'How fast the read-point zigzags back and forth over the chosen spot (Hz). This rate becomes the perceived pitch.' },
      { name: 'dur', label: 'Duration (s)', min: 0.5, max: 6, default: 2, step: 0.5, help: 'Length of the output, in seconds.' },
    ],
    blurb: 'Hover over a point in the file, zigzag-reading it at a frequency.',
  },
];
