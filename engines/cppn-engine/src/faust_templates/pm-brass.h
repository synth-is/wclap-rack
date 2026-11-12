/* ------------------------------------------------------------
name: "pm-brass"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_pm_brass -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_pm_brass_H__
#define  __T_pm_brass_H__

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
#define FAUSTCLASS T_pm_brass
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

static float T_pm_brass_faustpower2_f(float value) {
	return value * value;
}

class T_pm_brass : public FaustDsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst2;
	float fRec8[2];
	float fConst3;
	FAUSTFLOAT fHslider1;
	float fSlow1;
	float fRec9[2];
	float fRec0[3];
	FAUSTFLOAT fHslider2;
	float fSlow2;
	float fRec10[2];
	float fRec1[2];
	float fRec2[2];
	float fVec0[2];
	float fRec3[2];
	float fConst4;
	int IOTA0;
	float fConst5;
	float fRec4[2];
	float fRec5[2048];
	float fRec6[2048];
	float fVec1[2];
	float fRec7[2];
	
 public:
	T_pm_brass() {
	}
	
	T_pm_brass(const T_pm_brass&) = default;
	
	virtual ~T_pm_brass() = default;
	
	T_pm_brass& operator=(const T_pm_brass&) = default;
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_pm_brass -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/fdelay4:author", "Julius O. Smith III");
		m->declare("delays.lib/fdelayltv:author", "Julius O. Smith III");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("filename", "pm-brass.dsp");
		m->declare("filters.lib/dcblocker:author", "Julius O. Smith III");
		m->declare("filters.lib/dcblocker:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/dcblocker:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/pole:author", "Julius O. Smith III");
		m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/pole:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/version", "1.9.0");
		m->declare("filters.lib/zero:author", "Julius O. Smith III");
		m->declare("filters.lib/zero:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/zero:license", "LicenseRef-STK-4.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "pm-brass");
		m->declare("physmodels.lib/name", "Faust Physical Models Library");
		m->declare("physmodels.lib/version", "1.2.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.4.0");
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
		fConst3 = 2136.283f / fConst0;
		fConst4 = 0.0014705883f * fConst0;
		fConst5 = 0.00882353f * fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.8f);
		fHslider1 = static_cast<FAUSTFLOAT>(1.0f);
		fHslider2 = static_cast<FAUSTFLOAT>(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = faust_wrap_add(l0, 1)) {
			fRec8[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec9[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 3; l2 = faust_wrap_add(l2, 1)) {
			fRec0[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = faust_wrap_add(l3, 1)) {
			fRec10[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = faust_wrap_add(l4, 1)) {
			fRec1[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = faust_wrap_add(l5, 1)) {
			fRec2[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = faust_wrap_add(l6, 1)) {
			fVec0[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = faust_wrap_add(l7, 1)) {
			fRec3[l7] = 0.0f;
		}
		IOTA0 = 0;
		for (int l8 = 0; l8 < 2; l8 = faust_wrap_add(l8, 1)) {
			fRec4[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2048; l9 = faust_wrap_add(l9, 1)) {
			fRec5[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2048; l10 = faust_wrap_add(l10, 1)) {
			fRec6[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = faust_wrap_add(l11, 1)) {
			fVec1[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = faust_wrap_add(l12, 1)) {
			fRec7[l12] = 0.0f;
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
	
	virtual T_pm_brass* clone() {
		return new T_pm_brass(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("pm-brass");
		ui_interface->addHorizontalSlider("mute", &fHslider2, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("pressure", &fHslider0, FAUSTFLOAT(0.8f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("tubeLength", &fHslider1, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.5f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst1 * static_cast<float>(fHslider0);
		fSlow1 = fConst1 * static_cast<float>(fHslider1);
		fSlow2 = fConst1 * static_cast<float>(fHslider2);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		fRec8[0] = fSlow0 + fConst2 * fRec8[1];
		fRec9[0] = fSlow1 + fConst2 * fRec9[1];
		fRec0[0] = 0.03f * (0.3f * fRec8[0] - 0.85f * fRec2[1]) + 1.994f * fRec0[1] * std::cos(fConst3 * (std::pow(4.0f, static_cast<float>(inputs[0])) / fRec9[0])) - 0.994009f * fRec0[2];
		fRec10[0] = fSlow2 + fConst2 * fRec10[1];
		fRec1[0] = (1.0f - fRec10[0]) * fRec7[1] + fRec10[0] * fRec1[1];
		fRec2[0] = fRec4[1];
		float fTemp0 = std::max<float>(-1.0f, std::min<float>(1.0f, T_pm_brass_faustpower2_f(fRec0[0])));
		float fTemp1 = 0.85f * fRec2[1] * (1.0f - fTemp0);
		float fTemp2 = 0.3f * fRec8[0] * fTemp0;
		fVec0[0] = fTemp2 + fTemp1;
		fRec3[0] = fTemp1 + 0.995f * fRec3[1] + fTemp2 - fVec0[1];
		float fTemp3 = fConst4 * fRec9[0];
		float fTemp4 = -1.499995f + fTemp3;
		float fTemp5 = std::floor(fTemp4);
		float fTemp6 = -4.0f - fTemp5 + fTemp3;
		float fTemp7 = -3.0f - fTemp5 + fTemp3;
		float fTemp8 = -2.0f - fTemp5 + fTemp3;
		int iTemp9 = static_cast<int>(fTemp4);
		int iTemp10 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst5, static_cast<float>(std::max<int>(0, iTemp9)))));
		float fTemp11 = -1.0f - fTemp5 + fTemp3;
		float fTemp12 = fTemp3 - fTemp5;
		int iTemp13 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst5, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp9))))));
		float fTemp14 = fTemp12 * fTemp11;
		int iTemp15 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst5, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp9))))));
		float fTemp16 = fTemp14 * fTemp8;
		int iTemp17 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst5, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp9))))));
		float fTemp18 = fTemp16 * fTemp7;
		int iTemp19 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst5, static_cast<float>(std::max<int>(0, faust_wrap_add(4, iTemp9))))));
		fRec4[0] = fTemp6 * (fTemp7 * (fTemp8 * (0.041666668f * fRec5[(faust_wrap_sub(IOTA0, iTemp10)) & 2047] * fTemp11 - 0.16666667f * fTemp12 * fRec5[(faust_wrap_sub(IOTA0, iTemp13)) & 2047]) + 0.25f * fTemp14 * fRec5[(faust_wrap_sub(IOTA0, iTemp15)) & 2047]) - 0.16666667f * fTemp16 * fRec5[(faust_wrap_sub(IOTA0, iTemp17)) & 2047]) + 0.041666668f * fTemp18 * fRec5[(faust_wrap_sub(IOTA0, iTemp19)) & 2047];
		fRec5[IOTA0 & 2047] = fRec1[0];
		fRec6[IOTA0 & 2047] = fRec3[0];
		fVec1[0] = fTemp6 * (fTemp7 * (fTemp8 * (0.041666668f * fTemp11 * fRec6[(faust_wrap_sub(IOTA0, iTemp10)) & 2047] - 0.16666667f * fTemp12 * fRec6[(faust_wrap_sub(IOTA0, iTemp13)) & 2047]) + 0.25f * fTemp14 * fRec6[(faust_wrap_sub(IOTA0, iTemp15)) & 2047]) - 0.16666667f * fTemp16 * fRec6[(faust_wrap_sub(IOTA0, iTemp17)) & 2047]) + 0.041666668f * fTemp18 * fRec6[(faust_wrap_sub(IOTA0, iTemp19)) & 2047];
		fRec7[0] = fVec1[1];
		outputs[0] = static_cast<FAUSTFLOAT>(fRec7[0]);
		fRec8[1] = fRec8[0];
		fRec9[1] = fRec9[0];
		fRec0[2] = fRec0[1];
		fRec0[1] = fRec0[0];
		fRec10[1] = fRec10[0];
		fRec1[1] = fRec1[0];
		fRec2[1] = fRec2[0];
		fVec0[1] = fVec0[0];
		fRec3[1] = fRec3[0];
		IOTA0 = faust_wrap_add(IOTA0, 1);
		fRec4[1] = fRec4[0];
		fVec1[1] = fVec1[0];
		fRec7[1] = fRec7[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
