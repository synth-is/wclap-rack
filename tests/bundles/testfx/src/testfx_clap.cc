// Copyright 2026 Björn Þór Jónsson (kromosynth-burst). Licensed under the MIT License; see ../LICENSE.
//
// testfx: a tiny CLAP audio effect of our own, the test fixture for the host's new audio-input path
// (packages/wclap-host-node). Written from scratch for kromosynth-burst; layout and build mirror engines/dx7.
//
//   * Feature: audio-effect. One stereo audio input port and one stereo output port (both id 0, "main";
//     the input's in-place pair is the output, so in-place processing is supported).
//   * Parameters (clap.params):
//       id 0 "Gain":  dB, -24..+12, default 0, continuous; value-to-text is one decimal plus " dB" ("-6.0 dB").
//       id 1 "Delay": stepped 0/1, default 0 ("off"/"on"); when on, the signal is delayed by exactly 64 samples.
//   * Extensions: clap.audio-ports, clap.params, clap.state (the two param values), clap.latency (64 when the
//     delay is on, 0 when off), clap.tail (0), and clap.webview/3 — whose receive() implements the
//     wclap.files/1 file-delivery frames of the wclap-rack standard (SPEC §8): type 240 writes a file
//     (u16 pathLen, path, u32 dataLen, data) into the plugin's in-memory file map, type 241 clears it.
//     Every received frame is logged to stdout with an FNV-1a digest of path+bytes, so hosts can assert
//     delivery in tests (the reference host collects stdout).
//   * process(): no threads, no allocation, any block size (the loop handles any frames_count, up to 4096 and
//     beyond). Parameter events apply at their sample offset. The input sample is read before the output sample
//     is written, so in-place buffers are safe. With no audio input (rendered as a rack source), the input is
//     silence, so the fixture can stand in as a source device in rack tests.

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <new>
#include <string>
#include <vector>

#include <clap/clap.h>
#include <clap/ext/draft/webview.h>

namespace {

constexpr uint32_t kDelaySamples = 64;
constexpr clap_id kParamGain = 0;
constexpr clap_id kParamDelay = 1;
constexpr uint32_t kParamCount = 2;
constexpr double kGainMin = -24.0;
constexpr double kGainMax = 12.0;
constexpr double kGainDefault = 0.0;
constexpr int kStateVersion = 1;

const char* const kFeatures[] = {CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, nullptr};

const clap_plugin_descriptor_t kDescriptor = {
    CLAP_VERSION_INIT,
    "is.synth.kromosynth.testfx",
    "Kromosynth TestFX",
    "kromosynth-burst",
    nullptr,
    nullptr,
    nullptr,
    "1.0.0",
    "Tiny test effect (gain + 64-sample delay) for the host's audio-input path",
    kFeatures};

struct TestFxPlugin {
  clap_plugin_t plugin;
  const clap_host_t* host;
  double gainDb = kGainDefault;   // param 0
  double gainLin = 1.0;           // 10^(gainDb/20), recomputed on every gain change
  int delayOn = 0;                // param 1
  float delayLine[2][kDelaySamples];  // ring buffer, always fed; fixed storage, no allocation in process()
  uint32_t delayPos = 0;
  bool active = false;
  // wclap.files/1 (SPEC §8): files delivered over the clap.webview/3 port before activate
  std::map<std::string, std::vector<uint8_t>> files;
  uint32_t filesDigest = 0x811c9dc5u;  // FNV-1a offset basis, over path + bytes of every write since the last clear
};

TestFxPlugin* self(const clap_plugin_t* p) { return static_cast<TestFxPlugin*>(p->plugin_data); }

double clampDouble(double v, double lo, double hi) { return std::min(hi, std::max(lo, v)); }

void setParam(TestFxPlugin* s, clap_id id, double value) {
  if (id == kParamGain) {
    s->gainDb = clampDouble(value, kGainMin, kGainMax);
    s->gainLin = std::pow(10.0, s->gainDb / 20.0);
  } else if (id == kParamDelay) {
    s->delayOn = (int)std::lround(clampDouble(value, 0.0, 1.0));
  }
}

void clearDelayLine(TestFxPlugin* s) {
  std::memset(s->delayLine, 0, sizeof(s->delayLine));
  s->delayPos = 0;
}

// ---------------------------------------------------------------------------------------------------------------
// clap.params

uint32_t paramsCount(const clap_plugin_t*) { return kParamCount; }

bool paramsInfo(const clap_plugin_t*, uint32_t i, clap_param_info_t* info) {
  if (i >= kParamCount) return false;
  std::memset(info, 0, sizeof(*info));
  info->id = i;
  info->flags = CLAP_PARAM_IS_AUTOMATABLE;
  if (i == kParamGain) {
    std::snprintf(info->name, sizeof(info->name), "Gain");
    info->min_value = kGainMin;
    info->max_value = kGainMax;
    info->default_value = kGainDefault;
  } else {
    info->flags |= CLAP_PARAM_IS_STEPPED;
    std::snprintf(info->name, sizeof(info->name), "Delay");
    info->min_value = 0;
    info->max_value = 1;
    info->default_value = 0;
  }
  return true;
}

bool paramsGetValue(const clap_plugin_t* p, clap_id id, double* out) {
  TestFxPlugin* s = self(p);
  if (id == kParamGain) *out = s->gainDb;
  else if (id == kParamDelay) *out = (double)s->delayOn;
  else return false;
  return true;
}

bool paramsValueToText(const clap_plugin_t*, clap_id id, double value, char* out, uint32_t cap) {
  if (id == kParamGain) {
    std::snprintf(out, cap, "%.1f dB", clampDouble(value, kGainMin, kGainMax));
    return true;
  }
  if (id == kParamDelay) {
    std::snprintf(out, cap, "%s", std::lround(clampDouble(value, 0.0, 1.0)) ? "on" : "off");
    return true;
  }
  return false;
}

bool paramsTextToValue(const clap_plugin_t*, clap_id id, const char* text, double* out) {
  if (id != kParamGain && id != kParamDelay) return false;
  char* end = nullptr;
  double v = std::strtod(text, &end);
  if (end == text) {
    if (id == kParamDelay) {
      if (!std::strcmp(text, "on")) { *out = 1; return true; }
      if (!std::strcmp(text, "off")) { *out = 0; return true; }
    }
    return false;
  }
  *out = v;
  return true;
}

void applyEvents(TestFxPlugin* s, const clap_input_events_t* ev, uint32_t& cursor, uint32_t upto, uint32_t total) {
  while (cursor < total) {
    const clap_event_header_t* h = ev->get(ev, cursor);
    if (h->time > upto) break;
    ++cursor;
    if (h->space_id != CLAP_CORE_EVENT_SPACE_ID) continue;
    if (h->type == CLAP_EVENT_PARAM_VALUE) {
      auto* e = reinterpret_cast<const clap_event_param_value_t*>(h);
      setParam(s, e->param_id, e->value);
    }
  }
}

void paramsFlush(const clap_plugin_t* p, const clap_input_events_t* in, const clap_output_events_t*) {
  TestFxPlugin* s = self(p);
  uint32_t total = in->size(in), cursor = 0;
  applyEvents(s, in, cursor, 0xffffffffu, total);
}

const clap_plugin_params_t kParams = {paramsCount, paramsInfo, paramsGetValue, paramsValueToText, paramsTextToValue,
                                      paramsFlush};

// ---------------------------------------------------------------------------------------------------------------
// clap.state: [version=1][gain double][delay double]

bool stateSave(const clap_plugin_t* p, const clap_ostream_t* out) {
  TestFxPlugin* s = self(p);
  unsigned char buf[1 + 2 * sizeof(double)];
  buf[0] = kStateVersion;
  double vals[2] = {s->gainDb, (double)s->delayOn};
  std::memcpy(buf + 1, vals, sizeof(vals));
  int64_t done = 0;
  while (done < (int64_t)sizeof(buf)) {
    int64_t n = out->write(out, buf + done, sizeof(buf) - done);
    if (n <= 0) return false;
    done += n;
  }
  return true;
}

bool stateLoad(const clap_plugin_t* p, const clap_istream_t* in) {
  unsigned char buf[1 + 2 * sizeof(double) + 1];
  int64_t total = 0;
  for (;;) {
    int64_t n = in->read(in, buf + total, sizeof(buf) - total);
    if (n < 0) return false;
    if (n == 0) break;
    total += n;
    if (total >= (int64_t)sizeof(buf)) break;
  }
  if (total != (int64_t)(1 + 2 * sizeof(double)) || buf[0] != kStateVersion) return false;
  double vals[2];
  std::memcpy(vals, buf + 1, sizeof(vals));
  TestFxPlugin* s = self(p);
  setParam(s, kParamGain, vals[0]);
  setParam(s, kParamDelay, vals[1]);
  return true;
}

const clap_plugin_state_t kState = {stateSave, stateLoad};

// ---------------------------------------------------------------------------------------------------------------
// clap.audio-ports: one stereo input, one stereo output

uint32_t audioPortsCount(const clap_plugin_t*, bool) { return 1; }
bool audioPortsGet(const clap_plugin_t*, uint32_t i, bool isInput, clap_audio_port_info_t* info) {
  if (i != 0) return false;
  std::memset(info, 0, sizeof(*info));
  info->id = 0;
  std::snprintf(info->name, sizeof(info->name), "main");
  info->flags = CLAP_AUDIO_PORT_IS_MAIN;
  info->channel_count = 2;
  info->port_type = CLAP_PORT_STEREO;
  info->in_place_pair = isInput ? 0 : CLAP_INVALID_ID;
  return true;
}
const clap_plugin_audio_ports_t kAudioPorts = {audioPortsCount, audioPortsGet};

// ---------------------------------------------------------------------------------------------------------------
// clap.latency / clap.tail

uint32_t latencyGet(const clap_plugin_t* p) { return self(p)->delayOn ? kDelaySamples : 0; }
const clap_plugin_latency_t kLatency = {latencyGet};

uint32_t tailGet(const clap_plugin_t*) { return 0; }
const clap_plugin_tail_t kTail = {tailGet};

// ---------------------------------------------------------------------------------------------------------------
// clap.webview/3 — no editor page; receive() carries wclap.files/1 frames (wclap-rack SPEC §8).
// Frame: u8 type, u8 flags, u16 reserved, payload. Type 240 = write file (u16 pathLen, path, u32
// dataLen, data); type 241 = clear all received files.

int32_t webviewGetUri(const clap_plugin_t*, char* uri, uint32_t cap) {
  if (cap > 0 && uri != nullptr) uri[0] = '\0';
  return 1;
}
bool webviewGetResource(const clap_plugin_t*, const char*, char*, uint32_t, const clap_ostream_t*) { return false; }

bool webviewReceive(const clap_plugin_t* p, const void* buffer, uint32_t size) {
  TestFxPlugin* s = self(p);
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
  h ^= 0; h *= 0x01000193u;  // path/data separator
  for (uint32_t i = 0; i < dataLen; ++i) { h ^= data[i]; h *= 0x01000193u; }
  s->filesDigest = h;
  std::printf("wclap.files/1 write path=%s bytes=%u files=%u digest=%08x\n", path.c_str(), dataLen,
              (unsigned)s->files.size(), s->filesDigest);
  return true;
}

const clap_plugin_webview_t kWebview = {webviewGetUri, webviewGetResource, webviewReceive};

// ---------------------------------------------------------------------------------------------------------------
// plugin

bool pluginInit(const clap_plugin_t*) { return true; }
void pluginDestroy(const clap_plugin_t* p) {
  TestFxPlugin* s = self(p);
  s->~TestFxPlugin();
  std::free(s);
}

bool pluginActivate(const clap_plugin_t* p, double, uint32_t, uint32_t) {
  TestFxPlugin* s = self(p);
  clearDelayLine(s);
  s->active = true;
  return true;
}
void pluginDeactivate(const clap_plugin_t* p) { self(p)->active = false; }
bool pluginStartProcessing(const clap_plugin_t*) { return true; }
void pluginStopProcessing(const clap_plugin_t*) {}
void pluginReset(const clap_plugin_t* p) { clearDelayLine(self(p)); }  // param values remain unchanged

clap_process_status pluginProcess(const clap_plugin_t* p, const clap_process_t* proc) {
  TestFxPlugin* s = self(p);
  if (!s->active || proc->audio_outputs_count < 1) return CLAP_PROCESS_ERROR;
  // no audio input (rendered as a rack source): the input is silence
  const clap_audio_buffer_t* in = proc->audio_inputs_count > 0 ? &proc->audio_inputs[0] : nullptr;
  const clap_audio_buffer_t* out = &proc->audio_outputs[0];
  if (!out->data32 || out->channel_count < 1) return CLAP_PROCESS_ERROR;
  if (in && (!in->data32 || in->channel_count < 1)) return CLAP_PROCESS_ERROR;
  const uint32_t frames = proc->frames_count;
  const uint32_t inCh = in ? std::min(in->channel_count, 2u) : 0u;
  const uint32_t outCh = out->channel_count;
  const uint32_t total = proc->in_events ? proc->in_events->size(proc->in_events) : 0;
  uint32_t cursor = 0;
  for (uint32_t i = 0; i < frames; ++i) {
    if (proc->in_events) applyEvents(s, proc->in_events, cursor, i, total);
    for (uint32_t ch = 0; ch < outCh; ++ch) {
      const float x = (in && ch < inCh) ? in->data32[ch][i] : 0.0f;  // read before writing: in-place safe
      float y;
      if (ch < 2) {
        y = s->delayOn ? s->delayLine[ch][s->delayPos] : x;
        s->delayLine[ch][s->delayPos] = x;  // the line stays fed, so toggling the delay on is seamless
      } else {
        y = x;
      }
      out->data32[ch][i] = y * (float)s->gainLin;
    }
    if (++s->delayPos >= kDelaySamples) s->delayPos = 0;
  }
  if (proc->in_events) applyEvents(s, proc->in_events, cursor, 0xffffffffu, total);
  return CLAP_PROCESS_CONTINUE;
}

const void* pluginGetExtension(const clap_plugin_t*, const char* id) {
  if (!std::strcmp(id, CLAP_EXT_AUDIO_PORTS)) return &kAudioPorts;
  if (!std::strcmp(id, CLAP_EXT_PARAMS)) return &kParams;
  if (!std::strcmp(id, CLAP_EXT_STATE)) return &kState;
  if (!std::strcmp(id, CLAP_EXT_LATENCY)) return &kLatency;
  if (!std::strcmp(id, CLAP_EXT_TAIL)) return &kTail;
  if (!std::strcmp(id, CLAP_EXT_WEBVIEW)) return &kWebview;
  return nullptr;
}
void pluginOnMainThread(const clap_plugin_t*) {}

const clap_plugin_t* createPlugin(const clap_host_t* host) {
  void* mem = std::calloc(1, sizeof(TestFxPlugin));  // zeroed storage, then value-initialised members
  TestFxPlugin* s = new (mem) TestFxPlugin();
  s->host = host;
  setParam(s, kParamGain, kGainDefault);
  setParam(s, kParamDelay, 0);
  s->plugin = {&kDescriptor, s, pluginInit, pluginDestroy, pluginActivate, pluginDeactivate, pluginStartProcessing,
               pluginStopProcessing, pluginReset, pluginProcess, pluginGetExtension, pluginOnMainThread};
  return &s->plugin;
}

// factory

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
