/* ------------------------------------------------------------
name: "pm-flute"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_pm_flute -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_pm_flute_H__
#define  __T_pm_flute_H__

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
#define FAUSTCLASS T_pm_flute
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

static float T_pm_flute_faustpower2_f(float value) {
	return value * value;
}

class T_pm_flute : public FaustDsp {
	
 private:
	
	float fRec1[2];
	int IOTA0;
	float fRec2[2048];
	float fRec3[2];
	float fRec4[2048];
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst3;
	float fRec9[2];
	FAUSTFLOAT fHslider1;
	float fSlow1;
	float fRec10[2];
	float fConst4;
	float fVec0[2];
	float fVec1[2048];
	float fVec2[2];
	float fRec5[2];
	float fRec6[2];
	float fRec7[2048];
	float fRec8[2];
	float fRec0[2];
	
 public:
	T_pm_flute() {
	}
	
	T_pm_flute(const T_pm_flute&) = default;
	
	virtual ~T_pm_flute() = default;
	
	T_pm_flute& operator=(const T_pm_flute&) = default;
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_pm_flute -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/fdelay4:author", "Julius O. Smith III");
		m->declare("delays.lib/fdelayltv:author", "Julius O. Smith III");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("filename", "pm-flute.dsp");
		m->declare("filters.lib/dcblocker:author", "Julius O. Smith III");
		m->declare("filters.lib/dcblocker:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/dcblocker:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/pole:author", "Julius O. Smith III");
		m->declare("filters.lib/pole:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/pole:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/version", "1.9.0");
		m->declare("filters.lib/zero:author", "Julius O. Smith III");
		m->declare("filters.lib/zero:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/zero:license", "LicenseRef-STK-4.3");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "pm-flute");
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
		fConst1 = 0.0014705883f * fConst0;
		fConst2 = 44.1f / fConst0;
		fConst3 = 1.0f - fConst2;
		fConst4 = 0.00882353f * fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.4f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = faust_wrap_add(l0, 1)) {
			fRec1[l0] = 0.0f;
		}
		IOTA0 = 0;
		for (int l1 = 0; l1 < 2048; l1 = faust_wrap_add(l1, 1)) {
			fRec2[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = faust_wrap_add(l2, 1)) {
			fRec3[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2048; l3 = faust_wrap_add(l3, 1)) {
			fRec4[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = faust_wrap_add(l4, 1)) {
			fRec9[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = faust_wrap_add(l5, 1)) {
			fRec10[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = faust_wrap_add(l6, 1)) {
			fVec0[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2048; l7 = faust_wrap_add(l7, 1)) {
			fVec1[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = faust_wrap_add(l8, 1)) {
			fVec2[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = faust_wrap_add(l9, 1)) {
			fRec5[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = faust_wrap_add(l10, 1)) {
			fRec6[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2048; l11 = faust_wrap_add(l11, 1)) {
			fRec7[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = faust_wrap_add(l12, 1)) {
			fRec8[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = faust_wrap_add(l13, 1)) {
			fRec0[l13] = 0.0f;
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
	
	virtual T_pm_flute* clone() {
		return new T_pm_flute(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("pm-flute");
		ui_interface->addHorizontalSlider("mouthPosition", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("tubeLength", &fHslider0, FAUSTFLOAT(0.4f), FAUSTFLOAT(0.05f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst2 * static_cast<float>(fHslider0);
		fSlow1 = fConst2 * static_cast<float>(fHslider1);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		fRec1[0] = 0.285f * fRec5[1] + 0.7f * fRec1[1];
		fRec2[IOTA0 & 2047] = 0.95f * fRec3[1];
		fRec3[0] = fRec8[1];
		fRec4[IOTA0 & 2047] = fRec6[1];
		fRec9[0] = fSlow0 + fConst3 * fRec9[1];
		float fTemp0 = 0.27f + fRec9[0];
		fRec10[0] = fSlow1 + fConst3 * fRec10[1];
		float fTemp1 = 0.4f * (-0.5f + fRec10[0]);
		float fTemp2 = fConst1 * fTemp0 * (0.73f - fTemp1);
		float fTemp3 = -1.499995f + fTemp2;
		float fTemp4 = std::floor(fTemp3);
		float fTemp5 = -4.0f - fTemp4 + fTemp2;
		float fTemp6 = -3.0f - fTemp4 + fTemp2;
		float fTemp7 = -2.0f - fTemp4 + fTemp2;
		float fTemp8 = fConst1 * fTemp0 * (0.27f + fTemp1);
		float fTemp9 = -1.499995f + fTemp8;
		float fTemp10 = std::floor(fTemp9);
		float fTemp11 = -4.0f - fTemp10 + fTemp8;
		float fTemp12 = -3.0f - fTemp10 + fTemp8;
		float fTemp13 = -2.0f - fTemp10 + fTemp8;
		int iTemp14 = static_cast<int>(fTemp9);
		int iTemp15 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, iTemp14)))));
		float fTemp16 = -1.0f - fTemp10 + fTemp8;
		float fTemp17 = fTemp8 - fTemp10;
		int iTemp18 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp14))))));
		float fTemp19 = fTemp17 * fTemp16;
		int iTemp20 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp14))))));
		float fTemp21 = fTemp19 * fTemp13;
		int iTemp22 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp14))))));
		float fTemp23 = fTemp21 * fTemp12;
		int iTemp24 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, faust_wrap_add(4, iTemp14))))));
		fVec0[0] = fTemp11 * (fTemp12 * (fTemp13 * (0.041666668f * fRec2[(faust_wrap_sub(IOTA0, iTemp15)) & 2047] * fTemp16 - 0.16666667f * fTemp17 * fRec2[(faust_wrap_sub(IOTA0, iTemp18)) & 2047]) + 0.25f * fTemp19 * fRec2[(faust_wrap_sub(IOTA0, iTemp20)) & 2047]) - 0.16666667f * fTemp21 * fRec2[(faust_wrap_sub(IOTA0, iTemp22)) & 2047]) + 0.041666668f * fTemp23 * fRec2[(faust_wrap_sub(IOTA0, iTemp24)) & 2047];
		float fTemp25 = 0.3f + 0.5f * (std::fabs(static_cast<float>(inputs[0])) - fVec0[1]);
		float fTemp26 = 0.5f * fRec6[1] + std::max<float>(-1.0f, std::min<float>(1.0f, fTemp25 * (-1.0f + T_pm_flute_faustpower2_f(fTemp25))));
		fVec1[IOTA0 & 2047] = fTemp26;
		int iTemp27 = static_cast<int>(fTemp3);
		int iTemp28 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, iTemp27)))));
		float fTemp29 = -1.0f - fTemp4 + fTemp2;
		float fTemp30 = fTemp2 - fTemp4;
		int iTemp31 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp27))))));
		float fTemp32 = fTemp30 * fTemp29;
		int iTemp33 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp27))))));
		float fTemp34 = fTemp32 * fTemp7;
		int iTemp35 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp27))))));
		float fTemp36 = fTemp34 * fTemp6;
		int iTemp37 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, faust_wrap_add(4, iTemp27))))));
		fVec2[0] = fTemp5 * (fTemp6 * (fTemp7 * (0.041666668f * fVec1[(faust_wrap_sub(IOTA0, iTemp28)) & 2047] * fTemp29 - 0.16666667f * fTemp30 * fVec1[(faust_wrap_sub(IOTA0, iTemp31)) & 2047]) + 0.25f * fTemp32 * fVec1[(faust_wrap_sub(IOTA0, iTemp33)) & 2047]) - 0.16666667f * fTemp34 * fVec1[(faust_wrap_sub(IOTA0, iTemp35)) & 2047]) + 0.041666668f * fTemp36 * fVec1[(faust_wrap_sub(IOTA0, iTemp37)) & 2047];
		fRec5[0] = fVec2[1];
		fRec6[0] = fTemp5 * (fTemp6 * (fTemp7 * (0.041666668f * fTemp29 * fRec7[(faust_wrap_sub(IOTA0, iTemp28)) & 2047] - 0.16666667f * fTemp30 * fRec7[(faust_wrap_sub(IOTA0, iTemp31)) & 2047]) + 0.25f * fTemp32 * fRec7[(faust_wrap_sub(IOTA0, iTemp33)) & 2047]) - 0.16666667f * fTemp34 * fRec7[(faust_wrap_sub(IOTA0, iTemp35)) & 2047]) + 0.041666668f * fTemp36 * fRec7[(faust_wrap_sub(IOTA0, iTemp37)) & 2047];
		fRec7[IOTA0 & 2047] = fRec1[0];
		fRec8[0] = fTemp11 * (fTemp12 * (fTemp13 * (0.041666668f * fTemp16 * fRec4[(faust_wrap_sub(IOTA0, iTemp15)) & 2047] - 0.16666667f * fTemp17 * fRec4[(faust_wrap_sub(IOTA0, iTemp18)) & 2047]) + 0.25f * fTemp19 * fRec4[(faust_wrap_sub(IOTA0, iTemp20)) & 2047]) - 0.16666667f * fTemp21 * fRec4[(faust_wrap_sub(IOTA0, iTemp22)) & 2047]) + 0.041666668f * fTemp23 * fRec4[(faust_wrap_sub(IOTA0, iTemp24)) & 2047];
		fRec0[0] = 0.995f * fRec0[1] + fRec5[0] - fRec5[1];
		outputs[0] = static_cast<FAUSTFLOAT>(fRec0[0]);
		fRec1[1] = fRec1[0];
		IOTA0 = faust_wrap_add(IOTA0, 1);
		fRec3[1] = fRec3[0];
		fRec9[1] = fRec9[0];
		fRec10[1] = fRec10[0];
		fVec0[1] = fVec0[0];
		fVec2[1] = fVec2[0];
		fRec5[1] = fRec5[0];
		fRec6[1] = fRec6[0];
		fRec8[1] = fRec8[0];
		fRec0[1] = fRec0[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
