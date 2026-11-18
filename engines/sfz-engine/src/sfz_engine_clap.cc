// Copyright 2026 Björn Þór Jónsson (kromosynth-burst). Licensed under the MIT License; see ../LICENSE.
//
// sfz-engine: a minimal SFZ sampler engine as a WCLAP instrument, the reference engine plugin for
// the wclap-rack standard's file delivery (SPEC §8, wclap.files/1). Written from scratch for
// kromosynth-burst; layout and build mirror tests/bundles/testfx (engines/<name>).
//
//   * Feature: instrument + sampler. No audio input; one stereo audio output ("main"). One note input
//     port (clap.note-ports, CLAP dialect; raw MIDI note on/off is also understood).
//   * Files: the host delivers the instrument files (the .sfz and its samples) BEFORE activate via
//     wclap.files/1 binary frames on the clap.webview/3 port channel (SPEC §8): type 240 writes a file
//     (u16 pathLen, path, u32 dataLen, data) into the plugin's in-memory file map, type 241 clears it.
//     Every frame is logged to stdout with an FNV-1a digest of path+bytes (same format as testfx), so
//     hosts/tests can assert delivery. On activate the engine picks the first delivered "*.sfz", parses
//     it, and loads the samples it references (paths relative to the .sfz, '\' normalized to '/').
//   * SFZ subset: <global>/<group>/<master>/<region> headers with opcode=value pairs (// comments);
//     region opcodes sample, key, lokey, hikey, pitch_keycenter, lovel, hivel, volume, pan, tune, offset,
//     loop_mode (no_loop/loop_continuous/one_shot), loop_start, loop_end, ampeg_attack/hold/decay/
//     sustain/release. Multiple matching regions stack (one voice each), as in SFZ.
//   * WAV subset: RIFF/WAVE, PCM 16/24/32-bit or IEEE float 32-bit, any channel count (first two used),
//     linear-interpolated playback at sample.sampleRate * 2^((key - pitch_keycenter + tune/100)/12).
//   * Voices: fixed pool of 32, no allocation in process(); note on/off sample-accurate; amp envelope
//     attack -> hold -> decay -> sustain -> release (linear segments); velocity -> gain as vel^2;
//     constant-power pan. Deterministic (no RNG), any block size.
//   * Parameters (clap.params): id 0 "Volume" dB, -60..+12, default 0. State: [u8 version][f64 volume]
//     [u16 sfz path length][path bytes]. clap.latency = 0; clap.tail = the longest ampeg_release.
//   * No editor page (clap.webview/3 get_uri is empty); receive() carries only wclap.files/1 frames.

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

namespace {

constexpr clap_id kParamVolume = 0;
constexpr uint32_t kParamCount = 1;
constexpr double kVolMin = -60.0;
constexpr double kVolMax = 12.0;
constexpr double kVolDefault = 0.0;
constexpr uint32_t kVoiceCount = 32;
constexpr int kStateVersion = 1;

const char* const kFeatures[] = {CLAP_PLUGIN_FEATURE_INSTRUMENT, CLAP_PLUGIN_FEATURE_SAMPLER, nullptr};

const clap_plugin_descriptor_t kDescriptor = {
    CLAP_VERSION_INIT,
    "is.synth.kromosynth.sfz-engine",
    "Kromosynth SFZ Engine",
    "kromosynth-burst",
    nullptr,
    nullptr,
    nullptr,
    "1.0.0",
    "Minimal SFZ sampler engine; instrument files delivered via wclap.files/1 (wclap-rack SPEC §8)",
    kFeatures};

// ── samples and regions ────────────────────────────────────────────────────────────────────────────────

struct Sample {
  std::vector<float> data;  // interleaved channels
  uint32_t channels = 0;
  uint32_t frames = 0;
  double sampleRate = 0;
};

struct Region {
  std::string samplePath;  // as written in the .sfz
  const Sample* sample = nullptr;
  int lokey = 0, hikey = 127, pitchKeycenter = 60;
  int lovel = 0, hivel = 127;
  double volumeDb = 0, pan = 0, tuneCents = 0;
  uint32_t offset = 0;
  int loopMode = 0;  // 0 = no_loop, 1 = loop_continuous/loop_sustain, 2 = one_shot
  uint32_t loopStart = 0, loopEnd = 0;
  double attack = 0, hold = 0, decay = 0, sustain = 1.0, release = 0.01;  // seconds; sustain linear 0..1
};

struct Voice {
  bool active = false;
  uint64_t age = 0;
  const Region* region = nullptr;
  int key = 0;
  double velocity = 0;
  int32_t noteId = -1;
  int16_t channel = 0;
  double pos = 0;   // position in sample frames
  double step = 0;  // frames advanced per output sample
  double env = 0;
  int stage = 0;  // 0 attack, 1 hold, 2 decay, 3 sustain, 4 release
  uint32_t stageLeft = 0;
  double stageTarget = 0;
  double stageStep = 0;
};

struct SfzEngine {
  clap_plugin_t plugin;
  const clap_host_t* host;
  double sampleRate = 48000;
  double volumeDb = kVolDefault;
  // wclap.files/1 (SPEC §8): files delivered over the clap.webview/3 port before activate
  std::map<std::string, std::vector<uint8_t>> files;
  uint32_t filesDigest = 0x811c9dc5u;  // FNV-1a over path + 0 + bytes of every write since the last clear
  // the parsed instrument
  std::string sfzPath;
  std::vector<Sample> samples;
  std::vector<Region> regions;
  Voice voices[kVoiceCount];
  uint64_t voiceCounter = 0;
  uint32_t tailFrames = 0;
  bool active = false;
};

SfzEngine* self(const clap_plugin_t* p) { return static_cast<SfzEngine*>(p->plugin_data); }

double clampDouble(double v, double lo, double hi) { return std::min(hi, std::max(lo, v)); }

// ── WAV loading ────────────────────────────────────────────────────────────────────────────────────────

uint16_t rd16(const uint8_t* p) { return (uint16_t)(p[0] | (p[1] << 8)); }
uint32_t rd32(const uint8_t* p) {
  return (uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24);
}

bool loadWav(const uint8_t* d, size_t n, Sample& out) {
  if (n < 12 || std::memcmp(d, "RIFF", 4) || std::memcmp(d + 8, "WAVE", 4)) return false;
  uint16_t format = 0, channels = 0, bits = 0;
  uint32_t rate = 0;
  const uint8_t* data = nullptr;
  uint32_t dataLen = 0;
  size_t pos = 12;
  while (pos + 8 <= n) {
    const uint8_t* body = d + pos + 8;
    const uint32_t sz = rd32(d + pos + 4);
    if (!std::memcmp(d + pos, "fmt ", 4) && sz >= 16 && pos + 8 + 16 <= n) {
      format = rd16(body);
      channels = rd16(body + 2);
      rate = rd32(body + 4);
      bits = rd16(body + 14);
    } else if (!std::memcmp(d + pos, "data", 4) && pos + 8 + sz <= n) {
      data = body;
      dataLen = sz;
    }
    pos += 8 + sz + (sz & 1);
  }
  if (!data || !rate || !channels || !bits) return false;
  if (format != 1 && format != 3) return false;  // PCM or IEEE float
  if (format == 1 && bits != 16 && bits != 24 && bits != 32) return false;
  if (format == 3 && bits != 32) return false;
  const uint32_t bytesPer = bits / 8;
  if (channels > 8) return false;
  const uint32_t frames = dataLen / (channels * bytesPer);
  out.channels = channels;
  out.frames = frames;
  out.sampleRate = rate;
  out.data.resize((size_t)frames * channels);
  for (uint32_t f = 0; f < frames; ++f) {
    for (uint32_t c = 0; c < channels; ++c) {
      const uint8_t* p = data + ((size_t)f * channels + c) * bytesPer;
      float v = 0;
      if (format == 3) {
        uint32_t u = rd32(p);
        float fv;
        std::memcpy(&fv, &u, 4);
        v = fv;
      } else if (bits == 16) {
        v = (int16_t)rd16(p) / 32768.0f;
      } else if (bits == 24) {
        int32_t s = (int32_t)((uint32_t)p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16));
        if (s & 0x800000) s |= ~0xffffff;  // sign extend
        v = s / 8388608.0f;
      } else {
        v = (int32_t)rd32(p) / 2147483648.0f;
      }
      out.data[(size_t)f * channels + c] = v;
    }
  }
  return true;
}

// ── SFZ parsing ────────────────────────────────────────────────────────────────────────────────────────

std::string trim(const std::string& s) {
  size_t a = s.find_first_not_of(" \t\r\n");
  if (a == std::string::npos) return "";
  size_t b = s.find_last_not_of(" \t\r\n");
  return s.substr(a, b - a + 1);
}

std::string dirOf(const std::string& path) {
  size_t s = path.find_last_of('/');
  return s == std::string::npos ? "" : path.substr(0, s + 1);
}

std::string normalizeSamplePath(const std::string& dir, const std::string& sample) {
  std::string p;
  p.reserve(dir.size() + sample.size());
  for (char c : dir) p += (c == '\\' ? '/' : c);
  for (char c : sample) p += (c == '\\' ? '/' : c);
  // collapse any leading "./" and duplicate slashes after the dir join
  std::string out;
  for (size_t i = 0; i < p.size(); ++i) {
    if (p[i] == '/' && !out.empty() && out.back() == '/') continue;
    out += p[i];
  }
  if (out.rfind("./", 0) == 0) out = out.substr(2);
  return out;
}

double opNum(const std::map<std::string, std::string>& cur, const std::map<std::string, std::string>& master,
             const std::map<std::string, std::string>& group, const std::map<std::string, std::string>& global,
             const char* key, double def) {
  for (const auto* m : {&cur, &master, &group, &global}) {
    auto it = m->find(key);
    if (it != m->end()) return std::strtod(it->second.c_str(), nullptr);
  }
  return def;
}

const std::string* opStr(const std::map<std::string, std::string>& cur,
                         const std::map<std::string, std::string>& master,
                         const std::map<std::string, std::string>& group,
                         const std::map<std::string, std::string>& global, const char* key) {
  for (const auto* m : {&cur, &master, &group, &global}) {
    auto it = m->find(key);
    if (it != m->end()) return &it->second;
  }
  return nullptr;
}

void parseSfz(SfzEngine* s, const std::string& sfzPath, const std::string& text) {
  std::map<std::string, std::string> globalOps, groupOps, masterOps, curOps, controlOps;
  std::map<std::string, std::string>* sink = nullptr;  // where opcodes of the current section go
  size_t i = 0;
  const size_t n = text.size();
  auto finalizeRegion = [&]() {
    Region r;
    const std::string* sample = opStr(curOps, masterOps, groupOps, globalOps, "sample");
    if (!sample || sample->empty()) return;
    r.samplePath = *sample;
    // key sets lokey=hikey=pitch_keycenter
    bool hasKey = opStr(curOps, masterOps, groupOps, globalOps, "key") != nullptr;
    double key = opNum(curOps, masterOps, groupOps, globalOps, "key", -1);
    double lokey = opNum(curOps, masterOps, groupOps, globalOps, "lokey", hasKey ? key : 0);
    double hikey = opNum(curOps, masterOps, groupOps, globalOps, "hikey", hasKey ? key : 127);
    double pkc = opNum(curOps, masterOps, groupOps, globalOps, "pitch_keycenter", hasKey ? key : (lokey > 0 ? lokey : 60));
    r.lokey = (int)clampDouble(lokey, 0, 127);
    r.hikey = (int)clampDouble(hikey, 0, 127);
    r.pitchKeycenter = (int)clampDouble(pkc, 0, 127);
    r.lovel = (int)clampDouble(opNum(curOps, masterOps, groupOps, globalOps, "lovel", 0), 0, 127);
    r.hivel = (int)clampDouble(opNum(curOps, masterOps, groupOps, globalOps, "hivel", 127), 0, 127);
    r.volumeDb = opNum(curOps, masterOps, groupOps, globalOps, "volume", 0);
    r.pan = clampDouble(opNum(curOps, masterOps, groupOps, globalOps, "pan", 0), -100, 100);
    r.tuneCents = opNum(curOps, masterOps, groupOps, globalOps, "tune", 0);
    r.offset = (uint32_t)std::max(0.0, opNum(curOps, masterOps, groupOps, globalOps, "offset", 0));
    const std::string* lm = opStr(curOps, masterOps, groupOps, globalOps, "loop_mode");
    r.loopMode = 0;
    if (lm) {
      if (*lm == "loop_continuous" || *lm == "loop_sustain") r.loopMode = 1;
      else if (*lm == "one_shot" || *lm == "one_shot_keyrelease") r.loopMode = 2;
    }
    r.attack = std::max(0.0, opNum(curOps, masterOps, groupOps, globalOps, "ampeg_attack", 0));
    r.hold = std::max(0.0, opNum(curOps, masterOps, groupOps, globalOps, "ampeg_hold", 0));
    r.decay = std::max(0.0, opNum(curOps, masterOps, groupOps, globalOps, "ampeg_decay", 0));
    double sus = opNum(curOps, masterOps, groupOps, globalOps, "ampeg_sustain", 100);
    r.sustain = sus >= 0 ? clampDouble(sus / 100.0, 0, 1) : clampDouble(std::pow(10.0, sus / 20.0), 0, 1);
    r.release = std::max(0.0, opNum(curOps, masterOps, groupOps, globalOps, "ampeg_release", 0.01));
    s->regions.push_back(r);
  };
  while (i < n) {
    char c = text[i];
    if ((unsigned char)c == ' ' || c == '\t' || c == '\r' || c == '\n') { ++i; continue; }
    if (c == '/' && i + 1 < n && text[i + 1] == '/') {
      while (i < n && text[i] != '\n') ++i;
      continue;
    }
    if (c == '<') {
      size_t j = text.find('>', i);
      if (j == std::string::npos) break;
      const std::string section = trim(text.substr(i + 1, j - i - 1));
      i = j + 1;
      // finalize the pending region on ANY header: the last region of a group must be finalized
      // before the next <group> overwrites groupOps
      finalizeRegion();
      curOps.clear();
      if (section == "region") {
        sink = &curOps;
      } else if (section == "group") {
        groupOps.clear();
        masterOps.clear();
        sink = &groupOps;
      } else if (section == "master") {
        masterOps.clear();
        sink = &masterOps;
      } else if (section == "global") {
        globalOps.clear();
        groupOps.clear();
        masterOps.clear();
        sink = &globalOps;
      } else if (section == "control") {
        controlOps.clear();
        sink = &controlOps;  // e.g. default_path=samples/
      } else {
        sink = nullptr;  // <curve>, <effect>, …: opcodes ignored
      }
      continue;
    }
    if (c == '\\' && (i + 1 >= n || text[i + 1] == '\n' || text[i + 1] == '\r' || text[i + 1] == ' ' ||
                      text[i + 1] == '\t')) {
      ++i;  // line continuation (a lone backslash at a token start); not a path separator here
      continue;
    }
    size_t eq = text.find('=', i);
    if (eq == std::string::npos) break;
    std::string opcode = trim(text.substr(i, eq - i));
    i = eq + 1;
    while (i < n && (text[i] == ' ' || text[i] == '\t')) ++i;
    std::string value;
    if (i < n && text[i] == '"') {
      size_t j = text.find('"', i + 1);
      if (j == std::string::npos) j = n;
      value = text.substr(i + 1, j - i - 1);
      i = j + 1;
    } else {
      size_t j = i;
      while (j < n && text[j] != ' ' && text[j] != '\t' && text[j] != '\r' && text[j] != '\n') ++j;
      value = text.substr(i, j - i);
      i = j;
    }
    if (!opcode.empty() && sink) (*sink)[opcode] = value;
  }
  finalizeRegion();  // the last <region> has no following header
  // resolve samples (honour <control> default_path=, as kromosynth SFZ renditions use it).
  // Reserve first: regions keep pointers into s->samples, which must not reallocate.
  s->samples.reserve(s->regions.size());
  const std::string dir = dirOf(sfzPath);
  std::string defaultPath;
  {
    auto it = controlOps.find("default_path");
    if (it != controlOps.end()) defaultPath = it->second;
  }
  for (auto& r : s->regions) {
    std::string rel = r.samplePath;
    if (!defaultPath.empty() && !rel.empty() && rel[0] != '/') rel = defaultPath + rel;
    const std::string path = normalizeSamplePath(dir, rel);
    auto it = s->files.find(path);
    if (it == s->files.end()) {
      // also try the raw/default_path-joined sample path (absolute delivery)
      it = s->files.find(r.samplePath);
      if (it == s->files.end() && !rel.empty() && rel != r.samplePath) it = s->files.find(rel);
      if (it == s->files.end()) {
        std::printf("sfz-engine: missing sample %s (region skipped)\n", path.c_str());
        r.sample = nullptr;
        continue;
      }
    }
    Sample smp;
    if (!loadWav(it->second.data(), it->second.size(), smp)) {
      std::printf("sfz-engine: cannot decode wav %s (region skipped)\n", path.c_str());
      r.sample = nullptr;
      continue;
    }
    std::printf("sfz-engine: sample %s frames=%u ch=%u sr=%u\n", path.c_str(), smp.frames, smp.channels,
                (unsigned)smp.sampleRate);
    if (r.loopMode == 1) {
      if (r.loopEnd == 0 || r.loopEnd >= smp.frames) r.loopEnd = smp.frames > 0 ? smp.frames - 1 : 0;
      if (r.loopStart >= smp.frames) r.loopStart = 0;
    }
    s->samples.push_back(std::move(smp));
    r.sample = &s->samples.back();
  }
  // drop regions whose sample failed (keep region list compact)
  s->regions.erase(std::remove_if(s->regions.begin(), s->regions.end(),
                                  [](const Region& r) { return r.sample == nullptr; }),
                   s->regions.end());
  // careful: r.sample pointers point into s->samples, which is stable now (no more push_back)
  double maxRelease = 0;
  for (const auto& r : s->regions) maxRelease = std::max(maxRelease, r.release);
  s->tailFrames = (uint32_t)std::min(maxRelease * s->sampleRate, 30.0 * s->sampleRate);
}

void loadInstrument(SfzEngine* s) {
  s->regions.clear();
  s->samples.clear();
  s->sfzPath.clear();
  // pick the first delivered *.sfz (sorted for determinism)
  std::string best;
  for (const auto& kv : s->files) {
    const std::string& p = kv.first;
    if (p.size() >= 4) {
      char e0 = p[p.size() - 4] == '.' ? p[p.size() - 3] : 0;
      (void)e0;
      std::string ext = p.size() >= 4 ? p.substr(p.size() - 4) : "";
      for (auto& c : ext) c = (char)std::tolower((unsigned char)c);
      if (ext == ".sfz" && (best.empty() || p < best)) best = p;
    }
  }
  if (best.empty()) {
    std::printf("sfz-engine: no .sfz among %u delivered file(s); instrument empty\n", (unsigned)s->files.size());
    s->tailFrames = 0;
    return;
  }
  s->sfzPath = best;
  auto it = s->files.find(best);
  std::string text(it->second.begin(), it->second.end());
  parseSfz(s, best, text);
  std::printf("sfz-engine: loaded sfz=%s regions=%u samples=%u tail=%u\n", best.c_str(), (unsigned)s->regions.size(),
              (unsigned)s->samples.size(), s->tailFrames);
}

// ── voices ─────────────────────────────────────────────────────────────────────────────────────────────

void setStage(SfzEngine* s, Voice& v, int stage) {
  const Region& r = *v.region;
  v.stage = stage;
  double durSec = 0, target = 0;
  switch (stage) {
    case 0: durSec = r.attack; target = 1.0; break;
    case 1: durSec = r.hold; target = 1.0; break;
    case 2: durSec = r.decay; target = r.sustain; break;
    case 3: v.stageLeft = 0xffffffffu; v.env = r.sustain; v.stageTarget = r.sustain; v.stageStep = 0; return;
    case 4: durSec = r.release; target = 0.0; break;
  }
  uint32_t dur = (uint32_t)std::max(1.0, durSec * s->sampleRate);
  v.stageLeft = dur;
  v.stageTarget = target;
  v.stageStep = (target - v.env) / (double)dur;
}

void advanceStage(SfzEngine* s, Voice& v) {
  if (v.stage < 4) setStage(s, v, v.stage + 1);
  if (v.stage == 4 && v.env <= 0.0) v.active = false;
}

void noteOn(SfzEngine* s, int key, double velocity, int32_t noteId, int16_t channel) {
  const int vel7 = (int)clampDouble(velocity * 127.0, 0, 127);
  for (const auto& r : s->regions) {
    if (key < r.lokey || key > r.hikey || vel7 < r.lovel || vel7 > r.hivel) continue;
    Voice* v = nullptr;
    for (auto& cand : s->voices)
      if (!cand.active) { v = &cand; break; }
    if (!v) {  // steal the oldest
      v = &s->voices[0];
      for (auto& cand : s->voices)
        if (cand.age < v->age) v = &cand;
    }
    v->active = true;
    v->age = ++s->voiceCounter;
    v->region = &r;
    v->key = key;
    v->velocity = velocity;
    v->noteId = noteId;
    v->channel = channel;
    v->pos = std::min((double)r.offset, (double)(r.sample->frames > 0 ? r.sample->frames - 1 : 0));
    v->step = r.sample->sampleRate * std::pow(2.0, (key - r.pitchKeycenter + r.tuneCents / 100.0) / 12.0) /
              s->sampleRate;
    v->env = 0;
    setStage(s, *v, 0);
  }
}

void noteOff(SfzEngine* s, int key, int32_t noteId, int16_t channel, bool choke) {
  for (auto& v : s->voices) {
    if (!v.active || v.key != key) continue;
    if (noteId >= 0 && v.noteId >= 0 && v.noteId != noteId) continue;
    if (channel >= 0 && v.channel != channel) continue;
    if (choke || v.region->loopMode == 2) {
      if (choke) {
        v.active = false;
        v.env = 0;
      }
      continue;  // one_shot ignores note off
    }
    setStage(s, v, 4);
  }
}

// ── clap.params ────────────────────────────────────────────────────────────────────────────────────────

uint32_t paramsCount(const clap_plugin_t*) { return kParamCount; }

bool paramsInfo(const clap_plugin_t*, uint32_t i, clap_param_info_t* info) {
  if (i >= kParamCount) return false;
  std::memset(info, 0, sizeof(*info));
  info->id = kParamVolume;
  info->flags = CLAP_PARAM_IS_AUTOMATABLE;
  std::snprintf(info->name, sizeof(info->name), "Volume");
  info->module[0] = '\0';
  info->min_value = kVolMin;
  info->max_value = kVolMax;
  info->default_value = kVolDefault;
  return true;
}

bool paramsGetValue(const clap_plugin_t* p, clap_id id, double* out) {
  if (id != kParamVolume) return false;
  *out = self(p)->volumeDb;
  return true;
}

bool paramsValueToText(const clap_plugin_t*, clap_id id, double value, char* out, uint32_t cap) {
  if (id != kParamVolume) return false;
  std::snprintf(out, cap, "%.1f dB", clampDouble(value, kVolMin, kVolMax));
  return true;
}

bool paramsTextToValue(const clap_plugin_t*, clap_id id, const char* text, double* out) {
  if (id != kParamVolume) return false;
  char* end = nullptr;
  double v = std::strtod(text, &end);
  if (end == text) return false;
  *out = v;
  return true;
}

void paramsFlush(const clap_plugin_t* p, const clap_input_events_t* in, const clap_output_events_t*) {
  SfzEngine* s = self(p);
  const uint32_t total = in->size(in);
  for (uint32_t i = 0; i < total; ++i) {
    const clap_event_header_t* h = in->get(in, i);
    if (h->space_id != CLAP_CORE_EVENT_SPACE_ID || h->type != CLAP_EVENT_PARAM_VALUE) continue;
    auto* e = reinterpret_cast<const clap_event_param_value_t*>(h);
    if (e->param_id == kParamVolume) s->volumeDb = clampDouble(e->value, kVolMin, kVolMax);
  }
}

const clap_plugin_params_t kParams = {paramsCount, paramsInfo, paramsGetValue, paramsValueToText, paramsTextToValue,
                                      paramsFlush};

// ── clap.state ─────────────────────────────────────────────────────────────────────────────────────────

bool stateSave(const clap_plugin_t* p, const clap_ostream_t* out) {
  SfzEngine* s = self(p);
  std::string path = s->sfzPath;
  std::vector<unsigned char> buf;
  buf.push_back(kStateVersion);
  unsigned char v8[8];
  double vol = s->volumeDb;
  std::memcpy(v8, &vol, 8);
  buf.insert(buf.end(), v8, v8 + 8);
  uint16_t plen = (uint16_t)std::min<size_t>(path.size(), 0xffff);
  buf.push_back((unsigned char)(plen & 0xff));
  buf.push_back((unsigned char)(plen >> 8));
  buf.insert(buf.end(), path.begin(), path.begin() + plen);
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
  if (buf.size() < 11 || buf[0] != kStateVersion) return false;
  double vol;
  std::memcpy(&vol, buf.data() + 1, 8);
  SfzEngine* s = self(p);
  s->volumeDb = clampDouble(vol, kVolMin, kVolMax);
  uint16_t plen = (uint16_t)(buf[9] | (buf[10] << 8));
  if (buf.size() < (size_t)(11 + plen)) return false;
  s->sfzPath.assign((const char*)buf.data() + 11, plen);
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

uint32_t tailGet(const clap_plugin_t* p) { return self(p)->tailFrames; }
const clap_plugin_tail_t kTail = {tailGet};

// ── clap.webview/3 — no editor page; receive() carries wclap.files/1 frames (wclap-rack SPEC §8) ───────

int32_t webviewGetUri(const clap_plugin_t*, char* uri, uint32_t cap) {
  if (cap > 0 && uri != nullptr) uri[0] = '\0';
  return 1;
}
bool webviewGetResource(const clap_plugin_t*, const char*, char*, uint32_t, const clap_ostream_t*) { return false; }

bool webviewReceive(const clap_plugin_t* p, const void* buffer, uint32_t size) {
  SfzEngine* s = self(p);
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
  SfzEngine* s = self(p);
  s->~SfzEngine();
  std::free(s);
}

bool pluginActivate(const clap_plugin_t* p, double sampleRate, uint32_t, uint32_t) {
  SfzEngine* s = self(p);
  s->sampleRate = sampleRate > 0 ? sampleRate : 48000;
  for (auto& v : s->voices) v.active = false;
  loadInstrument(s);
  s->active = true;
  return true;
}
void pluginDeactivate(const clap_plugin_t* p) {
  SfzEngine* s = self(p);
  for (auto& v : s->voices) v.active = false;
  s->active = false;
}
bool pluginStartProcessing(const clap_plugin_t*) { return true; }
void pluginStopProcessing(const clap_plugin_t*) {}
void pluginReset(const clap_plugin_t* p) {
  SfzEngine* s = self(p);
  for (auto& v : s->voices) v.active = false;  // param values and the instrument remain
}

void handleEvent(SfzEngine* s, const clap_event_header_t* h) {
  if (h->space_id != CLAP_CORE_EVENT_SPACE_ID) return;
  if (h->type == CLAP_EVENT_PARAM_VALUE) {
    auto* e = reinterpret_cast<const clap_event_param_value_t*>(h);
    if (e->param_id == kParamVolume) s->volumeDb = clampDouble(e->value, kVolMin, kVolMax);
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
    if (status == 0x90 && e->data[2] > 0)
      noteOn(s, e->data[1], e->data[2] / 127.0, -1, (int16_t)(e->data[0] & 0x0f));
    else if (status == 0x80 || (status == 0x90 && e->data[2] == 0))
      noteOff(s, e->data[1], -1, (int16_t)(e->data[0] & 0x0f), false);
  }
}

clap_process_status pluginProcess(const clap_plugin_t* p, const clap_process_t* proc) {
  SfzEngine* s = self(p);
  if (!s->active || proc->audio_outputs_count < 1) return CLAP_PROCESS_ERROR;
  const clap_audio_buffer_t* out = &proc->audio_outputs[0];
  if (!out->data32 || out->channel_count < 1) return CLAP_PROCESS_ERROR;
  const uint32_t frames = proc->frames_count;
  const uint32_t outCh = std::min(out->channel_count, 2u);
  const uint32_t total = proc->in_events ? proc->in_events->size(proc->in_events) : 0;
  uint32_t cursor = 0;
  const double master = std::pow(10.0, s->volumeDb / 20.0);
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
      const Region& r = *v.region;
      const Sample& smp = *r.sample;
      // envelope
      if (v.stageLeft > 0 && v.stageLeft != 0xffffffffu) {
        v.env += v.stageStep;
        if (--v.stageLeft == 0) advanceStage(s, v);
        if (!v.active) continue;
      }
      // sample read with linear interpolation
      double pos = v.pos;
      if (r.loopMode == 1 && smp.frames > 0 && r.loopEnd > r.loopStart && pos >= r.loopEnd)
        pos = r.loopStart + std::fmod(pos - r.loopStart, (double)(r.loopEnd - r.loopStart));
      if (pos >= smp.frames) {
        if (r.loopMode != 1) {
          v.active = false;
          continue;
        }
        pos = 0;
      }
      uint32_t i0 = (uint32_t)pos;
      if (i0 >= smp.frames) i0 = smp.frames - 1;
      double fr = pos - i0;
      uint32_t i1 = i0 + 1;
      if (r.loopMode == 1 && i1 >= r.loopEnd) i1 = r.loopStart;
      if (i1 >= smp.frames) i1 = smp.frames - 1;
      const uint32_t ch = std::min(smp.channels, 2u);
      float l = smp.data[(size_t)i0 * ch + 0] * (float)(1.0 - fr) + smp.data[(size_t)i1 * ch + 0] * (float)fr;
      float rr = ch > 1 ? smp.data[(size_t)i0 * ch + 1] * (float)(1.0 - fr) + smp.data[(size_t)i1 * ch + 1] * (float)fr
                        : l;
      const double velGain = v.velocity * v.velocity;
      const double regGain = std::pow(10.0, r.volumeDb / 20.0);
      const double panA = (r.pan + 100.0) / 200.0 * (M_PI / 2.0);
      const float gl = (float)(v.env * velGain * regGain * master * std::cos(panA));
      const float gr = (float)(v.env * velGain * regGain * master * std::sin(panA));
      accL += l * gl;
      accR += rr * gr;
      v.pos += v.step;
      if (r.loopMode != 1 && v.pos >= smp.frames) v.active = false;
    }
    if (outCh > 0) out->data32[0][i] = accL;
    if (outCh > 1) out->data32[1][i] = accR;
  }
  if (proc->in_events) {
    while (cursor < total) handleEvent(s, proc->in_events->get(proc->in_events, cursor++));
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
  return nullptr;
}
void pluginOnMainThread(const clap_plugin_t*) {}

const clap_plugin_t* createPlugin(const clap_host_t* host) {
  void* mem = std::calloc(1, sizeof(SfzEngine));
  SfzEngine* s = new (mem) SfzEngine();
  s->host = host;
  s->volumeDb = kVolDefault;
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
