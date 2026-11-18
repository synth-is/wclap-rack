// cdp-fx — WCLAP audio-fx placeholder for a CDP (Composer's Desktop Project) effect stage.
//
// CDP effects are OFFLINE programs (file -> file, see cdp-wasm-suite): they cannot run in
// a realtime CLAP process(). So this module is a bit-transparent stereo passthrough whose
// params select the effect; the actual processing happens on the HOST side, in the
// wclap.cdp/1 bridge (reference-host/cdp-bridge.mjs): when a rack chains this device after
// a cdp-engine source, the page-side CDPBridge reads this device's params (from the rack
// document) and applies the catalog effect to each rendered note between the generator
// render and the cdp_deliver — see cdpFxStagesFromRack in the web app's wclapRackHost.js
// and CDPBridge.setFxChain. At extract/QD time the same chain runs over the full rendered
// audio via CDPBridge.applyEffectToWav / applyEffectIndexToWav (tools/qd-rack.mjs).
//
// Params: 0 = Effect (catalog index into engines/cdp-engine/catalog.json effects),
//         1..12 = positional P1..P12 for that effect (-1e9 = catalog default).
// In a DAW or a host without the bridge this device is a plain passthrough.
//
// MIT, from scratch — links nothing of the CDP suite (see engines/cdp-engine/NOTICE).

#include <clap/clap.h>
#include <clap/ext/audio-ports.h>
#include <clap/ext/gui.h>
#include <clap/ext/params.h>
#include <clap/ext/state.h>
#include <clap/ext/draft/webview.h>

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

namespace {

constexpr clap_id kParamEffect = 0;
constexpr clap_id kParamP1 = 1;  // .. kParamP12 = 12
constexpr uint32_t kParamCount = 13;
constexpr double kUnset = -1e9;
constexpr int kStateVersion = 1;

const char* const kFeatures[] = {CLAP_PLUGIN_FEATURE_AUDIO_EFFECT, CLAP_PLUGIN_FEATURE_STEREO, nullptr};

const clap_plugin_descriptor_t kDescriptor = {
    CLAP_VERSION_INIT,
    "is.synth.kromosynth.cdp-fx",
    "Kromosynth CDP FX (offline stage)",
    "kromosynth-burst",
    nullptr,
    nullptr,
    nullptr,
    "1.0.0",
    "Passthrough placeholder for a CDP effect; the host's wclap.cdp/1 bridge applies the catalog effect offline per rendered note (see cdp-engine)",
    kFeatures};

struct CdpFx {
  clap_plugin_t plugin;
  const clap_host_t* host = nullptr;
  const clap_host_webview_t* hostWebview = nullptr;
  double sampleRate = 48000;
  double effect = 0;
  double p[12];
  bool paramOut[kParamCount];
  bool active = false;
  bool guiOpen = false;
  CdpFx() {
    for (auto& v : p) v = kUnset;
    for (auto& o : paramOut) o = false;
  }
};

CdpFx* self(const clap_plugin_t* p) { return static_cast<CdpFx*>(p->plugin_data); }
double clampDouble(double v, double lo, double hi) { return std::min(hi, std::max(lo, v)); }

bool pluginInit(const clap_plugin_t* p) {
  CdpFx* s = self(p);
  if (s->host && s->host->get_extension)
    s->hostWebview = static_cast<const clap_host_webview_t*>(s->host->get_extension(s->host, CLAP_EXT_WEBVIEW));
  return true;
}
void pluginDestroy(const clap_plugin_t*) {}
bool pluginActivate(const clap_plugin_t* p, double sampleRate, uint32_t, uint32_t) {
  self(p)->sampleRate = sampleRate;
  self(p)->active = true;
  return true;
}
void pluginDeactivate(const clap_plugin_t* p) { self(p)->active = false; }
bool pluginStartProcessing(const clap_plugin_t*) { return true; }
void pluginStopProcessing(const clap_plugin_t*) {}
void pluginReset(const clap_plugin_t*) {}

// ── clap.params ────────────────────────────────────────────────────────────────────────────────────

uint32_t paramsCount(const clap_plugin_t*) { return kParamCount; }

bool paramsInfo(const clap_plugin_t*, uint32_t i, clap_param_info_t* info) {
  if (i >= kParamCount || !info) return false;
  std::memset(info, 0, sizeof(*info));
  info->id = i;
  info->flags = CLAP_PARAM_IS_AUTOMATABLE;
  info->cookie = nullptr;
  if (i == kParamEffect) {
    std::snprintf(info->name, sizeof(info->name), "Effect");
    std::snprintf(info->module, sizeof(info->module), "cdp catalog index (see cdp-engine/catalog.json effects)");
    info->min_value = 0;
    info->max_value = 255;
    info->default_value = 0;
  } else {
    std::snprintf(info->name, sizeof(info->name), "P%d", i);
    std::snprintf(info->module, sizeof(info->module), "positional effect param (see catalog)");
    info->min_value = -1e9;
    info->max_value = 1e9;
    info->default_value = kUnset;
  }
  return true;
}

double paramGet(CdpFx* s, clap_id id) {
  if (id == kParamEffect) return s->effect;
  return id >= kParamP1 && id < kParamCount ? s->p[id - kParamP1] : 0;
}

void paramSet(CdpFx* s, clap_id id, double v) {
  if (id == kParamEffect) {
    s->effect = clampDouble(std::floor(v + 0.5), 0, 255);
    // P1..P12 are positional per effect: reset to catalog defaults on an effect change
    // (stale values from the previous effect are usually out of range for the new one)
    // and report the reset to the host/editor page as out-events.
    for (int i = 0; i < 12; ++i) {
      s->p[i] = kUnset;
      s->paramOut[kParamP1 + i] = true;
    }
  } else if (id >= kParamP1 && id < kParamCount) {
    s->p[id - kParamP1] = v;
  }
}

bool paramsGetValue(const clap_plugin_t* p, clap_id id, double* out) {
  if (id >= kParamCount || !out) return false;
  *out = paramGet(self(p), id);
  return true;
}

bool paramsValueToText(const clap_plugin_t* p, clap_id id, double value, char* out, uint32_t cap) {
  if (id >= kParamCount || !out) return false;
  if (id == kParamEffect) {
    std::snprintf(out, cap, "%d", (int)clampDouble(std::floor(value + 0.5), 0, 255));
  } else if (value <= kUnset + 1) {
    std::snprintf(out, cap, "default");
  } else {
    std::snprintf(out, cap, "%.6g", value);
  }
  (void)p;
  return true;
}

bool paramsTextToValue(const clap_plugin_t*, clap_id id, const char* text, double* out) {
  if (id >= kParamCount || !text || !out) return false;
  char* end = nullptr;
  double v = std::strtod(text, &end);
  if (end == text) return false;
  *out = v;
  return true;
}

void paramsFlush(const clap_plugin_t* p, const clap_input_events_t* in, const clap_output_events_t*) {
  CdpFx* s = self(p);
  const uint32_t total = in->size(in);
  for (uint32_t i = 0; i < total; ++i) {
    const clap_event_header_t* h = in->get(in, i);
    if (h->space_id != CLAP_CORE_EVENT_SPACE_ID || h->type != CLAP_EVENT_PARAM_VALUE) continue;
    auto* e = reinterpret_cast<const clap_event_param_value_t*>(h);
    if (e->param_id < kParamCount) paramSet(s, e->param_id, e->value);
  }
}

const clap_plugin_params_t kParams = {paramsCount, paramsInfo, paramsGetValue, paramsValueToText, paramsTextToValue,
                                      paramsFlush};

// ── clap.state ─────────────────────────────────────────────────────────────────────────────────────

bool stateSave(const clap_plugin_t* p, const clap_ostream_t* out) {
  CdpFx* s = self(p);
  std::vector<unsigned char> buf;
  buf.push_back(kStateVersion);
  double vals[13];
  vals[0] = s->effect;
  for (int i = 0; i < 12; ++i) vals[1 + i] = s->p[i];
  buf.insert(buf.end(), (unsigned char*)vals, (unsigned char*)vals + sizeof(vals));
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
  if (buf.size() < 1 + 13 * 8 || buf[0] != kStateVersion) return false;
  double vals[13];
  std::memcpy(vals, buf.data() + 1, sizeof(vals));
  CdpFx* s = self(p);
  s->effect = clampDouble(std::floor(vals[0] + 0.5), 0, 255);
  for (int i = 0; i < 12; ++i) s->p[i] = vals[1 + i];
  return true;
}

const clap_plugin_state_t kState = {stateSave, stateLoad};

// ── clap.audio-ports: one stereo in, one stereo out ────────────────────────────────────────────────

uint32_t audioPortsCount(const clap_plugin_t*, bool isInput) { return 1; }
bool audioPortsGet(const clap_plugin_t*, uint32_t i, bool isInput, clap_audio_port_info_t* info) {
  if (i != 0 || !info) return false;
  std::memset(info, 0, sizeof(*info));
  info->id = 0;
  std::snprintf(info->name, sizeof(info->name), "%s", isInput ? "main in" : "main out");
  info->flags = CLAP_AUDIO_PORT_IS_MAIN;
  info->channel_count = 2;
  info->port_type = CLAP_PORT_STEREO;
  info->in_place_pair = isInput ? 0 : CLAP_INVALID_ID;
  return true;
}
const clap_plugin_audio_ports_t kAudioPorts = {audioPortsCount, audioPortsGet};

// ── clap.webview/3 + clap.gui: the editor page ui/index.html ships in the bundle ─────────────────────
//
//   page -> plugin   64 hello            (reply: 80 description, 82 values)
//                    65 set parameter    u32 id, f64 value
//                    69 poll             (reply: 82 values)
//   plugin -> page   80 description      JSON {params:[{id,name,min,max,def}]}
//                    82 values           u32 count, then per parameter f64 value
// The page merges the description with ui/catalog.json (a copy of the cdp-engine catalog's
// effects, shipped next to this page) for effect names + per-effect param labels.
// Param changes from the page are reported to the host as PARAM_VALUE out-events in process(),
// so the host-side wclap.cdp/1 bridge picks them up for the offline chain (paramOut -> page).

const char kUiUri[] = "ui/index.html";
int32_t webviewGetUri(const clap_plugin_t*, char* uri, uint32_t cap) {
  if (cap > 0 && uri != nullptr) std::snprintf(uri, cap, "%s", kUiUri);
  return (int32_t)sizeof(kUiUri);
}
bool webviewGetResource(const clap_plugin_t*, const char*, char*, uint32_t, const clap_ostream_t*) { return false; }

void pageSend(CdpFx* s, uint8_t type, const void* payload, size_t n) {
  if (!s->guiOpen || !s->hostWebview || !s->hostWebview->send) return;
  std::vector<uint8_t> f(4 + n);
  f[0] = type;
  if (n) std::memcpy(f.data() + 4, payload, n);
  s->hostWebview->send(s->host, f.data(), (uint32_t)f.size());
}

void pageSendDescription(CdpFx* s) {
  char buf[2048];
  int n = std::snprintf(buf, sizeof(buf), "{\"params\":[");
  for (uint32_t i = 0; i < kParamCount && n > 0 && n < (int)sizeof(buf) - 96; ++i) {
    clap_param_info_t info;
    if (!paramsInfo(nullptr, i, &info)) continue;
    n += std::snprintf(buf + n, sizeof(buf) - n, "%s{\"id\":%u,\"name\":\"%s\",\"min\":%.9g,\"max\":%.9g,\"def\":%.9g}",
                       i ? "," : "", i, info.name, info.min_value, info.max_value, info.default_value);
  }
  if (n > 0 && n < (int)sizeof(buf) - 8) n += std::snprintf(buf + n, sizeof(buf) - n, "]}");
  pageSend(s, 80, buf, (size_t)std::max(n, 0));
}

void pageSendValues(CdpFx* s) {
  std::vector<uint8_t> f(4 + 4 + kParamCount * 8);
  f[0] = 82;
  uint32_t count = kParamCount;
  std::memcpy(f.data() + 4, &count, 4);
  for (uint32_t i = 0; i < kParamCount; ++i) {
    double v = paramGet(s, i);
    std::memcpy(f.data() + 8 + i * 8, &v, 8);
  }
  pageSend(s, 82, f.data() + 4, f.size() - 4);
}

bool webviewReceive(const clap_plugin_t* p, const void* buffer, uint32_t size) {
  CdpFx* s = self(p);
  if (size < 4) return false;
  const uint8_t* b = static_cast<const uint8_t*>(buffer);
  const uint8_t* pl = b + 4;
  const uint32_t plen = size - 4;
  if (b[0] < 64 || b[0] >= 80) return false;
  switch (b[0]) {
    case 64:  // hello
      pageSendDescription(s);
      pageSendValues(s);
      return true;
    case 65: {  // set parameter
      if (plen < 12) return false;
      uint32_t id;
      double v;
      std::memcpy(&id, pl, 4);
      std::memcpy(&v, pl + 4, 8);
      if (id < kParamCount) {
        paramSet(s, id, v);
        s->paramOut[id] = true;
      }
      return true;
    }
    case 69:  // poll
      pageSendValues(s);
      return true;
    default:
      return false;
  }
}

const clap_plugin_webview_t kWebview = {webviewGetUri, webviewGetResource, webviewReceive};

// ── clap.gui: the "webview" window api only (the host shows ui/index.html) ─────────────────────────────

bool guiIsApiSupported(const clap_plugin_t*, const char* api, bool floating) {
  return api && !std::strcmp(api, CLAP_WINDOW_API_WEBVIEW) && !floating;
}
bool guiGetPreferredApi(const clap_plugin_t*, const char** api, bool* floating) {
  *api = CLAP_WINDOW_API_WEBVIEW;
  *floating = false;
  return true;
}
bool guiCreate(const clap_plugin_t* p, const char* api, bool floating) {
  if (!guiIsApiSupported(p, api, floating)) return false;
  self(p)->guiOpen = true;
  return true;
}
void guiDestroy(const clap_plugin_t* p) { self(p)->guiOpen = false; }
bool guiSetScale(const clap_plugin_t*, double) { return false; }
bool guiGetSize(const clap_plugin_t*, uint32_t* w, uint32_t* h) {
  *w = 560;
  *h = 480;
  return true;
}
bool guiCanResize(const clap_plugin_t*) { return false; }
bool guiGetResizeHints(const clap_plugin_t*, clap_gui_resize_hints_t*) { return false; }
bool guiAdjustSize(const clap_plugin_t*, uint32_t* w, uint32_t* h) { *w = 560; *h = 480; return true; }
bool guiSetSize(const clap_plugin_t*, uint32_t, uint32_t) { return false; }
bool guiSetParent(const clap_plugin_t*, const clap_window_t*) { return false; }
bool guiSetTransient(const clap_plugin_t*, const clap_window_t*) { return false; }
void guiSuggestTitle(const clap_plugin_t*, const char*) {}
bool guiShow(const clap_plugin_t*) { return true; }
bool guiHide(const clap_plugin_t*) { return true; }

const clap_plugin_gui_t kGui = {guiIsApiSupported, guiGetPreferredApi, guiCreate, guiDestroy, guiSetScale, guiGetSize,
                                guiCanResize,      guiGetResizeHints, guiAdjustSize, guiSetSize, guiSetParent,
                                guiSetTransient,   guiSuggestTitle,   guiShow,       guiHide};

// ── process: bit-transparent passthrough ──────────────────────────────────────────────────────────

clap_process_status pluginProcess(const clap_plugin_t* p, const clap_process_t* proc) {
  CdpFx* s = self(p);
  if (!s->active || proc->audio_inputs_count < 1 || proc->audio_outputs_count < 1) return CLAP_PROCESS_ERROR;
  const clap_audio_buffer_t* in = &proc->audio_inputs[0];
  const clap_audio_buffer_t* out = &proc->audio_outputs[0];
  if (!in->data32 || !out->data32) return CLAP_PROCESS_ERROR;
  const uint32_t frames = proc->frames_count;
  const uint32_t ch = std::min(in->channel_count, out->channel_count);
  for (uint32_t c = 0; c < ch; ++c)
    std::memcpy(out->data32[c], in->data32[c], frames * sizeof(float));
  if (proc->in_events) {
    const uint32_t total = proc->in_events->size(proc->in_events);
    for (uint32_t i = 0; i < total; ++i) {
      const clap_event_header_t* h = proc->in_events->get(proc->in_events, i);
      if (h->space_id != CLAP_CORE_EVENT_SPACE_ID) continue;
      if (h->type == CLAP_EVENT_PARAM_VALUE) {
        auto* e = reinterpret_cast<const clap_event_param_value_t*>(h);
        if (e->param_id < kParamCount) paramSet(s, e->param_id, e->value);
      }
    }
  }
  // parameters moved by the editor page are reported to the host
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
        ev.note_id = -1;
        ev.port_index = -1;
        ev.channel = -1;
        ev.key = -1;
        ev.value = paramGet(s, i);
        proc->out_events->try_push(proc->out_events, &ev.header);
        s->paramOut[i] = false;
      }
  return CLAP_PROCESS_CONTINUE;
}

const void* pluginGetExtension(const clap_plugin_t*, const char* id) {
  if (!std::strcmp(id, CLAP_EXT_AUDIO_PORTS)) return &kAudioPorts;
  if (!std::strcmp(id, CLAP_EXT_PARAMS)) return &kParams;
  if (!std::strcmp(id, CLAP_EXT_STATE)) return &kState;
  if (!std::strcmp(id, CLAP_EXT_WEBVIEW)) return &kWebview;
  if (!std::strcmp(id, CLAP_EXT_GUI)) return &kGui;
  return nullptr;
}
void pluginOnMainThread(const clap_plugin_t*) {}

const clap_plugin_t* createPlugin(const clap_host_t* host) {
  void* mem = std::calloc(1, sizeof(CdpFx));
  CdpFx* s = new (mem) CdpFx();
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
