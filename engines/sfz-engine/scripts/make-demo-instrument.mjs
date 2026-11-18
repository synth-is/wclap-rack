#!/usr/bin/env node
// make-demo-instrument.mjs — generate the demo instrument used by examples/sfz-rack and the tests:
//   <out>/demo.sfz                      one region over the whole key range
//   <out>/samples/c4.wav               1 s plucked-sine sample (16-bit PCM mono, 48 kHz)
// Usage: node engines/sfz-engine/scripts/make-demo-instrument.mjs <out-dir>
import fs from 'node:fs';
import path from 'node:path';

const outDir = process.argv[2];
if (!outDir) { console.error('usage: make-demo-instrument.mjs <out-dir>'); process.exit(2); }

const SR = 48000;
const SECONDS = 1.0;
const FREQ = 261.6255653; // C4
const N = Math.floor(SR * SECONDS);

// 16-bit PCM mono WAV: pluck-like envelope (fast attack, exponential decay)
const data = new Int16Array(N);
for (let i = 0; i < N; i++) {
	const t = i / SR;
	const env = (1 - Math.exp(-t * 2000)) * Math.exp(-t * 3.0);
	data[i] = Math.round(0.8 * env * Math.sin(2 * Math.PI * FREQ * t) * 32767);
}
const wav = Buffer.alloc(44 + N * 2);
wav.write('RIFF', 0); wav.writeUInt32LE(36 + N * 2, 4); wav.write('WAVE', 8);
wav.write('fmt ', 12); wav.writeUInt32LE(16, 16); wav.writeUInt16LE(1, 20); wav.writeUInt16LE(1, 22);
wav.writeUInt32LE(SR, 24); wav.writeUInt32LE(SR * 2, 28); wav.writeUInt16LE(2, 32); wav.writeUInt16LE(16, 34);
wav.write('data', 36); wav.writeUInt32LE(N * 2, 40);
for (let i = 0; i < N; i++) wav.writeInt16LE(data[i], 44 + i * 2);

const sfz = `// demo instrument for the sfz-engine example rack (examples/sfz-rack)
// one sample, whole key range, pitched from C4 (pitch_keycenter=60)
<global> volume=-3
<region> sample=samples/c4.wav lokey=0 hikey=127 pitch_keycenter=60 \\
         ampeg_attack=0.005 ampeg_hold=0 ampeg_decay=0.3 ampeg_sustain=0.6 ampeg_release=0.25 \\
         loop_mode=no_loop pan=0 tune=0
`;

fs.mkdirSync(path.join(outDir, 'samples'), { recursive: true });
fs.writeFileSync(path.join(outDir, 'demo.sfz'), sfz);
fs.writeFileSync(path.join(outDir, 'samples', 'c4.wav'), wav);
console.log(`wrote ${path.join(outDir, 'demo.sfz')} and ${path.join(outDir, 'samples', 'c4.wav')} (${wav.length} bytes)`);
