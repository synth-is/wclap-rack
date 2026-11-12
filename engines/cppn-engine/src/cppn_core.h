// cppn_core.h — kromosynth's CPPN activation core ported to dependency-free C++.
//
// Ported from neatjs `networkDecode` (neatjs/neatHelp/neatDecoder.js) + cppnjs
// `CPPN.recursiveActivation` (cppnjs/networks/cppn.js) + kromosynth's
// CpuBackend input schemes (kromosynth/cppn-neat/backends/cpu-backend.js) and
// activation-function table (cppn-neat/activation-functions.js). Verified
// BIT-EXACT against neatjs/cppnjs on the real Synth.is genome
// 01KR0R5T4S7SQF4W8M1G99CGS9 (maxAbsDiff = 0.0 over 1,728,000 samples; see
// WCLAP/poc-cppn-wasm). Part of engines/cppn-engine (MIT).
#pragma once

#include <cmath>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include "mini_json.h"

namespace cppn {

enum ActFn {
  ACT_NULL, ACT_LINEAR, ACT_BIPOLAR_SIGMOID, ACT_PLAIN_SIGMOID, ACT_GAUSSIAN,
  ACT_SINE, ACT_SINE2, ACT_STEP, ACT_SIN, ACT_COS, ACT_ARCTAN,
  ACT_SPIKE, ACT_TRIANGLE, ACT_SAWTOOTH, ACT_UNKNOWN
};

inline ActFn actFromName(const std::string& n) {
  if (n == "NullFn") return ACT_NULL;
  if (n == "Linear") return ACT_LINEAR;
  if (n == "BipolarSigmoid") return ACT_BIPOLAR_SIGMOID;
  if (n == "PlainSigmoid") return ACT_PLAIN_SIGMOID;
  if (n == "Gaussian") return ACT_GAUSSIAN;
  if (n == "Sine") return ACT_SINE;
  if (n == "Sine2") return ACT_SINE2;
  if (n == "StepFunction") return ACT_STEP;  // kromosynth override: x<=0 ? -1 : 1
  if (n == "sin") return ACT_SIN;
  if (n == "cos") return ACT_COS;
  if (n == "arctan") return ACT_ARCTAN;
  if (n == "spike") return ACT_SPIKE;
  if (n == "triangle") return ACT_TRIANGLE;
  if (n == "sawtooth") return ACT_SAWTOOTH;
  return ACT_UNKNOWN;
}

// JS-faithful activation functions (cppnjs built-ins + kromosynth additions).
inline double applyAct(ActFn f, double x) {
  switch (f) {
    case ACT_NULL: return 0.0;
    case ACT_LINEAR: return std::fabs(x);  // cppnjs "Linear" is |x|
    case ACT_BIPOLAR_SIGMOID: return 2.0 / (1.0 + std::exp(-4.9 * x)) - 1.0;
    case ACT_PLAIN_SIGMOID: return 1.0 / (1.0 + std::exp(-x));
    case ACT_GAUSSIAN: return 2.0 * std::exp(-std::pow(x * 2.5, 2)) - 1.0;
    case ACT_SINE: return std::sin(x);
    case ACT_SINE2: return std::sin(2.0 * x);
    case ACT_STEP: return x <= 0.0 ? -1.0 : 1.0;
    case ACT_SIN: return std::sin(x);
    case ACT_COS: return std::cos(x);
    case ACT_ARCTAN: return std::atan(x);
    case ACT_SPIKE: {
      double fl = std::floor(x);
      if (std::fmod(fl, 2.0) == 0.0) return 1.0 - 2.0 * (x - fl);
      return -1.0 + 2.0 * (x - fl);
    }
    case ACT_TRIANGLE: return (2.0 / M_PI) * std::asin(std::sin(x));
    case ACT_SAWTOOTH: return 2.0 * ((x / (2.0 * M_PI)) - std::floor(0.5 + (x / (2.0 * M_PI))));
    default: return x;
  }
}

enum Scheme { SCHEME_LEGACY_2INPUT = 2, SCHEME_PHASE_MODULATED = 4, SCHEME_HYBRID_PHASE = 6 };

struct Cppn {
  int biasCount = 0, inputCount = 0, outputCount = 0, total = 0, totalInput = 0;
  Scheme scheme = SCHEME_LEGACY_2INPUT;
  double envelopePeriods = 5.0;
  std::vector<ActFn> act;
  std::vector<double> signals, lastActivation;
  std::vector<char> activated, inActivation;
  std::vector<std::vector<std::pair<int, double>>> incoming;  // per node: (source, weight)

  // Decode a neatjs offspring genome object ({nodes, connections, inputNodeCount, ...}).
  // rollWeights / weightShift: the web app's Weights pad, off centre, adds ONE shift to every CPPN
  // weight and wraps it into [-3, 3). (In its mapper CPPN nodes have no `id`, so every node lands on
  // the same map key and every connection gets the position of the last node.)
  void decode(const minijson::Value& offspring, bool rollWeights = false, double weightShift = 0.0) {
    const minijson::Value* nodes = offspring.find("nodes");
    const minijson::Value* conns = offspring.find("connections");
    inputCount = (int)offspring.getNum("inputNodeCount", 0);
    outputCount = (int)offspring.getNum("outputNodeCount", 0);
    envelopePeriods = offspring.getNum("envelopePeriods", 5.0);
    total = nodes && nodes->type == minijson::Value::Arr ? (int)nodes->arr.size() : 0;
    biasCount = 0;
    if (nodes) {
      for (const auto& n : nodes->arr) {
        const minijson::Value* t = n.find("nodeType");
        if (t && t->type == minijson::Value::Str && t->str == "Bias") biasCount++;
        else break;
      }
    }
    totalInput = biasCount + inputCount;
    scheme = inputCount == 6 ? SCHEME_HYBRID_PHASE : inputCount == 4 ? SCHEME_PHASE_MODULATED : SCHEME_LEGACY_2INPUT;
    act.assign(total, ACT_UNKNOWN);
    if (nodes) {
      for (int i = 0; i < total; i++) {
        const minijson::Value* a = nodes->arr[i].find("activationFunction");
        act[i] = a && a->type == minijson::Value::Str ? actFromName(a->str) : ACT_UNKNOWN;
      }
    }
    std::map<std::string, int> nodeTable;
    if (nodes)
      for (int i = 0; i < total; i++) {
        const minijson::Value* g = nodes->arr[i].find("gid");
        if (g && g->type == minijson::Value::Str) nodeTable[g->str] = i;
      }
    signals.assign(total, 0.0);
    for (int i = 0; i < biasCount; i++) signals[i] = 1.0;
    lastActivation.assign(total, 0.0);
    activated.assign(total, 0);
    inActivation.assign(total, 0);
    incoming.assign(total, {});
    if (conns && conns->type == minijson::Value::Arr) {
      for (const auto& c : conns->arr) {
        // cppnjs reads no `enabled` flag here: every listed connection is used
        const minijson::Value* s = c.find("sourceID");
        const minijson::Value* t = c.find("targetID");
        if (!s || !t || s->type != minijson::Value::Str || t->type != minijson::Value::Str) continue;
        auto si = nodeTable.find(s->str), ti = nodeTable.find(t->str);
        if (si == nodeTable.end() || ti == nodeTable.end()) continue;
        double w = c.getNum("weight", 0.0);
        if (rollWeights) { double off = std::fmod(w + weightShift + 3.0, 6.0); if (off < 0) off += 6.0; w = -3.0 + off; }
        incoming[ti->second].push_back({si->second, w});
      }
      // cppnjs keeps ONE weight per source/target pair (the last listed) but walks its incoming
      // list once per listed connection, so a repeated connection counts once per repeat at that
      // last weight. Measured: genomes with repeated CPPN connections were the unexplained misses.
      for (auto& in : incoming)
        for (size_t i = 0; i < in.size(); i++)
          for (size_t j = i + 1; j < in.size(); j++)
            if (in[j].first == in[i].first) in[i].second = in[j].second;
    }
  }

  // Flat evaluation order for the outputs actually used by a patch. Built once; used when the
  // network has no cycle (the usual case: recurrence is disallowed when genomes are created). The
  // sums run in the same connection order as the recursive form, so results are bit-identical.
  std::vector<int> order;
  bool flat = false;
  void prepare(const std::vector<int>& usedOutputs) {
    order.clear();
    flat = true;
    std::vector<char> state(total, 0);  // 0 new, 1 on the stack, 2 done
    struct Frame { int node; size_t next; };
    std::vector<Frame> stack;
    for (int o : usedOutputs) {
      const int root = totalInput + o;
      if (o < 0 || root >= total || state[root] == 2) continue;
      stack.push_back({root, 0});
      state[root] = 1;
      while (!stack.empty()) {
        Frame& f = stack.back();
        if (f.node < totalInput) { state[f.node] = 2; stack.pop_back(); continue; }
        if (f.next < incoming[f.node].size()) {
          const int src = incoming[f.node][f.next++].first;
          if (state[src] == 1) { flat = false; }           // a cycle: keep the recursive form
          else if (state[src] == 0) { state[src] = 1; stack.push_back({src, 0}); }
        } else {
          state[f.node] = 2;
          order.push_back(f.node);
          stack.pop_back();
        }
      }
    }
    if (!flat) order.clear();
  }

  void clearSignals() {
    for (int i = biasCount; i < total; i++) signals[i] = 0.0;
  }

  void recursiveActivateNode(int node) {
    if (activated[node]) { inActivation[node] = 0; return; }
    inActivation[node] = 1;
    double pre = 0.0;
    for (auto& e : incoming[node]) {
      int src = e.first;
      if (inActivation[src]) pre += lastActivation[src] * e.second;
      else {
        if (!activated[src]) recursiveActivateNode(src);
        pre += signals[src] * e.second;
      }
    }
    activated[node] = 1;
    inActivation[node] = 0;
    signals[node] = applyAct(act[node], pre);
  }

  void recursiveActivation() {
    for (int i = 0; i < total; i++) {
      activated[i] = i < totalInput ? 1 : 0;
      inActivation[i] = 0;
      if (i >= totalInput) lastActivation[i] = signals[i];
    }
    for (int o = 0; o < outputCount; o++) recursiveActivateNode(totalInput + o);
  }

  // Evaluate all outputs for one sample. totalSamples = render length in samples,
  // n = sample index, freq = this activation's frequency (Hz), sr = sample rate,
  // velocity, x, y = control inputs. Mirrors CpuBackend's input schemes.
  void evaluate(long n, long totalSamples, double freq, double sr, double velocity, double x, double y) {
    double main = totalSamples > 1 ? -1.0 + 2.0 * (double)n / (double)(totalSamples - 1) : -1.0;
    double in[6] = {0, 0, 0, 0, 0, 0};
    if (scheme == SCHEME_HYBRID_PHASE) {
      double phase = std::fmod(n * freq / sr, 1.0);
      in[0] = phase;
      in[1] = std::sin(2.0 * M_PI * phase);
      in[2] = main;
      in[3] = velocity;
      in[4] = x;
      in[5] = y;
    } else if (scheme == SCHEME_PHASE_MODULATED) {
      double inputPeriods = freq * ((double)totalSamples / sr);
      in[0] = std::sin(inputPeriods * main);
      in[1] = std::sin(envelopePeriods * main);
      in[2] = main;
      in[3] = velocity;
    } else {
      double inputPeriods = freq * ((double)totalSamples / sr);
      in[0] = std::sin(inputPeriods * main) * velocity;  // variationOnPeriods = true
      in[1] = main;
    }
    if (flat) {
      for (int i = 0; i < inputCount; i++) signals[biasCount + i] = i < 6 ? in[i] : 0.0;
      for (int node : order) {
        double pre = 0.0;
        for (const auto& e : incoming[node]) pre += signals[e.first] * e.second;
        signals[node] = applyAct(act[node], pre);
      }
      return;
    }
    clearSignals();
    for (int i = 0; i < inputCount && i < 6; i++) signals[biasCount + i] = in[i];
    recursiveActivation();
  }

  double output(int o) const { return signals[totalInput + o]; }
};

}  // namespace cppn
