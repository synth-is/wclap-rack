// demo.dsp — the demo genome for examples/faust-rack and the tests: a small free-form Faust
// synth with five exposed hsliders plus the note-driven freq/gain/gate. Compiled to
// genome.wasm + genome.json by scripts/make-demo-genome.mjs (faust2wasm, single precision).
declare name "demo-genome";
declare version "0.1.0";
import("stdfaust.lib");

freq    = hslider("freq", 440, 20, 20000, 1);
gain    = hslider("gain", 0.5, 0, 1, 0.01);
gate    = checkbox("gate");
cutoff  = hslider("cutoff", 8000, 100, 18000, 1) : si.smoo;
drive   = hslider("drive", 1, 0.1, 10, 0.1);
attack  = hslider("attack", 0.01, 0, 2, 0.001);
release = hslider("release", 0.2, 0, 3, 0.001);

process = os.sawtooth(freq) * en.adsr(attack, 0.1, 0.7, release, gate) * gain * drive
        : fi.lowpass(2, cutoff) <: _, _;
