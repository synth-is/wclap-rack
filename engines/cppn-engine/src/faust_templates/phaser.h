/* ------------------------------------------------------------
name: "phaser"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_phaser -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_phaser_H__
#define  __T_phaser_H__

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
#define FAUSTCLASS T_phaser
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

static float T_phaser_faustpower2_f(float value) {
	return value * value;
}
static float T_phaser_faustpower3_f(float value) {
	return value * value * value;
}
static float T_phaser_faustpower4_f(float value) {
	return value * value * value * value;
}

class T_phaser : public FaustDsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst2;
	int iVec0[2];
	float fRec0[2];
	FAUSTFLOAT fHslider1;
	int iSlow1;
	float fConst3;
	FAUSTFLOAT fHslider2;
	float fSlow2;
	float fRec6[2];
	FAUSTFLOAT fHslider3;
	float fSlow3;
	float fRec7[2];
	float fConst4;
	FAUSTFLOAT fHslider4;
	float fSlow4;
	float fRec8[2];
	FAUSTFLOAT fHslider5;
	float fSlow5;
	float fRec9[2];
	FAUSTFLOAT fHslider6;
	float fSlow6;
	float fRec10[2];
	float fConst5;
	FAUSTFLOAT fHslider7;
	float fSlow7;
	float fRec13[2];
	float fRec11[2];
	float fRec12[2];
	FAUSTFLOAT fHslider8;
	float fSlow8;
	float fRec14[2];
	float fRec1[3];
	float fRec2[3];
	float fRec3[3];
	float fRec4[3];
	float fRec5[2];
	
 public:
	T_phaser() {
	}
	
	T_phaser(const T_phaser&) = default;
	
	virtual ~T_phaser() = default;
	
	T_phaser& operator=(const T_phaser&) = default;
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_phaser -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "phaser.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/nlf2:author", "Julius O. Smith III");
		m->declare("filters.lib/nlf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/nlf2:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/version", "1.9.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "phaser");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.8.0");
		m->declare("phaflangers.lib/name", "Faust Phaser and Flanger Library");
		m->declare("phaflangers.lib/version", "1.1.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Routing Library");
		m->declare("signals.lib/version", "1.7.0");
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
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 3.1415927f / fConst0;
		fConst4 = 1.0f / fConst0;
		fConst5 = 6.2831855f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(1.0f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider2 = static_cast<FAUSTFLOAT>(1e+03f);
		fHslider3 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider4 = static_cast<FAUSTFLOAT>(1.5f);
		fHslider5 = static_cast<FAUSTFLOAT>(1e+02f);
		fHslider6 = static_cast<FAUSTFLOAT>(8e+02f);
		fHslider7 = static_cast<FAUSTFLOAT>(0.5f);
		fHslider8 = static_cast<FAUSTFLOAT>(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = faust_wrap_add(l0, 1)) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = faust_wrap_add(l2, 1)) {
			fRec6[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = faust_wrap_add(l3, 1)) {
			fRec7[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = faust_wrap_add(l4, 1)) {
			fRec8[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = faust_wrap_add(l5, 1)) {
			fRec9[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = faust_wrap_add(l6, 1)) {
			fRec10[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = faust_wrap_add(l7, 1)) {
			fRec13[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = faust_wrap_add(l8, 1)) {
			fRec11[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = faust_wrap_add(l9, 1)) {
			fRec12[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = faust_wrap_add(l10, 1)) {
			fRec14[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 3; l11 = faust_wrap_add(l11, 1)) {
			fRec1[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 3; l12 = faust_wrap_add(l12, 1)) {
			fRec2[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 3; l13 = faust_wrap_add(l13, 1)) {
			fRec3[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 3; l14 = faust_wrap_add(l14, 1)) {
			fRec4[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = faust_wrap_add(l15, 1)) {
			fRec5[l15] = 0.0f;
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
	
	virtual T_phaser* clone() {
		return new T_phaser(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("phaser");
		ui_interface->addHorizontalSlider("depth", &fHslider0, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("fb", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(-0.999f), FAUSTFLOAT(0.999f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("fratio", &fHslider4, FAUSTFLOAT(1.5f), FAUSTFLOAT(1.01f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("frqMax", &fHslider6, FAUSTFLOAT(8e+02f), FAUSTFLOAT(2e+02f), FAUSTFLOAT(8e+03f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("frqMin", &fHslider5, FAUSTFLOAT(1e+02f), FAUSTFLOAT(5e+01f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("invert", &fHslider1, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("phase", &fHslider8, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("speed", &fHslider7, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("width", &fHslider2, FAUSTFLOAT(1e+03f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(4e+03f), FAUSTFLOAT(1.0f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst1 * static_cast<float>(fHslider0);
		iSlow1 = static_cast<int>(static_cast<float>(fHslider1));
		fSlow2 = fConst1 * static_cast<float>(fHslider2);
		fSlow3 = fConst1 * static_cast<float>(fHslider3);
		fSlow4 = fConst1 * static_cast<float>(fHslider4);
		fSlow5 = fConst1 * static_cast<float>(fHslider5);
		fSlow6 = fConst1 * static_cast<float>(fHslider6);
		fSlow7 = fConst1 * static_cast<float>(fHslider7);
		fSlow8 = fConst1 * static_cast<float>(fHslider8);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		float fTemp0 = static_cast<float>(inputs[0]);
		iVec0[0] = 1;
		fRec0[0] = fSlow0 + fConst2 * fRec0[1];
		float fTemp1 = 0.5f * fRec0[0];
		fRec6[0] = fSlow2 + fConst2 * fRec6[1];
		float fTemp2 = std::exp(-(fConst3 * fRec6[0]));
		float fTemp3 = T_phaser_faustpower2_f(fTemp2);
		fRec7[0] = fSlow3 + fConst2 * fRec7[1];
		fRec8[0] = fSlow4 + fConst2 * fRec8[1];
		fRec9[0] = fSlow5 + fConst2 * fRec9[1];
		fRec10[0] = fSlow6 + fConst2 * fRec10[1];
		fRec13[0] = fSlow7 + fConst2 * fRec13[1];
		float fTemp4 = fConst5 * fRec13[0];
		float fTemp5 = std::sin(fTemp4);
		float fTemp6 = std::cos(fTemp4);
		fRec11[0] = static_cast<float>(faust_wrap_sub(1, iVec0[1])) - fRec12[1] * fTemp5 + fRec11[1] * fTemp6;
		fRec12[0] = fRec11[1] * fTemp5 + fTemp6 * fRec12[1];
		fRec14[0] = fSlow8 + fConst2 * fRec14[1];
		float fTemp7 = 6.2831855f * fRec9[0] - 3.1415927f * (fRec9[0] - fRec10[0]) * (1.0f - (fRec11[0] * fRec14[0] + fRec12[0] * (1.0f - fRec14[0])));
		float fTemp8 = std::cos(fConst4 * fRec8[0] * fTemp7);
		fRec1[0] = fTemp0 - fRec1[2] * fTemp3 + fRec5[1] * fRec7[0] + 2.0f * fRec1[1] * fTemp2 * fTemp8;
		float fTemp9 = fRec2[1] * std::cos(fConst4 * fTemp7 * T_phaser_faustpower2_f(fRec8[0]));
		fRec2[0] = fRec1[2] + fTemp3 * (fRec1[0] - fRec2[2]) - 2.0f * fTemp2 * (fRec1[1] * fTemp8 - fTemp9);
		float fTemp10 = fRec3[1] * std::cos(fConst4 * fTemp7 * T_phaser_faustpower3_f(fRec8[0]));
		fRec3[0] = fRec2[2] + fTemp3 * (fRec2[0] - fRec3[2]) - 2.0f * fTemp2 * (fTemp9 - fTemp10);
		float fTemp11 = std::cos(fConst4 * fTemp7 * T_phaser_faustpower4_f(fRec8[0]));
		fRec4[0] = fRec3[2] + fTemp3 * (fRec3[0] - fRec4[2]) - 2.0f * fTemp2 * (fTemp10 - fRec4[1] * fTemp11);
		fRec5[0] = fRec4[2] + fTemp3 * fRec4[0] - 2.0f * fTemp2 * fRec4[1] * fTemp11;
		outputs[0] = static_cast<FAUSTFLOAT>(fTemp0 * (1.0f - fTemp1) + ((iSlow1) ? -fTemp1 : fTemp1) * fRec5[0]);
		iVec0[1] = iVec0[0];
		fRec0[1] = fRec0[0];
		fRec6[1] = fRec6[0];
		fRec7[1] = fRec7[0];
		fRec8[1] = fRec8[0];
		fRec9[1] = fRec9[0];
		fRec10[1] = fRec10[0];
		fRec13[1] = fRec13[0];
		fRec11[1] = fRec11[0];
		fRec12[1] = fRec12[0];
		fRec14[1] = fRec14[0];
		fRec1[2] = fRec1[1];
		fRec1[1] = fRec1[0];
		fRec2[2] = fRec2[1];
		fRec2[1] = fRec2[0];
		fRec3[2] = fRec3[1];
		fRec3[1] = fRec3[0];
		fRec4[2] = fRec4[1];
		fRec4[1] = fRec4[0];
		fRec5[1] = fRec5[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
