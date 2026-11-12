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
constexpr uint32_t kParamCount = 15;
// The key at which a genome plays at its own pitch (noteDelta 0). kromosynth-vi's sample
// instruments use the same key, so a sound auditioned as samples is in tune with the live engine.
constexpr int kRootKey = 60;
constexpr uint32_t kVoiceCount = 8;
constexpr int kStateVersion = 5;

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
  double offset = 0;          // measured at load: mean of the root note
  double trim = 1;            // measured at load: 1/peak of the root note when it would exceed full scale
  cgraph::VoiceState scratch; // used only for that measurement
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

// The root note is rendered once (at most 4 s of it) to get the fixed offset and level trim.
// Called when a genome is loaded, and again if the note length is changed before the first note.
void measureLevel(CppnEngine* s) {
  // Level: the renderer removes the mean and divides by the peak when it exceeds 1. A live voice
  // cannot know its mean or peak, so the root note is rendered once here (at most 4 s of it) and the same
  // rule gives a fixed trim for the instrument. Other keys and durations can differ from it.
  {
    s->scratch.allocate(s->plan);
    s->scratch.reset(1);
    s->convScaleCache.clear();
    cgraph::RenderCtx rc;
    rc.total = (long)std::max(1.0, (double)std::lround(s->durationSec * s->sampleRate));
    rc.keyRatio = 1.0; rc.velocity = 1.0; rc.x = s->x; rc.y = s->y;
    rc.limiter = s->limiter >= 0.5;
    cgraph::prepareConvolvers(s->plan, s->net, s->scratch, rc);
    const long len = std::min(rc.total, (long)(4.0 * s->sampleRate));
    double sum = 0, lo = 0, hi = 0;
    for (long i = 0; i < len; i++) {
      rc.n = i;
      double v = cgraph::renderSample(s->plan, s->net, s->scratch, rc);
      if (rc.limiter) {
        const double* o = &s->scratch.out[(size_t)s->plan.output * cgraph::kMaxCh];
        v = cgraph::outputLimiter(s->scratch.limiter, o[1], o[2], s->plan.limiterChans, i, s->sampleRate);
      }
      sum += v;
      if (i == 0 || v < lo) lo = v;
      if (i == 0 || v > hi) hi = v;
    }
    const double mean = len > 0 ? sum / (double)len : 0.0;
    const double peak = std::max(hi - mean, mean - lo);
    s->offset = mean;
    s->trim = peak > 1.0 ? 1.0 / peak : 1.0;
  }
}

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
  measureLevel(s);
  int patchEdges = 0;
  for (auto& n : s->plan.nodes) {
    patchEdges += (int)n.ins.size();
    for (auto& pr : n.p) patchEdges += (int)pr.mods.size();
  }
  int cppnConns = 0;
  for (auto& in : s->net.incoming) cppnConns += (int)in.size();
  std::string unsupported;
  for (auto& u : s->plan.unsupported) unsupported += (unsupported.empty() ? "" : ",") + u;
  std::printf("cppn-engine: loaded genome=%s cppnNodes=%d cppnConns=%d outputs=%d scheme=%s patchNodes=%zu patchEdges=%d netOuts=%zu signals=%zu frequencies=%zu unsupported=%s trim=%.4f\n",
              genomeFile.c_str(), s->net.total, cppnConns, s->net.outputCount, s->schemeName.c_str(),
              s->src.nodes.size(), patchEdges, s->src.netOuts.size(), s->plan.slots.size(), s->plan.groups.size(),
              unsupported.empty() ? "none" : unsupported.c_str(), s->trim);
  s->loaded = true;
  return true;
}

// ── per-note set-up (no allocation: voice state was sized when the genome was loaded) ───────────────────

void setupVoice(CppnEngine* s, CppnEngine::Voice& v, int key, double velocity) {
  v.key = key;
  v.velocity = velocity;
  v.n = 0;
  v.released = false;
  v.env = 0;
  v.total = std::max(1.0, (double)std::lround(s->durationSec * s->sampleRate));
  v.noteRatio = std::pow(2.0, (key - kRootKey) / 12.0);
  v.dc = 0;
  v.dcPrimed = false;
  v.st.reset(++s->noteCounter);
  if (s->plan.numConvs > 0) {
    // the convolver's scale needs the whole response: one CPPN pass over the note, remembered per key
    CppnEngine::ConvScale* hit = nullptr;
    for (auto& c : s->convScaleCache)
      if (c.key == key && c.total == v.total && c.velocity == velocity && c.pitch == s->pitch && c.x == s->x && c.y == s->y) hit = &c;
    if (hit) { for (int i = 0; i < s->plan.numConvs; i++) v.st.conv[i].scale = hit->scales[i]; }
    else {
      cgraph::RenderCtx rc;
      rc.n = 0; rc.total = v.total; rc.keyRatio = v.noteRatio * s->pitchRatio; rc.velocity = velocity; rc.x = s->x; rc.y = s->y;
      cgraph::prepareConvolvers(s->plan, s->net, v.st, rc);
      if (s->convScaleCache.size() >= 64) s->convScaleCache.erase(s->convScaleCache.begin());
      CppnEngine::ConvScale c{key, v.total, velocity, s->pitch, s->x, s->y, {}};
      for (int i = 0; i < s->plan.numConvs; i++) c.scales.push_back(v.st.conv[i].scale);
      s->convScaleCache.push_back(c);
    }
  }
  // envelope: attack then sustain; release handled on note off / end
  double atk = std::max(0.0, s->attackSec) * s->sampleRate;
  v.envLeft = (long)atk;
  v.envStep = atk > 0 ? 1.0 / atk : 1.0;
  if (v.envLeft == 0) v.env = 1.0;
}

// render one sample of the voice's graph; returns mono output
float renderVoiceSample(CppnEngine* s, CppnEngine::Voice& v) {
  cgraph::RenderCtx rc;
  rc.n = v.n;
  rc.total = v.total;
  rc.keyRatio = v.noteRatio * s->pitchRatio;  // the Pitch parameter bends sounding notes
  rc.velocity = v.velocity;
  rc.x = s->x;
  rc.y = s->y;
  rc.limiter = s->limiter >= 0.5;
  double out = cgraph::renderSample(s->plan, s->net, v.st, rc);
  // the render server's output limiter (it runs with it in production); it sees the raw sum, offset and all
  if (rc.limiter) {
    const double* o = &v.st.out[(size_t)s->plan.output * cgraph::kMaxCh];
    out = cgraph::outputLimiter(v.st.limiter, o[1], o[2], s->plan.limiterChans, v.n, s->sampleRate);
  }
  // The renderer removes the whole render's mean. A live voice cannot know its own, so the mean of
  // the root note, measured once when the genome was loaded, is subtracted (a fixed offset: slow
  // sub-audio movement in the sound is kept, as the renderer keeps it).
  if (s->dcFilter >= 0.5) out -= s->offset;
  return (float)out;
}

// ── notes ──────────────────────────────────────────────────────────────────────────────────────────────

void noteOn(CppnEngine* s, int key, double velocity, int32_t noteId, int16_t channel) {
  if (!s->loaded) return;
  for (auto& v : s->voices) {
    if (v.active && v.key == key && v.noteId == noteId) { v.released = true; break; }  // retrigger same note
  }
  for (auto& v : s->voices) {
    if (!v.active) {
      v.active = true;
      v.noteId = noteId;
      v.channel = channel;
      setupVoice(s, v, key, velocity);
      return;
    }
  }
  // steal the oldest released / longest-running voice
  for (auto& v : s->voices) {
    if (v.released) {
      v.active = true;
      v.noteId = noteId;
      v.channel = channel;
      setupVoice(s, v, key, velocity);
      return;
    }
  }
  CppnEngine::Voice& v = s->voices[0];
  v.active = true;
  v.noteId = noteId;
  v.channel = channel;
  setupVoice(s, v, key, velocity);
}

void noteOff(CppnEngine* s, int key, int32_t noteId, int16_t, bool choke) {
  for (auto& v : s->voices) {
    if (v.active && v.key == key && (noteId < 0 || v.noteId == noteId)) {
      if (choke) {
        v.active = false;
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
    default: return nullptr;
  }
}

uint32_t paramsCount(const clap_plugin_t*) { return kParamCount; }

bool paramsInfo(const clap_plugin_t* p, uint32_t i, clap_param_info_t* info) {
  if (i >= kParamCount) return false;
  std::memset(info, 0, sizeof(*info));
  info->id = kParamDefs[i].id;
  info->flags = CLAP_PARAM_IS_AUTOMATABLE;
  if (info->id == kParamDcFilter || info->id == kParamAutoLevel || info->id == kParamLimiter || info->id == kParamSoftClip) info->flags |= CLAP_PARAM_IS_STEPPED;
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
  if (def->unit[0]) std::snprintf(out, cap, "%.3g %s", clamped, def->unit);
  else std::snprintf(out, cap, "%.3g", clamped);
  return true;
}

bool paramsTextToValue(const clap_plugin_t* p, clap_id id, const char* text, double* out) {
  double* v = paramPtr(self(p), id);
  if (!v) return false;
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
  *v = clampDouble(value, def->minV, def->maxV);
  if (id == kParamPitch) s->pitchRatio = std::pow(2.0, s->pitch / 12.0);
  if (id >= kParamWeightsX && id <= kParamFreqsY && s->loaded) {
    // while notes sound: rebuilt at a block start, at most 50 times a second. With nothing sounding
    // (a host setting up, or a pad moved between notes) it is done at once, so the next note has it.
    bool any = false;
    for (auto& v : s->voices) any = any || v.active;
    if (any) s->geoDirty = true;
    else { buildPlan(s); s->convScaleCache.clear(); s->geoDirty = false; s->geoMeasurePending = true; }
  }
  // a host normally sets the note length right after activating; until the first note the
  // measurement follows it (afterwards it is left alone, so playing is never interrupted)
  if (id == kParamDuration && s->loaded && s->plan.numConvs > 0) {
    // a longer note needs longer convolver buffers (this allocates; it happens on a parameter change only)
    const int want = (int)(std::min(10.0, s->durationSec) * s->sampleRate) + 1;
    if (want > s->plan.convCap) { s->plan.convCap = want; for (auto& v : s->voices) { v.active = false; v.st.allocate(s->plan); } s->scratch.allocate(s->plan); }
  }
  if ((id == kParamDuration || id == kParamLimiter) && s->loaded && s->noteCounter == 0) measureLevel(s);
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
  // parameter values per version: 1: 7, 2: 9, 3: 10, 4: 11, 5: all 15 plus the controller map
  const size_t count = buf[0] == 1 ? 7 : buf[0] == 2 ? 9 : buf[0] == 3 ? 10 : buf[0] == 4 ? 11 : kParamCount;
  const size_t ccBytes = buf[0] >= 5 ? 2 * kParamCount : 0;
  if (buf.size() < 1 + 8 * count + ccBytes + 2) return false;
  CppnEngine* s = self(p);
  for (uint32_t i = 0; i < kParamCount; i++) {
    double v = kParamDefs[i].defV;
    if (i < count) std::memcpy(&v, buf.data() + 1 + 8 * i, 8);
    v = clampDouble(v, kParamDefs[i].minV, kParamDefs[i].maxV);
    if (i >= kParamDcFilter && i <= kParamSoftClip) v = v >= 0.5 ? 1 : 0;
    *paramPtr(s, i) = v;
  }
  s->pitchRatio = std::pow(2.0, s->pitch / 12.0);
  size_t at = 1 + 8 * count;
  for (uint32_t i = 0; i < kParamCount; i++) {
    s->ccMap[i] = -1;
    if (ccBytes) { const int16_t cc = (int16_t)(buf[at + 2 * i] | (buf[at + 2 * i + 1] << 8)); s->ccMap[i] = (cc >= 0 && cc < 128) ? cc : -1; }
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

void pageSendValues(CppnEngine* s) {
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
    const bool stepped = i >= kParamDcFilter && i <= kParamSoftClip;
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
          const bool stepped = i >= kParamDcFilter && i <= kParamSoftClip;
          const double t = e->data[2] / 127.0;
          applyParam(s, i, stepped ? (t >= 0.5 ? 1.0 : 0.0) : d.minV + (d.maxV - d.minV) * t);
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
      for (auto& v : s->voices) { v.active = false; v.st.allocate(s->plan); }   // should not happen: the pads keep the structure
    s->convScaleCache.clear();
    s->geoDirty = false;
    s->geoMeasurePending = true;
    s->geoWait = 0;
  }
  if (s->geoMeasurePending && !s->geoDirty) {
    bool any = false;
    for (auto& v : s->voices) any = any || v.active;
    if (!any) { const uint32_t keep = s->noteCounter; measureLevel(s); s->noteCounter = keep; s->geoMeasurePending = false; }
  }
  for (auto& n : s->pageNotes) { if (n.on) noteOn(s, n.key, n.velocity, -1, 0); else noteOff(s, n.key, -1, 0, false); }
  s->pageNotes.clear();
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
      if (!v.active) continue;
      // envelope
      if (v.envLeft > 0) {
        v.env += v.envStep;
        if (--v.envLeft == 0) {
          if (v.released) v.env = 0;
          else v.env = 1.0;
        }
        if (v.env <= 0 && v.released) { v.active = false; continue; }
      }
      float sample = renderVoiceSample(s, v);
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
      const double master = std::pow(10.0, s->volumeDb / 20.0) * (s->autoLevel >= 0.5 ? s->trim : 1.0);
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
  return nullptr;
}
void pluginOnMainThread(const clap_plugin_t*) {}

const clap_plugin_t* createPlugin(const clap_host_t* host) {
  void* mem = std::calloc(1, sizeof(CppnEngine));
  CppnEngine* s = new (mem) CppnEngine();
  s->host = host;
  for (auto& c : s->ccMap) c = -1;
  for (auto& o : s->paramOut) o = false;
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
