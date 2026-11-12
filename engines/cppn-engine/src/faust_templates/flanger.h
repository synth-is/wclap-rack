/* ------------------------------------------------------------
name: "flanger"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_flanger -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_flanger_H__
#define  __T_flanger_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>
#ifndef FAUST_INT_WRAP
#define FAUST_INT_WRAP
inline int faust_wrap_add(int a, int b) { return int((unsigned int)a + (unsigned int)b); }
inline int faust_wrap_sub(int a, int b) { return int((unsigned int)a - (unsigned int)b); }
inline int faust_wrap_mul(int a, int b) { return int((unsigned int)a * (unsigned int)b); }
#endif


#ifndef FAUSTCLASS 
#define FAUSTCLASS T_flanger
#endif

#ifdef __APPLE__ 
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif


class T_flanger : public FaustDsp {
	
 private:
	
	FAUSTFLOAT fCheckbox0;
	int iSlow0;
	FAUSTFLOAT fHslider0;
	float fSlow1;
	int iVec0[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider1;
	float fSlow2;
	float fSlow3;
	float fSlow4;
	float fRec0[2];
	float fRec1[2];
	FAUSTFLOAT fHbargraph0;
	FAUSTFLOAT fCheckbox1;
	FAUSTFLOAT fHslider2;
	float fSlow5;
	float fSlow6;
	FAUSTFLOAT fHslider3;
	float fSlow7;
	int IOTA0;
	float fVec1[4096];
	FAUSTFLOAT fHslider4;
	float fSlow8;
	FAUSTFLOAT fHslider5;
	float fSlow9;
	float fRec2[2];
	float fVec2[4096];
	float fRec3[2];
	
 public:
	T_flanger() {
	}
	
	T_flanger(const T_flanger&) = default;
	
	virtual ~T_flanger() = default;
	
	T_flanger& operator=(const T_flanger&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/bypass2:author", "Julius Smith");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_flanger -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("demos.lib/flanger_demo:author", "Julius O. Smith III");
		m->declare("demos.lib/flanger_demo:licence", "MIT");
		m->declare("demos.lib/name", "Faust Demos Library");
		m->declare("demos.lib/version", "1.4.0");
		m->declare("filename", "flanger.dsp");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/nlf2:author", "Julius O. Smith III");
		m->declare("filters.lib/nlf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/nlf2:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/version", "1.9.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "flanger");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.8.0");
		m->declare("phaflangers.lib/name", "Faust Phaser and Flanger Library");
		m->declare("phaflangers.lib/version", "1.1.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.4.0");
	}

	virtual int getNumInputs() {
		return 1;
	}
	virtual int getNumOutputs() {
		return 1;
	}
	
	static void classInit(int sample_rate) {
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 6.2831855f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fCheckbox0 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider0 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.5f);
		fCheckbox1 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider2 = static_cast<FAUSTFLOAT>(1.0f);
		fHslider3 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider4 = static_cast<FAUSTFLOAT>(1.0f);
		fHslider5 = static_cast<FAUSTFLOAT>(1e+01f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = faust_wrap_add(l0, 1)) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = faust_wrap_add(l2, 1)) {
			fRec1[l2] = 0.0f;
		}
		IOTA0 = 0;
		for (int l3 = 0; l3 < 4096; l3 = faust_wrap_add(l3, 1)) {
			fVec1[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = faust_wrap_add(l4, 1)) {
			fRec2[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 4096; l5 = faust_wrap_add(l5, 1)) {
			fVec2[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = faust_wrap_add(l6, 1)) {
			fRec3[l6] = 0.0f;
		}
	}
	
	virtual void init(int sample_rate) {
		classInit(sample_rate);
		instanceInit(sample_rate);
	}
	
	virtual void instanceInit(int sample_rate) {
		instanceConstants(sample_rate);
		instanceResetUserInterface();
		instanceClear();
	}
	
	virtual T_flanger* clone() {
		return new T_flanger(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->declare(0, "tooltip", "Reference: https://ccrma.stanford.edu/~jos/pasp/Flanging.html");
		ui_interface->openVerticalBox("FLANGER");
		ui_interface->declare(0, "0", "");
		ui_interface->openHorizontalBox("0x00");
		ui_interface->declare(&fCheckbox0, "0", "");
		ui_interface->declare(&fCheckbox0, "tooltip", "When this is checked, the flanger         has no effect");
		ui_interface->addCheckButton("Bypass", &fCheckbox0);
		ui_interface->declare(&fCheckbox1, "1", "");
		ui_interface->addCheckButton("Invert Flange Sum", &fCheckbox1);
		ui_interface->declare(&fHbargraph0, "2", "");
		ui_interface->declare(&fHbargraph0, "style", "led");
		ui_interface->declare(&fHbargraph0, "tooltip", "Display sum of flange delays");
		ui_interface->addHorizontalBargraph("Flange LFO", &fHbargraph0, FAUSTFLOAT(-1.5f), FAUSTFLOAT(1.5f));
		ui_interface->closeBox();
		ui_interface->declare(0, "1", "");
		ui_interface->openHorizontalBox("0x00");
		ui_interface->declare(&fHslider1, "1", "");
		ui_interface->declare(&fHslider1, "style", "knob");
		ui_interface->declare(&fHslider1, "unit", "Hz");
		ui_interface->addHorizontalSlider("Speed", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
		ui_interface->declare(&fHslider2, "2", "");
		ui_interface->declare(&fHslider2, "style", "knob");
		ui_interface->addHorizontalSlider("Depth", &fHslider2, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider3, "3", "");
		ui_interface->declare(&fHslider3, "style", "knob");
		ui_interface->addHorizontalSlider("Feedback", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(-0.999f), FAUSTFLOAT(0.999f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
		ui_interface->declare(0, "2", "");
		ui_interface->openHorizontalBox("Delay Controls");
		ui_interface->declare(&fHslider5, "1", "");
		ui_interface->declare(&fHslider5, "style", "knob");
		ui_interface->declare(&fHslider5, "unit", "ms");
		ui_interface->addHorizontalSlider("Flange Delay", &fHslider5, FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.001f));
		ui_interface->declare(&fHslider4, "2", "");
		ui_interface->declare(&fHslider4, "style", "knob");
		ui_interface->declare(&fHslider4, "unit", "ms");
		ui_interface->addHorizontalSlider("Delay Offset", &fHslider4, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.001f));
		ui_interface->closeBox();
		ui_interface->declare(0, "3", "");
		ui_interface->openHorizontalBox("0x00");
		ui_interface->declare(&fHslider0, "unit", "dB");
		ui_interface->addHorizontalSlider("Flanger Output Level", &fHslider0, FAUSTFLOAT(0.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
		ui_interface->closeBox();
	}
	
	void control() {
		iSlow0 = static_cast<int>(static_cast<float>(fCheckbox0));
		fSlow1 = std::pow(1e+01f, 0.05f * static_cast<float>(fHslider0));
		fSlow2 = fConst1 * static_cast<float>(fHslider1);
		fSlow3 = std::sin(fSlow2);
		fSlow4 = std::cos(fSlow2);
		fSlow5 = static_cast<float>(fHslider2);
		fSlow6 = ((static_cast<int>(static_cast<float>(fCheckbox1))) ? -fSlow5 : fSlow5);
		fSlow7 = static_cast<float>(fHslider3);
		fSlow8 = 0.001f * static_cast<float>(fHslider4);
		fSlow9 = 0.0005f * static_cast<float>(fHslider5);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		iVec0[0] = 1;
		fRec0[0] = static_cast<float>(faust_wrap_sub(1, iVec0[1])) - fSlow3 * fRec1[1] + fSlow4 * fRec0[1];
		fRec1[0] = fSlow3 * fRec0[1] + fSlow4 * fRec1[1];
		fHbargraph0 = static_cast<FAUSTFLOAT>(fRec0[0] + fRec1[0]);
		float fTemp0 = static_cast<float>(inputs[0]);
		float fTemp1 = ((iSlow0) ? 0.0f : fTemp0);
		float fTemp2 = fSlow1 * fTemp1;
		float fTemp3 = fSlow7 * fRec2[1] - fTemp2;
		fVec1[IOTA0 & 4095] = fTemp3;
		float fTemp4 = fConst0 * (fSlow8 + fSlow9 * (1.0f + fRec1[0]));
		int iTemp5 = static_cast<int>(fTemp4);
		float fTemp6 = std::floor(fTemp4);
		fRec2[0] = fVec1[(faust_wrap_sub(IOTA0, std::min<int>(2049, std::max<int>(0, iTemp5)))) & 4095] * (1.0f - fTemp4 + fTemp6) + (fTemp4 - fTemp6) * fVec1[(faust_wrap_sub(IOTA0, std::min<int>(2049, std::max<int>(0, faust_wrap_add(1, iTemp5))))) & 4095];
		float fTemp7 = fSlow1 * fTemp1;
		float fTemp8 = fSlow7 * fRec3[1] - fTemp7;
		fVec2[IOTA0 & 4095] = fTemp8;
		float fTemp9 = fConst0 * (fSlow8 + fSlow9 * (1.0f + fRec0[0]));
		int iTemp10 = static_cast<int>(fTemp9);
		float fTemp11 = std::floor(fTemp9);
		fRec3[0] = fVec2[(faust_wrap_sub(IOTA0, std::min<int>(2049, std::max<int>(0, iTemp10)))) & 4095] * (1.0f - fTemp9 + fTemp11) + (fTemp9 - fTemp11) * fVec2[(faust_wrap_sub(IOTA0, std::min<int>(2049, std::max<int>(0, faust_wrap_add(1, iTemp10))))) & 4095];
		outputs[0] = static_cast<FAUSTFLOAT>(0.5f * (((iSlow0) ? fTemp0 : 0.5f * (fTemp2 + fSlow6 * fRec2[0])) + ((iSlow0) ? fTemp0 : 0.5f * (fTemp7 + fSlow6 * fRec3[0]))));
		iVec0[1] = iVec0[0];
		fRec0[1] = fRec0[0];
		fRec1[1] = fRec1[0];
		IOTA0 = faust_wrap_add(IOTA0, 1);
		fRec2[1] = fRec2[0];
		fRec3[1] = fRec3[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
