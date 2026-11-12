// faust_nodes.h — FaustNode support: the platform's Faust templates (kromosynth/faust-templates/*.dsp)
// compiled to C++ (src/faust_templates/, regenerate with scripts/gen-faust-templates.sh) and run as one
// more node kind of the patch. In the production renderer a FaustNode is an AudioWorkletNode made by
// libfaust from the same .dsp: mono or stereo in, mono or stereo out, no added latency, its sliders
// exposed as parameters named by their full path ("/pm-brass/mute") and read once per 128 frames.
#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>
#include <memory>
#include <string>
#include <vector>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

namespace fdsp {

struct Meta {
  virtual ~Meta() = default;
  virtual void declare(const char*, const char*) {}
};
struct UI {
  virtual ~UI() = default;
  virtual void openTabBox(const char*) {}
  virtual void openHorizontalBox(const char*) {}
  virtual void openVerticalBox(const char*) {}
  virtual void closeBox() {}
  virtual void addButton(const char*, FAUSTFLOAT*) {}
  virtual void addCheckButton(const char*, FAUSTFLOAT*) {}
  virtual void addVerticalSlider(const char*, FAUSTFLOAT*, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT) {}
  virtual void addHorizontalSlider(const char*, FAUSTFLOAT*, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT) {}
  virtual void addNumEntry(const char*, FAUSTFLOAT*, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT, FAUSTFLOAT) {}
  virtual void addHorizontalBargraph(const char*, FAUSTFLOAT*, FAUSTFLOAT, FAUSTFLOAT) {}
  virtual void addVerticalBargraph(const char*, FAUSTFLOAT*, FAUSTFLOAT, FAUSTFLOAT) {}
  virtual void declare(FAUSTFLOAT*, const char*, const char*) {}
};
struct FaustDsp {
  virtual ~FaustDsp() = default;
  virtual int getNumInputs() = 0;
  virtual int getNumOutputs() = 0;
  virtual void buildUserInterface(UI* ui) = 0;
  virtual int getSampleRate() = 0;
  virtual void init(int sampleRate) = 0;
  virtual void instanceInit(int sampleRate) = 0;
  virtual void instanceConstants(int sampleRate) = 0;
  virtual void instanceResetUserInterface() = 0;
  virtual void instanceClear() = 0;
  virtual FaustDsp* clone() = 0;
  virtual void compute(int count, FAUSTFLOAT** inputs, FAUSTFLOAT** outputs) = 0;
  virtual void control() = 0;                                       // slider-rate work
  virtual void frame(FAUSTFLOAT* inputs, FAUSTFLOAT* outputs) = 0;  // one sample
};

#include "faust_templates/all.h"

// The sliders of one instance, with the path names libfaust gives them (faust/gui/PathBuilder.h).
struct ParamInfo { std::string path; FAUSTFLOAT* zone; double init, lo, hi; };
struct Collector : UI {
  std::vector<std::string> levels;
  std::vector<ParamInfo> params;
  std::string pathOf(const char* label) const {
    std::string res = "/";
    for (const auto& l : levels) res += l + "/";
    res += label;
    for (char& c : res)
      if (c == ' ' || c == '#' || c == '*' || c == ',' || c == '?' || c == '[' || c == ']' || c == '{' || c == '}' || c == '(' || c == ')') c = '_';
    return res;
  }
  void open(const char* l) { levels.push_back(l); }
  void openTabBox(const char* l) override { open(l); }
  void openHorizontalBox(const char* l) override { open(l); }
  void openVerticalBox(const char* l) override { open(l); }
  void closeBox() override { if (!levels.empty()) levels.pop_back(); }
  void addButton(const char* l, FAUSTFLOAT* z) override { params.push_back({pathOf(l), z, 0, 0, 1}); }
  void addCheckButton(const char* l, FAUSTFLOAT* z) override { params.push_back({pathOf(l), z, 0, 0, 1}); }
  void addVerticalSlider(const char* l, FAUSTFLOAT* z, FAUSTFLOAT i, FAUSTFLOAT a, FAUSTFLOAT b, FAUSTFLOAT) override { params.push_back({pathOf(l), z, i, a, b}); }
  void addHorizontalSlider(const char* l, FAUSTFLOAT* z, FAUSTFLOAT i, FAUSTFLOAT a, FAUSTFLOAT b, FAUSTFLOAT) override { params.push_back({pathOf(l), z, i, a, b}); }
  void addNumEntry(const char* l, FAUSTFLOAT* z, FAUSTFLOAT i, FAUSTFLOAT a, FAUSTFLOAT b, FAUSTFLOAT) override { params.push_back({pathOf(l), z, i, a, b}); }
};

// One running instance (one per Faust node per voice).
struct Instance {
  std::unique_ptr<FaustDsp> dsp;
  std::vector<FAUSTFLOAT*> zones;   // in the order of Collector::params
  int ins = 0, outs = 0;
  bool make(const std::string& name, int sampleRate) {
    dsp.reset(createTemplate(name));
    zones.clear();
    if (!dsp) return false;
    dsp->init(sampleRate);
    Collector c;
    dsp->buildUserInterface(&c);
    for (auto& p : c.params) zones.push_back(p.zone);
    ins = dsp->getNumInputs();
    outs = dsp->getNumOutputs();
    return true;
  }
};

}  // namespace fdsp
