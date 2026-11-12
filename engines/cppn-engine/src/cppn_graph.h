// Copyright 2026 Björn Þór Jónsson (kromosynth-burst). Licensed under the MIT License; see ../LICENSE.
//
// cppn_graph.h — the DSP patch of a CPPN+DSP genome, compiled once into an index-based plan and
// rendered per sample. No strings, maps or allocation while sounding.
//
// THE SPECIFICATION IS THE PRODUCTION RENDERER, not the legacy batch one:
//   kromosynth-render/render-socket/src/worklet-offline-renderer.js   (renderWithWorkletOffline)
//   kromosynth/cppn-neat/network-rendering.js, mode 'streaming'
//   kromosynth/util/audio-graph-asNEAT-bridge.js                      (patchFromAsNEATnetwork)
//   kromosynth/wavekilde.js                                           (pitch transform, prune)
// and, for node maths, the Rust `web-audio-api` crate v1.6.0 that node-web-audio-api 2.1.0 wraps.
// Rules taken from them (each was a measured difference against the first engine version):
//   * every connection in the genome is wired, the `enabled` flag is not read;
//   * a connection into the output node carries NO weight; connections repeated on the same
//     source/target/parameter share one weight (the later one) and count once per repeat;
//   * node properties: only the schema's names, and only truthy values (so the genome's lowercase
//     filter `q` is never applied and a zero falls back to the Web Audio default);
//   * a network output is split by role (buffer / partialBuffer / other); only the first two follow
//     the played key, partials at frequency*partialNumber (no inharmonicity on this path);
//   * CPPN signals on a parameter ADD (value = midpoint of the last ranged connection + sum of
//     half-range*signal); without a range the base is 0;
//   * a buffer source is a pass-through gain: its audio inputs plus the last buffer signal;
//   * wavetable: plain 0..1 triangular crossfade by the mix signal, all waves at unity when there is
//     no mix signal, silent when there are no waves; additive: partials keyed by partialNumber;
//   * wavetable/additive outputs are 6-channel (a channel merger with everything in channel 0),
//     so Web Audio's 5.1 down-mix puts them at 0.7071 at the output while mono signals mixed with
//     them stay at unity (measured by least-squares fit, residual -134 dB);
//   * oscillators: sample first, then advance; sawtooth and triangle start at 0; PolyBLEP on
//     sawtooth and square; silent at or above Nyquist; those that feed audio follow the played key;
//   * biquad coefficients as in the crate (Q in dB for low/highpass, shelves with S = 1).
#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "cppn_core.h"
#include "faust_nodes.h"
#include "mini_json.h"

namespace cgraph {

constexpr int kMaxCh = 6;

enum Kind : uint8_t { K_OUTPUT, K_GAIN, K_BIQUAD, K_OSC, K_BUFSRC, K_WAVETABLE, K_ADDITIVE, K_SHAPER, K_DELAY, K_FBDELAY, K_COMP, K_CONV, K_FAUST, K_PASS, K_SILENT };
enum Role : uint8_t { R_BUFFER = 0, R_PARTIAL = 1, R_OTHER = 2 };
enum OscType : uint8_t { OSC_SINE, OSC_SQUARE, OSC_SAW, OSC_TRI };
enum FiltType : uint8_t { F_LOWPASS, F_HIGHPASS, F_BANDPASS, F_NOTCH, F_ALLPASS, F_PEAKING, F_LOWSHELF, F_HIGHSHELF };

// ── the patch as written in the genome ────────────────────────────────────────────────────────────

struct SrcNode {
  std::string id, type;                 // type = virtual-audio-graph name (gain, biquadFilter, ...)
  std::map<std::string, double> num;    // copied properties
  std::string choice;                   // oscillator waveform / filter type
  std::vector<float> curve;             // a wave shaper's own static curve, if any
  std::string tpl;                      // faust: template name
  std::map<std::string, double> fparams;  // faust: stored slider values by full path
};
struct SrcEdge {                        // DSP node -> DSP node (or its parameter), or -> output
  std::string src, tgt, param;
  bool toOutput = false;
  double weight = 1;
};
struct SrcSigConn {                     // network output -> DSP node parameter
  std::string tgt, param;
  bool hasRange = false;
  double lo = 0, hi = 1, weight = 1, partialNumber = 0, inharmonicity = 0;
};
struct SrcNetOut {
  std::string id;
  int cppnOut = 0;
  bool noise = false;
  double frequency = 440;
  std::vector<SrcSigConn> conns;
};
struct SrcPatch {
  std::vector<SrcNode> nodes;
  std::vector<SrcEdge> edges;
  std::vector<SrcNetOut> netOuts;
  bool present = false;
  // which oscillators have an audio connection is decided differently for a pre-converted patch
  std::vector<std::string> transposedOscIds;
  bool preConverted = false;
};

inline double frequencyOfStepsFromRootNote(double steps) { return 440.0 * std::pow(2.0, steps / 12.0); }

inline std::string vagTypeOf(const std::string& asNeatName) {
  if (asNeatName == "FilterNode") return "biquadFilter";
  if (asNeatName == "ConvolverNode") return "convolver";
  if (asNeatName == "DelayNode") return "delay";
  if (asNeatName == "CompressorNode") return "dynamicsCompressor";
  if (asNeatName == "GainNode") return "gain";
  if (asNeatName == "WaveShaperNode") return "waveShaper";
  if (asNeatName == "WavetableNode") return "wavetable";
  if (asNeatName == "AdditiveNode") return "additive";
  if (asNeatName == "FeedbackDelayNode") return "feedbackDelay";
  if (asNeatName == "OscillatorNode" || asNeatName == "NoteOscillatorNode") return "oscillator";
  if (asNeatName == "AudioBufferSourceNode") return "bufferSource";
  if (asNeatName == "FaustNode") return "faust";
  return "";
}

inline bool isNetworkOutputName(const std::string& n) {
  return n == "NetworkOutputNode" || n == "NoteNetworkOutputNode" || n == "PartialNetworkOutputNode" ||
         n == "PartialEnvelopeNetworkOutputNode";
}

// the schema's parameter names per node type (kromosynth/util/audio-graph.js)
inline const char* const* schemaNumNames(const std::string& type) {
  static const char* const filt[] = {"frequency", "detune", "Q", "gain", nullptr};
  static const char* const gain[] = {"gain", nullptr};
  static const char* const osc[] = {"frequency", "detune", "noteOffset", nullptr};
  static const char* const buf[] = {"detune", "playbackRate", nullptr};
  static const char* const delay[] = {"delayTime", nullptr};
  static const char* const fbd[] = {"delayTime", "feedbackRatio", nullptr};
  static const char* const comp[] = {"threshold", "knee", "ratio", "attack", "release", "reduction", nullptr};
  static const char* const add[] = {"masterGain", nullptr};
  static const char* const none[] = {nullptr};
  if (type == "biquadFilter") return filt;
  if (type == "gain") return gain;
  if (type == "oscillator") return osc;
  if (type == "bufferSource") return buf;
  if (type == "delay") return delay;
  if (type == "feedbackDelay") return fbd;
  if (type == "dynamicsCompressor") return comp;
  if (type == "additive") return add;
  return none;
}

// a genome array item is either an object or a JSON string holding one; always a FRESH value
inline bool itemObject(const minijson::Value& v, minijson::Value& out) {
  if (v.type == minijson::Value::Obj) { out = v; return true; }
  if (v.type == minijson::Value::Str) {
    minijson::Parser p(v.str);
    minijson::Value fresh;
    if (p.parse(fresh) && fresh.type == minijson::Value::Obj) { out = std::move(fresh); return true; }
  }
  return false;
}

inline void readNetOutType(const minijson::Value* t, SrcNetOut& no) {
  if (!t) return;
  if (t->type == minijson::Value::Num) no.cppnOut = (int)t->num;
  else if (t->type == minijson::Value::Str) {
    if (t->str.compare(0, 5, "noise") == 0) no.noise = true;
    else no.cppnOut = (int)std::strtol(t->str.c_str(), nullptr, 10);
  }
}

// The web app's Weights and Frequencies pads (kromosynth-desktop utils/GenomeGeometryMapper.js), as
// engine parameters. Every node gets a fixed pseudo-random position from a hash of its id, every
// connection sits midway between its two nodes, and a pad position (dx, dy from the centre, -1..1)
// shifts each value by its position's projection on that direction:
//   weights:      w += (dx*cx + dy*cy) * 5, wrapped into [-3, 3)   (every DSP connection)
//   frequencies:  noteOffset += (dx*nx + dy*ny) * 24 for note outputs, frequency += ... * 500 Hz
//                 (clamped 20..10000) for every other node that has a frequency
// Nothing changes within 0.01 of the centre. Verified bit-identical against the JS mapper.
struct Geo {
  double wx = 0.5, wy = 0.5, fx = 0.5, fy = 0.5;
  bool weightsOn() const { return !(std::fabs((wx - 0.5) * 2) < 0.01 && std::fabs((wy - 0.5) * 2) < 0.01); }
  bool freqsOn() const { return !(std::fabs((fx - 0.5) * 2) < 0.01 && std::fabs((fy - 0.5) * 2) < 0.01); }
};
inline double rollValue(double v, double lo, double hi) {
  const double range = hi - lo;
  double off = std::fmod(v - lo, range);
  if (off < 0) off += range;
  return lo + off;
}
inline uint32_t jsHash(const std::string& str) {  // GenomeGeometryMapper.hashString
  int32_t h = 0;
  for (unsigned char c : str) h = (int32_t)(((uint32_t)h << 5) - (uint32_t)h + (uint32_t)c);
  return (uint32_t)(h < 0 ? -(int64_t)h : (int64_t)h);
}
struct GeoPos { double x = 0, y = 0; bool known = false; };
inline GeoPos nodePosition(const std::string& name, const std::string& idOrIndex) {
  const uint32_t h = jsHash("patch-" + idOrIndex);
  GeoPos p;
  p.known = true;
  if (name == "OscillatorNode" || name == "NoteOscillatorNode") { p.x = -0.8; p.y = (double)(h % 100) / 50.0 - 1.0; }
  else if (name == "OutNode" || name == "NetworkOutputNode") { p.x = 0.8; p.y = (double)(h % 100) / 50.0 - 1.0; }
  else { p.x = (double)(h % 200) / 100.0 - 1.0; p.y = (double)((h >> 8) % 200) / 100.0 - 1.0; }
  return p;
}

// split a genome's {nodes, connections} into objects once (each item is an object or a JSON string)
inline void splitAsNeat(const minijson::Value& asNeat, std::vector<minijson::Value>& nodeObjs, std::vector<minijson::Value>& connObjs) {
  nodeObjs.clear(); connObjs.clear();
  const minijson::Value* nodes = asNeat.find("nodes");
  const minijson::Value* conns = asNeat.find("connections");
  if (nodes && nodes->type == minijson::Value::Arr)
    for (const auto& nv : nodes->arr) { minijson::Value n; if (itemObject(nv, n)) nodeObjs.push_back(std::move(n)); else nodeObjs.push_back(minijson::Value()); }
  if (conns && conns->type == minijson::Value::Arr)
    for (const auto& cv : conns->arr) { minijson::Value c; if (itemObject(cv, c)) connObjs.push_back(std::move(c)); else connObjs.push_back(minijson::Value()); }
}

// asNEAT network (already split) -> SrcPatch, as patchFromAsNEATnetwork does, with the pads applied
inline void parseAsNeat(const std::vector<minijson::Value>& nodeObjs, const std::vector<minijson::Value>& connObjs, SrcPatch& sp, const Geo& geo = Geo()) {
  sp.present = true;
  const double wdx = (geo.wx - 0.5) * 2, wdy = (geo.wy - 0.5) * 2, fdx = (geo.fx - 0.5) * 2, fdy = (geo.fy - 0.5) * 2;
  const bool wOn = geo.weightsOn(), fOn = geo.freqsOn();
  std::map<std::string, int> netOutIx, nodeIx;
  std::map<std::string, std::pair<double, double>> partialInfo;  // netOut id -> (partialNumber, inharmonicity)
  std::map<std::string, GeoPos> pos;   // by node id; the output node (numeric id 0) under "#0"
  for (size_t i = 0; i < nodeObjs.size(); i++) {
    const minijson::Value& n = nodeObjs[i];
    if (n.type != minijson::Value::Obj) continue;
    const std::string name = n.getStr("name");
    const std::string id = n.getStr("id");
    const minijson::Value* idv = n.find("id");
    const bool numericZeroId = idv && idv->type == minijson::Value::Num && idv->num == 0;
    // `${prefix}-${node.id || i}`: a falsy id (the output node's 0) falls back to the array index
    const GeoPos np = nodePosition(name, id.empty() ? std::to_string(i) : id);
    if (!id.empty()) pos[id] = np; else if (numericZeroId) pos["#0"] = np;
    if (name == "OutNode" || id.empty()) continue;
    const double infl = fdx * np.x + fdy * np.y;
    double noteOffset = n.getNum("noteOffset", 0);
    const minijson::Value* fv = n.find("frequency");
    bool hasFrequency = fv != nullptr;
    double frequency = fv && fv->type == minijson::Value::Num ? fv->num : 440;
    if (fOn) {
      if (name == "NoteNetworkOutputNode") noteOffset += infl * 24.0;
      else if (hasFrequency) frequency = std::min(10000.0, std::max(20.0, frequency + infl * 500.0));
    }
    if (isNetworkOutputName(name)) {
      SrcNetOut no;
      no.id = id;
      readNetOutType(n.find("type"), no);
      no.frequency = (name == "NoteNetworkOutputNode" && noteOffset != 0) ? frequencyOfStepsFromRootNote(noteOffset)
                                                                           : (hasFrequency ? frequency : 440);
      if (name == "PartialNetworkOutputNode" || name == "PartialEnvelopeNetworkOutputNode")
        partialInfo[id] = {n.getNum("partialNumber", 0), n.getNum("inharmonicityFactor", 0)};
      netOutIx[id] = (int)sp.netOuts.size();
      sp.netOuts.push_back(std::move(no));
      continue;
    }
    SrcNode sn;
    sn.id = id;
    sn.type = vagTypeOf(name);
    for (const char* const* k = schemaNumNames(sn.type); *k; ++k) {
      const minijson::Value* v = n.find(*k);
      if (v && v->type == minijson::Value::Num && v->num != 0) sn.num[*k] = v->num;  // truthy only
    }
    if (fOn && hasFrequency && (sn.type == "biquadFilter" || sn.type == "oscillator")) {
      if (frequency != 0) sn.num["frequency"] = frequency; else sn.num.erase("frequency");
    }
    if (sn.type == "biquadFilter" || sn.type == "oscillator") sn.choice = n.getStr("type");
    if (sn.type == "waveShaper")
      if (const minijson::Value* c = n.find("curve"))
        if (c->type == minijson::Value::Arr)
          for (const auto& x : c->arr) if (x.type == minijson::Value::Num) sn.curve.push_back((float)x.num);
    if (name == "NoteOscillatorNode") {
      if (noteOffset != 0) sn.num["frequency"] = frequencyOfStepsFromRootNote(noteOffset);
    }
    if (sn.type == "faust") {
      sn.tpl = n.getStr("templateName");
      if (const minijson::Value* pv = n.find("paramValues"))
        if (pv->type == minijson::Value::Obj)
          for (auto& kv : pv->obj) if (kv.second.type == minijson::Value::Num) sn.fparams[kv.first] = kv.second.num;
    }
    nodeIx[id] = (int)sp.nodes.size();
    sp.nodes.push_back(std::move(sn));
  }
  for (const auto& c : connObjs) {
    if (c.type != minijson::Value::Obj) continue;
    // NOTE: `enabled` is deliberately not read. The renderer wires every connection.
    const std::string src = c.getStr("sourceNode");
    const minijson::Value* tv = c.find("targetNode");
    const bool toOutput = tv && tv->type == minijson::Value::Num && tv->num == 0;
    const std::string tgt = tv && tv->type == minijson::Value::Str ? tv->str : "";
    const std::string param = c.getStr("targetParameter");
    double weight = c.getNum("weight", 1.0);
    if (wOn) {
      auto ps = pos.find(src), pt = pos.find(toOutput ? std::string("#0") : tgt);
      double cx = 0, cy = 0;
      if (ps != pos.end() && pt != pos.end()) { cx = (ps->second.x + pt->second.x) / 2; cy = (ps->second.y + pt->second.y) / 2; }
      weight = rollValue(weight + (wdx * cx + wdy * cy) * 5.0, -3.0, 3.0);
    }
    bool hasRange = false;
    double lo = 0, hi = 1;
    if (const minijson::Value* r = c.find("targetParameterRange"))
      if (r->type == minijson::Value::Arr && r->arr.size() >= 2) { hasRange = true; lo = r->arr[0].num; hi = r->arr[1].num; }
    auto noIt = netOutIx.find(src);
    if (noIt != netOutIx.end()) {
      if (toOutput || tgt.empty()) continue;  // a network output aimed at the output node reaches nothing
      SrcSigConn sc;
      sc.tgt = tgt; sc.param = param; sc.hasRange = hasRange; sc.lo = lo; sc.hi = hi; sc.weight = weight;
      auto pi = partialInfo.find(src);
      if (pi != partialInfo.end()) { sc.partialNumber = pi->second.first; sc.inharmonicity = pi->second.second; }
      sp.netOuts[noIt->second].conns.push_back(sc);
    } else if (nodeIx.count(src)) {
      if (!toOutput && tgt.empty()) continue;
      SrcEdge e;
      e.src = src; e.tgt = tgt; e.param = param; e.toOutput = toOutput; e.weight = weight;
      sp.edges.push_back(e);
    }
  }
}

// a pre-converted synthIsPatch {audioGraph: {id: [type, conns, params]}, networkOutputs: [...]}
inline void parseSynthIsPatch(const minijson::Value& patch, SrcPatch& sp) {
  sp.present = true;
  sp.preConverted = true;
  const minijson::Value* ag = patch.find("audioGraph");
  if (ag && ag->type == minijson::Value::Obj) {
    for (auto& kv : ag->obj) {
      if (kv.second.type != minijson::Value::Arr || kv.second.arr.size() < 2) continue;
      SrcNode sn;
      sn.id = kv.first;
      sn.type = kv.second.arr[0].type == minijson::Value::Str ? kv.second.arr[0].str : "";
      if (kv.second.arr.size() >= 3 && kv.second.arr[2].type == minijson::Value::Obj)
        for (auto& p : kv.second.arr[2].obj) {
          if (p.second.type == minijson::Value::Num) sn.num[p.first] = p.second.num;
          else if (p.second.type == minijson::Value::Str && p.first == "type") sn.choice = p.second.str;
          else if (p.second.type == minijson::Value::Str && p.first == "templateName") sn.tpl = p.second.str;
          else if (p.second.type == minijson::Value::Obj && p.first == "paramValues") {
            for (auto& fv : p.second.obj) if (fv.second.type == minijson::Value::Num) sn.fparams[fv.first] = fv.second.num;
          }
          else if (p.second.type == minijson::Value::Arr && p.first == "curve")
            for (auto& c : p.second.arr) if (c.type == minijson::Value::Num) sn.curve.push_back((float)c.num);
        }
      bool audioOut = false;
      if (kv.second.arr[1].type == minijson::Value::Arr)
        for (auto& cv : kv.second.arr[1].arr) {
          SrcEdge e;
          e.src = kv.first;
          if (cv.type == minijson::Value::Str) {
            e.tgt = cv.str;
            // "<a>_<b>-weight" is an audio weight node, "<a>_<b>-<param>-weight" feeds a parameter
            if (cv.str == "output" || std::count(cv.str.begin(), cv.str.end(), '-') == 1) audioOut = true;
          } else if (cv.type == minijson::Value::Obj) {
            e.tgt = cv.getStr("key");
            e.param = cv.getStr("destination");
          }
          if (e.tgt.empty()) continue;
          if (e.tgt == "output") { e.toOutput = true; e.tgt.clear(); }
          sp.edges.push_back(e);
        }
      if (sn.type == "oscillator" && audioOut) sp.transposedOscIds.push_back(sn.id);
      sp.nodes.push_back(std::move(sn));
    }
  }
  const minijson::Value* nos = patch.find("networkOutputs");
  if (nos && nos->type == minijson::Value::Arr)
    for (auto& nov : nos->arr) {
      SrcNetOut no;
      no.id = nov.getStr("id");
      readNetOutType(nov.find("networkOutput"), no);
      no.frequency = nov.getNum("frequency", 440);
      if (const minijson::Value* agn = nov.find("audioGraphNodes"))
        if (agn->type == minijson::Value::Obj)
          for (auto& tgt : agn->obj)
            if (tgt.second.type == minijson::Value::Arr)
              for (auto& c : tgt.second.arr) {
                SrcSigConn sc;
                sc.tgt = tgt.first;
                sc.param = c.getStr("paramName");
                sc.weight = c.getNum("weight", 1);
                sc.partialNumber = c.getNum("partialNumber", 0);
                sc.inharmonicity = c.getNum("inharmonicityFactor", 0);
                if (const minijson::Value* r = c.find("range"))
                  if (r->type == minijson::Value::Arr && r->arr.size() >= 2) { sc.hasRange = true; sc.lo = r->arr[0].num; sc.hi = r->arr[1].num; }
                no.conns.push_back(sc);
              }
      sp.netOuts.push_back(std::move(no));
    }
}

// The sliders of a Faust template (path, default, range), found once per template by making an instance.
struct FaustMeta { bool unknown = true; std::vector<fdsp::ParamInfo> params; };
inline const FaustMeta& faustParamsOf(const std::string& tpl, int sampleRate) {
  static std::map<std::string, FaustMeta> cache;
  auto it = cache.find(tpl);
  if (it != cache.end()) return it->second;
  FaustMeta m;
  fdsp::Instance inst;
  if (inst.make(tpl, sampleRate)) {
    fdsp::Collector c;
    inst.dsp->buildUserInterface(&c);
    m.params = c.params;
    for (auto& p : m.params) p.zone = nullptr;
    m.unknown = false;
  }
  return cache[tpl] = m;
}

// ── the compiled plan ─────────────────────────────────────────────────────────────────────────────

struct SigRef { int slot = -1; double half = 1, mid = 0; };  // half*signal + mid
struct AudioIn { int node; double weight; };
struct Param {
  double base = 0;
  double lo = -1e300, hi = 1e300;  // the AudioParam's own range; the summed value is clamped to it
  bool followsKey = false;         // base is multiplied by the played-key ratio (oscillator frequency)
  std::vector<SigRef> sigs;
  std::vector<AudioIn> mods;
  bool dynamic() const { return !sigs.empty() || !mods.empty(); }
};
struct Partial {
  std::vector<SigRef> bufs, envs;
  double envBase = 1, weight = 1;
};
struct Node {
  Kind kind = K_PASS;
  int ch = 1;               // output channel count (true count; layouts are recognised for 1, 2, 4, 6)
  int state = -1;           // index into the voice's filter / oscillator state
  uint8_t sub = 0;          // OscType or FiltType
  std::vector<AudioIn> ins;
  Param p[5];               // gain: gain | biquad: frequency, detune, Q, gain | osc: frequency, detune
                            // compressor: threshold, knee, ratio, attack, release (read once per 128 frames)
  SigRef buf;               // buffer source: last buffer signal; wave shaper: CPPN curve
  std::vector<float> curve; // wave shaper: static curve
  std::vector<SigRef> waves;
  SigRef mix;
  std::vector<Partial> partials;
  double coef[5] = {1, 0, 0, 0, 0};  // biquad b0 b1 b2 a1 a2 when no parameter moves
  int delayIx = -1;         // delay / feedback delay: index of its lines in the voice state
  int delayLen = 0;         // samples kept per channel
  double feedback = 0;      // feedback delay: the ratio
  std::string tpl;          // faust: template name
  std::vector<Param> fp;    // faust: one per slider, in the template's own order
  std::vector<std::string> fpath;
};
struct Slot {
  int cppnOut = 0;
  bool noise = false;
  double baseFreq = 440;   // already multiplied by the partial number where that applies
  bool followsKey = false;
  int group = -1;
};
struct Group { double baseFreq; bool followsKey; std::vector<int> slots; };

struct Plan {
  std::vector<Node> nodes;
  std::vector<int> order;
  int output = -1;
  int limiterChans = 1;     // channels of the limiter's input: 2 when any output-connected node is multi-channel
  std::vector<Slot> slots;
  std::vector<Group> groups;
  int numBiquads = 0, numOscs = 0, numDelays = 0, numComps = 0, numConvs = 0;
  std::vector<std::string> faustTpl;     // per Faust node: its template
  int convCap = 0;          // samples of impulse response kept per convolver (set by the engine from the note length)
  std::vector<int> convChans;
  std::vector<int> delayLens;            // per delay node: samples kept per channel
  std::vector<int> delayChans;           // per delay node: channels
  double sampleRate = 48000;
  std::vector<std::string> unsupported;  // node types played as plain pass-through
};

struct CompState {            // DynamicsCompressorNode, as the crate: 3 render quanta of look-ahead delay
  float line[2][384];
  float prev = 0, threshold = 0, halfKnee = 0, kneePartial = 0, ratio = 1, attackTau = 0, releaseTau = 0, makeup = 0;
};

// ConvolverNode whose impulse response is a CPPN signal lasting the whole note. The crate convolves
// with an FFT convolver (mathematically a plain convolution, no latency) and scales the response by
// its 'normalize' rule. Here the response arrives one sample per sample, so the convolution is split:
//   y[n] = sum over (i + k = n) of x[i] * h[k]
//   * k in the first block of h, any i        -> direct sum (kConvB products per sample)
//   * i in the first block of x, k beyond it  -> direct sum (kConvB products per sample)
//   * everything else, block pair (a >= 1, j >= 1) -> spectra multiplied at each block boundary, by
//     which time both blocks are complete and their output has not started yet.
constexpr int kConvB = 512, kConvN = 1024;
struct ConvState {
  int cap = 0, parts = 0;
  float scale = 1;
  std::vector<float> h, x[2];        // unscaled response, input history
  std::vector<float> H, X[2];        // per block: kConvB + 1 complex bins (re, im)
  float ola[2][kConvB], tail[2][kConvB];
  void allocate(int capSamples, int chans) {
    parts = (capSamples + kConvB - 1) / kConvB;
    cap = parts * kConvB;
    h.assign(cap, 0.f);
    H.assign((size_t)parts * (kConvB + 1) * 2, 0.f);
    for (int c = 0; c < 2; c++) {
      x[c].assign(c < chans ? cap : 0, 0.f);
      X[c].assign(c < chans ? (size_t)parts * (kConvB + 1) * 2 : 0, 0.f);
    }
    clear();
  }
  void clear() {
    std::fill(h.begin(), h.end(), 0.f);
    for (int c = 0; c < 2; c++) { std::fill(x[c].begin(), x[c].end(), 0.f); std::fill(ola[c], ola[c] + kConvB, 0.f); std::fill(tail[c], tail[c] + kConvB, 0.f); }
  }
};

// in-place radix-2 FFT, kConvN points
inline void convFft(float* re, float* im, bool inverse) {
  static float cs[kConvN / 2], sn[kConvN / 2];
  static bool init = false;
  if (!init) { for (int i = 0; i < kConvN / 2; i++) { cs[i] = (float)std::cos(2.0 * M_PI * i / kConvN); sn[i] = (float)std::sin(2.0 * M_PI * i / kConvN); } init = true; }
  for (int i = 1, j = 0; i < kConvN; i++) {
    int bit = kConvN >> 1;
    for (; j & bit; bit >>= 1) j ^= bit;
    j ^= bit;
    if (i < j) { std::swap(re[i], re[j]); std::swap(im[i], im[j]); }
  }
  for (int len = 2; len <= kConvN; len <<= 1) {
    const int half = len >> 1, step = kConvN / len;
    for (int i = 0; i < kConvN; i += len)
      for (int k = 0; k < half; k++) {
        const float wr = cs[k * step], wi = inverse ? sn[k * step] : -sn[k * step];
        const float ur = re[i + k], ui = im[i + k];
        const float vr = re[i + k + half] * wr - im[i + k + half] * wi, vi = re[i + k + half] * wi + im[i + k + half] * wr;
        re[i + k] = ur + vr; im[i + k] = ui + vi;
        re[i + k + half] = ur - vr; im[i + k + half] = ui - vi;
      }
  }
  if (inverse) for (int i = 0; i < kConvN; i++) { re[i] /= kConvN; im[i] /= kConvN; }
}

struct VoiceState {
  std::vector<double> out;     // nodes * kMaxCh
  std::vector<double> sig;     // slots
  std::vector<double> bq;      // biquads * kMaxCh * 4   (x1 x2 y1 y2)
  std::vector<double> phase;   // oscillators
  std::vector<uint32_t> rng;   // slots
  std::vector<CompState> comp;
  std::vector<ConvState> conv;
  CompState limiter;            // the output limiter of this voice
  std::vector<std::vector<float>> delay;  // per delay node: kMaxCh lines of delayLens[i] samples (float, as the renderer stores them)
  std::vector<fdsp::Instance> faust;      // per Faust node
  std::vector<std::string> faustMade;     // the templates the instances were made from (and at which rate)
  int faustRate = 0;
  void allocate(const Plan& p) {
    if (faustMade != p.faustTpl || faustRate != (int)p.sampleRate) {   // instances are big: remade only when the set changes
      faust.clear();
      faust.resize(p.faustTpl.size());
      for (size_t i = 0; i < p.faustTpl.size(); i++) faust[i].make(p.faustTpl[i], (int)p.sampleRate);
      faustMade = p.faustTpl;
      faustRate = (int)p.sampleRate;
    }
    out.assign(p.nodes.size() * kMaxCh, 0.0);
    sig.assign(p.slots.size(), 0.0);
    bq.assign((size_t)p.numBiquads * kMaxCh * 4, 0.0);
    phase.assign(p.numOscs, 0.0);
    rng.assign(p.slots.size(), 1u);
    comp.assign(p.numComps, CompState());
    conv.assign(p.numConvs, ConvState());
    for (int i = 0; i < p.numConvs; i++) conv[i].allocate(p.convCap, p.convChans[i]);
    delay.assign(p.numDelays, {});
    for (int i = 0; i < p.numDelays; i++) delay[i].assign((size_t)p.delayLens[i] * p.delayChans[i], 0.0f);
  }
  void reset(uint32_t noteSeed) {
    std::fill(out.begin(), out.end(), 0.0);
    std::fill(sig.begin(), sig.end(), 0.0);
    std::fill(bq.begin(), bq.end(), 0.0);
    std::fill(phase.begin(), phase.end(), 0.0);
    for (auto& d : delay) std::fill(d.begin(), d.end(), 0.0f);
    for (auto& c : comp) c = CompState();
    for (auto& c : conv) c.clear();
    for (auto& f : faust) if (f.dsp) f.dsp->instanceClear();
    limiter = CompState();
    for (size_t i = 0; i < rng.size(); i++) {
      uint32_t s = 0x9e3779b9u ^ ((uint32_t)(i + 1) * 0x85ebca6bu) ^ (noteSeed * 0xc2b2ae35u);
      rng[i] = s ? s : 1u;
    }
  }
};

// biquad coefficients exactly as web-audio-api-rs v1.6.0 src/node/biquad_filter.rs
inline void biquadCoefs(uint8_t type, double sampleRate, double f0, double gainDb, double q, double* c) {
  const double nyquist = sampleRate / 2.0;
  double freq = f0 / nyquist;
  freq = freq < 0 ? 0 : (freq > 1 ? 1 : freq);
  auto set = [&](double b0, double b1, double b2, double a1, double a2) { c[0] = b0; c[1] = b1; c[2] = b2; c[3] = a1; c[4] = a2; };
  auto norm = [&](double b0, double b1, double b2, double a0, double a1, double a2) {
    const double s = 1.0 / a0;
    set(b0 * s, b1 * s, b2 * s, a1 * s, a2 * s);
  };
  const double w0 = M_PI * freq, sinw = std::sin(w0), cosw = std::cos(w0);
  const double A = std::pow(10.0, gainDb / 40.0);
  switch (type) {
    case F_LOWPASS: {
      if (freq == 1) { set(1, 0, 0, 0, 0); break; }
      const double a = sinw / (2.0 * std::pow(10.0, q / 20.0)), beta = (1.0 - cosw) / 2.0;
      norm(beta, 2.0 * beta, beta, 1.0 + a, -2.0 * cosw, 1.0 - a);
      break;
    }
    case F_HIGHPASS: {
      if (freq == 1) { set(0, 0, 0, 0, 0); break; }
      if (freq == 0) { set(1, 0, 0, 0, 0); break; }
      const double a = sinw / (2.0 * std::pow(10.0, q / 20.0)), beta = (1.0 + cosw) / 2.0;
      norm(beta, -2.0 * beta, beta, 1.0 + a, -2.0 * cosw, 1.0 - a);
      break;
    }
    case F_BANDPASS: {
      if (!(freq > 0 && freq < 1)) { set(0, 0, 0, 0, 0); break; }
      if (!(q > 0)) { set(1, 0, 0, 0, 0); break; }
      const double a = sinw / (2.0 * q);
      norm(a, 0, -a, 1.0 + a, -2.0 * cosw, 1.0 - a);
      break;
    }
    case F_NOTCH: {
      if (!(freq > 0 && freq < 1)) { set(1, 0, 0, 0, 0); break; }
      if (!(q > 0)) { set(0, 0, 0, 0, 0); break; }
      const double a = sinw / (2.0 * q);
      norm(1, -2.0 * cosw, 1, 1.0 + a, -2.0 * cosw, 1.0 - a);
      break;
    }
    case F_ALLPASS: {
      if (!(freq > 0 && freq < 1)) { set(1, 0, 0, 0, 0); break; }
      if (!(q > 0)) { set(-1, 0, 0, 0, 0); break; }
      const double a = sinw / (2.0 * q);
      norm(1.0 - a, -2.0 * cosw, 1.0 + a, 1.0 + a, -2.0 * cosw, 1.0 - a);
      break;
    }
    case F_PEAKING: {
      if (!(freq > 0 && freq < 1)) { set(1, 0, 0, 0, 0); break; }
      if (!(q > 0)) { set(A * A, 0, 0, 0, 0); break; }
      const double a = sinw / (2.0 * q);
      norm(1.0 + a * A, -2.0 * cosw, 1.0 - a * A, 1.0 + a / A, -2.0 * cosw, 1.0 - a / A);
      break;
    }
    case F_LOWSHELF: {
      if (freq == 1) { set(A * A, 0, 0, 0, 0); break; }
      if (freq == 0) { set(1, 0, 0, 0, 0); break; }
      const double t = 2.0 * (sinw / 2.0 * M_SQRT2) * std::sqrt(A), ap = A + 1.0, am = A - 1.0;
      norm(A * (ap - am * cosw + t), 2.0 * A * (am - ap * cosw), A * (ap - am * cosw - t),
           ap + am * cosw + t, -2.0 * (am + ap * cosw), ap + am * cosw - t);
      break;
    }
    default: {  // F_HIGHSHELF
      if (freq == 1) { set(1, 0, 0, 0, 0); break; }
      if (!(freq > 0)) { set(A * A, 0, 0, 0, 0); break; }
      const double t = 2.0 * (sinw / 2.0 * M_SQRT2) * std::sqrt(A), ap = A + 1.0, am = A - 1.0;
      norm(A * (ap + am * cosw + t), -2.0 * A * (am + ap * cosw), A * (ap + am * cosw - t),
           ap - am * cosw + t, 2.0 * (am - ap * cosw), ap - am * cosw - t);
      break;
    }
  }
}

inline uint8_t filtTypeOf(const std::string& s) {
  if (s == "highpass") return F_HIGHPASS;
  if (s == "bandpass") return F_BANDPASS;
  if (s == "notch") return F_NOTCH;
  if (s == "allpass") return F_ALLPASS;
  if (s == "peaking") return F_PEAKING;
  if (s == "lowshelf") return F_LOWSHELF;
  if (s == "highshelf") return F_HIGHSHELF;
  return F_LOWPASS;
}
inline uint8_t oscTypeOf(const std::string& s) {
  if (s == "square") return OSC_SQUARE;
  if (s == "sawtooth") return OSC_SAW;
  if (s == "triangle") return OSC_TRI;
  return OSC_SINE;
}

inline void compile(const SrcPatch& sp, double sampleRate, Plan& plan) {
  plan = Plan();
  plan.sampleRate = sampleRate;
  const double nyquist = sampleRate / 2.0;
  const int N = (int)sp.nodes.size();
  const int OUT = N;  // index of the output node
  std::map<std::string, int> ix;
  for (int i = 0; i < N; i++) ix[sp.nodes[i].id] = i;

  // 1. edges, with the renderer's collapsing of duplicates
  struct E { int src, tgt; std::string param; double weight; };
  std::vector<E> edges;
  std::vector<int> edgeCount;
  std::map<std::string, int> seen;
  for (const auto& e : sp.edges) {
    auto si = ix.find(e.src);
    if (si == ix.end()) continue;
    int tgt;
    if (e.toOutput) tgt = OUT;
    else { auto ti = ix.find(e.tgt); if (ti == ix.end()) continue; tgt = ti->second; }
    const double w = e.toOutput ? 1.0 : e.weight;  // no weight on the way into the output
    const std::string key = std::to_string(si->second) + ">" + std::to_string(tgt) + ">" + e.param;
    auto it = seen.find(key);
    if (it != seen.end() && !sp.preConverted) {
      // Measured (fit residual -142 dB): a repeated connection is made again, so it counts once per
      // repeat. The bridge keeps ONE weight node per source/target/parameter (the later weight
      // replaces the earlier) and lists it once per repeat.
      edges[it->second].weight = w;
      edgeCount[it->second]++;
      continue;
    }
    seen[key] = (int)edges.size();
    edges.push_back({si->second, tgt, e.param, w});
    edgeCount.push_back(1);
  }
  for (size_t i = 0; i < edges.size(); i++) edges[i].weight *= edgeCount[i];

  // 2. reachability from the output, over audio and parameter edges alike
  std::vector<char> reach(N + 1, 0);
  {
    std::vector<int> stack{OUT};
    reach[OUT] = 1;
    while (!stack.empty()) {
      const int t = stack.back();
      stack.pop_back();
      for (const auto& e : edges)
        if (e.tgt == t && !reach[e.src]) { reach[e.src] = 1; stack.push_back(e.src); }
    }
  }

  // 3. nodes
  plan.nodes.assign(N + 1, Node());
  plan.output = OUT;
  plan.nodes[OUT].kind = K_OUTPUT;
  for (int i = 0; i < N; i++) {
    const SrcNode& sn = sp.nodes[i];
    Node& n = plan.nodes[i];
    auto num = [&](const char* k, double def) { auto it = sn.num.find(k); return it == sn.num.end() ? def : it->second; };
    if (!reach[i]) { n.kind = K_SILENT; continue; }
    if (sn.type == "gain") {
      n.kind = K_GAIN;
      n.p[0].base = num("gain", 1.0);
    } else if (sn.type == "biquadFilter") {
      n.kind = K_BIQUAD;
      n.sub = filtTypeOf(sn.choice);
      n.state = plan.numBiquads++;
      n.p[0].base = num("frequency", 350.0); n.p[0].lo = 0; n.p[0].hi = nyquist;
      n.p[1].base = num("detune", 0.0); n.p[1].lo = -153600; n.p[1].hi = 153600;
      n.p[2].base = num("Q", 1.0);
      n.p[3].base = num("gain", 0.0);
    } else if (sn.type == "oscillator") {
      n.kind = K_OSC;
      n.sub = oscTypeOf(sn.choice);
      n.state = plan.numOscs++;
      n.p[0].base = num("frequency", 440.0); n.p[0].lo = -nyquist; n.p[0].hi = nyquist;
      n.p[1].base = num("detune", 0.0); n.p[1].lo = -153600; n.p[1].hi = 153600;
      bool audioOut = false;
      if (sp.preConverted) audioOut = std::find(sp.transposedOscIds.begin(), sp.transposedOscIds.end(), sn.id) != sp.transposedOscIds.end();
      else for (const auto& e : edges) if (e.src == i && e.param.empty()) audioOut = true;
      n.p[0].followsKey = audioOut;
    } else if (sn.type == "bufferSource") {
      n.kind = K_BUFSRC;
    } else if (sn.type == "wavetable") {
      n.kind = K_WAVETABLE;
    } else if (sn.type == "additive") {
      n.kind = K_ADDITIVE;
    } else if (sn.type == "waveShaper") {
      n.kind = K_SHAPER;
      n.curve = sn.curve;
    } else if (sn.type == "delay") {
      // a plain DelayNode is created with the default maximum of 1 s, so longer genome values clamp to it
      n.kind = K_DELAY;
      n.p[0].base = num("delayTime", 0.0); n.p[0].lo = 0; n.p[0].hi = 1.0;
    } else if (sn.type == "feedbackDelay") {
      // input -> pass-through -> output, and pass-through -> delay (max 5 s) -> gain(ratio) -> pass-through.
      // The delay sits in a loop, so it is at least one render quantum (128 frames).
      // MEASURED 2026-11-11: in the production renderer this node outputs nothing at all (probe of
      // genome ...233D4096T4CA, 1 s and 4 s: reference rms 0). So it is silent here too. The echo
      // itself is implemented below (K_FBDELAY) and switched off by this flag, for the day the
      // renderer is changed to make it sound.
      const bool feedbackDelaySoundsInRenderer = false;
      n.kind = feedbackDelaySoundsInRenderer ? K_FBDELAY : K_SILENT;
      n.p[0].base = num("delayTime", 0.0); n.p[0].lo = 128.0 / sampleRate; n.p[0].hi = 5.0;
      n.feedback = num("feedbackRatio", 0.2);
    } else if (sn.type == "dynamicsCompressor") {
      n.kind = K_COMP;
      n.state = plan.numComps++;
      n.p[0].base = num("threshold", -24.0); n.p[0].lo = -100; n.p[0].hi = 0;
      n.p[1].base = num("knee", 30.0); n.p[1].lo = 0; n.p[1].hi = 40;
      n.p[2].base = num("ratio", 12.0); n.p[2].lo = 1; n.p[2].hi = 20;
      n.p[3].base = num("attack", 0.003); n.p[3].lo = 0; n.p[3].hi = 1;
      n.p[4].base = num("release", 0.25); n.p[4].lo = 0; n.p[4].hi = 1;
    } else if (sn.type == "convolver") {
      n.kind = K_CONV;   // without a buffer connection it passes its input through, as the crate does
    } else if (sn.type == "faust" && !faustParamsOf(sn.tpl, (int)sampleRate).unknown) {
      // a Faust template (src/faust_nodes.h). Its sliders are AudioParams in the renderer: the stored
      // value where the genome has one, the template's default otherwise, clamped to the slider's range.
      const FaustMeta& fm = faustParamsOf(sn.tpl, (int)sampleRate);
      n.kind = K_FAUST;
      n.tpl = sn.tpl;
      n.state = (int)plan.faustTpl.size();
      plan.faustTpl.push_back(sn.tpl);
      for (const auto& pi : fm.params) {
        Param q;
        auto it = sn.fparams.find(pi.path);
        q.base = it == sn.fparams.end() ? pi.init : it->second;
        q.lo = pi.lo; q.hi = pi.hi;
        n.fp.push_back(q);
        n.fpath.push_back(pi.path);
      }
    } else if (sn.type == "faust") {
      n.kind = K_SILENT;   // a template this build does not have: the renderer also leaves such a node silent
      plan.unsupported.push_back("faust:" + sn.tpl);
    } else {
      n.kind = K_PASS;
      if (std::find(plan.unsupported.begin(), plan.unsupported.end(), sn.type) == plan.unsupported.end())
        plan.unsupported.push_back(sn.type.empty() ? "unknown" : sn.type);
    }
  }
  auto paramIndex = [](Kind k, const std::string& p) -> int {
    if (k == K_GAIN) return p == "gain" ? 0 : -1;
    if (k == K_BIQUAD) return p == "frequency" ? 0 : p == "detune" ? 1 : p == "Q" ? 2 : p == "gain" ? 3 : -1;
    if (k == K_OSC) return p == "frequency" ? 0 : p == "detune" ? 1 : -1;
    if (k == K_DELAY) return p == "delayTime" ? 0 : -1;
    if (k == K_COMP) return p == "threshold" ? 0 : p == "knee" ? 1 : p == "ratio" ? 2 : p == "attack" ? 3 : p == "release" ? 4 : -1;
    return -1;
  };
  auto faustParam = [](Node& t, const std::string& p) -> Param* {
    if (t.kind != K_FAUST) return nullptr;
    for (size_t i = 0; i < t.fpath.size(); i++) if (t.fpath[i] == p) return &t.fp[i];
    return nullptr;
  };
  auto takesAudio = [](Kind k) { return k == K_FAUST || k == K_OUTPUT || k == K_GAIN || k == K_BIQUAD || k == K_BUFSRC || k == K_SHAPER || k == K_DELAY || k == K_FBDELAY || k == K_COMP || k == K_CONV || k == K_PASS; };
  for (const auto& e : edges) {
    if (!reach[e.src] || !reach[e.tgt]) continue;
    Node& t = plan.nodes[e.tgt];
    if (e.param.empty()) { if (takesAudio(t.kind)) t.ins.push_back({e.src, e.weight}); }
    else if (Param* fq = faustParam(t, e.param)) fq->mods.push_back({e.src, e.weight});
    else { const int pi = paramIndex(t.kind, e.param); if (pi >= 0) t.p[pi].mods.push_back({e.src, e.weight}); }
  }

  // 4. CPPN signals: one slot per (network output, role)
  struct PendingPartial { int node; int slot; SrcSigConn c; };
  std::vector<PendingPartial> partialBufs, partialEnvs;
  std::vector<int> partialCount(N, 0);
  for (size_t oi = 0; oi < sp.netOuts.size(); oi++) {
    const SrcNetOut& no = sp.netOuts[oi];
    int slotOf[3] = {-1, -1, -1};
    const SrcSigConn* firstPartial = nullptr;
    for (const auto& c : no.conns) {
      auto ti = ix.find(c.tgt);
      if (ti == ix.end() || !reach[ti->second]) continue;
      if (c.param == "partialBuffer" && !firstPartial) firstPartial = &c;
    }
    auto slot = [&](Role r) {
      if (slotOf[r] >= 0) return slotOf[r];
      Slot s;
      s.cppnOut = no.cppnOut;
      s.noise = no.noise;
      s.baseFreq = no.frequency;
      s.followsKey = r != R_OTHER;
      if (r == R_PARTIAL && firstPartial && firstPartial->partialNumber > 1) s.baseFreq = no.frequency * firstPartial->partialNumber;
      plan.slots.push_back(s);
      return slotOf[r] = (int)plan.slots.size() - 1;
    };
    for (const auto& c : no.conns) {
      auto ti = ix.find(c.tgt);
      if (ti == ix.end() || !reach[ti->second]) continue;
      const int ni = ti->second;
      Node& n = plan.nodes[ni];
      const double half = c.hasRange ? (c.hi - c.lo) / 2.0 : 1.0;
      const double mid = c.hasRange ? (c.hi + c.lo) / 2.0 : 0.0;
      if (c.param == "buffer") {
        const int s = slot(R_BUFFER);
        if (n.kind == K_WAVETABLE) n.waves.push_back({s, half, mid});
        else if (n.kind == K_BUFSRC || n.kind == K_CONV) n.buf = {s, half, mid};  // the last one wins
      } else if (c.param == "partialBuffer") {
        const int s = slot(R_PARTIAL);
        if (n.kind == K_ADDITIVE) { partialCount[ni]++; partialBufs.push_back({ni, s, c}); }
      } else {
        const int s = slot(R_OTHER);
        if (c.param == "partialGainEnvelope") { if (n.kind == K_ADDITIVE) partialEnvs.push_back({ni, s, c}); }
        else if (c.param == "mix") { if (n.kind == K_WAVETABLE) n.mix = {s, half, mid}; }
        else if (c.param == "curve") { if (n.kind == K_SHAPER) n.buf = {s, half, mid}; }
        else if (Param* fq = faustParam(n, c.param)) {
          fq->base = mid;
          fq->sigs.push_back({s, half, 0.0});
        } else {
          const int pi = paramIndex(n.kind, c.param);
          if (pi >= 0) {
            n.p[pi].base = mid;            // param.value = midpoint (0 without a range); the last connection wins
            n.p[pi].followsKey = false;
            n.p[pi].sigs.push_back({s, half, 0.0});
          }
        }
      }
    }
  }
  for (int i = 0; i < N; i++) {
    Node& n = plan.nodes[i];
    if (n.kind == K_WAVETABLE) {
      if (n.waves.empty()) n.kind = K_SILENT;
      else n.ch = 6;  // measured: the merger is always 6 channels (numberOfInputs is not honoured)
    } else if (n.kind == K_ADDITIVE) {
      if (partialCount[i] == 0) { n.kind = K_SILENT; continue; }
      n.partials.assign(partialCount[i], Partial());
      n.ch = 6;  // as for the wavetable: a 6-channel merger with everything in channel 0
      std::vector<double> weights;
      for (const auto& pe : partialEnvs) if (pe.node == i) weights.push_back(pe.c.weight != 0 ? pe.c.weight : 1.0);
      for (int k = 1; k < partialCount[i]; k++) n.partials[k].weight = k < (int)weights.size() ? weights[k] : 1.0;
      for (const auto& pb : partialBufs) {
        if (pb.node != i) continue;
        const int k = pb.c.partialNumber != 0 ? (int)pb.c.partialNumber : 1;
        if (k < 1 || k > partialCount[i]) continue;
        const double half = pb.c.hasRange ? (pb.c.hi - pb.c.lo) / 2.0 : 1.0, mid = pb.c.hasRange ? (pb.c.hi + pb.c.lo) / 2.0 : 0.0;
        n.partials[k - 1].bufs.push_back({pb.slot, half, mid});
      }
      for (const auto& pe : partialEnvs) {
        if (pe.node != i) continue;
        const int k = pe.c.partialNumber != 0 ? (int)pe.c.partialNumber : 1;
        if (k < 1 || k > partialCount[i]) continue;
        const double lo = pe.c.hasRange ? pe.c.lo : 0.0, hi = pe.c.hasRange ? pe.c.hi : 1.0;
        n.partials[k - 1].envBase = (lo + hi) / 2.0;
        n.partials[k - 1].envs.push_back({pe.slot, (hi - lo) / 2.0, 0.0});
      }
    }
  }

  // 5. evaluation order: sources first (a cycle is cut where it closes)
  {
    std::vector<char> state(N + 1, 0);
    std::vector<int>& order = plan.order;
    struct Frame { int node; size_t next; };
    std::vector<Frame> stack;
    auto deps = [&](int node, std::vector<int>& out) {
      out.clear();
      const Node& n = plan.nodes[node];
      for (const auto& a : n.ins) out.push_back(a.node);
      for (const auto& p : n.p) for (const auto& m : p.mods) out.push_back(m.node);
    };
    std::vector<std::vector<int>> depList(N + 1);
    for (int i = 0; i <= N; i++) deps(i, depList[i]);
    stack.push_back({OUT, 0});
    state[OUT] = 1;
    while (!stack.empty()) {
      Frame& f = stack.back();
      if (f.next < depList[f.node].size()) {
        const int d = depList[f.node][f.next++];
        if (state[d] == 0) { state[d] = 1; stack.push_back({d, 0}); }
      } else {
        state[f.node] = 2;
        order.push_back(f.node);
        stack.pop_back();
      }
    }
  }

  // 5b. a feedback loop with no delay in it is muted by Web Audio: every node on such a loop is silent
  {
    std::vector<std::vector<int>> dep(N + 1);
    for (int i = 0; i <= N; i++) {
      for (const auto& a : plan.nodes[i].ins) dep[i].push_back(a.node);
      for (const auto& p : plan.nodes[i].p) for (const auto& m : p.mods) dep[i].push_back(m.node);
      for (const auto& p : plan.nodes[i].fp) for (const auto& m : p.mods) dep[i].push_back(m.node);
    }
    // reach[a][b]: b can be reached from a along dependencies (graphs are small)
    std::vector<std::vector<char>> r(N + 1, std::vector<char>(N + 1, 0));
    for (int a = 0; a <= N; a++) {
      std::vector<int> st(dep[a].begin(), dep[a].end());
      while (!st.empty()) {
        const int b = st.back(); st.pop_back();
        if (r[a][b]) continue;
        r[a][b] = 1;
        for (int c : dep[b]) if (!r[a][c]) st.push_back(c);
      }
    }
    for (int a = 0; a < N; a++)
      if (r[a][a]) {
        // ...unless the loop passes through a delay node (Web Audio allows that; not implemented
        // here: the nodes are left running with a one-sample delay, an approximation)
        bool viaDelay = false;
        for (int b = 0; b < N; b++) if (r[a][b] && r[b][a] && (plan.nodes[b].kind == K_DELAY || plan.nodes[b].kind == K_FBDELAY)) viaDelay = true;
        if (viaDelay) continue;
        Node& n = plan.nodes[a]; n.kind = K_SILENT; n.ins.clear(); for (auto& p : n.p) p.mods.clear(); for (auto& p : n.fp) p.mods.clear();
      }
  }

  // 6. channel counts ('max' of the audio inputs; sources are mono; the mergers set their own)
  for (int pass = 0; pass < 2; pass++)
    for (int idx : plan.order) {
      Node& n = plan.nodes[idx];
      if (n.kind == K_GAIN || n.kind == K_BIQUAD || n.kind == K_BUFSRC || n.kind == K_SHAPER || n.kind == K_DELAY || n.kind == K_FBDELAY || n.kind == K_PASS) {
        int c = 1;
        for (const auto& a : n.ins) c = std::max(c, plan.nodes[a.node].ch);
        n.ch = c;
      } else if (n.kind == K_COMP || n.kind == K_CONV) {
        int c = 1;
        for (const auto& a : n.ins) c = std::max(c, plan.nodes[a.node].ch);
        n.ch = std::min(c, 2);  // clamped-max 2
      }
    }

  for (auto& n : plan.nodes)
    if (n.kind == K_CONV && n.buf.slot >= 0) { n.state = plan.numConvs++; plan.convChans.push_back(std::min(n.ch, 2)); }

  // 6b. delay lines: sized for the longest delay the node can reach
  for (auto& n : plan.nodes)
    if (n.kind == K_DELAY || n.kind == K_FBDELAY) {
      const double maxSec = n.p[0].dynamic() ? n.p[0].hi : std::min(std::max(n.p[0].base, n.p[0].lo), n.p[0].hi);
      n.delayLen = (int)std::ceil(maxSec * sampleRate) + 4;
      n.delayIx = plan.numDelays++;
      plan.delayLens.push_back(n.delayLen);
      plan.delayChans.push_back(std::min(n.ch, kMaxCh));
    }

  for (const auto& a : plan.nodes[OUT].ins) if (plan.nodes[a.node].ch > 1) plan.limiterChans = 2;

  // 7. static filter coefficients, and CPPN evaluation groups
  for (auto& n : plan.nodes)
    if (n.kind == K_BIQUAD && !n.p[0].dynamic() && !n.p[1].dynamic() && !n.p[2].dynamic() && !n.p[3].dynamic()) {
      const double f = (double)(float)n.p[0].base, d = (double)(float)n.p[1].base;
      biquadCoefs(n.sub, sampleRate, d != 0 ? (double)((float)f * std::exp2((float)d / 1200.0f)) : f, (double)(float)n.p[3].base, (double)(float)n.p[2].base, n.coef);
    }
  for (size_t si = 0; si < plan.slots.size(); si++) {
    Slot& s = plan.slots[si];
    if (s.noise) continue;
    int g = -1;
    for (size_t gi = 0; gi < plan.groups.size(); gi++)
      if (plan.groups[gi].baseFreq == s.baseFreq && plan.groups[gi].followsKey == s.followsKey) { g = (int)gi; break; }
    if (g < 0) { plan.groups.push_back({s.baseFreq, s.followsKey, {}}); g = (int)plan.groups.size() - 1; }
    plan.groups[g].slots.push_back((int)si);
    s.group = g;
  }
}


// The render server's output stage when it runs with USE_COMPRESSOR (it does in production): a
// DynamicsCompressorNode with threshold 0 dB, knee 0, ratio 20, attack 1 ms, release 10 ms between
// the graph and the destination. It works on the raw sum, offset included, and adds 384 frames of
// delay. Same arithmetic as K_COMP (the crate's, in f32); mono, one per voice.
// In the renderer every output-connected node is re-plugged into the compressor, whose input is mixed
// to at most two channels (speaker rules), and the compressor's output is then folded to the mono
// destination. So a 6-channel wavetable/additive signal reaches the output at 0.5 through this stage
// (0.7071 without it), the detector sees max(|L|, |R|), and a repeated output connection counts once.
inline double outputLimiter(CompState& cs, double l, double r, int chans, long n, double sr) {
  const float x0 = (float)l, x1 = (float)r;
  if (n % 128 == 0) {
    cs.attackTau = std::exp(-1.f / (0.001f * (float)sr));
    cs.releaseTau = std::exp(-1.f / (0.01f * (float)sr));
  }
  float mx = std::fabs(x0);
  if (chans > 1 && std::fabs(x1) > mx) mx = std::fabs(x1);
  const float db = mx == 0.f ? -1000.f : 20.f * std::log10(mx);
  const float att = db <= 0.f ? db : db / 20.f;   // threshold 0, hard knee, ratio 20
  const float want = db - att;
  const float det = want > cs.prev ? cs.attackTau * cs.prev + (1.f - cs.attackTau) * want
                                   : cs.releaseTau * cs.prev + (1.f - cs.releaseTau) * want;
  cs.prev = det;
  const float g = std::pow(10.f, -det / 20.f);    // makeup gain is 0 dB for these settings
  const long w = n % 384;
  const float d0 = n >= 384 ? cs.line[0][w] : 0.f, d1 = n >= 384 ? cs.line[1][w] : 0.f;
  cs.line[0][w] = x0; cs.line[1][w] = x1;
  return chans > 1 ? (double)(0.5f * (d0 * g + d1 * g)) : (double)(d0 * g);
}

// ── per-sample render ─────────────────────────────────────────────────────────────────────────────

inline const float* sineTable() {
  static float table[2048];
  static bool init = false;
  if (!init) {
    for (int x = 0; x < 2048; x++) table[x] = std::sin((float)x * 2.0f * (float)M_PI * (1.0f / 2048.0f));
    init = true;
  }
  return table;
}

inline double polyBlep(double t, double dt) {
  if (t < dt) { t /= dt; return t + t - t * t - 1.0; }
  if (t > 1.0 - dt) { t = (t - 1.0) / dt; return t * t + t + t + 1.0; }
  return 0.0;
}

// add src (srcCh channels) * w into dst (dstCh channels), Web Audio 'speakers' up-mix; dstCh >= srcCh
inline void mixIn(double* dst, int dstCh, const double* src, int srcCh, double w) {
  if (srcCh == dstCh) { for (int i = 0; i < std::min(dstCh, kMaxCh); i++) dst[i] += src[i] * w; return; }
  if (srcCh <= 6 && dstCh <= 6) {
    if (srcCh == 1 && (dstCh == 2 || dstCh == 4)) { dst[0] += src[0] * w; dst[1] += src[0] * w; return; }
    if (srcCh == 1 && dstCh == 6) { dst[2] += src[0] * w; return; }
    if (srcCh == 2 && (dstCh == 4 || dstCh == 6)) { dst[0] += src[0] * w; dst[1] += src[1] * w; return; }
    if (srcCh == 4 && dstCh == 5) { dst[0] += src[0] * w; dst[1] += src[1] * w; dst[3] += src[2] * w; dst[4] += src[3] * w; return; }
    if (srcCh == 4 && dstCh == 6) { dst[0] += src[0] * w; dst[1] += src[1] * w; dst[4] += src[2] * w; dst[5] += src[3] * w; return; }
  }
  for (int i = 0; i < std::min(std::min(srcCh, dstCh), kMaxCh); i++) dst[i] += src[i] * w;  // discrete
}
inline double downMixToMono(const double* s, int ch) {
  if (ch == 2) return 0.5 * (s[0] + s[1]);
  if (ch == 4) return 0.25 * (s[0] + s[1] + s[2] + s[3]);
  if (ch == 6) return 0.7071067811865476 * (s[0] + s[1]) + s[2] + 0.5 * (s[4] + s[5]);
  return s[0];
}

struct RenderCtx {
  long n, total;          // sample index within the note, note length in samples
  double keyRatio;        // 2^((key - root + pitch)/12)
  double velocity, x, y;
  bool limiter = false;   // the render server's output limiter is on: the output node also prepares its input
};

inline double paramValue(const Param& p, const VoiceState& vs, const Plan& plan, double keyRatio) {
  double v = p.followsKey ? p.base * keyRatio : p.base;
  for (const auto& s : p.sigs) v += s.half * vs.sig[s.slot];
  for (const auto& m : p.mods) v += vs.out[(size_t)m.node * kMaxCh] * m.weight;  // parameters read channel 0 (discrete)
  return v < p.lo ? p.lo : (v > p.hi ? p.hi : v);
}

// inputs of a node whose input is mixed to at most two channels (compressor, convolver)
inline void mixToTwo(const Plan& plan, const VoiceState& vs, const Node& n, float x[2]) {
  x[0] = x[1] = 0;
  for (const auto& a : n.ins) {
    const double* sv = &vs.out[(size_t)a.node * kMaxCh];
    const int sc = plan.nodes[a.node].ch;
    double l, r;
    if (sc == 1) { l = r = sv[0]; }
    else if (sc == 2) { l = sv[0]; r = sv[1]; }
    else if (sc == 4) { l = 0.5 * (sv[0] + sv[2]); r = 0.5 * (sv[1] + sv[3]); }
    else if (sc == 6) { l = sv[0] + 0.7071067811865476 * (sv[2] + sv[4]); r = sv[1] + 0.7071067811865476 * (sv[2] + sv[5]); }
    else { l = sv[0]; r = sc > 1 ? sv[1] : 0; }
    if (n.ch == 1) x[0] += (float)(sv[0] * a.weight);
    else { x[0] += (float)(l * a.weight); x[1] += (float)(r * a.weight); }
  }
}

// The convolver's 'normalize' scale needs the power of the WHOLE response, i.e. the CPPN signal over
// the whole note at this key. Called at note-on; costs one CPPN evaluation per sample of the note
// for each convolver, so the engine caches the result per key / length / velocity.
// Resumable form, so the pre-rendered playing mode can spread it over audio blocks: `node`, `i` and
// `sum` carry the position between calls; at most `budget` CPPN evaluations are done (counted in
// `spent`). Returns true when every convolver has its scale. The sums run in the same order as one
// uninterrupted pass, so the result is identical.
inline bool prepareConvolversStep(const Plan& plan, cppn::Cppn& net, VoiceState& vs, const RenderCtx& rc, size_t& node, long& i,
                                  double& sum, long budget, long& spent) {
  for (; node < plan.nodes.size(); node++, i = 0, sum = 0) {
    const Node& n = plan.nodes[node];
    if (n.kind != K_CONV || n.buf.slot < 0) continue;
    const Slot& sl = plan.slots[n.buf.slot];
    const double f = sl.followsKey ? sl.baseFreq * rc.keyRatio : sl.baseFreq;
    if (sl.noise) sum = (n.buf.half * n.buf.half / 3.0 + n.buf.mid * n.buf.mid) * (double)rc.total;
    else for (; i < rc.total; i++) {
      if (spent >= budget) return false;
      spent++;
      net.evaluate(i, rc.total, f, plan.sampleRate, rc.velocity, rc.x, rc.y);
      const float v = (float)(n.buf.half * net.output(sl.cppnOut) + n.buf.mid);
      sum += (double)(v * v);
    }
    float power = (float)std::sqrt(sum / (double)rc.total);
    if (!std::isfinite(power) || power < 0.000125f) power = 0.000125f;
    vs.conv[n.state].scale = 1.f / power * 0.00125f * (44100.f / (float)plan.sampleRate);
  }
  return true;
}
inline void prepareConvolvers(const Plan& plan, cppn::Cppn& net, VoiceState& vs, const RenderCtx& rc) {
  size_t node = 0; long i = 0, spent = 0; double sum = 0;
  prepareConvolversStep(plan, net, vs, rc, node, i, sum, 0x7fffffffL, spent);
}

// one sample of one voice; returns the mono output of the graph
inline double renderSample(const Plan& plan, cppn::Cppn& net, VoiceState& vs, const RenderCtx& rc) {
  const double sr = plan.sampleRate;
  // 1. CPPN signals: one evaluation per distinct frequency
  for (const auto& g : plan.groups) {
    net.evaluate(rc.n, rc.total, g.followsKey ? g.baseFreq * rc.keyRatio : g.baseFreq, sr, rc.velocity, rc.x, rc.y);
    for (int si : g.slots) vs.sig[si] = rc.n < rc.total ? net.output(plan.slots[si].cppnOut) : 0.0;
  }
  for (size_t si = 0; si < plan.slots.size(); si++)
    if (plan.slots[si].noise) {
      uint32_t r = vs.rng[si];
      r ^= r << 13; r ^= r >> 17; r ^= r << 5;
      vs.rng[si] = r;
      vs.sig[si] = rc.n < rc.total ? (double)r / 2147483648.0 - 1.0 : 0.0;
    }
  const bool playing = rc.n < rc.total;  // the renderer's buffer sources stop at the note length
  // 2. nodes
  for (int idx : plan.order) {
    const Node& n = plan.nodes[idx];
    double* out = &vs.out[(size_t)idx * kMaxCh];
    double in[kMaxCh] = {0, 0, 0, 0, 0, 0};
    for (const auto& a : n.ins) mixIn(in, n.ch, &vs.out[(size_t)a.node * kMaxCh], plan.nodes[a.node].ch, a.weight);
    const int chans = std::min(n.ch, kMaxCh);
    switch (n.kind) {
      case K_OUTPUT: {
        double acc = 0;
        for (const auto& a : n.ins) acc += downMixToMono(&vs.out[(size_t)a.node * kMaxCh], plan.nodes[a.node].ch) * a.weight;
        out[0] = acc;
        if (rc.limiter) {  // the limiter's own input: each source once, mixed to one or two channels
          double l = 0, r = 0;
          for (const auto& a : n.ins) {
            const double* sv = &vs.out[(size_t)a.node * kMaxCh];
            const int sc = plan.nodes[a.node].ch;
            if (plan.limiterChans == 1) { l += sv[0]; continue; }
            if (sc == 1) { l += sv[0]; r += sv[0]; }
            else if (sc == 2) { l += sv[0]; r += sv[1]; }
            else if (sc == 4) { l += 0.5 * (sv[0] + sv[2]); r += 0.5 * (sv[1] + sv[3]); }
            else if (sc == 6) { l += sv[0] + 0.7071067811865476 * (sv[2] + sv[4]); r += sv[1] + 0.7071067811865476 * (sv[2] + sv[5]); }
            else { l += sv[0]; r += sv[1]; }
          }
          out[1] = l; out[2] = r;
        }
        break;
      }
      case K_GAIN: {
        const double g = paramValue(n.p[0], vs, plan, rc.keyRatio);
        for (int c = 0; c < chans; c++) out[c] = in[c] * g;
        break;
      }
      case K_PASS:
        for (int c = 0; c < chans; c++) out[c] = in[c];
        break;
      case K_FAUST: {
        fdsp::Instance& fi = vs.faust[n.state];
        if (!fi.dsp) { out[0] = 0.0; break; }
        if ((rc.n & 127) == 0) {   // the worklet reads its parameters once per render quantum
          for (size_t k = 0; k < n.fp.size() && k < fi.zones.size(); k++) *fi.zones[k] = (float)paramValue(n.fp[k], vs, plan, rc.keyRatio);
          fi.dsp->control();
        }
        // the worklet node has one input channel ('explicit', 'speakers'): every source is mixed down to mono
        double acc = 0;
        for (const auto& a : n.ins) acc += downMixToMono(&vs.out[(size_t)a.node * kMaxCh], plan.nodes[a.node].ch) * a.weight;
        float x = (float)acc, y = 0.f;
        fi.dsp->frame(&x, &y);
        if (!std::isfinite(y)) { fi.dsp->instanceClear(); y = 0.f; }   // a template that blows up is silence, never NaN
        out[0] = (double)y;
        break;
      }
      case K_CONV: {
        if (n.buf.slot < 0) { for (int c = 0; c < chans; c++) out[c] = in[c]; break; }
        ConvState& cv = vs.conv[n.state];
        float xin[2];
        mixToTwo(plan, vs, n, xin);
        const long nn = rc.n;
        if (nn >= cv.cap) { for (int c = 0; c < chans; c++) out[c] = 0.0; break; }   // beyond the kept length
        cv.h[nn] = playing ? (float)(n.buf.half * vs.sig[n.buf.slot] + n.buf.mid) : 0.f;
        for (int c = 0; c < chans; c++) cv.x[c][nn] = xin[c];
        const long off = nn % kConvB, t = nn / kConvB;
        if (off == 0 && t >= 1) {
          // block t-1 of the input and of the response are complete: store their spectra, then
          // sum the products of every pair (a, j) with a + j = t, a >= 1, j >= 1
          static thread_local float re[kConvN], im[kConvN];
          const int bins = kConvB + 1;
          auto store = [&](const float* src, float* dst) {
            for (int i = 0; i < kConvB; i++) { re[i] = src[i]; im[i] = 0; }
            for (int i = kConvB; i < kConvN; i++) { re[i] = 0; im[i] = 0; }
            convFft(re, im, false);
            for (int i = 0; i < bins; i++) { dst[2 * i] = re[i]; dst[2 * i + 1] = im[i]; }
          };
          store(&cv.h[(t - 1) * kConvB], &cv.H[(size_t)(t - 1) * bins * 2]);
          for (int c = 0; c < chans; c++) {
            store(&cv.x[c][(t - 1) * kConvB], &cv.X[c][(size_t)(t - 1) * bins * 2]);
            float* tail = cv.tail[c];
            float* ola = cv.ola[c];
            if (t >= 2) {
              static thread_local float sr2[kConvB + 1], si2[kConvB + 1];
              for (int i = 0; i < bins; i++) { sr2[i] = 0; si2[i] = 0; }
              for (long a = 1; a <= t - 1; a++) {
                const float* xa = &cv.X[c][(size_t)a * bins * 2];
                const float* hj = &cv.H[(size_t)(t - a) * bins * 2];
                for (int i = 0; i < bins; i++) {
                  sr2[i] += xa[2 * i] * hj[2 * i] - xa[2 * i + 1] * hj[2 * i + 1];
                  si2[i] += xa[2 * i] * hj[2 * i + 1] + xa[2 * i + 1] * hj[2 * i];
                }
              }
              for (int i = 0; i < bins; i++) { re[i] = sr2[i]; im[i] = si2[i]; }
              for (int i = 1; i < kConvB; i++) { re[kConvN - i] = sr2[i]; im[kConvN - i] = -si2[i]; }
              convFft(re, im, true);
              for (int i = 0; i < kConvB; i++) { ola[i] = re[i] + tail[i]; tail[i] = re[i + kConvB]; }
            } else {
              for (int i = 0; i < kConvB; i++) { ola[i] = tail[i]; tail[i] = 0; }
            }
          }
        }
        for (int c = 0; c < chans; c++) {
          const float* x = cv.x[c].data();
          const float* h = cv.h.data();
          float acc = (t >= 1) ? cv.ola[c][off] : 0.f;
          const long k1 = std::min<long>(kConvB - 1, nn);
          for (long k = 0; k <= k1; k++) acc += h[k] * x[nn - k];           // first block of the response
          const long i1 = std::min<long>(kConvB - 1, nn - kConvB);
          for (long i = 0; i <= i1; i++) acc += x[i] * h[nn - i];           // first block of the input
          out[c] = (double)(acc * cv.scale);
        }
        break;
      }
      case K_COMP: {
        // web-audio-api-rs v1.6.0 dynamics_compressor.rs, in f32 as there. Its input is mixed to at
        // most two channels, so the generic `in` (mixed for n.ch) is rebuilt here from the sources.
        float x[2] = {0, 0};
        for (const auto& a : n.ins) {
          const double* sv = &vs.out[(size_t)a.node * kMaxCh];
          const int sc = plan.nodes[a.node].ch;
          double l, r;
          if (sc == 1) { l = r = sv[0]; }
          else if (sc == 2) { l = sv[0]; r = sv[1]; }
          else if (sc == 4) { l = 0.5 * (sv[0] + sv[2]); r = 0.5 * (sv[1] + sv[3]); }
          else if (sc == 6) { l = sv[0] + 0.7071067811865476 * (sv[2] + sv[4]); r = sv[1] + 0.7071067811865476 * (sv[2] + sv[5]); }
          else { l = sv[0]; r = sc > 1 ? sv[1] : 0; }
          if (n.ch == 1) x[0] += (float)(sv[0] * a.weight);
          else { x[0] += (float)(l * a.weight); x[1] += (float)(r * a.weight); }
        }
        CompState& cs = vs.comp[n.state];
        const long q = rc.n % 128;
        if (q == 0) {  // parameters are read once per render quantum
          float threshold = (float)paramValue(n.p[0], vs, plan, rc.keyRatio);
          const float knee = (float)paramValue(n.p[1], vs, plan, rc.keyRatio);
          const float ratio = (float)paramValue(n.p[2], vs, plan, rc.keyRatio);
          const float attack = (float)paramValue(n.p[3], vs, plan, rc.keyRatio), release = (float)paramValue(n.p[4], vs, plan, rc.keyRatio);
          if (knee > 0.f) threshold += knee / 2.f;
          cs.threshold = threshold; cs.halfKnee = knee / 2.f; cs.ratio = ratio;
          cs.kneePartial = (1.f / ratio - 1.f) / (2.f * knee);
          cs.attackTau = std::exp(-1.f / (attack * (float)sr));
          cs.releaseTau = std::exp(-1.f / (release * (float)sr));
          const float fullRangeGain = threshold + (-threshold / ratio);
          const float fullRangeMakeup = 1.f / std::pow(10.f, fullRangeGain / 20.f);
          const float mk = std::pow(fullRangeMakeup, 0.6f);
          cs.makeup = mk == 0.f ? -1000.f : 20.f * std::log10(mk);
        }
        float mx = std::fabs(x[0]);
        if (n.ch > 1 && std::fabs(x[1]) > mx) mx = std::fabs(x[1]);
        const float db = mx == 0.f ? -1000.f : 20.f * std::log10(mx);
        float att;
        if (db <= cs.threshold - cs.halfKnee) att = db;
        else if (db <= cs.threshold + cs.halfKnee) { const float d = db - cs.threshold + cs.halfKnee; att = db + d * d * cs.kneePartial; }
        else att = cs.threshold + (db - cs.threshold) / cs.ratio;
        const float want = db - att;
        const float det = want > cs.prev ? cs.attackTau * cs.prev + (1.f - cs.attackTau) * want
                                         : cs.releaseTau * cs.prev + (1.f - cs.releaseTau) * want;
        cs.prev = det;
        const float g = std::pow(10.f, (-det + cs.makeup) / 20.f);
        const long w = rc.n % 384;   // the sample written 384 frames ago sits where this one goes
        for (int c = 0; c < chans; c++) { const float delayed = rc.n >= 384 ? cs.line[c][w] : 0.f; cs.line[c][w] = x[c]; out[c] = (double)(delayed * g); }
        break;
      }
      case K_DELAY:
      case K_FBDELAY: {
        // as the crate's DelayReader: position = n - delay*sampleRate, linear interpolation between
        // the two stored frames around it; frames before the note started are silence
        const double dsec = (double)(float)paramValue(n.p[0], vs, plan, rc.keyRatio);
        const double pos = (double)rc.n - dsec * sr;
        const double pf = std::floor(pos);
        const float k = (float)(pos - pf);
        const long p0 = (long)pf, len = n.delayLen;
        float* lines = vs.delay[n.delayIx].data();
        for (int c = 0; c < chans; c++) {
          float* line = lines + (size_t)c * len;
          const float x = (float)in[c];
          if (n.kind == K_DELAY) line[rc.n % len] = x;  // written first: a zero delay reads the current frame
          auto at = [&](long i) -> float { return (i < 0 || i > rc.n || rc.n - i >= len) ? 0.0f : line[i % len]; };
          const float d = (1.0f - k) * at(p0) + k * at(p0 + 1);
          if (n.kind == K_DELAY) out[c] = (double)d;
          else {
            const float y = x + (float)n.feedback * d;   // pass-through = input + ratio * delayed pass-through
            line[rc.n % len] = y;
            out[c] = (double)y;
          }
        }
        break;
      }
      case K_BUFSRC: {
        const double b = (n.buf.slot >= 0 && playing) ? n.buf.half * vs.sig[n.buf.slot] + n.buf.mid : 0.0;
        if (n.ch == 1) out[0] = in[0] + b;
        else { for (int c = 0; c < chans; c++) out[c] = in[c]; double m[kMaxCh] = {0, 0, 0, 0, 0, 0}; mixIn(m, n.ch, &b, 1, 1.0); for (int c = 0; c < chans; c++) out[c] += m[c]; }
        break;
      }
      case K_BIQUAD: {
        double cf[5];
        const double* c5 = n.coef;
        if (n.p[0].dynamic() || n.p[1].dynamic() || n.p[2].dynamic() || n.p[3].dynamic()) {
          const float f = (float)paramValue(n.p[0], vs, plan, rc.keyRatio), d = (float)paramValue(n.p[1], vs, plan, rc.keyRatio);
          const float q = (float)paramValue(n.p[2], vs, plan, rc.keyRatio), g = (float)paramValue(n.p[3], vs, plan, rc.keyRatio);
          biquadCoefs(n.sub, sr, (double)(d != 0 ? f * std::exp2(d / 1200.0f) : f), (double)g, (double)q, cf);
          c5 = cf;
        }
        for (int c = 0; c < chans; c++) {
          double* st = &vs.bq[((size_t)n.state * kMaxCh + c) * 4];
          const double x = (double)(float)in[c];
          double y = c5[0] * x + c5[1] * st[0] + c5[2] * st[1] - c5[3] * st[2] - c5[4] * st[3];
          if (!std::isnormal(y)) y = 0.0;
          st[1] = st[0]; st[0] = x; st[3] = st[2]; st[2] = y;
          out[c] = (double)(float)y;
        }
        break;
      }
      case K_OSC: {
        const double f = (double)(float)paramValue(n.p[0], vs, plan, rc.keyRatio), d = (double)(float)paramValue(n.p[1], vs, plan, rc.keyRatio);
        const double computed = f * std::exp2(d / 1200.0), incr = computed / sr;
        double& phase = vs.phase[n.state];
        if (std::fabs(computed) >= sr / 2.0) {
          out[0] = 0.0;
          phase = phase + incr - std::floor(phase + incr);
          break;
        }
        double s;
        switch (n.sub) {
          case OSC_SAW: {
            double p = phase + 0.5; if (p >= 1.0) p -= 1.0;
            s = 2.0 * p - 1.0 - polyBlep(p, incr);
            break;
          }
          case OSC_SQUARE: {
            s = phase < 0.5 ? 1.0 : -1.0;
            s += polyBlep(phase, incr);
            double p = phase + 0.5; if (p >= 1.0) p -= 1.0;
            s -= polyBlep(p, incr);
            break;
          }
          case OSC_TRI: {
            s = -4.0 * phase + 2.0;
            if (s > 1.0) s = 2.0 - s; else if (s < -1.0) s = -2.0 - s;
            break;
          }
          default: {
            const float* t = sineTable();
            const double pos = phase * 2048.0, fl = std::floor(pos);
            int i0 = (int)fl, i1 = i0 + 1;
            if (i0 >= 2048) i0 = 2047;
            if (i1 >= 2048) i1 = 0;
            const float k = (float)(pos - fl);
            s = (double)(t[i0] * (1.0f - k) + t[i1] * k);
          }
        }
        out[0] = (double)(float)s;
        phase += incr;
        if (phase >= 1.0) phase -= 1.0; else if (phase < 0.0) phase += 1.0;
        break;
      }
      case K_WAVETABLE: {
        double acc = 0;
        const int W = (int)n.waves.size();
        if (playing) {
          if (n.mix.slot < 0) {
            for (const auto& w : n.waves) acc += w.half * vs.sig[w.slot] + w.mid;
          } else {
            const double m = n.mix.half * vs.sig[n.mix.slot] + n.mix.mid;
            const double fraction = 2.0 / W, halfFraction = fraction / 2.0;
            for (int i = 0; i < W; i++) {
              const double start0 = i * fraction - 1.0;
              const double start = start0 - (i ? halfFraction : 0.0), middle = start0 + halfFraction;
              const double end = start0 + fraction + ((i + 1) < W ? halfFraction : 0.0);
              if (m > start && m < end) {
                const double g = std::max(0.0, 1.0 - std::fabs(middle - m) / fraction);
                acc += (n.waves[i].half * vs.sig[n.waves[i].slot] + n.waves[i].mid) * (double)(float)g;
              }
            }
          }
        }
        out[0] = acc;  // channel 0 of the merger; the other channels stay silent
        for (int c = 1; c < chans; c++) out[c] = 0.0;
        break;
      }
      case K_ADDITIVE: {
        double acc = 0;
        if (playing)
          for (const auto& p : n.partials) {
            if (p.bufs.empty()) continue;
            double b = 0, e = p.envBase;
            for (const auto& s : p.bufs) b += s.half * vs.sig[s.slot] + s.mid;
            for (const auto& s : p.envs) e += s.half * vs.sig[s.slot];
            acc += b * e * p.weight;
          }
        out[0] = acc;
        for (int c = 1; c < chans; c++) out[c] = 0.0;
        break;
      }
      case K_SHAPER: {
        if (n.buf.slot >= 0) {
          // the curve is the whole-note CPPN signal: index it directly instead of storing it
          const Slot& sl = plan.slots[n.buf.slot];
          const double f = sl.followsKey ? sl.baseFreq * rc.keyRatio : sl.baseFreq;
          const double len = (double)rc.total;
          for (int c = 0; c < chans; c++) {
            const double v = (len - 1.0) / 2.0 * ((double)(float)in[c] + 1.0);
            double y;
            auto at = [&](long k) { net.evaluate(k, rc.total, f, sr, rc.velocity, rc.x, rc.y); return n.buf.half * net.output(sl.cppnOut) + n.buf.mid; };
            if (v <= 0) y = at(0);
            else if (v >= len - 1.0) y = at(rc.total - 1);
            else { const double k = std::floor(v), fr = v - k; y = (1.0 - fr) * at((long)k) + fr * at((long)k + 1); }
            out[c] = y;
          }
        } else if (!n.curve.empty()) {
          const double len = (double)n.curve.size();
          for (int c = 0; c < chans; c++) {
            const double v = (len - 1.0) / 2.0 * (in[c] + 1.0);
            if (v <= 0) out[c] = n.curve[0];
            else if (v >= len - 1.0) out[c] = n.curve[n.curve.size() - 1];
            else { const double k = std::floor(v), fr = v - k; out[c] = (1.0 - fr) * n.curve[(size_t)k] + fr * n.curve[(size_t)k + 1]; }
          }
        } else {
          for (int c = 0; c < chans; c++) out[c] = in[c];
        }
        break;
      }
      default:  // K_SILENT
        for (int c = 0; c < chans; c++) out[c] = 0.0;
    }
  }
  return plan.output >= 0 ? vs.out[(size_t)plan.output * kMaxCh] : 0.0;
}


// Microseconds per sample of one instance of each Faust template, measured on the calibration machine
// (see poc-cppn-wasm/cost-calibrate.mjs). Unknown templates count as a mid-weight one.
inline double faustCostWeight(const std::string& tpl) {
  // measured 2026-11-12, M1 Pro, Node 22 reference host (cost-calibrate-results.json)
  static const std::pair<const char*, double> kUs[] = {
      {"church-bell", 0.862}, {"compressor-mono", 0.104}, {"crybaby", 0.040}, {"cubic-nl", 0.037}, {"dattorro", 0.031},
      {"dattorro-default", 0.050}, {"djembe", 0.130}, {"ef-echo", 0.024}, {"ef-transpose", 0.037}, {"ef-wavefold", 0.023},
      {"english-bell", 0.885}, {"flanger", 0.063}, {"freeverb", 0.045}, {"french-bell", 0.875}, {"german-bell", 0.893},
      {"greyhole", 0.242}, {"ja-processor", 0.258}, {"jpverb", 13.423}, {"marimba", 0.372}, {"marimba-bar", 1.139},
      {"moog-ladder", 0.081}, {"phaser", 0.067}, {"pm-brass", 0.050}, {"pm-clarinet", 0.037}, {"pm-flute", 0.048},
      {"pm-formant-bp", 0.113}, {"pm-ks", 0.045}, {"pm-violin", 0.065}, {"raw", 0.012}, {"russian-bell", 0.880},
      {"springreverb", 0.166}, {"standard-bell", 0.931}, {"string-exciter", 0.038}, {"subtractive", 0.054}, {"svf", 0.035},
      {"tanh-aa", 0.063}, {"vital-rev", 0.513}, {"wavefolder", 0.043}, {"zita-rev", 0.242}};
  for (const auto& e : kUs) if (tpl == e.first) return e.second;
  return 0.2;
}

// ── cost of one voice-sample, for the pre-rendered playing mode ──────────────────────────────────
// The engine has no usable clock in our hosts (they give a virtual one), so whether a genome is too
// slow to play live is estimated from the compiled plan. Features, per sample of one voice:
constexpr int kCostFeatures = 11;
inline void costFeatures(const Plan& plan, const cppn::Cppn& net, double f[kCostFeatures]) {
  for (int i = 0; i < kCostFeatures; i++) f[i] = 0;
  double cppnEval = 0;   // node + connection visits of one CPPN evaluation
  if (net.flat) for (int node : net.order) cppnEval += 1.0 + (double)net.incoming[node].size();
  else { cppnEval = net.total; for (auto& in : net.incoming) cppnEval += (double)in.size(); }
  f[0] = 1;                                         // fixed per-sample overhead
  f[1] = cppnEval * (double)plan.groups.size();     // CPPN evaluations
  f[2] = (double)plan.nodes.size();                 // per-node overhead (input mixing over 6 channels)
  for (const auto& n : plan.nodes) {
    double edges = (double)n.ins.size();
    for (const auto& pr : n.p) edges += (double)(pr.sigs.size() + pr.mods.size());
    for (const auto& pr : n.fp) edges += (double)(pr.sigs.size() + pr.mods.size());
    f[3] += edges;                                  // connections and parameter signals
    const bool dyn = n.p[0].dynamic() || n.p[1].dynamic() || n.p[2].dynamic() || n.p[3].dynamic();
    switch (n.kind) {
      case K_BIQUAD: f[dyn ? 5 : 4] += 1; break;    // a moving biquad recomputes its coefficients every sample
      case K_OSC: f[6] += 1; break;
      case K_WAVETABLE: f[7] += (double)n.waves.size(); break;
      case K_ADDITIVE: f[7] += (double)n.partials.size(); break;
      case K_DELAY: case K_FBDELAY: case K_COMP: case K_SHAPER: f[8] += 1; break;
      case K_CONV: f[9] += 1; break;
      case K_FAUST: f[10] += faustCostWeight(n.tpl); break;
      default: break;
    }
  }
}

}  // namespace cgraph
