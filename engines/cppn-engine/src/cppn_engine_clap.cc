// Copyright 2026 Björn Þór Jónsson (kromosynth-burst). Licensed under the MIT License; see ../LICENSE.
//
// cppn-engine: a CPPN+DSP substrate engine as a WCLAP instrument — the live, real-time counterpart
// of kromosynth's sample-based (SFZ) delivery of CPPN+DSP sounds. Written for wclap-rack SPEC §8.
//
// In the Synth.is CPPN+DSP substrate a genome is TWO networks: a waveNetwork (a CPPN whose outputs
// are signals — audio-rate waves and/or control-rate envelopes/LFOs, each rendered at its own
// frequency) and an asNEATPatch (the DSP graph those signals feed, realized in kromosynth as a
// Web Audio / virtual-audio-graph patch). This engine evaluates BOTH in real time, per sample,
// inside the plugin:
//
//   * Files: the host delivers the genome JSON (raw Synth.is API envelope, a bare waveNetwork, or
//     a bare offspring; the asNEATPatch may be embedded char-split or as a string, or delivered as
//     a separate pre-converted *.json with {audioGraph, networkOutputs}) BEFORE activate via
//     wclap.files/1 frames on the clap.webview/3 port channel (type 240 write / 241 clear),
//     FNV-1a digest logged per frame, identical to testfx/sfz-engine/faust-runner.
//   * CPPN core: cppn_core.h — a bit-exact C++ port of neatjs networkDecode + cppnjs
//     recursiveActivation + kromosynth's CpuBackend input schemes (verified maxAbsDiff=0.0 over
//     1.7M samples on genome 01KR0R5T4S7SQF4W8M1G99CGS9; see WCLAP/poc-cppn-wasm). No GPU: the
//     audio path is CPU wasm only (AudioWorklets have no WebGPU; DAW webviews restrict it).
//   * Patch: the asNEAT patch is parsed and compiled once into an index-based plan (cppn_graph.h),
//     following the PRODUCTION renderer (renderWithWorkletOffline / network-rendering.js in
//     'streaming' mode / patchFromAsNEATnetwork) and the Rust web-audio-api crate's node maths;
//     the rules are listed at the top of cppn_graph.h. Implemented node types: gain, biquadFilter,
//     oscillator, bufferSource, wavetable, additive, waveShaper. Not yet: delay, feedbackDelay,
//     dynamicsCompressor, convolver, faust (played as plain pass-through and named in the load log).
//   * Per note: the CPPN is evaluated once per unique signal frequency per sample (legacy-2input /
//     phase-modulated / hybrid-phase auto-detected from inputNodeCount or genome.cppnInputScheme),
//     then the graph runs per sample. One-shot over the Duration param, then release.
//   * Voices: fixed pool of 8, no allocation in process(); sample-accurate note events; linear
//     attack/release envelope; velocity -> CPPN velocity input and gain^2. Deterministic.
//   * Parameters (clap.params): 0 Volume dB, 1 Duration s, 2 Attack s, 3 Release s, 6 Pitch
//     (semitones added to the note, like the web app's noteDelta pad), 4/5 "CPPN X"/"CPPN Y"
//     (hybrid-phase navigation coords — HIDDEN unless the genome is hybrid-phase; the web app's
//     geometric weight pads re-deliver a pad-adjusted genome via wclap.files/1 instead).
//   * No editor page; receive() carries only wclap.files/1 frames.

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <functional>
#include <map>
#include <new>
#include <string>
#include <vector>

#include <clap/clap.h>
#include <clap/ext/draft/webview.h>
#include <clap/ext/gui.h>
#include <clap/ext/note-ports.h>

#include "cppn_core.h"
#include "cppn_graph.h"
#include "mini_json.h"

namespace {

constexpr clap_id kParamVolume = 0;
constexpr clap_id kParamDuration = 1;
constexpr clap_id kParamAttack = 2;
constexpr clap_id kParamRelease = 3;
constexpr clap_id kParamX = 4;
constexpr clap_id kParamY = 5;
constexpr clap_id kParamPitch = 6;
constexpr clap_id kParamDcFilter = 7;
constexpr clap_id kParamAutoLevel = 8;
constexpr clap_id kParamLimiter = 9;
constexpr clap_id kParamSoftClip = 10;
constexpr clap_id kParamWeightsX = 11;
constexpr clap_id kParamWeightsY = 12;
constexpr clap_id kParamFreqsX = 13;
constexpr clap_id kParamFreqsY = 14;
constexpr clap_id kParamPrerender = 15;   // 0 Auto, 1 Live, 2 Pre-rendered
constexpr uint32_t kParamCount = 16;
// The key at which a genome plays at its own pitch (noteDelta 0). kromosynth-vi's sample
// instruments use the same key, so a sound auditioned as samples is in tune with the live engine.
constexpr int kRootKey = 60;
constexpr uint32_t kVoiceCount = 8;
constexpr int kStateVersion = 6;
// Pre-rendered playing mode (for genomes too slow to compute live): keys are rendered into buffers,
// in slices at the end of each process() call, and played as a sampler.
constexpr double kLiveThreshold = 12.0;   // Auto: pre-render below this many times real time for one voice
constexpr double kRenderShare = 0.5;      // part of a block's time given to live voices + rendering (0.7 left
                                          // too little headroom for the host's jitter: p99 over the block)
constexpr int kKeyLo = 24, kKeyHi = 96;   // the keys rendered; others play the nearest, transposed
constexpr size_t kMemCap = (size_t)64 << 20;
constexpr double kDebounceSec = 0.3;      // re-rendering starts this long after the last change
constexpr double kStreamShare = 0.6;      // the share while a played note waits for, or plays from, the key
                                          // being rendered (0.65 put 8% of blocks over on a genome whose
                                          // speed the estimate overstates by 30%)
constexpr double kConvPairUs = 0.75;      // a convolver's spectral product of one earlier block pair, per channel
                                          // (fitted, this laptop): all of a block boundary's pairs are done in
                                          // one sample, 0.7 ms at 10 s into a note, so the render budget counts them
constexpr double kPrepShare = 0.6;        // Live: the share for a convolver's pre-pass while nothing else sounds
bool isStepped(uint32_t id) { return (id >= kParamDcFilter && id <= kParamSoftClip) || id == kParamPrerender; }

const char* const kFeatures[] = {CLAP_PLUGIN_FEATURE_INSTRUMENT, CLAP_PLUGIN_FEATURE_SYNTHESIZER, nullptr};

const clap_plugin_descriptor_t kDescriptor = {
    CLAP_VERSION_INIT,
    "is.synth.kromosynth.cppn-engine",
    "Kromosynth CPPN+DSP Engine",
    "kromosynth-burst",
    nullptr,
    nullptr,
    nullptr,
    "1.0.0",
    "Real-time CPPN+DSP substrate engine; genome JSON delivered via wclap.files/1 (wclap-rack SPEC §8)",
    kFeatures};

struct CppnEngine {
  clap_plugin_t plugin;
  const clap_host_t* host;
  double sampleRate = 48000;
  double volumeDb = 0, durationSec = 2, attackSec = 0.005, releaseSec = 0.1, x = 0.5, y = 0.5, pitch = 0, dcFilter = 1, autoLevel = 1, limiter = 1, softClip = 1;
  double wx = 0.5, wy = 0.5, fx = 0.5, fy = 0.5;   // the Weights and Frequencies pads (0.5 = centre = the genome as it is)
  double prerenderParam = 0;  // 0 Auto, 1 Live, 2 Pre-rendered
  double offset = 0;          // measured at load: mean of the root note
  double trim = 1;            // measured at load: 1/peak of the root note when it would exceed full scale
  cgraph::VoiceState scratch; // used for that measurement and for rendering keys in the pre-rendered mode
  // ── pre-rendered playing mode ──
  bool pre = false;           // the mode in force
  bool offline = false;       // clap.render: the host renders offline (a bounce): time is no object, so
                              // everything is live and a convolver's pre-pass is done at note on, at once
  int preReason = 0;          // 0 auto (estimated speed), 1 auto (measured speed), 2 forced
  double speed = 1e9;         // times real time for one voice: estimated from the plan, or measured
  bool speedMeasured = false;
  double cppnShare = 0.25;    // the CPPN's part of one voice-sample's cost (the convolver pre-pass is CPPN only)
  struct Buf {
    std::vector<float> data;  // the key's note, post offset, as renderVoiceSample returns it
    long len = 0, total = 0;  // samples rendered (note + release tail), note length
    long filled = 0;          // samples written so far (== len when done; less while it is being rendered)
    double offset = 0;        // the level offset subtracted when it was rendered (DC Filter on)
    int key = -1;
    double keyRatio = 1, pitchRatio = 1;
    uint32_t gen = 0;         // the settings generation it was rendered with
  };
  std::vector<Buf> pool;
  int keyBuf[128];            // pool entry per key, -1 = none
  uint32_t gen = 1;           // bumped by every change of what a rendered note depends on
  std::vector<int> planKeys;  // the keys to render, in order
  int gridStep = 3;
  long tailLen = 0;           // release samples rendered after each note
  long staleWait = 0;         // samples left before re-rendering starts
  bool measureNeeded = false; // offset/trim must be measured again before keys are rendered
  int priorityKey = -1;       // a played key that is not rendered yet: rendered next
  bool modeDirty = false;
  bool replan = false;        // a state was loaded: the plan and the rendered keys follow its values
  struct Job {
    int kind = 0;             // 0 none, 1 level measurement (root note), 2 one key
    int key = 60, pool = -1;
    long n = 0, len = 0, total = 0, statLen = 0, keep = 0;
    double keyRatio = 1, pitchRatio = 1, offset = 0;
    size_t convNode = 0; long convI = 0; double convSum = 0; bool convDone = false;
    double sum = 0, lo = 0, hi = 0;
    uint32_t gen = 0;
  } job;
  std::vector<double> rootKeep; // the measured root note, kept to become key 60's buffer
  double renderRate = 0;      // the last slice's render budget, in buffer samples per output sample
  double renderCarry = 0;     // budget a slice left unspent before a convolver boundary: the next slice gets it
  struct ConvScale { int key; long total; double velocity, pitch, x, y; std::vector<float> scales; };
  std::vector<ConvScale> convScaleCache;  // the convolver scales of notes already played
  double pitchRatio = 1;      // 2^(pitch/12), updated when the Pitch parameter changes
  double dcCoef = 0;          // 1 - exp(-2*pi*5/sampleRate)
  uint32_t noteCounter = 0;   // seeds the noise outputs, so renders stay deterministic
  std::map<std::string, std::vector<uint8_t>> files;
  uint32_t filesDigest = 0x811c9dc5u;
  // parsed genome
  std::string genomePath;
  cppn::Cppn net;
  std::string schemeName = "legacy-2input";
  cgraph::SrcPatch src;
  cgraph::Plan plan;
  // kept so the pads can rebuild the plan without re-reading the delivered file
  minijson::Value genomeDoc;
  std::vector<minijson::Value> asNeatNodes, asNeatConns;
  bool hasAsNeat = false;
  bool geoDirty = false, geoMeasurePending = false;
  long geoWait = 0;
  // the plugin's page, MIDI learn, and parameter changes to report to the host
  const clap_host_webview_t* hostWebview = nullptr;
  bool guiOpen = false;
  int ccMap[kParamCount];
  int learnParam = -1;
  bool paramOut[kParamCount];
  struct PageNote { bool on; int key; double velocity; };
  std::vector<PageNote> pageNotes;
  float meterPeak = 0;
  bool loaded = false;
  bool active = false;
  // per-voice render state
  struct Voice {
    bool active = false;
    int key = 0;
    double velocity = 0;
    int32_t noteId = -1;
    int16_t channel = 0;
    long n = 0;             // samples since note on
    long total = 0;         // duration*sr
    bool released = false;
    double env = 0;
    long envLeft = 0;
    double envStep = 0;
    double noteRatio = 1;   // 2^((key - kRootKey)/12)
    double dc = 0;          // offset estimate of the DC filter
    bool dcPrimed = false;
    double offset = 0, trim = 1;  // the level measurement in force when the note started
    int buf = -1;           // pre-rendered mode: the pool entry this voice plays, -1 = rendered live
    double pos = 0;         // read position in that buffer
    // a note that cannot start yet: waiting for enough of its key to be rendered (pre-rendered mode), or
    // for its convolver pre-pass, which is done in slices over the following blocks (Live)
    bool waiting = false;
    bool offPending = false;  // released while waiting: released again once its attack has run
    bool prep = false;        // Live: the convolver pre-pass is not done yet
    size_t prepNode = 0; long prepI = 0; double prepSum = 0; double prepKeyRatio = 1;
    cgraph::VoiceState st;  // preallocated when the genome is loaded
  };
  Voice voices[kVoiceCount];
  uint32_t tailFrames = 0;
};

CppnEngine* self(const clap_plugin_t* p) { return static_cast<CppnEngine*>(p->plugin_data); }

double clampDouble(double v, double lo, double hi) { return std::min(hi, std::max(lo, v)); }

// ── genome + patch parsing ─────────────────────────────────────────────────────────────────────────────

const minijson::Value* findOffspring(const minijson::Value& root) {
  if (const minijson::Value* o = root.find("offspring")) return o;
  if (const minijson::Value* g = root.find("genome"))
    if (const minijson::Value* w = g->find("waveNetwork"))
      if (const minijson::Value* o = w->find("offspring")) return o;
  if (const minijson::Value* w = root.find("waveNetwork"))  // a genome without the {_id, genome} envelope
    if (const minijson::Value* o = w->find("offspring")) return o;
  if (root.find("nodes") && root.find("connections")) return &root;
  return nullptr;
}

// asNEATPatch may be a JSON string or a char-split object {"0":"{", "1":"\"", ...}
bool asNeatString(const minijson::Value& v, std::string& out) {
  if (v.type == minijson::Value::Str) { out = v.str; return true; }
  if (v.type == minijson::Value::Obj && !v.obj.empty()) {
    std::vector<std::pair<long, std::string>> chars;
    bool numericKeys = true;
    for (auto& kv : v.obj) {
      char* end = nullptr;
      long k = std::strtol(kv.first.c_str(), &end, 10);
      if (!end || *end != '\0' || kv.second.type != minijson::Value::Str || kv.second.str.size() != 1) {
        numericKeys = false;
        break;
      }
      chars.push_back({k, kv.second.str});
    }
    if (!numericKeys) return false;
    std::sort(chars.begin(), chars.end());
    out.clear();
    for (auto& c : chars) out += c.second;
    return true;
  }
  return false;
}

// The root note is rendered once (at most 4 s of it) to get the fixed offset and level trim (defined
// with the pre-rendered mode below, which renders it in slices).
void measureLevel(CppnEngine* s);
void decideMode(CppnEngine* s);
double estimateSpeed(CppnEngine* s);
void jobAbort(CppnEngine* s);
void poolTrim(CppnEngine* s);
long noteTotal(const CppnEngine* s);

// Decode the CPPN and compile the patch from the stored genome with the pads' current positions.
// Called on load and whenever a Weights / Frequencies parameter has moved. The pads change values,
// never the structure, so sounding voices carry on with their state.
void buildPlan(CppnEngine* s) {
  const cgraph::Geo geo{s->wx, s->wy, s->fx, s->fy};
  const minijson::Value* off = findOffspring(s->genomeDoc);
  if (!off) return;
  double cppnShift = 0;
  if (geo.weightsOn()) {
    // position of the last CPPN node in the mapper's layout: hash of `wave-${index}`
    const minijson::Value* cn = off->find("nodes");
    const size_t count = cn && cn->type == minijson::Value::Arr ? cn->arr.size() : 0;
    if (count > 0) {
      const uint32_t h = cgraph::jsHash("wave-" + std::to_string(count - 1));
      const double px = (double)(h % 200) / 100.0 - 1.0, py = (double)((h >> 8) % 200) / 100.0 - 1.0;
      cppnShift = ((geo.wx - 0.5) * 2 * px + (geo.wy - 0.5) * 2 * py) * 5.0;
    }
  }
  s->net.decode(*off, geo.weightsOn(), cppnShift);
  // scheme override from the genome envelope
  const minijson::Value* schemeV = nullptr;
  if (const minijson::Value* g = s->genomeDoc.find("genome")) schemeV = g->find("cppnInputScheme");
  if (!schemeV) schemeV = s->genomeDoc.find("cppnInputScheme");
  if (schemeV && schemeV->type == minijson::Value::Str) {
    const std::string& sn = schemeV->str;
    if (sn == "hybrid-phase") s->net.scheme = cppn::SCHEME_HYBRID_PHASE;
    else if (sn == "phase-modulated") s->net.scheme = cppn::SCHEME_PHASE_MODULATED;
    else s->net.scheme = cppn::SCHEME_LEGACY_2INPUT;
  }
  s->schemeName = s->net.scheme == cppn::SCHEME_HYBRID_PHASE
                      ? "hybrid-phase"
                      : s->net.scheme == cppn::SCHEME_PHASE_MODULATED ? "phase-modulated" : "legacy-2input";
  if (s->hasAsNeat) {
    s->src = cgraph::SrcPatch();
    cgraph::parseAsNeat(s->asNeatNodes, s->asNeatConns, s->src, geo);
  }
  // default graph ONLY when the genome carries no patch at all (a bare CPPN): output 0 at the
  // played key -> output. A patch without network outputs is played as it is.
  if (!s->src.present && s->net.outputCount > 0) {
    cgraph::SrcNetOut no;
    no.id = "default";
    no.cppnOut = 0;
    no.frequency = 440;
    cgraph::SrcSigConn sc;
    sc.tgt = "defaultBuf";
    sc.param = "buffer";
    no.conns.push_back(sc);
    s->src.netOuts.push_back(no);
    cgraph::SrcNode sn;
    sn.id = "defaultBuf";
    sn.type = "bufferSource";
    s->src.nodes.push_back(sn);
    cgraph::SrcEdge e;
    e.src = "defaultBuf";
    e.toOutput = true;
    s->src.edges.push_back(e);
    s->src.present = true;
  }
  const int keepCap = s->plan.convCap;
  cgraph::compile(s->src, s->sampleRate, s->plan);
  s->plan.convCap = keepCap;
  {
    std::vector<int> used;
    for (const auto& sl : s->plan.slots) if (!sl.noise) used.push_back(sl.cppnOut);
    s->net.prepare(used);
  }
}

// Times real time one voice runs at, estimated from the compiled plan (cgraph::costFeatures) with
// weights fitted on the calibration machine by poc-cppn-wasm/cost-calibrate.mjs (microseconds per
// unit; the Faust feature is already in microseconds).
// Fitted 2026-11-12 on 49 genomes (M1 Pro, Node 22 reference host): estimate within a factor 2.8 of the
// measured speed, and within 5 % for the genomes measured between 11 and 13x.
constexpr double kCostWeights[cgraph::kCostFeatures - 1] = {0.06017, 0.001905, 0.0, 0.02766, 0.0, 0.3083, 0.1669, 0.04900, 0.01381, 5.399};
double estimateSpeed(CppnEngine* s) {
  double f[cgraph::kCostFeatures];
  cgraph::costFeatures(s->plan, s->net, f);
  double us = f[cgraph::kCostFeatures - 1];
  for (int i = 0; i < cgraph::kCostFeatures - 1; i++) us += f[i] * kCostWeights[i];
  const double cppnUs = f[1] / (double)std::max<size_t>(1, s->plan.groups.size()) * kCostWeights[1];   // one evaluation
  s->cppnShare = us > 0 ? std::max(0.02, cppnUs / us) : 0.25;
  if (us <= 0) return 1e9;
  return 1e6 / (us * s->sampleRate);
}

bool loadGenome(CppnEngine* s) {
  // pick the first delivered *.json with an offspring (genome) — a second *.json with audioGraph is a
  // pre-converted patch
  std::string genomeFile, patchFile;
  for (auto& kv : s->files) {
    const std::string& path = kv.first;
    if (path.size() < 5 || path.compare(path.size() - 5, 5, ".json") != 0) continue;
    minijson::Value root;
    std::string text((const char*)kv.second.data(), kv.second.size());
    minijson::Parser p(text);
    if (!p.parse(root)) continue;
    if (root.find("audioGraph")) {
      patchFile = path;
      continue;
    }
    if (findOffspring(root) && genomeFile.empty()) genomeFile = path;
  }
  if (genomeFile.empty()) {
    std::printf("cppn-engine: no genome json delivered\n");
    return false;
  }
  s->genomePath = genomeFile;
  const std::vector<uint8_t>& bytes = s->files[genomeFile];
  minijson::Value root;
  std::string text((const char*)bytes.data(), bytes.size());
  minijson::Parser p(text);
  if (!p.parse(root)) return false;
  const minijson::Value* off = findOffspring(root);
  if (!off) return false;
  s->genomeDoc = std::move(root);
  // patch: separate pre-converted patch file wins; else embedded asNEATPatch
  s->src = cgraph::SrcPatch();
  s->hasAsNeat = false;
  s->asNeatNodes.clear();
  s->asNeatConns.clear();
  if (!patchFile.empty()) {
    const std::vector<uint8_t>& pb = s->files[patchFile];
    minijson::Value proot;
    std::string ptext((const char*)pb.data(), pb.size());
    minijson::Parser pp(ptext);
    if (pp.parse(proot)) cgraph::parseSynthIsPatch(proot, s->src);
  } else {
    const minijson::Value* asNeat = nullptr;
    std::string asNeatStr;
    if (const minijson::Value* g = s->genomeDoc.find("genome")) asNeat = g->find("asNEATPatch");
    if (!asNeat) asNeat = s->genomeDoc.find("asNEATPatch");
    if (asNeat && asNeat->type == minijson::Value::Obj && asNeat->find("nodes")) {
      // already an object {nodes, connections} (the web app's genome cache holds it parsed)
      cgraph::splitAsNeat(*asNeat, s->asNeatNodes, s->asNeatConns);
      s->hasAsNeat = !s->asNeatNodes.empty();
    } else if (asNeat && asNeatString(*asNeat, asNeatStr)) {
      minijson::Value an;
      minijson::Parser ap(asNeatStr);
      if (ap.parse(an)) { cgraph::splitAsNeat(an, s->asNeatNodes, s->asNeatConns); s->hasAsNeat = !s->asNeatNodes.empty(); }
    }
  }
  buildPlan(s);
  s->plan.convCap = (int)(std::min(10.0, s->durationSec) * s->sampleRate) + 1;  // convolver responses are kept up to 10 s
  for (auto& v : s->voices) v.st.allocate(s->plan);
  // pre-rendered mode: start over for this genome, with the speed estimated from the plan
  s->pre = false;
  jobAbort(s);
  for (int& k : s->keyBuf) k = -1;
  s->priorityKey = -1;
  s->measureNeeded = false;
  poolTrim(s);
  s->speed = estimateSpeed(s);
  s->speedMeasured = false;
  decideMode(s);
  // the level measurement renders the root note; when the host's clock moves during it (a real clock:
  // our hosts give a virtual one that stands still), its speed replaces the estimate
  {
    timespec t0{}, t1{};
    const bool c0 = clock_gettime(CLOCK_MONOTONIC, &t0) == 0;
    measureLevel(s);
    const bool c1 = clock_gettime(CLOCK_MONOTONIC, &t1) == 0;
    const double secs = (double)(t1.tv_sec - t0.tv_sec) + 1e-9 * (double)(t1.tv_nsec - t0.tv_nsec);
    const double audio = (double)std::min(noteTotal(s), (long)(4.0 * s->sampleRate)) / s->sampleRate;
    if (c0 && c1 && secs > 0.002 && audio > 0) {
      s->speed = audio / secs;
      s->speedMeasured = true;
      decideMode(s);
    }
  }
  int patchEdges = 0;
  for (auto& n : s->plan.nodes) {
    patchEdges += (int)n.ins.size();
    for (auto& pr : n.p) patchEdges += (int)pr.mods.size();
  }
  int cppnConns = 0;
  for (auto& in : s->net.incoming) cppnConns += (int)in.size();
  std::string unsupported;
  for (auto& u : s->plan.unsupported) unsupported += (unsupported.empty() ? "" : ",") + u;
  std::printf("cppn-engine: playing mode=%s reason=%s speed=%.2fx keys=%zu step=%d\n", s->pre ? "pre-rendered" : "live",
              s->preReason == 2 ? "forced" : s->preReason == 1 ? "measured" : "estimated", s->speed, s->planKeys.size(), s->gridStep);
  std::printf("cppn-engine: loaded genome=%s cppnNodes=%d cppnConns=%d outputs=%d scheme=%s patchNodes=%zu patchEdges=%d netOuts=%zu signals=%zu frequencies=%zu unsupported=%s trim=%.4f\n",
              genomeFile.c_str(), s->net.total, cppnConns, s->net.outputCount, s->schemeName.c_str(),
              s->src.nodes.size(), patchEdges, s->src.netOuts.size(), s->plan.slots.size(), s->plan.groups.size(),
              unsupported.empty() ? "none" : unsupported.c_str(), s->trim);
  {
    double f[cgraph::kCostFeatures];
    cgraph::costFeatures(s->plan, s->net, f);
    std::printf("cppn-engine: cost features=");
    for (int i = 0; i < cgraph::kCostFeatures; i++) std::printf("%s%.6g", i ? "," : "", f[i]);
    std::printf("\n");
  }
  s->loaded = true;
  return true;
}

// ── per-note set-up (no allocation: voice state was sized when the genome was loaded) ───────────────────

void setupVoice(CppnEngine* s, CppnEngine::Voice& v, int key, double velocity) {
  v.waiting = false; v.offPending = false; v.prep = false;
  v.key = key;
  v.velocity = velocity;
  v.n = 0;
  v.released = false;
  v.env = 0;
  v.total = std::max(1.0, (double)std::lround(s->durationSec * s->sampleRate));
  v.noteRatio = std::pow(2.0, (key - kRootKey) / 12.0);
  v.dc = 0;
  v.dcPrimed = false;
  v.offset = s->offset;
  v.trim = s->trim;
  v.buf = -1;
  v.st.reset(++s->noteCounter);
  if (s->plan.numConvs > 0) {
    // the convolver's scale needs the whole response: one CPPN pass over the note, remembered per key
    CppnEngine::ConvScale* hit = nullptr;
    for (auto& c : s->convScaleCache)
      if (c.key == key && c.total == v.total && c.velocity == velocity && c.pitch == s->pitch && c.x == s->x && c.y == s->y) hit = &c;
    if (hit) { for (int i = 0; i < s->plan.numConvs; i++) v.st.conv[i].scale = hit->scales[i]; }
    else if (s->offline) {
      cgraph::RenderCtx rc;
      rc.n = 0; rc.total = v.total; rc.keyRatio = v.noteRatio * s->pitchRatio; rc.velocity = velocity; rc.x = s->x; rc.y = s->y;
      cgraph::prepareConvolvers(s->plan, s->net, v.st, rc);
    } else {
      // not done here: one CPPN pass over the whole note stalled the audio thread (0.76 s for a 30 s note
      // measured); prepSlice does it at the end of the following blocks and the note waits for it
      v.prep = true;
      v.waiting = true;
      v.prepNode = 0; v.prepI = 0; v.prepSum = 0;
      v.prepKeyRatio = v.noteRatio * s->pitchRatio;
    }
  }
  // envelope: attack then sustain; release handled on note off / end
  double atk = std::max(0.0, s->attackSec) * s->sampleRate;
  v.envLeft = (long)atk;
  v.envStep = atk > 0 ? 1.0 / atk : 1.0;
  if (v.envLeft == 0) v.env = 1.0;
}

// one sample of a note's graph, with the render server's output limiter, before the offset
inline double renderRaw(CppnEngine* s, cgraph::VoiceState& st, long n, long total, double keyRatio, double velocity) {
  cgraph::RenderCtx rc;
  rc.n = n;
  rc.total = total;
  rc.keyRatio = keyRatio;
  rc.velocity = velocity;
  rc.x = s->x;
  rc.y = s->y;
  rc.limiter = s->limiter >= 0.5;
  double out = cgraph::renderSample(s->plan, s->net, st, rc);
  // the render server's output limiter (it runs with it in production); it sees the raw sum, offset and all
  if (rc.limiter) {
    const double* o = &st.out[(size_t)s->plan.output * cgraph::kMaxCh];
    out = cgraph::outputLimiter(st.limiter, o[1], o[2], s->plan.limiterChans, n, s->sampleRate);
  }
  return out;
}

// render one sample of the voice's graph; returns mono output
float renderVoiceSample(CppnEngine* s, CppnEngine::Voice& v) {
  // the Pitch parameter bends sounding notes
  double out = renderRaw(s, v.st, v.n, v.total, v.noteRatio * s->pitchRatio, v.velocity);
  // The renderer removes the whole render's mean. A live voice cannot know its own, so the mean of
  // the root note, measured when the genome was loaded, is subtracted (a fixed offset: slow
  // sub-audio movement in the sound is kept, as the renderer keeps it).
  if (s->dcFilter >= 0.5) out -= v.offset;
  return (float)out;
}

// ── pre-rendered playing mode ──────────────────────────────────────────────────────────────────────────
// For genomes too slow to play live (decideMode): the keys of planKeys are rendered, one at a time, in
// slices at the end of process() (no host here gives the plugin a thread: the browser's AudioWorklet
// is one thread, and the reference host has no thread pool), and played back as a sampler. A key
// rendered at its own pitch is the live render of that key, sample for sample (same code path,
// velocity 1, the voice's gain applied afterwards). Anything a rendered note depends on (pads,
// Duration, Pitch, CPPN X/Y, Limiter, DC Filter, a longer Release) bumps `gen`: older buffers keep
// playing until each key is rendered again, 0.3 s after the last change.

bool bufFresh(const CppnEngine* s, int key) {
  if (key < 0 || key > 127) return false;
  const int b = s->keyBuf[key];
  if (b < 0) return false;
  const CppnEngine::Buf& e = s->pool[(size_t)b];
  return e.gen == s->gen && e.filled >= e.len && (s->dcFilter < 0.5 || e.offset == s->offset);
}

bool poolInUse(const CppnEngine* s, int idx) {
  if (s->job.kind == 2 && s->job.pool == idx) return true;
  for (int k = 0; k < 128; k++) if (s->keyBuf[k] == idx) return true;
  for (const auto& v : s->voices) if (v.active && v.buf == idx) return true;
  return false;
}

size_t poolBytes(const CppnEngine* s) {
  size_t b = s->rootKeep.capacity() * sizeof(double);
  for (const auto& e : s->pool) b += e.data.capacity() * sizeof(float);
  return b;
}

// a pool entry that nothing plays or maps, sized for `need` samples (allocates only when it must)
int poolTake(CppnEngine* s, long need) {
  int idx = -1;
  for (size_t i = 0; i < s->pool.size() && idx < 0; i++)   // an entry already big enough first (no allocation)
    if (!poolInUse(s, (int)i) && (long)s->pool[i].data.capacity() >= need) idx = (int)i;
  for (size_t i = 0; i < s->pool.size() && idx < 0; i++) if (!poolInUse(s, (int)i)) idx = (int)i;
  if (idx < 0) { s->pool.emplace_back(); idx = (int)s->pool.size() - 1; }
  CppnEngine::Buf& e = s->pool[(size_t)idx];
  if ((long)e.data.size() < need) e.data.resize((size_t)need, 0.f);
  e.len = need;
  e.filled = 0;
  e.gen = 0;
  e.key = -1;
  return idx;
}

// release the memory of entries nothing uses any more
void poolTrim(CppnEngine* s) {
  for (size_t i = 0; i < s->pool.size(); i++)
    if (!poolInUse(s, (int)i)) { std::vector<float>().swap(s->pool[i].data); s->pool[i].len = 0; s->pool[i].gen = 0; }
  while (!s->pool.empty() && s->pool.back().data.capacity() == 0 && !poolInUse(s, (int)s->pool.size() - 1)) s->pool.pop_back();
}

long noteTotal(const CppnEngine* s) { return (long)std::max(1.0, (double)std::lround(s->durationSec * s->sampleRate)); }

// Which keys to render: the root, every gridStep-th key out to kKeyLo..kKeyHi, then the keys between,
// as far as kMemCap allows (the grid widens when even the coarse keys do not fit).
void makePlan(CppnEngine* s) {
  s->tailLen = (long)(std::max(0.0, s->releaseSec) * s->sampleRate) + 1;
  const size_t perKey = (size_t)(noteTotal(s) + s->tailLen) * sizeof(float);
  const int maxKeys = (int)std::max<size_t>(1, kMemCap / perKey - 1);   // one spare for the key being rendered
  const int steps[] = {3, 4, 6, 12};
  s->gridStep = 12;
  for (int st : steps) {
    int coarse = 0;
    for (int k = kKeyLo; k <= kKeyHi; k++) if ((k - kRootKey) % st == 0) coarse++;
    if (coarse <= maxKeys) { s->gridStep = st; break; }
  }
  std::vector<int> coarse, fill;
  for (int d = 0; d <= kKeyHi - kKeyLo; d++)
    for (int sign : {1, -1}) {
      if (d == 0 && sign < 0) continue;
      const int k = kRootKey + sign * d;
      if (k < kKeyLo || k > kKeyHi) continue;
      (d % s->gridStep == 0 ? coarse : fill).push_back(k);
    }
  s->planKeys.clear();
  for (int k : coarse) if ((int)s->planKeys.size() < maxKeys) s->planKeys.push_back(k);
  for (int k : fill) if ((int)s->planKeys.size() < maxKeys) s->planKeys.push_back(k);
  // keys no longer planned are dropped (voices still playing them keep their entry until they end)
  for (int k = 0; k < 128; k++)
    if (s->keyBuf[k] >= 0 && std::find(s->planKeys.begin(), s->planKeys.end(), k) == s->planKeys.end()) s->keyBuf[k] = -1;
  poolTrim(s);
  // Reserve every buffer now (at load, or when Duration / Release change), not while rendering: growing
  // the wasm memory in the middle of a render slice cost 10-30 ms blocks in the reference host.
  // Only `want` entries are sized: reserving every entry of the pool (the old keys' too) took 137 MB
  // after a 4 s -> 30 s Duration change, against the 64 MB cap.
  const size_t want = s->planKeys.size() + 1, samples = perKey / sizeof(float);
  size_t ready = 0;
  for (const auto& e : s->pool) if (e.data.capacity() >= samples) ready++;
  for (size_t i = 0; i < s->pool.size() && ready < want; i++)
    if (!poolInUse(s, (int)i) && s->pool[i].data.capacity() < samples) {
      std::vector<float>().swap(s->pool[i].data);   // release the shorter buffer before sizing it again
      s->pool[i].data.reserve(samples); s->pool[i].len = 0; ready++;
    }
  while (ready < want) { s->pool.emplace_back(); s->pool.back().data.reserve(samples); ready++; }
}

bool keyPlanned(const CppnEngine* s, int key) { return std::find(s->planKeys.begin(), s->planKeys.end(), key) != s->planKeys.end(); }

// a note waits for, or plays from, the buffer of the key job: the job is not given up for anything less
bool jobPinned(const CppnEngine* s) {
  if (s->job.kind != 2) return false;
  for (const auto& v : s->voices) if (v.active && v.buf == s->job.pool) return true;
  return false;
}

void jobAbort(CppnEngine* s) {
  s->renderCarry = 0;
  // notes on the unfinished buffer would stop where the render stopped: they end here
  if (s->job.kind == 2)
    for (auto& v : s->voices) if (v.active && v.buf == s->job.pool) v.active = false;
  s->job.kind = 0; s->job.pool = -1;
}

// something a rendered note depends on has changed (called only when the value really changed)
void renderSettingsChanged(CppnEngine* s, bool remeasure) {
  if (!s->pre) return;
  s->gen++;
  // keys rendered with the old settings are not played any more (startVoice): their entries are free
  // for the new renders (notes still sounding from them keep them until they end)
  for (int& k : s->keyBuf) k = -1;
  s->staleWait = (long)(kDebounceSec * s->sampleRate);
  // a key a note is playing from is finished with the settings it started with (it is then out of date)
  if (!jobPinned(s)) jobAbort(s);
  if (remeasure) s->measureNeeded = true;
}

void jobStartMeasure(CppnEngine* s) {
  // the measurement of measureLevel: the root note at velocity 1, at most 4 s of it, root key ratio 1
  s->scratch.allocate(s->plan);
  s->scratch.reset(1);
  s->convScaleCache.clear();
  CppnEngine::Job& j = s->job;
  j = CppnEngine::Job();
  j.kind = 1;
  j.key = kRootKey;
  j.total = noteTotal(s);
  j.statLen = std::min(j.total, (long)(4.0 * s->sampleRate));
  j.keyRatio = 1.0;
  j.pitchRatio = s->pitchRatio;
  j.gen = s->gen;
  // in the pre-rendered mode the same render is key 60's note when it is all of it (and Pitch is 0)
  j.keep = (s->pre && s->pitchRatio == 1.0 && j.total <= j.statLen && keyPlanned(s, kRootKey)) ? j.total + s->tailLen : 0;
  j.len = std::max(j.statLen, j.keep);
  if (j.keep) s->rootKeep.resize((size_t)j.keep);
}

void jobStartKey(CppnEngine* s, int key) {
  s->scratch.reset(1u + (uint32_t)key);   // a fixed seed per key: a rendered noise output is the same on every press
  CppnEngine::Job& j = s->job;
  j = CppnEngine::Job();
  j.kind = 2;
  j.key = key;
  j.total = noteTotal(s);
  j.len = j.total + s->tailLen;
  j.pitchRatio = s->pitchRatio;
  j.keyRatio = std::pow(2.0, (key - kRootKey) / 12.0) * s->pitchRatio;   // as setupVoice + renderVoiceSample
  j.gen = s->gen;
  j.offset = s->offset;   // before a pending level measurement, if a played note could not wait for it
  j.pool = poolTake(s, j.len);
  CppnEngine::Buf& e = s->pool[(size_t)j.pool];
  // described now: a note can play it while it is being rendered
  e.total = j.total; e.key = key; e.keyRatio = std::pow(2.0, (key - kRootKey) / 12.0); e.pitchRatio = j.pitchRatio;
  e.offset = j.offset; e.filled = 0; e.gen = 0;
}

// Advance the job by at most `budget` voice-samples; returns what is left of the budget.
double jobStep(CppnEngine* s, double budget) {
  CppnEngine::Job& j = s->job;
  if (j.kind == 0) return budget;
  if (!j.convDone) {
    cgraph::RenderCtx rc;
    rc.n = 0; rc.total = j.total; rc.keyRatio = j.keyRatio; rc.velocity = 1.0; rc.x = s->x; rc.y = s->y;
    rc.limiter = s->limiter >= 0.5;
    long spent = 0;
    const double cap = budget / std::max(1e-3, s->cppnShare);
    const long allowed = cap > 2e9 ? 0x7fffffffL : (long)cap;
    j.convDone = cgraph::prepareConvolversStep(s->plan, s->net, s->scratch, rc, j.convNode, j.convI, j.convSum, allowed, spent);
    budget -= (double)spent * s->cppnShare;
    if (!j.convDone) return 0;
  }
  const bool dc = s->dcFilter >= 0.5;
  float* dst = j.kind == 2 ? s->pool[(size_t)j.pool].data.data() : nullptr;
  const double voiceSampleUs = 1e6 / (std::max(1e-3, s->speed) * s->sampleRate);
  const long startN = j.n;
  while (j.n < j.len && budget >= 1.0) {
    const long i = j.n;
    if (s->plan.numConvs > 0 && i >= cgraph::kConvB && i % cgraph::kConvB == 0) {
      // a convolver block boundary: every earlier block pair at once (see kConvPairUs)
      const long t = i / cgraph::kConvB;
      double pairs = 0;
      for (int c = 0; c < s->plan.numConvs; c++) {
        const long parts = s->scratch.conv[(size_t)c].parts;
        if (t < parts) pairs += (double)s->plan.convChans[(size_t)c] * (double)(t - 1);
      }
      const double extra = pairs * kConvPairUs / voiceSampleUs;
      if (extra > budget && j.n > startN) break;   // the next slice starts with it (renderSlice keeps the rest)
      budget -= extra;
    }
    const double v = renderRaw(s, s->scratch, i, j.total, j.keyRatio, 1.0);
    if (j.kind == 1) {
      if (i < j.statLen) {
        j.sum += v;
        if (i == 0 || v < j.lo) j.lo = v;
        if (i == 0 || v > j.hi) j.hi = v;
      }
      if (i < j.keep) s->rootKeep[(size_t)i] = v;
    } else {
      dst[i] = (float)(dc ? v - j.offset : v);
    }
    j.n++;
    budget -= 1.0;
  }
  if (j.kind == 2) s->pool[(size_t)j.pool].filled = j.n;
  if (j.n < j.len) return budget;
  if (j.kind == 1) {
    const long len = j.statLen;
    const double mean = len > 0 ? j.sum / (double)len : 0.0;
    const double peak = std::max(j.hi - mean, mean - j.lo);
    s->offset = mean;
    s->trim = peak > 1.0 ? 1.0 / peak : 1.0;
    s->measureNeeded = false;
    if (j.keep && j.gen == s->gen) {
      const int idx = poolTake(s, j.keep);
      CppnEngine::Buf& e = s->pool[(size_t)idx];
      for (long i = 0; i < j.keep; i++) e.data[(size_t)i] = (float)(dc ? s->rootKeep[(size_t)i] - s->offset : s->rootKeep[(size_t)i]);
      e.total = j.total; e.key = kRootKey; e.keyRatio = 1.0; e.pitchRatio = j.pitchRatio; e.gen = j.gen;
      e.offset = s->offset; e.filled = j.keep;
      s->keyBuf[kRootKey] = idx;
    }
    std::vector<double>().swap(s->rootKeep);
  } else {
    CppnEngine::Buf& e = s->pool[(size_t)j.pool];
    e.gen = j.gen;
    e.filled = e.len;
    s->keyBuf[j.key] = j.pool;
    if (s->priorityKey == j.key) s->priorityKey = -1;
  }
  jobAbort(s);
  return budget;
}

void measureLevel(CppnEngine* s) {
  // Level: the renderer removes the mean and divides by the peak when it exceeds 1. A live voice
  // cannot know its mean or peak, so the root note is rendered once here (at most 4 s of it) and the same
  // rule gives a fixed trim for the instrument. Other keys and durations can differ from it.
  jobStartMeasure(s);
  jobStep(s, 1e18);
}

int liveSlots(const CppnEngine* s) {
  if (!s->pre) return (int)kVoiceCount;
  if (s->speed < 1.6) return 0;
  if (s->plan.numConvs > 0) return 0;   // a live note on a convolver genome stalls for its CPPN pre-pass (50 ms measured)
  return std::min((int)kVoiceCount, std::max(1, (int)std::floor(kRenderShare * s->speed)));
}

int liveVoicesSounding(const CppnEngine* s) {
  int n = 0;
  for (const auto& v : s->voices) if (v.active && v.buf < 0) n++;
  return n;
}

// the next key to render: a played key first, then the plan in order
int nextKey(const CppnEngine* s) {
  if (s->priorityKey >= 0 && !bufFresh(s, s->priorityKey)) return s->priorityKey;
  for (int k : s->planKeys) if (!bufFresh(s, k)) return k;
  return -1;
}

// end of process(): render with what is left of the block
double renderBudget(const CppnEngine* s, uint32_t frames) {
  const double share = jobPinned(s) ? kStreamShare : kRenderShare;
  return share * frames * s->speed - (double)liveVoicesSounding(s) * frames;
}

void renderSlice(CppnEngine* s, uint32_t frames) {
  if (!s->pre || !s->loaded) return;
  // a played note waiting for its key is not held back by the debounce
  const bool played = jobPinned(s) || (s->priorityKey >= 0 && !bufFresh(s, s->priorityKey));
  if (s->staleWait > 0 && !played) { s->staleWait -= (long)frames; return; }
  s->staleWait = 0;
  double budget = renderBudget(s, frames);
  s->renderRate = std::max(0.0, budget / std::max(1u, frames));
  budget += s->renderCarry;
  s->renderCarry = 0;
  for (int guard = 0; budget >= 1.0 && guard < 4; guard++) {
    if (s->job.kind == 0) {
      // a played key before the level measurement: the measurement is redone after it
      const bool pri = s->priorityKey >= 0 && !bufFresh(s, s->priorityKey);
      if (s->measureNeeded && !pri) jobStartMeasure(s);
      else {
        const int k = nextKey(s);
        if (k < 0) { if (s->measureNeeded) jobStartMeasure(s); else return; }
        else jobStartKey(s, k);
      }
    }
    budget = jobStep(s, budget);
  }
  // what is left (a fraction of a sample, or a slice that stopped before a convolver boundary) is kept
  // for the next slice while there is work: the note-start check counts on every bit of the budget
  if (s->job.kind != 0 && budget > 0) s->renderCarry = std::min(s->renderCarry + budget, renderBudget(s, frames));
}

// The key job's work left, in voice-samples: the convolver pre-pass, the samples, and the convolver
// block boundaries still to come (their cost grows along the note, see kConvPairUs).
// Up to buffer index `upto` (the whole job when upto >= len).
double jobWorkLeft(const CppnEngine* s, long upto = -1) {
  const CppnEngine::Job& j = s->job;
  if (j.kind == 0) return 0;
  const long end = upto < 0 ? j.len : std::min(upto, j.len);
  double w = (double)std::max(0L, end - j.n);
  if (s->plan.numConvs > 0) {
    if (!j.convDone) {
      long left = 0;
      for (size_t i = j.convNode; i < s->plan.nodes.size(); i++)
        if (s->plan.nodes[i].kind == cgraph::K_CONV && s->plan.nodes[i].buf.slot >= 0) left += j.total;
      w += (double)std::max(0L, left - j.convI) * s->cppnShare;
    }
    const double voiceSampleUs = 1e6 / (std::max(1e-3, s->speed) * s->sampleRate);
    const long t0 = std::max(2L, (j.n + cgraph::kConvB - 1) / cgraph::kConvB), t1 = (end - 1) / cgraph::kConvB;
    for (int c = 0; c < s->plan.numConvs; c++) {
      const long hi = std::min(t1, (long)s->scratch.conv[(size_t)c].parts - 1);
      if (hi < t0) continue;
      const double pairs = (double)(hi - 1) * hi / 2.0 - (double)(t0 - 2) * (t0 - 1) / 2.0;   // sum of (t - 1), t0..hi
      w += s->plan.convChans[(size_t)c] * pairs * kConvPairUs / voiceSampleUs;
    }
  }
  return w;
}

// A note reading the job's buffer at `rate` (buffer samples per output sample) can start when the job
// will be done before the note reaches the end of the buffer, two blocks early. The render slows along
// the note while a convolver's boundaries last (they cost more and more) and the note reads at a steady
// pace, so the lead (rendered minus read) is concave over that stretch: its smallest value is at the
// start (startWaiting), where the boundaries stop, or at the end. Returns the smallest margin of the
// last two in voice-samples (>= 0: in time).
// Checked at the end and where a convolver's boundaries stop (its kept length): there the render speeds
// up again, so the lead can be smallest there too (a 10 s note held for 0.2 s near its end without it).
// The lead kept in hand, in output samples: two blocks, and with a convolver two of its blocks as well,
// because its render arrives in bursts of kConvB samples, each after a boundary (holds were measured
// near the end of a 10 s note with two process blocks of lead).
double streamMargin(const CppnEngine* s, uint32_t frames) {
  return 2.0 * frames + (s->plan.numConvs > 0 ? 2.0 * cgraph::kConvB : 0.0) + 8.0;
}

double streamSlack(const CppnEngine* s, const CppnEngine::Buf& e, double rate, uint32_t frames) {
  const double r = s->renderRate, m = streamMargin(s, frames);
  auto at = [&](long c) { return r * ((double)c / rate - m) - jobWorkLeft(s, c); };
  double slack = at(e.len);
  for (int c = 0; c < s->plan.numConvs; c++) {
    const long cap = (long)s->scratch.conv[(size_t)c].parts * cgraph::kConvB;
    if (cap < e.len && cap > s->job.n) slack = std::min(slack, at(cap));
  }
  return slack;
}

double voiceRate(const CppnEngine* s, const CppnEngine::Voice& v, const CppnEngine::Buf& e) {
  return (v.noteRatio * s->pitchRatio) / (e.keyRatio * e.pitchRatio);
}

// at the start of a block: notes waiting for their key start once enough of it is rendered
void startWaiting(CppnEngine* s, uint32_t frames) {
  for (auto& v : s->voices) {
    if (!v.active || !v.waiting || v.prep || v.buf < 0) continue;
    const CppnEngine::Buf& e = s->pool[(size_t)v.buf];
    const bool rendering = s->job.kind == 2 && s->job.pool == v.buf;
    if (!rendering || e.filled >= e.len) { v.waiting = false; continue; }
    // the render may be slower than the note at first, then the end decides (both ends checked)
    const double rate = voiceRate(s, v, e);
    const bool headOk = (double)e.filled >= rate * streamMargin(s, frames);
    if (s->job.convDone && headOk && streamSlack(s, e, rate, frames) >= 0) v.waiting = false;
  }
}

// seconds until the first waiting note can start (an estimate from the render budget), -1 = none waiting
double waitSeconds(const CppnEngine* s) {
  double best = -1;
  for (const auto& v : s->voices) {
    if (!v.active || !v.waiting) continue;
    double samples = 0, rate = 0;   // voice-samples of work before it can start, and done per output sample
    if (v.prep) {
      long left = 0;
      for (size_t i = v.prepNode; i < s->plan.nodes.size(); i++)
        if (s->plan.nodes[i].kind == cgraph::K_CONV && s->plan.nodes[i].buf.slot >= 0) left += v.total;
      samples = (double)std::max(0L, left - v.prepI) * s->cppnShare;
      rate = std::max(1e-3, kPrepShare * s->speed);
    } else if (v.buf >= 0 && s->job.kind == 2 && s->job.pool == v.buf) {
      const CppnEngine::Buf& e = s->pool[(size_t)v.buf];
      samples = std::max(0.0, -streamSlack(s, e, voiceRate(s, v, e), 128));
      rate = std::max(1e-3, s->renderRate);
    } else continue;
    const double sec = samples / rate / s->sampleRate;
    if (best < 0 || sec < best) best = sec;
  }
  return best;
}

// Live: the convolver pre-pass of waiting notes, in slices at the end of process()
void prepSlice(CppnEngine* s, uint32_t frames) {
  int live = 0;
  for (const auto& v : s->voices) if (v.active && !v.waiting && v.buf < 0) live++;
  const double share = live ? kRenderShare : kPrepShare;
  double budget = std::max(0.25 * s->speed, share * s->speed - live) * frames;   // in voice-samples
  for (auto& v : s->voices) {
    if (!v.active || !v.prep || budget < 1.0) continue;
    cgraph::RenderCtx rc;
    rc.n = 0; rc.total = v.total; rc.keyRatio = v.prepKeyRatio; rc.velocity = v.velocity; rc.x = s->x; rc.y = s->y;
    long spent = 0;
    const long allowed = (long)std::min(2e9, budget / std::max(1e-3, s->cppnShare));
    const bool done = cgraph::prepareConvolversStep(s->plan, s->net, v.st, rc, v.prepNode, v.prepI, v.prepSum, allowed, spent);
    budget -= (double)spent * s->cppnShare;
    if (!done) continue;
    v.prep = false;
    v.waiting = false;
    if (s->convScaleCache.size() >= 64) s->convScaleCache.erase(s->convScaleCache.begin());
    CppnEngine::ConvScale c{v.key, v.total, v.velocity, s->pitch, s->x, s->y, {}};
    for (int i = 0; i < s->plan.numConvs; i++) c.scales.push_back(v.st.conv[i].scale);
    s->convScaleCache.push_back(c);
  }
}

// Auto / Live / Pre-rendered
void decideMode(CppnEngine* s) {
  s->modeDirty = false;
  const int forced = (int)std::lround(s->prerenderParam);
  const bool want = s->offline ? false : forced == 1 ? false : forced == 2 ? true : s->speed < kLiveThreshold;
  s->preReason = forced != 0 ? 2 : s->speedMeasured ? 1 : 0;
  if (want == s->pre) return;
  s->pre = want;
  jobAbort(s);
  s->priorityKey = -1;
  if (s->pre) {
    makePlan(s);
    s->gen++;
    s->staleWait = 0;
  } else {
    for (int& k : s->keyBuf) k = -1;
    poolTrim(s);
  }
}

// ── notes ──────────────────────────────────────────────────────────────────────────────────────────────

// a voice that plays a pre-rendered key: the same envelope and gain as a live one
void setupSampler(CppnEngine* s, CppnEngine::Voice& v, int key, double velocity, int b) {
  const CppnEngine::Buf& e = s->pool[(size_t)b];
  v.key = key;
  v.velocity = velocity;
  v.n = 0;
  v.released = false;
  v.env = 0;
  v.noteRatio = std::pow(2.0, (key - kRootKey) / 12.0);
  const double rate = (v.noteRatio * s->pitchRatio) / (e.keyRatio * e.pitchRatio);
  v.total = rate == 1.0 ? e.total : std::max(1L, (long)std::lround((double)e.total / rate));
  v.dc = 0;
  v.dcPrimed = false;
  v.offset = s->offset;
  v.trim = s->trim;
  v.buf = b;
  v.pos = 0;
  ++s->noteCounter;
  double atk = std::max(0.0, s->attackSec) * s->sampleRate;
  v.envLeft = (long)atk;
  v.envStep = atk > 0 ? 1.0 / atk : 1.0;
  if (v.envLeft == 0) v.env = 1.0;
}

// one sample of a pre-rendered key; resampled (cubic Hermite) when the key or Pitch differs from the render's
float samplerSample(CppnEngine* s, CppnEngine::Voice& v) {
  const CppnEngine::Buf& e = s->pool[(size_t)v.buf];
  const double rate = (v.noteRatio * s->pitchRatio) / (e.keyRatio * e.pitchRatio);
  const long i = (long)v.pos;
  // the note caught up with its key's render (the budget fell after it started): hold until it is there
  if (e.filled < e.len && i + 3 >= e.filled) return 0.f;
  const double fr = v.pos - (double)i;
  auto at = [&](long k) { return (k >= 0 && k < e.len) ? (double)e.data[(size_t)k] : 0.0; };
  double x;
  if (fr == 0.0) x = at(i);
  else {
    const double xm = at(i - 1), x0 = at(i), x1 = at(i + 1), x2 = at(i + 2);
    const double c1 = 0.5 * (x1 - xm), c2 = xm - 2.5 * x0 + 2.0 * x1 - 0.5 * x2, c3 = 0.5 * (x2 - xm) + 1.5 * (x0 - x1);
    x = ((c3 * fr + c2) * fr + c1) * fr + x0;
  }
  v.pos += rate;
  return (float)x;
}

// Start a note on voice v. Live, or in the pre-rendered mode, in this order:
//   (1) its own key rendered, (2) a neighbour a semitone away (above first: its level error measured
//   smaller), (3) the planned key nearest to it when it is not planned itself, (4) a live voice if one
//   fits, (5) its key (or that nearest planned key) rendered now and played as it is rendered: the note
//   waits until enough of it is there (streamSlack), (6) when another played key is being rendered: the
//   nearest rendered key, else that key's render, resampled. Keys rendered with older settings are not
//   played: they would not be what the controls say (a Duration change was not heard for ~40 s).
// Returns false when nothing could play it.
bool startVoice(CppnEngine* s, CppnEngine::Voice& v, int key, double velocity) {
  v.buf = -1;
  v.waiting = false; v.offPending = false; v.prep = false;
  if (!s->pre) { setupVoice(s, v, key, velocity); return true; }
  int target = key;
  if (!keyPlanned(s, key)) {
    int best = -1;
    for (int k : s->planKeys) if (best < 0 || std::abs(k - key) < std::abs(best - key)) best = k;
    target = best;
  }
  int b = -1;
  if (bufFresh(s, key)) b = s->keyBuf[key];
  else if (bufFresh(s, key + 1)) b = s->keyBuf[key + 1];
  else if (bufFresh(s, key - 1)) b = s->keyBuf[key - 1];
  else if (target >= 0 && bufFresh(s, target)) b = s->keyBuf[target];
  if (b >= 0) { setupSampler(s, v, key, velocity, b); return true; }
  int live = 0;
  for (const auto& o : s->voices) if (&o != &v && o.active && o.buf < 0) live++;
  if (live < liveSlots(s)) { setupVoice(s, v, key, velocity); return true; }
  if (target < 0) return false;
  const bool jobOnTarget = s->job.kind == 2 && s->job.key == target && s->job.gen == s->gen;
  if (!jobOnTarget && !jobPinned(s)) {
    jobAbort(s);              // a key nobody waits for, or the level measurement (redone afterwards)
    s->staleWait = 0;
    jobStartKey(s, target);
  }
  if (s->job.kind == 2 && s->job.key == target) b = s->job.pool;
  else {
    // the render in progress belongs to another played key: this one is next
    s->priorityKey = target;
    int bestD = 1000;
    for (int k = 0; k < 128; k++)
      if (bufFresh(s, k) && std::abs(k - key) < bestD) { bestD = std::abs(k - key); b = s->keyBuf[k]; }
    if (b < 0 && s->job.kind == 2) b = s->job.pool;
  }
  if (b < 0) return false;
  setupSampler(s, v, key, velocity, b);
  const CppnEngine::Buf& e = s->pool[(size_t)b];
  v.waiting = e.filled < e.len;
  return true;
}

void noteOn(CppnEngine* s, int key, double velocity, int32_t noteId, int16_t channel) {
  if (!s->loaded) return;
  for (auto& v : s->voices) {
    if (v.active && v.key == key && v.noteId == noteId) { v.released = true; break; }  // retrigger same note
  }
  CppnEngine::Voice* pick = nullptr;
  for (auto& v : s->voices) if (!v.active) { pick = &v; break; }
  // steal the oldest released / longest-running voice
  if (!pick) for (auto& v : s->voices) if (v.released) { pick = &v; break; }
  if (!pick) pick = &s->voices[0];
  CppnEngine::Voice& v = *pick;
  v.active = false;
  if (!startVoice(s, v, key, velocity)) return;
  v.active = true;
  v.noteId = noteId;
  v.channel = channel;
}

void noteOff(CppnEngine* s, int key, int32_t noteId, int16_t, bool choke) {
  for (auto& v : s->voices) {
    if (v.active && v.key == key && (noteId < 0 || v.noteId == noteId)) {
      if (choke) {
        v.active = false;
      } else if (v.waiting) {
        v.offPending = true;   // released once it has started and its attack has run
      } else {
        v.released = true;
        double rel = std::max(0.001, s->releaseSec) * s->sampleRate;
        v.envLeft = (long)rel;
        v.envStep = -v.env / rel;
      }
    }
  }
}

// ── clap.params ────────────────────────────────────────────────────────────────────────────────────────

struct ParamDef {
  clap_id id;
  const char* name;
  double minV, maxV, defV;
  const char* unit;
};
constexpr ParamDef kParamDefs[kParamCount] = {
    {kParamVolume, "Volume", -60.0, 12.0, 0.0, "dB"},
    {kParamDuration, "Duration", 0.05, 30.0, 2.0, "s"},
    {kParamAttack, "Attack", 0.0, 2.0, 0.005, "s"},
    {kParamRelease, "Release", 0.0, 3.0, 0.1, "s"},
    {kParamX, "CPPN X", 0.0, 1.0, 0.5, ""},
    {kParamY, "CPPN Y", 0.0, 1.0, 0.5, ""},
    {kParamPitch, "Pitch", -36.0, 36.0, 0.0, "st"},
    {kParamDcFilter, "DC Filter", 0.0, 1.0, 1.0, ""},
    {kParamAutoLevel, "Auto Level", 0.0, 1.0, 1.0, ""},
    {kParamLimiter, "Limiter", 0.0, 1.0, 1.0, ""},
    {kParamSoftClip, "Soft Clip", 0.0, 1.0, 1.0, ""},
    {kParamWeightsX, "Weights X", 0.0, 1.0, 0.5, ""},
    {kParamWeightsY, "Weights Y", 0.0, 1.0, 0.5, ""},
    {kParamFreqsX, "Frequencies X", 0.0, 1.0, 0.5, ""},
    {kParamFreqsY, "Frequencies Y", 0.0, 1.0, 0.5, ""},
    {kParamPrerender, "Pre-render", 0.0, 2.0, 0.0, ""},
};

double* paramPtr(CppnEngine* s, clap_id id) {
  switch (id) {
    case kParamVolume: return &s->volumeDb;
    case kParamDuration: return &s->durationSec;
    case kParamAttack: return &s->attackSec;
    case kParamRelease: return &s->releaseSec;
    case kParamX: return &s->x;
    case kParamY: return &s->y;
    case kParamPitch: return &s->pitch;
    case kParamDcFilter: return &s->dcFilter;
    case kParamAutoLevel: return &s->autoLevel;
    case kParamLimiter: return &s->limiter;
    case kParamSoftClip: return &s->softClip;
    case kParamWeightsX: return &s->wx;
    case kParamWeightsY: return &s->wy;
    case kParamFreqsX: return &s->fx;
    case kParamFreqsY: return &s->fy;
    case kParamPrerender: return &s->prerenderParam;
    default: return nullptr;
  }
}

uint32_t paramsCount(const clap_plugin_t*) { return kParamCount; }

bool paramsInfo(const clap_plugin_t* p, uint32_t i, clap_param_info_t* info) {
  if (i >= kParamCount) return false;
  std::memset(info, 0, sizeof(*info));
  info->id = kParamDefs[i].id;
  info->flags = CLAP_PARAM_IS_AUTOMATABLE;
  if (isStepped(info->id)) info->flags |= CLAP_PARAM_IS_STEPPED;
  // X/Y are hybrid-phase navigation coordinates only — hide them for other schemes
  if ((info->id == kParamX || info->id == kParamY) && self(p)->net.scheme != cppn::SCHEME_HYBRID_PHASE)
    info->flags |= CLAP_PARAM_IS_HIDDEN;
  std::snprintf(info->name, sizeof(info->name), "%s", kParamDefs[i].name);
  if (info->id == kParamX || info->id == kParamY) std::snprintf(info->module, sizeof(info->module), "navigation");
  else info->module[0] = '\0';
  info->min_value = kParamDefs[i].minV;
  info->max_value = kParamDefs[i].maxV;
  info->default_value = kParamDefs[i].defV;
  return true;
}

bool paramsGetValue(const clap_plugin_t* p, clap_id id, double* out) {
  double* v = paramPtr(self(p), id);
  if (!v) return false;
  *out = *v;
  return true;
}

bool paramsValueToText(const clap_plugin_t* p, clap_id id, double value, char* out, uint32_t cap) {
  double* v = paramPtr(self(p), id);
  if (!v) return false;
  const ParamDef* def = nullptr;
  for (auto& d : kParamDefs) if (d.id == id) def = &d;
  double clamped = clampDouble(value, def->minV, def->maxV);
  if (id == kParamPrerender) { const int m = (int)std::lround(clamped); std::snprintf(out, cap, "%s", m == 1 ? "Live" : m == 2 ? "Pre-rendered" : "Auto"); return true; }
  if (def->unit[0]) std::snprintf(out, cap, "%.3g %s", clamped, def->unit);
  else std::snprintf(out, cap, "%.3g", clamped);
  return true;
}

bool paramsTextToValue(const clap_plugin_t* p, clap_id id, const char* text, double* out) {
  double* v = paramPtr(self(p), id);
  if (!v) return false;
  if (id == kParamPrerender) {
    if (!std::strcmp(text, "Auto")) { *out = 0; return true; }
    if (!std::strcmp(text, "Live")) { *out = 1; return true; }
    if (!std::strcmp(text, "Pre-rendered")) { *out = 2; return true; }
  }
  char* end = nullptr;
  double d = std::strtod(text, &end);
  if (end == text) return false;
  *out = d;
  return true;
}

void applyParam(CppnEngine* s, clap_id id, double value) {
  double* v = paramPtr(s, id);
  if (!v) return;
  const ParamDef* def = nullptr;
  for (auto& d : kParamDefs) if (d.id == id) def = &d;
  const double before = *v;
  *v = clampDouble(value, def->minV, def->maxV);
  if (id == kParamPrerender) { *v = std::round(*v); if (*v != before) s->modeDirty = true; return; }
  const bool changed = *v != before;
  if (id == kParamPitch) s->pitchRatio = std::pow(2.0, s->pitch / 12.0);
  if (id >= kParamWeightsX && id <= kParamFreqsY && s->loaded) {
    // while notes sound: rebuilt at a block start, at most 50 times a second. With nothing sounding
    // (a host setting up, or a pad moved between notes) it is done at once, so the next note has it.
    bool any = false;
    for (auto& v : s->voices) any = any || v.active;
    if (any) s->geoDirty = true;
    else if (changed || !s->pre) {
      buildPlan(s); s->convScaleCache.clear(); s->geoDirty = false;
      s->geoMeasurePending = !s->pre;
      renderSettingsChanged(s, true);
    }
  }
  // a host normally sets the note length right after activating; until the first note the
  // measurement follows it (afterwards it is left alone, so playing is never interrupted)
  if (id == kParamDuration && s->loaded && s->plan.numConvs > 0) {
    // a longer note needs longer convolver buffers (this allocates; it happens on a parameter change only)
    const int want = (int)(std::min(10.0, s->durationSec) * s->sampleRate) + 1;
    if (want > s->plan.convCap) { s->plan.convCap = want; for (auto& v : s->voices) { v.active = false; v.st.allocate(s->plan); } s->scratch.allocate(s->plan); jobAbort(s); }
  }
  if (!s->pre) {
    if ((id == kParamDuration || id == kParamLimiter) && s->loaded && s->noteCounter == 0) measureLevel(s);
  } else if (changed && s->loaded) {
    // pre-rendered mode: the measurement and the keys are redone in slices, after the debounce
    if (id == kParamDuration || id == kParamLimiter) { if (id == kParamDuration) makePlan(s); renderSettingsChanged(s, true); }
    else if (id == kParamPitch || id == kParamX || id == kParamY || id == kParamDcFilter) renderSettingsChanged(s, false);
    else if (id == kParamRelease && (long)(s->releaseSec * s->sampleRate) + 1 > s->tailLen) { makePlan(s); renderSettingsChanged(s, false); }
  }
  if (id == kParamRelease) {
    long tail = (long)(s->releaseSec * s->sampleRate) + 1;
    s->tailFrames = (uint32_t)std::max<uint32_t>(s->tailFrames, (uint32_t)tail);
  }
}

void paramsFlush(const clap_plugin_t* p, const clap_input_events_t* in, const clap_output_events_t*) {
  CppnEngine* s = self(p);
  const uint32_t total = in->size(in);
  for (uint32_t i = 0; i < total; ++i) {
    const clap_event_header_t* h = in->get(in, i);
    if (h->space_id != CLAP_CORE_EVENT_SPACE_ID || h->type != CLAP_EVENT_PARAM_VALUE) continue;
    auto* e = reinterpret_cast<const clap_event_param_value_t*>(h);
    applyParam(s, e->param_id, e->value);
  }
}

const clap_plugin_params_t kParams = {paramsCount, paramsInfo, paramsGetValue, paramsValueToText, paramsTextToValue,
                                      paramsFlush};

// ── clap.state ─────────────────────────────────────────────────────────────────────────────────────────

bool stateSave(const clap_plugin_t* p, const clap_ostream_t* out) {
  CppnEngine* s = self(p);
  std::vector<unsigned char> buf;
  buf.push_back(kStateVersion);
  for (uint32_t i = 0; i < kParamCount; i++) {
    const double d = *paramPtr(s, i);
    unsigned char v8[8];
    std::memcpy(v8, &d, 8);
    buf.insert(buf.end(), v8, v8 + 8);
  }
  for (uint32_t i = 0; i < kParamCount; i++) {  // MIDI controller assignments, -1 = none
    const int16_t cc = (int16_t)s->ccMap[i];
    buf.push_back((unsigned char)(cc & 0xff));
    buf.push_back((unsigned char)((cc >> 8) & 0xff));
  }
  uint16_t plen = (uint16_t)std::min<size_t>(s->genomePath.size(), 0xffff);
  buf.push_back((unsigned char)(plen & 0xff));
  buf.push_back((unsigned char)(plen >> 8));
  buf.insert(buf.end(), s->genomePath.begin(), s->genomePath.begin() + plen);
  int64_t done = 0;
  while (done < (int64_t)buf.size()) {
    int64_t n = out->write(out, buf.data() + done, buf.size() - done);
    if (n <= 0) return false;
    done += n;
  }
  return true;
}

bool stateLoad(const clap_plugin_t* p, const clap_istream_t* in) {
  std::vector<unsigned char> buf;
  unsigned char chunk[4096];
  for (;;) {
    int64_t n = in->read(in, chunk, sizeof(chunk));
    if (n < 0) return false;
    if (n == 0) break;
    buf.insert(buf.end(), chunk, chunk + n);
  }
  if (buf.empty() || buf[0] < 1 || buf[0] > kStateVersion) return false;
  // parameter values per version: 1: 7, 2: 9, 3: 10, 4: 11, 5: 15 plus the controller map of 15,
  // 6: 16 (Pre-render) plus the controller map of 16
  const size_t count = buf[0] == 1 ? 7 : buf[0] == 2 ? 9 : buf[0] == 3 ? 10 : buf[0] == 4 ? 11 : buf[0] == 5 ? 15 : kParamCount;
  const size_t ccCount = buf[0] >= 5 ? count : 0;
  const size_t ccBytes = 2 * ccCount;
  if (buf.size() < 1 + 8 * count + ccBytes + 2) return false;
  CppnEngine* s = self(p);
  for (uint32_t i = 0; i < kParamCount; i++) {
    double v = kParamDefs[i].defV;
    if (i < count) std::memcpy(&v, buf.data() + 1 + 8 * i, 8);
    v = clampDouble(v, kParamDefs[i].minV, kParamDefs[i].maxV);
    if (i >= kParamDcFilter && i <= kParamSoftClip) v = v >= 0.5 ? 1 : 0;
    if (i == kParamPrerender) v = std::round(v);
    *paramPtr(s, i) = v;
  }
  s->modeDirty = true;
  s->replan = true;
  s->pitchRatio = std::pow(2.0, s->pitch / 12.0);
  size_t at = 1 + 8 * count;
  for (uint32_t i = 0; i < kParamCount; i++) {
    s->ccMap[i] = -1;
    if (i < ccCount) { const int16_t cc = (int16_t)(buf[at + 2 * i] | (buf[at + 2 * i + 1] << 8)); s->ccMap[i] = (cc >= 0 && cc < 128) ? cc : -1; }
  }
  at += ccBytes;
  uint16_t plen = (uint16_t)(buf[at] | (buf[at + 1] << 8));
  if (buf.size() < at + 2 + plen) return false;
  s->genomePath.assign((const char*)buf.data() + at + 2, plen);
  if (s->loaded) s->geoDirty = true;
  return true;
}

const clap_plugin_state_t kState = {stateSave, stateLoad};

// ── ports / latency / tail ─────────────────────────────────────────────────────────────────────────────

uint32_t audioPortsCount(const clap_plugin_t*, bool isInput) { return isInput ? 0 : 1; }
bool audioPortsGet(const clap_plugin_t*, uint32_t i, bool isInput, clap_audio_port_info_t* info) {
  if (isInput || i != 0) return false;
  std::memset(info, 0, sizeof(*info));
  info->id = 0;
  std::snprintf(info->name, sizeof(info->name), "main");
  info->flags = CLAP_AUDIO_PORT_IS_MAIN;
  info->channel_count = 2;
  info->port_type = CLAP_PORT_STEREO;
  info->in_place_pair = CLAP_INVALID_ID;
  return true;
}
const clap_plugin_audio_ports_t kAudioPorts = {audioPortsCount, audioPortsGet};

uint32_t notePortsCount(const clap_plugin_t*, bool isInput) { return isInput ? 1 : 0; }
bool notePortsGet(const clap_plugin_t*, uint32_t i, bool isInput, clap_note_port_info_t* info) {
  if (!isInput || i != 0) return false;
  std::memset(info, 0, sizeof(*info));
  info->id = 0;
  info->supported_dialects = CLAP_NOTE_DIALECT_CLAP | CLAP_NOTE_DIALECT_MIDI;
  info->preferred_dialect = CLAP_NOTE_DIALECT_CLAP;
  std::snprintf(info->name, sizeof(info->name), "Note Input");
  return true;
}
const clap_plugin_note_ports_t kNotePorts = {notePortsCount, notePortsGet};

uint32_t latencyGet(const clap_plugin_t*) { return 0; }
const clap_plugin_latency_t kLatency = {latencyGet};
uint32_t tailGet(const clap_plugin_t* p) { return self(p)->tailFrames; }
const clap_plugin_tail_t kTail = {tailGet};

// ── clap.webview/3 — receive() carries wclap.files/1 frames (wclap-rack SPEC §8) ───────────────────────

// The editor page ships in the bundle next to module.wasm (ui/index.html); hosts open it relative to
// the bundle and relay binary frames both ways.
const char kUiUri[] = "ui/index.html";
int32_t webviewGetUri(const clap_plugin_t*, char* uri, uint32_t cap) {
  if (cap > 0 && uri != nullptr) std::snprintf(uri, cap, "%s", kUiUri);
  return (int32_t)sizeof(kUiUri);
}

// ── page protocol (frame: u8 type, u8 flags, u16 reserved, payload; little-endian) ─────────────────────
//   page -> plugin   64 hello            (reply: 80 description, 81 genome, 82 values)
//                    65 set parameter    u32 id, f64 value
//                    66 note             u8 on, u8 key, u16 0, f32 velocity
//                    67 MIDI learn       i32 parameter id (-1 cancels): the next controller received binds
//                    68 clear controller u32 parameter id
//                    69 poll             (reply: 82 values)
//   plugin -> page   80 description      JSON {params:[{id,name,min,max,def,stepped,hidden}], rootKey, scheme, unsupported}
//                    81 genome           the delivered genome file, as it is
//                    82 values           i32 learn, f32 peak, u32 voices, u32 count, then per parameter f64 value, i32 controller
//                    83 render status    u8 mode (0 live, 1 pre-rendered), u8 reason (0 auto from the estimated speed,
//                                        1 auto from the measured speed, 2 forced by parameter 15), u16 keys fresh,
//                                        u16 keys planned, u16 keys stale (rendered with older settings, still played),
//                                        f32 speed (times real time, one voice), u32 bytes of rendered notes,
//                                        u16 key being rendered (0xffff none), u16 grid step, u16 voices playing rendered
//                                        keys, u16 voices computed live. Sent just before every 82.
void pageSend(CppnEngine* s, uint8_t type, const void* payload, size_t n) {
  if (!s->hostWebview || !s->hostWebview->send) return;
  std::vector<uint8_t> f(4 + n);
  f[0] = type; f[1] = 0; f[2] = 0; f[3] = 0;
  if (n) std::memcpy(f.data() + 4, payload, n);
  s->hostWebview->send(s->host, f.data(), (uint32_t)f.size());
}
void pageSendValues(CppnEngine* s);
void pageSendDescription(CppnEngine* s);
void applyParam(CppnEngine* s, clap_id id, double value);
bool pageReceive(CppnEngine* s, uint8_t type, const uint8_t* pl, uint32_t plen) {
  auto rdU32 = [&](uint32_t o) { uint32_t v; std::memcpy(&v, pl + o, 4); return v; };
  switch (type) {
    case 64: pageSendDescription(s); pageSendValues(s); return true;
    case 65: {
      if (plen < 12) return false;
      const uint32_t id = rdU32(0);
      double v; std::memcpy(&v, pl + 4, 8);
      if (id < kParamCount) { applyParam(s, id, v); s->paramOut[id] = true; }
      return true;
    }
    case 66: {
      if (plen < 8) return false;
      float vel; std::memcpy(&vel, pl + 4, 4);
      if (s->pageNotes.size() < 64) s->pageNotes.push_back({pl[0] != 0, (int)pl[1], (double)vel});
      return true;
    }
    case 67: { if (plen < 4) return false; const int32_t id = (int32_t)rdU32(0); s->learnParam = (id >= 0 && id < (int32_t)kParamCount) ? id : -1; pageSendValues(s); return true; }
    case 68: { if (plen < 4) return false; const uint32_t id = rdU32(0); if (id < kParamCount) s->ccMap[id] = -1; pageSendValues(s); return true; }
    case 69: pageSendValues(s); return true;
    default: return false;
  }
}
bool webviewGetResource(const clap_plugin_t*, const char*, char*, uint32_t, const clap_ostream_t*) { return false; }

bool webviewReceive(const clap_plugin_t* p, const void* buffer, uint32_t size) {
  CppnEngine* s = self(p);
  if (size < 4) return false;
  const uint8_t* b = static_cast<const uint8_t*>(buffer);
  const uint8_t* pl = b + 4;
  const uint32_t plen = size - 4;
  if (b[0] >= 64 && b[0] < 80) return pageReceive(s, b[0], pl, plen);
  auto rd16 = [&](uint32_t o) { return (uint16_t)(pl[o] | (pl[o + 1] << 8)); };
  auto rd32 = [&](uint32_t o) {
    return (uint32_t)pl[o] | ((uint32_t)pl[o + 1] << 8) | ((uint32_t)pl[o + 2] << 16) | ((uint32_t)pl[o + 3] << 24);
  };
  if (b[0] == 241) {  // clear
    s->files.clear();
    s->filesDigest = 0x811c9dc5u;
    std::printf("wclap.files/1 clear files=0 digest=%08x\n", s->filesDigest);
    return true;
  }
  if (b[0] != 240 || plen < 6) return false;  // write
  const uint16_t pathLen = rd16(0);
  if (plen < (uint32_t)(2 + pathLen + 4)) return false;
  std::string path(reinterpret_cast<const char*>(pl) + 2, pathLen);
  const uint32_t dataLen = rd32(2 + pathLen);
  if (plen < (uint32_t)(6 + pathLen + dataLen)) return false;
  const uint8_t* data = pl + 6 + pathLen;
  s->files[path] = std::vector<uint8_t>(data, data + dataLen);
  uint32_t h = s->filesDigest;
  for (char c : path) { h ^= (uint8_t)c; h *= 0x01000193u; }
  h ^= 0; h *= 0x01000193u;
  for (uint32_t k = 0; k < dataLen; ++k) { h ^= data[k]; h *= 0x01000193u; }
  s->filesDigest = h;
  std::printf("wclap.files/1 write path=%s bytes=%u files=%u digest=%08x\n", path.c_str(), dataLen,
              (unsigned)s->files.size(), s->filesDigest);
  return true;
}

const clap_plugin_webview_t kWebview = {webviewGetUri, webviewGetResource, webviewReceive};

// clap.render: offline (a bounce) plays everything live, with no waiting notes
bool renderHasHardRealtime(const clap_plugin_t*) { return false; }
bool renderSet(const clap_plugin_t* p, clap_plugin_render_mode mode) {
  CppnEngine* s = self(p);
  const bool off = mode == CLAP_RENDER_OFFLINE;
  if (off != s->offline) { s->offline = off; s->modeDirty = true; }
  return true;
}
const clap_plugin_render_t kRender = {renderHasHardRealtime, renderSet};

void pageSendValues(CppnEngine* s) {
  // 83 render status
  uint8_t st[32] = {0};
  uint16_t fresh = 0, stale = 0;
  for (int k : s->planKeys) { if (bufFresh(s, k)) fresh++; else if (s->keyBuf[k] >= 0) stale++; }
  const uint16_t planned = (uint16_t)s->planKeys.size();
  const float sp = (float)std::min(s->speed, 1e6);
  const uint32_t bytes = (uint32_t)std::min<size_t>(poolBytes(s), 0xffffffffu);
  const uint16_t jk = s->job.kind == 2 ? (uint16_t)s->job.key : s->job.kind == 1 ? (uint16_t)kRootKey : (uint16_t)0xffff;
  const uint16_t step = (uint16_t)s->gridStep;
  st[0] = s->pre ? 1 : 0;
  st[1] = (uint8_t)s->preReason;
  std::memcpy(st + 2, &fresh, 2); std::memcpy(st + 4, &planned, 2); std::memcpy(st + 6, &stale, 2);
  std::memcpy(st + 8, &sp, 4); std::memcpy(st + 12, &bytes, 4); std::memcpy(st + 16, &jk, 2); std::memcpy(st + 18, &step, 2);
  uint16_t vs = 0, vl = 0;
  for (const auto& v : s->voices) if (v.active && !v.waiting) { if (v.buf >= 0) vs++; else vl++; }
  std::memcpy(st + 20, &vs, 2); std::memcpy(st + 22, &vl, 2);
  // notes waiting to start (for their key's render, or for a convolver pre-pass) and the seconds left
  uint16_t vw = 0;
  for (const auto& v : s->voices) if (v.active && v.waiting) vw++;
  const float ws = (float)waitSeconds(s);
  std::memcpy(st + 24, &vw, 2); std::memcpy(st + 28, &ws, 4);
  pageSend(s, 83, st, sizeof(st));
  std::vector<uint8_t> b(16 + kParamCount * 12);
  int32_t learn = s->learnParam;
  float peak = s->meterPeak;
  s->meterPeak = 0;
  uint32_t voices = 0, count = kParamCount;
  for (auto& v : s->voices) if (v.active) voices++;
  std::memcpy(b.data(), &learn, 4); std::memcpy(b.data() + 4, &peak, 4); std::memcpy(b.data() + 8, &voices, 4); std::memcpy(b.data() + 12, &count, 4);
  for (uint32_t i = 0; i < kParamCount; i++) {
    const double v = *paramPtr(s, i);
    const int32_t cc = s->ccMap[i];
    std::memcpy(b.data() + 16 + i * 12, &v, 8);
    std::memcpy(b.data() + 16 + i * 12 + 8, &cc, 4);
  }
  pageSend(s, 82, b.data(), b.size());
}
void pageSendDescription(CppnEngine* s) {
  std::string j = "{\"params\":[";
  for (uint32_t i = 0; i < kParamCount; i++) {
    const ParamDef& d = kParamDefs[i];
    const bool stepped = isStepped(i);
    const bool hidden = (i == kParamX || i == kParamY) && s->net.scheme != cppn::SCHEME_HYBRID_PHASE;
    char buf[256];
    std::snprintf(buf, sizeof(buf), "%s{\"id\":%u,\"name\":\"%s\",\"min\":%g,\"max\":%g,\"def\":%g,\"unit\":\"%s\",\"stepped\":%s,\"hidden\":%s}",
                  i ? "," : "", d.id, d.name, d.minV, d.maxV, d.defV, d.unit, stepped ? "true" : "false", hidden ? "true" : "false");
    j += buf;
  }
  std::string unsupported;
  for (auto& u : s->plan.unsupported) unsupported += (unsupported.empty() ? "\"" : ",\"") + u + "\"";
  char tail[256];
  std::snprintf(tail, sizeof(tail), "],\"rootKey\":%d,\"scheme\":\"%s\",\"loaded\":%s,\"unsupported\":[", kRootKey, s->schemeName.c_str(), s->loaded ? "true" : "false");
  j += tail + unsupported + "]}";
  pageSend(s, 80, j.data(), j.size());
  auto it = s->files.find(s->genomePath);
  if (it != s->files.end()) pageSend(s, 81, it->second.data(), it->second.size());
}

// ── clap.gui: the "webview" window api only (the host shows ui/index.html) ─────────────────────────────
bool guiIsApiSupported(const clap_plugin_t*, const char* api, bool floating) { return api && !std::strcmp(api, CLAP_WINDOW_API_WEBVIEW) && !floating; }
bool guiGetPreferredApi(const clap_plugin_t*, const char** api, bool* floating) { *api = CLAP_WINDOW_API_WEBVIEW; *floating = false; return true; }
bool guiCreate(const clap_plugin_t* p, const char* api, bool floating) {
  if (!guiIsApiSupported(p, api, floating)) return false;
  self(p)->guiOpen = true;
  return true;
}
void guiDestroy(const clap_plugin_t* p) { self(p)->guiOpen = false; self(p)->learnParam = -1; }
bool guiSetScale(const clap_plugin_t*, double) { return false; }
bool guiGetSize(const clap_plugin_t*, uint32_t* w, uint32_t* h) { *w = 860; *h = 640; return true; }
bool guiCanResize(const clap_plugin_t*) { return true; }
bool guiGetResizeHints(const clap_plugin_t*, clap_gui_resize_hints_t* hints) {
  hints->can_resize_horizontally = true; hints->can_resize_vertically = true; hints->preserve_aspect_ratio = false;
  hints->aspect_ratio_width = 0; hints->aspect_ratio_height = 0;
  return true;
}
bool guiAdjustSize(const clap_plugin_t*, uint32_t*, uint32_t*) { return true; }
bool guiSetSize(const clap_plugin_t*, uint32_t, uint32_t) { return true; }
bool guiSetParent(const clap_plugin_t*, const clap_window_t*) { return true; }
bool guiSetTransient(const clap_plugin_t*, const clap_window_t*) { return false; }
void guiSuggestTitle(const clap_plugin_t*, const char*) {}
bool guiShow(const clap_plugin_t*) { return true; }
bool guiHide(const clap_plugin_t*) { return true; }
const clap_plugin_gui_t kGui = {guiIsApiSupported, guiGetPreferredApi, guiCreate, guiDestroy, guiSetScale, guiGetSize, guiCanResize,
                                guiGetResizeHints, guiAdjustSize, guiSetSize, guiSetParent, guiSetTransient, guiSuggestTitle,
                                guiShow, guiHide};

// ── plugin ─────────────────────────────────────────────────────────────────────────────────────────────

bool pluginInit(const clap_plugin_t* p) {
  CppnEngine* s = self(p);
  if (s->host && s->host->get_extension)
    s->hostWebview = static_cast<const clap_host_webview_t*>(s->host->get_extension(s->host, CLAP_EXT_WEBVIEW));
  return true;
}
void pluginDestroy(const clap_plugin_t* p) {
  CppnEngine* s = self(p);
  s->~CppnEngine();
  std::free(s);
}

bool pluginActivate(const clap_plugin_t* p, double sampleRate, uint32_t, uint32_t) {
  CppnEngine* s = self(p);
  s->sampleRate = sampleRate > 0 ? sampleRate : 48000;
  for (auto& v : s->voices) v.active = false;
  s->tailFrames = (uint32_t)(s->releaseSec * s->sampleRate) + 1;
  s->dcCoef = 1.0 - std::exp(-2.0 * M_PI * 5.0 / s->sampleRate);
  s->pitchRatio = std::pow(2.0, s->pitch / 12.0);
  loadGenome(s);
  s->active = true;
  return true;
}
void pluginDeactivate(const clap_plugin_t* p) {
  CppnEngine* s = self(p);
  for (auto& v : s->voices) v.active = false;
  s->active = false;
}
bool pluginStartProcessing(const clap_plugin_t*) { return true; }
void pluginStopProcessing(const clap_plugin_t*) {}
void pluginReset(const clap_plugin_t* p) {
  CppnEngine* s = self(p);
  for (auto& v : s->voices) v.active = false;
}

void handleEvent(CppnEngine* s, const clap_event_header_t* h) {
  if (h->space_id != CLAP_CORE_EVENT_SPACE_ID) return;
  if (h->type == CLAP_EVENT_PARAM_VALUE) {
    auto* e = reinterpret_cast<const clap_event_param_value_t*>(h);
    applyParam(s, e->param_id, e->value);
  } else if (h->type == CLAP_EVENT_NOTE_ON) {
    auto* e = reinterpret_cast<const clap_event_note_t*>(h);
    noteOn(s, e->key, e->velocity, e->note_id, e->channel);
  } else if (h->type == CLAP_EVENT_NOTE_OFF) {
    auto* e = reinterpret_cast<const clap_event_note_t*>(h);
    noteOff(s, e->key, e->note_id, e->channel, false);
  } else if (h->type == CLAP_EVENT_NOTE_CHOKE) {
    auto* e = reinterpret_cast<const clap_event_note_t*>(h);
    noteOff(s, e->key, e->note_id, e->channel, true);
  } else if (h->type == CLAP_EVENT_MIDI) {
    auto* e = reinterpret_cast<const clap_event_midi_t*>(h);
    const uint8_t status = e->data[0] & 0xf0;
    if (status == 0xb0) {
      // a controller: bind it if the page armed MIDI learn, then move every parameter bound to it
      const int cc = e->data[1] & 0x7f;
      if (s->learnParam >= 0) { s->ccMap[s->learnParam] = cc; s->learnParam = -1; }
      for (uint32_t i = 0; i < kParamCount; i++)
        if (s->ccMap[i] == cc) {
          const ParamDef& d = kParamDefs[i];
          const double t = e->data[2] / 127.0;
          applyParam(s, i, isStepped(i) ? std::round(d.minV + (d.maxV - d.minV) * t) : d.minV + (d.maxV - d.minV) * t);
          s->paramOut[i] = true;
        }
      return;
    }
    if (status == 0x90 && e->data[2] > 0)
      noteOn(s, e->data[1], e->data[2] / 127.0, -1, (int16_t)(e->data[0] & 0x0f));
    else if (status == 0x80 || (status == 0x90 && e->data[2] == 0))
      noteOff(s, e->data[1], -1, (int16_t)(e->data[0] & 0x0f), false);
  }
}

clap_process_status pluginProcess(const clap_plugin_t* p, const clap_process_t* proc) {
  CppnEngine* s = self(p);
  if (!s->active || proc->audio_outputs_count < 1) return CLAP_PROCESS_ERROR;
  const clap_audio_buffer_t* out = &proc->audio_outputs[0];
  if (!out->data32 || out->channel_count < 1) return CLAP_PROCESS_ERROR;
  const uint32_t frames = proc->frames_count;
  // a Weights / Frequencies pad has moved: rebuild the plan, at most 50 times a second. The level
  // measurement is redone once no voice is sounding.
  s->geoWait += frames;
  if (s->geoDirty && s->loaded && s->geoWait >= (long)(s->sampleRate / 50)) {
    const size_t nodes = s->plan.nodes.size(), slots = s->plan.slots.size();
    const int nb = s->plan.numBiquads, no = s->plan.numOscs, nd = s->plan.numDelays, nc = s->plan.numComps, nv = s->plan.numConvs;
    buildPlan(s);
    if (nodes != s->plan.nodes.size() || slots != s->plan.slots.size() || nb != s->plan.numBiquads || no != s->plan.numOscs ||
        nd != s->plan.numDelays || nc != s->plan.numComps || nv != s->plan.numConvs)
      { for (auto& v : s->voices) { v.active = false; v.st.allocate(s->plan); } s->scratch.allocate(s->plan); jobAbort(s); }   // should not happen: the pads keep the structure
    s->convScaleCache.clear();
    s->geoDirty = false;
    s->geoMeasurePending = !s->pre;   // pre-rendered mode: measured in slices instead
    renderSettingsChanged(s, true);
    s->geoWait = 0;
  }
  if (s->geoMeasurePending && !s->geoDirty) {
    bool any = false;
    for (auto& v : s->voices) any = any || v.active;
    if (!any) { const uint32_t keep = s->noteCounter; measureLevel(s); s->noteCounter = keep; s->geoMeasurePending = false; }
  }
  for (auto& n : s->pageNotes) { if (n.on) noteOn(s, n.key, n.velocity, -1, 0); else noteOff(s, n.key, -1, 0, false); }
  s->pageNotes.clear();
  startWaiting(s, frames);
  const uint32_t outCh = std::min(out->channel_count, 2u);
  const uint32_t total = proc->in_events ? proc->in_events->size(proc->in_events) : 0;
  uint32_t cursor = 0;
  for (uint32_t i = 0; i < frames; ++i) {
    if (proc->in_events) {
      while (cursor < total) {
        const clap_event_header_t* h = proc->in_events->get(proc->in_events, cursor);
        if (h->time > i) break;
        ++cursor;
        handleEvent(s, h);
      }
    }
    float accL = 0, accR = 0;
    for (auto& v : s->voices) {
      if (!v.active || v.waiting) continue;
      // envelope
      if (v.envLeft > 0) {
        v.env += v.envStep;
        if (--v.envLeft == 0) {
          if (v.released) v.env = 0;
          else v.env = 1.0;
        }
        if (v.env <= 0 && v.released) { v.active = false; continue; }
      }
      float sample = v.buf >= 0 ? samplerSample(s, v) : renderVoiceSample(s, v);
      // the renderer's delivery fades: raised cosine, 128 samples in / 512 out
      {
        const long fi = 128, fo = 512;
        if (v.n < fi) sample *= (float)(0.5 * (1.0 - std::cos(M_PI * v.n / fi)));
        if (v.n >= v.total - fo) {
          long k = v.total - 1 - v.n;
          if (k >= 0) sample *= (float)(0.5 * (1.0 - std::cos(M_PI * k / fo)));
        }
      }
      const double velGain = v.velocity * v.velocity;
      const double master = std::pow(10.0, s->volumeDb / 20.0) * (s->autoLevel >= 0.5 ? v.trim : 1.0);
      const double g = v.env * velGain * master;
      accL += sample * (float)g;
      accR += sample * (float)g;
      v.n++;
      if (v.n >= v.total && !v.released) {
        v.released = true;
        double rel = std::max(0.001, s->releaseSec) * s->sampleRate;
        v.envLeft = (long)rel;
        v.envStep = -v.env / rel;
      }
      if (v.released && v.envLeft == 0) { v.active = false; continue; }
      if (v.offPending && v.envLeft == 0 && !v.released) {
        v.offPending = false;
        v.released = true;
        double rel = std::max(0.001, s->releaseSec) * s->sampleRate;
        v.envLeft = (long)rel;
        v.envStep = -v.env / rel;
      }
    }
    // The level trim is that of ONE root note; chords, other keys and pad-adjusted genomes can exceed
    // it. Soft Clip leaves everything below 0.9 untouched and bends the rest towards full scale.
    if (s->softClip >= 0.5) {
      auto bend = [](float x) { const float a = std::fabs(x); if (a <= 0.9f) return x; const float y = 0.9f + 0.1f * std::tanh((a - 0.9f) / 0.1f); return x < 0 ? -y : y; };
      accL = bend(accL); accR = bend(accR);
    }
    if (std::fabs(accL) > s->meterPeak) s->meterPeak = std::fabs(accL);
    if (outCh > 0) out->data32[0][i] = accL;
    if (outCh > 1) out->data32[1][i] = accR;
  }
  if (proc->in_events) {
    while (cursor < total) handleEvent(s, proc->in_events->get(proc->in_events, cursor++));
  }
  if (s->modeDirty && s->loaded) decideMode(s);
  if (s->replan && s->loaded) {
    s->replan = false;
    if (s->pre) { makePlan(s); renderSettingsChanged(s, true); }
  }
  renderSlice(s, frames);
  prepSlice(s, frames);
  // parameters moved by the page or by a MIDI controller are reported to the host
  if (proc->out_events && proc->out_events->try_push)
    for (uint32_t i = 0; i < kParamCount; i++)
      if (s->paramOut[i]) {
        clap_event_param_value_t ev;
        std::memset(&ev, 0, sizeof(ev));
        ev.header.size = sizeof(ev);
        ev.header.time = frames ? frames - 1 : 0;
        ev.header.space_id = CLAP_CORE_EVENT_SPACE_ID;
        ev.header.type = CLAP_EVENT_PARAM_VALUE;
        ev.param_id = i;
        ev.note_id = -1; ev.port_index = -1; ev.channel = -1; ev.key = -1;
        ev.value = *paramPtr(s, i);
        proc->out_events->try_push(proc->out_events, &ev.header);
        s->paramOut[i] = false;
      }
  return CLAP_PROCESS_CONTINUE;
}

const void* pluginGetExtension(const clap_plugin_t*, const char* id) {
  if (!std::strcmp(id, CLAP_EXT_AUDIO_PORTS)) return &kAudioPorts;
  if (!std::strcmp(id, CLAP_EXT_NOTE_PORTS)) return &kNotePorts;
  if (!std::strcmp(id, CLAP_EXT_PARAMS)) return &kParams;
  if (!std::strcmp(id, CLAP_EXT_STATE)) return &kState;
  if (!std::strcmp(id, CLAP_EXT_LATENCY)) return &kLatency;
  if (!std::strcmp(id, CLAP_EXT_TAIL)) return &kTail;
  if (!std::strcmp(id, CLAP_EXT_WEBVIEW)) return &kWebview;
  if (!std::strcmp(id, CLAP_EXT_GUI)) return &kGui;
  if (!std::strcmp(id, CLAP_EXT_RENDER)) return &kRender;
  return nullptr;
}
void pluginOnMainThread(const clap_plugin_t*) {}

const clap_plugin_t* createPlugin(const clap_host_t* host) {
  void* mem = std::calloc(1, sizeof(CppnEngine));
  CppnEngine* s = new (mem) CppnEngine();
  s->host = host;
  for (auto& c : s->ccMap) c = -1;
  for (auto& o : s->paramOut) o = false;
  for (auto& k : s->keyBuf) k = -1;
  s->plugin = {&kDescriptor, s, pluginInit, pluginDestroy, pluginActivate, pluginDeactivate, pluginStartProcessing,
               pluginStopProcessing, pluginReset, pluginProcess, pluginGetExtension, pluginOnMainThread};
  return &s->plugin;
}

uint32_t factoryCount(const clap_plugin_factory_t*) { return 1; }
const clap_plugin_descriptor_t* factoryDescriptor(const clap_plugin_factory_t*, uint32_t i) {
  return i == 0 ? &kDescriptor : nullptr;
}
const clap_plugin_t* factoryCreate(const clap_plugin_factory_t*, const clap_host_t* host, const char* id) {
  return std::strcmp(id, kDescriptor.id) ? nullptr : createPlugin(host);
}
const clap_plugin_factory_t kFactory = {factoryCount, factoryDescriptor, factoryCreate};

bool entryInit(const char*) { return true; }
void entryDeinit() {}
const void* entryGetFactory(const char* id) {
  return !std::strcmp(id, CLAP_PLUGIN_FACTORY_ID) ? &kFactory : nullptr;
}

}  // namespace

extern "C" {
CLAP_EXPORT const clap_plugin_entry_t clap_entry = {CLAP_VERSION_INIT, entryInit, entryDeinit, entryGetFactory};
}
