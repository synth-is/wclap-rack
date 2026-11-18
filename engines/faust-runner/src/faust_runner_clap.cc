// Copyright 2026 Björn Þór Jónsson (kromosynth-burst). Licensed under the MIT License; see ../LICENSE.
//
// faust-runner: a generic Faust-graph runner as a WCLAP instrument — a reference engine plugin for
// wclap-rack SPEC §8 (wclap.files/1 file delivery). Instead of baking one WCLAP per Faust graph,
// a rack delivers ANY free-form Faust genome (a faust2wasm module + its metadata JSON) to this
// engine and the engine runs it:
//
//   * Files: the host delivers the genome BEFORE activate via wclap.files/1 binary frames on the
//     clap.webview/3 port channel (SPEC §8): type 240 write (u16 pathLen, path, u32 dataLen, data),
//     type 241 clear; every frame logged to stdout with an FNV-1a digest (same format as testfx /
//     sfz-engine). On activate the engine picks the first delivered "*.json" (faust2wasm metadata:
//     size, compile_options, inputs/outputs, ui widget tree with a byte-offset "index" per widget)
//     and the first "*.wasm" (the compiled DSP).
//   * Execution: the genome wasm runs inside this module through an embedded wasm interpreter
//     (wasm3, vendored in third_party/wasm3, MIT). Its env math imports (_sin/_sinf/…) are linked
//     to libm; it exports its own linear memory, which the engine grows when needed. DSP instance
//     at address 0 (json.size bytes), then the u32 input/output pointer tables and the audio
//     channel buffers (the faust2wasm layout, same as faust-wasm-dsp.js). Exports used: init,
//     compute, getNumInputs, getNumOutputs, setParamValue.
//   * Parameters: the JSON's ui leaves (hslider/vslider/nentry/checkbox/button; bargraphs and the
//     note-driven freq/gain/gate are excluded) become a DYNAMIC clap.params set — count and info
//     are reported after delivery, so a host (e.g. the Synth.is web app) can build one slider per
//     parameter of whatever genome arrived. Values are written into the genome via setParamValue
//     (float32 in -single, float64 in -double, per compile_options).
//   * Notes: monophonic, mapped to the freq/gain/gate widgets when present
//     (freq = 440*2^((key-69)/12), gain = velocity, gate = 1/0). Sample-segment accurate: the
//     process block is split at event boundaries before each compute call.
//   * Audio: no audio input (instrument as a rack source), one stereo output "main"; a mono genome
//     output is duplicated, >2 outputs use the first two. compute() runs in chunks of ≤ 4096 frames.
//   * clap.latency = 0, clap.tail = 0. State: [u8 ver][u32 count][count × f64 values][u16 json path
//     len][path][u16 wasm path len][path]. No editor page (clap.webview/3 get_uri is empty).
//   * Deterministic: wasm3 + libm, no RNG, no threads, no allocation in process().

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <new>
#include <string>
#include <vector>

#include <clap/clap.h>
#include <clap/ext/draft/webview.h>
#include <clap/ext/note-ports.h>

#include "mini_json.h"

extern "C" {
#include "wasm3.h"
#include "m3_env.h"  // vendored wasm3 internal header: ResizeMemory + Module_Memory0
}

namespace {

constexpr uint32_t kMaxBlock = 4096;
constexpr int kStateVersion = 1;

const char* const kFeatures[] = {CLAP_PLUGIN_FEATURE_INSTRUMENT, CLAP_PLUGIN_FEATURE_SYNTHESIZER, nullptr};

const clap_plugin_descriptor_t kDescriptor = {
    CLAP_VERSION_INIT,
    "is.synth.kromosynth.faust-runner",
    "Kromosynth Faust Runner",
    "kromosynth-burst",
    nullptr,
    nullptr,
    nullptr,
    "1.0.0",
    "Generic Faust-graph runner: genome wasm+JSON delivered via wclap.files/1 (wclap-rack SPEC §8)",
    kFeatures};

// ── genome math imports (the env.* functions faust2wasm modules import) ────────────────────────────────

#define FAUST_MATH1_D(NAME, EXPR)                \
  m3ApiRawFunction(faust_##NAME##_d) {          \
    m3ApiReturnType(double);                    \
    m3ApiGetArg(double, x);                     \
    m3ApiReturn(EXPR);                          \
  }
#define FAUST_MATH1_F(NAME, EXPR)                \
  m3ApiRawFunction(faust_##NAME##_f) {          \
    m3ApiReturnType(float);                     \
    m3ApiGetArg(float, x);                      \
    m3ApiReturn(EXPR);                          \
  }
#define FAUST_MATH2_D(NAME, EXPR)                \
  m3ApiRawFunction(faust_##NAME##_d) {          \
    m3ApiReturnType(double);                    \
    m3ApiGetArg(double, x);                     \
    m3ApiGetArg(double, y);                     \
    m3ApiReturn(EXPR);                          \
  }
#define FAUST_MATH2_F(NAME, EXPR)                \
  m3ApiRawFunction(faust_##NAME##_f) {          \
    m3ApiReturnType(float);                     \
    m3ApiGetArg(float, x);                      \
    m3ApiGetArg(float, y);                      \
    m3ApiReturn(EXPR);                          \
  }
#define FAUST_MATH1I_D(NAME, EXPR)               \
  m3ApiRawFunction(faust_##NAME##_d) {          \
    m3ApiReturnType(int32_t);                   \
    m3ApiGetArg(double, x);                     \
    m3ApiReturn(EXPR);                          \
  }
#define FAUST_MATH1I_F(NAME, EXPR)               \
  m3ApiRawFunction(faust_##NAME##_f) {          \
    m3ApiReturnType(int32_t);                   \
    m3ApiGetArg(float, x);                      \
    m3ApiReturn(EXPR);                          \
  }

FAUST_MATH1_D(sin, std::sin(x)) FAUST_MATH1_F(sin, std::sin(x))
FAUST_MATH1_D(cos, std::cos(x)) FAUST_MATH1_F(cos, std::cos(x))
FAUST_MATH1_D(exp, std::exp(x)) FAUST_MATH1_F(exp, std::exp(x))
FAUST_MATH1_D(exp10, std::pow(10.0, x)) FAUST_MATH1_F(exp10, std::pow(10.0f, x))
FAUST_MATH1_D(log, std::log(x)) FAUST_MATH1_F(log, std::log(x))
FAUST_MATH1_D(log10, std::log10(x)) FAUST_MATH1_F(log10, std::log10(x))
FAUST_MATH1_D(tan, std::tan(x)) FAUST_MATH1_F(tan, std::tan(x))
FAUST_MATH1_D(atan, std::atan(x)) FAUST_MATH1_F(atan, std::atan(x))
FAUST_MATH1_D(asin, std::asin(x)) FAUST_MATH1_F(asin, std::asin(x))
FAUST_MATH1_D(acos, std::acos(x)) FAUST_MATH1_F(acos, std::acos(x))
FAUST_MATH1_D(round, std::round(x)) FAUST_MATH1_F(round, std::round(x))
FAUST_MATH1_D(acosh, std::acosh(x)) FAUST_MATH1_F(acosh, std::acosh(x))
FAUST_MATH1_D(asinh, std::asinh(x)) FAUST_MATH1_F(asinh, std::asinh(x))
FAUST_MATH1_D(atanh, std::atanh(x)) FAUST_MATH1_F(atanh, std::atanh(x))
FAUST_MATH1_D(cosh, std::cosh(x)) FAUST_MATH1_F(cosh, std::cosh(x))
FAUST_MATH1_D(sinh, std::sinh(x)) FAUST_MATH1_F(sinh, std::sinh(x))
FAUST_MATH1_D(tanh, std::tanh(x)) FAUST_MATH1_F(tanh, std::tanh(x))
FAUST_MATH2_D(pow, std::pow(x, y)) FAUST_MATH2_F(pow, std::pow(x, y))
FAUST_MATH2_D(atan2, std::atan2(x, y)) FAUST_MATH2_F(atan2, std::atan2(x, y))
FAUST_MATH2_D(fmod, std::fmod(x, y)) FAUST_MATH2_F(fmod, std::fmod(x, y))
FAUST_MATH2_D(remainder, std::remainder(x, y)) FAUST_MATH2_F(remainder, std::remainder(x, y))
FAUST_MATH2_D(copysign, std::copysign(x, y)) FAUST_MATH2_F(copysign, std::copysign(x, y))
FAUST_MATH1I_D(isnan, std::isnan(x) ? 1 : 0) FAUST_MATH1I_F(isnan, std::isnan(x) ? 1 : 0)
FAUST_MATH1I_D(isinf, std::isinf(x) ? 1 : 0) FAUST_MATH1I_F(isinf, std::isinf(x) ? 1 : 0)

struct MathLink {
  const char* name;
  const char* sig;
  M3RawCall fn;
};

const MathLink kMathLinks[] = {
    {"_sin", "d(d)", (M3RawCall)&faust_sin_d},   {"_sinf", "f(f)", (M3RawCall)&faust_sin_f},
    {"_cos", "d(d)", (M3RawCall)&faust_cos_d},   {"_cosf", "f(f)", (M3RawCall)&faust_cos_f},
    {"_exp", "d(d)", (M3RawCall)&faust_exp_d},   {"_expf", "f(f)", (M3RawCall)&faust_exp_f},
    {"_exp10", "d(d)", (M3RawCall)&faust_exp10_d}, {"_exp10f", "f(f)", (M3RawCall)&faust_exp10_f},
    {"_log", "d(d)", (M3RawCall)&faust_log_d},   {"_logf", "f(f)", (M3RawCall)&faust_log_f},
    {"_log10", "d(d)", (M3RawCall)&faust_log10_d}, {"_log10f", "f(f)", (M3RawCall)&faust_log10_f},
    {"_tan", "d(d)", (M3RawCall)&faust_tan_d},   {"_tanf", "f(f)", (M3RawCall)&faust_tan_f},
    {"_atan", "d(d)", (M3RawCall)&faust_atan_d}, {"_atanf", "f(f)", (M3RawCall)&faust_atan_f},
    {"_asin", "d(d)", (M3RawCall)&faust_asin_d}, {"_asinf", "f(f)", (M3RawCall)&faust_asin_f},
    {"_acos", "d(d)", (M3RawCall)&faust_acos_d}, {"_acosf", "f(f)", (M3RawCall)&faust_acos_f},
    {"_round", "d(d)", (M3RawCall)&faust_round_d}, {"_roundf", "f(f)", (M3RawCall)&faust_round_f},
    {"_acosh", "d(d)", (M3RawCall)&faust_acosh_d}, {"_acoshf", "f(f)", (M3RawCall)&faust_acosh_f},
    {"_asinh", "d(d)", (M3RawCall)&faust_asinh_d}, {"_asinhf", "f(f)", (M3RawCall)&faust_asinh_f},
    {"_atanh", "d(d)", (M3RawCall)&faust_atanh_d}, {"_atanhf", "f(f)", (M3RawCall)&faust_atanh_f},
    {"_cosh", "d(d)", (M3RawCall)&faust_cosh_d}, {"_coshf", "f(f)", (M3RawCall)&faust_cosh_f},
    {"_sinh", "d(d)", (M3RawCall)&faust_sinh_d}, {"_sinhf", "f(f)", (M3RawCall)&faust_sinh_f},
    {"_tanh", "d(d)", (M3RawCall)&faust_tanh_d}, {"_tanhf", "f(f)", (M3RawCall)&faust_tanh_f},
    {"_pow", "d(dd)", (M3RawCall)&faust_pow_d},  {"_powf", "f(ff)", (M3RawCall)&faust_pow_f},
    {"_atan2", "d(dd)", (M3RawCall)&faust_atan2_d}, {"_atan2f", "f(ff)", (M3RawCall)&faust_atan2_f},
    {"_fmod", "d(dd)", (M3RawCall)&faust_fmod_d}, {"_fmodf", "f(ff)", (M3RawCall)&faust_fmod_f},
    {"_remainder", "d(dd)", (M3RawCall)&faust_remainder_d}, {"_remainderf", "f(ff)", (M3RawCall)&faust_remainder_f},
    {"_copysign", "d(dd)", (M3RawCall)&faust_copysign_d}, {"_copysignf", "f(ff)", (M3RawCall)&faust_copysign_f},
    {"_isnan", "i(d)", (M3RawCall)&faust_isnan_d}, {"_isnanf", "i(f)", (M3RawCall)&faust_isnan_f},
    {"_isinf", "i(d)", (M3RawCall)&faust_isinf_d}, {"_isinf_f", "i(f)", (M3RawCall)&faust_isinf_f},
};

// ── engine state ───────────────────────────────────────────────────────────────────────────────────────

struct Param {
  std::string label;
  std::string address;
  std::string type;
  int32_t widgetIndex = 0;  // byte offset into the genome's state (JSON "index")
  double min = 0, max = 1, def = 0, step = 0;
  bool stepped = false;
  double value = 0;
};

struct FaustRunner {
  clap_plugin_t plugin;
  const clap_host_t* host;
  double sampleRate = 48000;
  // wclap.files/1 (SPEC §8)
  std::map<std::string, std::vector<uint8_t>> files;
  uint32_t filesDigest = 0x811c9dc5u;
  // the loaded genome
  std::string jsonPath, wasmPath;
  std::vector<Param> params;
  int widgetFreq = -1, widgetGain = -1, widgetGate = -1;  // widget indices of freq/gain/gate
  bool isDouble = false;
  uint32_t stateSize = 0;
  int numInputs = 0, numOutputs = 0;
  IM3Environment env = nullptr;
  IM3Runtime runtime = nullptr;
  IM3Module module = nullptr;
  IM3Function fnInit = nullptr, fnCompute = nullptr, fnSetParam = nullptr, fnGetNumInputs = nullptr,
              fnGetNumOutputs = nullptr;
  uint8_t* mem = nullptr;
  size_t memSize = 0;
  uint32_t inputsPtr = 0, outputsPtr = 0;
  std::vector<uint32_t> chanPtrs;
  bool loaded = false;
  bool active = false;
};

FaustRunner* self(const clap_plugin_t* p) { return static_cast<FaustRunner*>(p->plugin_data); }

double clampDouble(double v, double lo, double hi) { return std::min(hi, std::max(lo, v)); }
uint32_t alignUp(uint32_t v, uint32_t a) { return (v + a - 1) / a * a; }

// ── genome loading ─────────────────────────────────────────────────────────────────────────────────────

void collectUiLeaves(const minijson::Value& ui, std::vector<const minijson::Value*>& out) {
  if (ui.type != minijson::Value::Arr) return;
  for (const auto& item : ui.arr) {
    if (item.type == minijson::Value::Obj) {
      const auto* items = item.find("items");
      if (items && items->type == minijson::Value::Arr)
        collectUiLeaves(*items, out);
      else
        out.push_back(&item);
    }
  }
}

bool callInt(FaustRunner* s, IM3Function f, int32_t a0, int32_t& ret) {
  const void* args[1] = {&a0};
  M3Result r = m3_Call(f, 1, args);
  if (r) return false;
  const void* rets[1] = {&ret};
  return m3_GetResults(f, 1, rets) == nullptr;
}

// setParamValue(dsp=0, index, value) in faust2wasm is just a typed store at dsp+index (f32 in -single,
// f64 in -double) — write it directly. (m3_Call's f32 ARG marshalling is unreliable on wasm32 hosts;
// i32 args and f32 RETURNS via m3_Call/m3_GetResults are fine, and direct stores are exactly what the
// genome's own setParamValue does.)
bool callSetParam(FaustRunner* s, int32_t index, double value) {
  if (!s->mem || index < 0) return false;
  if (s->isDouble) {
    if ((size_t)(index + 8) > s->memSize) return false;
    double v = value;
    std::memcpy(s->mem + index, &v, 8);
  } else {
    if ((size_t)(index + 4) > s->memSize) return false;
    float v = (float)value;
    std::memcpy(s->mem + index, &v, 4);
  }
  return true;
}

void freeGenome(FaustRunner* s) {
  if (s->runtime) {
    m3_FreeRuntime(s->runtime);
    s->runtime = nullptr;
  }
  s->module = nullptr;  // freed with the environment below only if still linked; wasm3 frees modules with the env
  s->fnInit = s->fnCompute = s->fnSetParam = s->fnGetNumInputs = s->fnGetNumOutputs = nullptr;
  s->mem = nullptr;
  s->loaded = false;
  s->params.clear();
  s->widgetFreq = s->widgetGain = s->widgetGate = -1;
}

bool loadGenome(FaustRunner* s) {
  freeGenome(s);
  // pick the first delivered *.json and *.wasm (sorted, deterministic)
  std::string jsonPath, wasmPath;
  for (const auto& kv : s->files) {
    const std::string& p = kv.first;
    auto endsWith = [&](const char* ext) {
      size_t n = strlen(ext);
      if (p.size() < n) return false;
      std::string e = p.substr(p.size() - n);
      for (auto& c : e) c = (char)tolower((unsigned char)c);
      return e == ext;
    };
    if (endsWith(".json") && jsonPath.empty()) jsonPath = p;
    if (endsWith(".wasm") && wasmPath.empty()) wasmPath = p;
  }
  if (jsonPath.empty() || wasmPath.empty()) {
    std::printf("faust-runner: need a .json and a .wasm among %u delivered file(s); genome empty\n",
                (unsigned)s->files.size());
    return false;
  }
  s->jsonPath = jsonPath;
  s->wasmPath = wasmPath;
  const std::vector<uint8_t>& jsonBytes = s->files[jsonPath];
  const std::vector<uint8_t>& wasmBytes = s->files[wasmPath];

  minijson::Value root;
  if (!minijson::parse(std::string(jsonBytes.begin(), jsonBytes.end()), root)) {
    std::printf("faust-runner: cannot parse json %s\n", jsonPath.c_str());
    return false;
  }
  s->stateSize = (uint32_t)root.getNum("size", 0);
  s->isDouble = root.getStr("compile_options").find("-double") != std::string::npos;
  const minijson::Value* ui = root.find("ui");
  std::vector<const minijson::Value*> leaves;
  if (ui) collectUiLeaves(*ui, leaves);
  for (const minijson::Value* leaf : leaves) {
    const minijson::Value* index = leaf->find("index");
    if (!index || index->type != minijson::Value::Num) continue;
    std::string type = leaf->getStr("type");
    if (type.find("bargraph") != std::string::npos) continue;
    std::string label = leaf->getStr("label");
    std::string address = leaf->getStr("address");
    // note-driven widgets are driven by notes, not exposed as params
    if (label == "freq" || label == "gain" || label == "gate") {
      int32_t idx = (int32_t)index->num;
      if (label == "freq") s->widgetFreq = idx;
      if (label == "gain") s->widgetGain = idx;
      if (label == "gate") s->widgetGate = idx;
      continue;
    }
    Param prm;
    prm.label = label;
    prm.address = address;
    prm.type = type;
    prm.widgetIndex = (int32_t)index->num;
    prm.min = leaf->getNum("min", 0);
    prm.max = leaf->getNum("max", 1);
    prm.def = leaf->getNum("init", prm.min);
    prm.step = leaf->getNum("step", 0);
    prm.stepped = (type == "checkbox" || type == "button");
    if (prm.stepped) {
      prm.min = 0;
      prm.max = 1;
      prm.def = leaf->getNum("init", 0) != 0 ? 1 : 0;
    }
    prm.value = prm.def;
    s->params.push_back(std::move(prm));
  }

  // instantiate the genome wasm through wasm3
  s->env = m3_NewEnvironment();
  s->runtime = m3_NewRuntime(s->env, 4 * 1024 * 1024, nullptr);  // 1 MB interpreter stack, from the heap
  if (!s->runtime) { std::printf("faust-runner: m3_NewRuntime failed\n"); return false; }
  M3Result r = m3_ParseModule(s->env, &s->module, wasmBytes.data(), (uint32_t)wasmBytes.size());
  if (r) { std::printf("faust-runner: m3_ParseModule: %s\n", r); return false; }
  // wasm3 v0.5.0: link the env math imports before loading the module
  for (const MathLink& link : kMathLinks) {
    M3Result lr = m3_LinkRawFunction(s->module, "env", link.name, link.sig, link.fn);
    if (lr && lr != m3Err_functionLookupFailed) {
      std::printf("faust-runner: link %s: %s\n", link.name, lr);
      return false;
    }
  }
  r = m3_LoadModule(s->runtime, s->module);
  if (r) { std::printf("faust-runner: m3_LoadModule: %s\n", r); return false; }
  if (m3_FindFunction(&s->fnInit, s->runtime, "init")) { std::printf("faust-runner: no init export\n"); return false; }
  m3_FindFunction(&s->fnCompute, s->runtime, "compute");
  m3_FindFunction(&s->fnSetParam, s->runtime, "setParamValue");
  m3_FindFunction(&s->fnGetNumInputs, s->runtime, "getNumInputs");
  m3_FindFunction(&s->fnGetNumOutputs, s->runtime, "getNumOutputs");
  if (!s->fnCompute || !s->fnSetParam || !s->fnGetNumInputs || !s->fnGetNumOutputs) {
    std::printf("faust-runner: missing faust2wasm exports\n");
    return false;
  }
  int32_t zero = 0, nIn = 0, nOut = 0;
  if (!callInt(s, s->fnGetNumInputs, 0, nIn) || !callInt(s, s->fnGetNumOutputs, 0, nOut)) {
    std::printf("faust-runner: getNumInputs/Outputs failed\n");
    return false;
  }
  s->numInputs = nIn;
  s->numOutputs = nOut;

  // faust2wasm memory layout: [dsp state: json.size][u32 input table][u32 output table][channel buffers]
  const uint32_t bytes = s->isDouble ? 8 : 4;
  s->inputsPtr = alignUp(s->stateSize, 4);
  s->outputsPtr = s->inputsPtr + (uint32_t)s->numInputs * 4;
  uint32_t cursor = alignUp(s->outputsPtr + (uint32_t)s->numOutputs * 4, bytes);
  s->chanPtrs.clear();
  for (int c = 0; c < s->numInputs + s->numOutputs; ++c) {
    s->chanPtrs.push_back(cursor);
    cursor += kMaxBlock * bytes;
  }
  uint32_t memSize32 = 0;
  s->mem = m3_GetMemory(s->runtime, &memSize32, 0);
  s->memSize = memSize32;
  if (!s->mem) { std::printf("faust-runner: genome exports no memory\n"); return false; }
  const uint64_t needPages = (cursor + 65535) / 65536;
  if (needPages * 65536 > s->memSize) {
    r = ResizeMemory(s->runtime, (uint32_t)needPages);
    if (r) { std::printf("faust-runner: ResizeMemory: %s\n", r); return false; }
    s->mem = m3_GetMemory(s->runtime, &memSize32, 0);
    s->memSize = memSize32;
  }
  uint32_t* tables = reinterpret_cast<uint32_t*>(s->mem + s->inputsPtr);
  for (size_t c = 0; c < s->chanPtrs.size(); ++c) tables[c] = s->chanPtrs[c];

  // init(dsp=0, sampleRate) — i32 in -single, f64 in -double
  {
    int32_t a0 = 0;
    const void* args[2] = {&a0, nullptr};
    if (s->isDouble) {
      double sr = s->sampleRate;
      args[1] = &sr;
    } else {
      int32_t sr = (int32_t)s->sampleRate;
      args[1] = &sr;
    }
    r = m3_Call(s->fnInit, 2, args);
    if (r) { std::printf("faust-runner: init: %s\n", r); return false; }
  }
  for (const auto& prm : s->params) callSetParam(s, prm.widgetIndex, prm.value);
  s->loaded = true;
  std::printf("faust-runner: loaded json=%s wasm=%s params=%u inputs=%d outputs=%d double=%d state=%u freq=%d gain=%d gate=%d\n",
              jsonPath.c_str(), wasmPath.c_str(), (unsigned)s->params.size(), s->numInputs, s->numOutputs,
              (int)s->isDouble, s->stateSize, s->widgetFreq, s->widgetGain, s->widgetGate);
  return true;
}

// ── clap.params (dynamic: the delivered genome's widget table) ─────────────────────────────────────────

uint32_t paramsCount(const clap_plugin_t* p) { return (uint32_t)self(p)->params.size(); }

bool paramsInfo(const clap_plugin_t* p, uint32_t i, clap_param_info_t* info) {
  FaustRunner* s = self(p);
  if (i >= s->params.size()) return false;
  const Param& prm = s->params[i];
  std::memset(info, 0, sizeof(*info));
  info->id = i;
  info->flags = CLAP_PARAM_IS_AUTOMATABLE | (prm.stepped ? CLAP_PARAM_IS_STEPPED : 0);
  std::snprintf(info->name, sizeof(info->name), "%s", prm.label.c_str());
  info->min_value = prm.min;
  info->max_value = prm.max;
  info->default_value = prm.def;
  return true;
}

bool paramsGetValue(const clap_plugin_t* p, clap_id id, double* out) {
  FaustRunner* s = self(p);
  if (id >= s->params.size()) return false;
  *out = s->params[id].value;
  return true;
}

bool paramsValueToText(const clap_plugin_t* p, clap_id id, double value, char* out, uint32_t cap) {
  FaustRunner* s = self(p);
  if (id >= s->params.size()) return false;
  const Param& prm = s->params[id];
  if (prm.stepped) std::snprintf(out, cap, "%s", value >= 0.5 ? "on" : "off");
  else std::snprintf(out, cap, "%.6g", clampDouble(value, prm.min, prm.max));
  return true;
}

bool paramsTextToValue(const clap_plugin_t* p, clap_id id, const char* text, double* out) {
  FaustRunner* s = self(p);
  if (id >= s->params.size()) return false;
  const Param& prm = s->params[id];
  if (prm.stepped) {
    if (!std::strcmp(text, "on")) { *out = 1; return true; }
    if (!std::strcmp(text, "off")) { *out = 0; return true; }
  }
  char* end = nullptr;
  double v = std::strtod(text, &end);
  if (end == text) return false;
  *out = v;
  return true;
}

void applyParamEvent(FaustRunner* s, clap_id id, double value) {
  if (id >= s->params.size() || !s->loaded) return;
  Param& prm = s->params[id];
  prm.value = clampDouble(value, prm.min, prm.max);
  callSetParam(s, prm.widgetIndex, prm.value);
}

void paramsFlush(const clap_plugin_t* p, const clap_input_events_t* in, const clap_output_events_t*) {
  FaustRunner* s = self(p);
  const uint32_t total = in->size(in);
  for (uint32_t i = 0; i < total; ++i) {
    const clap_event_header_t* h = in->get(in, i);
    if (h->space_id != CLAP_CORE_EVENT_SPACE_ID || h->type != CLAP_EVENT_PARAM_VALUE) continue;
    auto* e = reinterpret_cast<const clap_event_param_value_t*>(h);
    applyParamEvent(s, e->param_id, e->value);
  }
}

const clap_plugin_params_t kParams = {paramsCount, paramsInfo, paramsGetValue, paramsValueToText, paramsTextToValue,
                                      paramsFlush};

// ── clap.state ─────────────────────────────────────────────────────────────────────────────────────────

bool stateSave(const clap_plugin_t* p, const clap_ostream_t* out) {
  FaustRunner* s = self(p);
  std::vector<unsigned char> buf;
  buf.push_back(kStateVersion);
  uint32_t count = (uint32_t)s->params.size();
  for (int i = 0; i < 4; ++i) buf.push_back((unsigned char)(count >> (8 * i)));
  for (const auto& prm : s->params) {
    double v = prm.value;
    unsigned char b[8];
    std::memcpy(b, &v, 8);
    buf.insert(buf.end(), b, b + 8);
  }
  auto putStr = [&](const std::string& str) {
    uint16_t n = (uint16_t)std::min<size_t>(str.size(), 0xffff);
    buf.push_back((unsigned char)(n & 0xff));
    buf.push_back((unsigned char)(n >> 8));
    buf.insert(buf.end(), str.begin(), str.begin() + n);
  };
  putStr(s->jsonPath);
  putStr(s->wasmPath);
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
  if (buf.size() < 5 || buf[0] != kStateVersion) return false;
  uint32_t count = (uint32_t)buf[1] | ((uint32_t)buf[2] << 8) | ((uint32_t)buf[3] << 16) | ((uint32_t)buf[4] << 24);
  if (buf.size() < (size_t)(5 + count * 8)) return false;
  FaustRunner* s = self(p);
  if (count != s->params.size()) return false;  // state belongs to a different genome
  for (uint32_t i = 0; i < count; ++i) {
    double v;
    std::memcpy(&v, buf.data() + 5 + i * 8, 8);
    s->params[i].value = v;
    if (s->loaded) callSetParam(s, s->params[i].widgetIndex, v);
  }
  return true;
}

const clap_plugin_state_t kState = {stateSave, stateLoad};

// ── clap.audio-ports / clap.note-ports ─────────────────────────────────────────────────────────────────

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

// ── clap.latency / clap.tail ───────────────────────────────────────────────────────────────────────────

uint32_t latencyGet(const clap_plugin_t*) { return 0; }
const clap_plugin_latency_t kLatency = {latencyGet};
uint32_t tailGet(const clap_plugin_t*) { return 0; }
const clap_plugin_tail_t kTail = {tailGet};

// ── clap.webview/3 — no editor page; receive() carries wclap.files/1 frames (wclap-rack SPEC §8) ───────

int32_t webviewGetUri(const clap_plugin_t*, char* uri, uint32_t cap) {
  if (cap > 0 && uri != nullptr) uri[0] = '\0';
  return 1;
}
bool webviewGetResource(const clap_plugin_t*, const char*, char*, uint32_t, const clap_ostream_t*) { return false; }

bool webviewReceive(const clap_plugin_t* p, const void* buffer, uint32_t size) {
  FaustRunner* s = self(p);
  if (size < 4) return false;
  const uint8_t* b = static_cast<const uint8_t*>(buffer);
  const uint8_t* pl = b + 4;
  const uint32_t plen = size - 4;
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

// ── plugin ─────────────────────────────────────────────────────────────────────────────────────────────

bool pluginInit(const clap_plugin_t*) { return true; }
void pluginDestroy(const clap_plugin_t* p) {
  FaustRunner* s = self(p);
  freeGenome(s);
  if (s->env) m3_FreeEnvironment(s->env);
  s->env = nullptr;
  s->~FaustRunner();
  std::free(s);
}

bool pluginActivate(const clap_plugin_t* p, double sampleRate, uint32_t, uint32_t) {
  FaustRunner* s = self(p);
  s->sampleRate = sampleRate > 0 ? sampleRate : 48000;
  loadGenome(s);
  s->active = true;
  return true;
}
void pluginDeactivate(const clap_plugin_t* p) {
  FaustRunner* s = self(p);
  freeGenome(s);
  s->active = false;
}
bool pluginStartProcessing(const clap_plugin_t*) { return true; }
void pluginStopProcessing(const clap_plugin_t*) {}
void pluginReset(const clap_plugin_t*) {}  // params and the loaded genome remain

void handleEvent(FaustRunner* s, const clap_event_header_t* h) {
  if (h->space_id != CLAP_CORE_EVENT_SPACE_ID) return;
  if (h->type == CLAP_EVENT_PARAM_VALUE) {
    auto* e = reinterpret_cast<const clap_event_param_value_t*>(h);
    applyParamEvent(s, e->param_id, e->value);
  } else if (h->type == CLAP_EVENT_NOTE_ON) {
    auto* e = reinterpret_cast<const clap_event_note_t*>(h);
    if (!s->loaded) return;
    if (s->widgetFreq >= 0) callSetParam(s, s->widgetFreq, 440.0 * std::pow(2.0, (e->key - 69) / 12.0));
    if (s->widgetGain >= 0) callSetParam(s, s->widgetGain, e->velocity);
    if (s->widgetGate >= 0) callSetParam(s, s->widgetGate, 1.0);
  } else if (h->type == CLAP_EVENT_NOTE_OFF || h->type == CLAP_EVENT_NOTE_CHOKE) {
    auto* e = reinterpret_cast<const clap_event_note_t*>(h);
    if (!s->loaded) return;
    if (s->widgetGate >= 0) callSetParam(s, s->widgetGate, h->type == CLAP_EVENT_NOTE_CHOKE ? 0.0 : 0.0);
  } else if (h->type == CLAP_EVENT_MIDI) {
    auto* e = reinterpret_cast<const clap_event_midi_t*>(h);
    const uint8_t status = e->data[0] & 0xf0;
    if (status == 0x90 && e->data[2] > 0) {
      if (!s->loaded) return;
      if (s->widgetFreq >= 0) callSetParam(s, s->widgetFreq, 440.0 * std::pow(2.0, (e->data[1] - 69) / 12.0));
      if (s->widgetGain >= 0) callSetParam(s, s->widgetGain, e->data[2] / 127.0);
      if (s->widgetGate >= 0) callSetParam(s, s->widgetGate, 1.0);
    } else if (status == 0x80 || (status == 0x90 && e->data[2] == 0)) {
      if (s->loaded && s->widgetGate >= 0) callSetParam(s, s->widgetGate, 0.0);
    }
  }
}

// run the genome for n frames starting at output offset `done`
void runSegment(FaustRunner* s, clap_audio_buffer_t* out, uint32_t outCh, uint32_t done, uint32_t n) {
  int32_t a0 = 0, a1 = (int32_t)n, a2 = (int32_t)s->inputsPtr, a3 = (int32_t)s->outputsPtr;
  const void* args[4] = {&a0, &a1, &a2, &a3};
  if (m3_Call(s->fnCompute, 4, args) != nullptr) return;
  if (s->numOutputs < 1) return;
  const uint32_t bytes = s->isDouble ? 8 : 4;
  for (uint32_t ch = 0; ch < outCh; ++ch) {
    const uint32_t srcCh = std::min(ch, (uint32_t)s->numOutputs - 1);
    const uint8_t* src = s->mem + s->chanPtrs[s->numInputs + srcCh];
    for (uint32_t i = 0; i < n; ++i) {
      double v = s->isDouble ? reinterpret_cast<const double*>(src)[i] : (double)reinterpret_cast<const float*>(src)[i];
      out->data32[ch][done + i] = (float)v;
    }
    (void)bytes;
  }
}

clap_process_status pluginProcess(const clap_plugin_t* p, const clap_process_t* proc) {
  FaustRunner* s = self(p);
  if (!s->active || proc->audio_outputs_count < 1) return CLAP_PROCESS_ERROR;
  const clap_audio_buffer_t* out = &proc->audio_outputs[0];
  if (!out->data32 || out->channel_count < 1) return CLAP_PROCESS_ERROR;
  const uint32_t frames = proc->frames_count;
  const uint32_t outCh = std::min(out->channel_count, 2u);
  for (uint32_t ch = 0; ch < outCh; ++ch)
    for (uint32_t i = 0; i < frames; ++i) out->data32[ch][i] = 0;
  if (!s->loaded) return CLAP_PROCESS_CONTINUE;
  const uint32_t total = proc->in_events ? proc->in_events->size(proc->in_events) : 0;
  uint32_t cursor = 0, done = 0;
  auto runTo = [&](uint32_t end) {
    while (done < end) {
      const uint32_t n = std::min(end - done, kMaxBlock);
      runSegment(s, const_cast<clap_audio_buffer_t*>(out), outCh, done, n);
      done += n;
    }
  };
  // split the block at event boundaries so notes/params take effect between compute calls
  while (cursor < total) {
    const clap_event_header_t* h = proc->in_events->get(proc->in_events, cursor);
    if (h->time >= frames) break;
    runTo(h->time);
    handleEvent(s, h);
    ++cursor;
  }
  runTo(frames);
  while (cursor < total) handleEvent(s, proc->in_events->get(proc->in_events, cursor++));
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
  return nullptr;
}
void pluginOnMainThread(const clap_plugin_t*) {}

const clap_plugin_t* createPlugin(const clap_host_t* host) {
  void* mem = std::calloc(1, sizeof(FaustRunner));
  FaustRunner* s = new (mem) FaustRunner();
  s->host = host;
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
