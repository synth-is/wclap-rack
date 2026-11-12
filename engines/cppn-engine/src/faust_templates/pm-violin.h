/* ------------------------------------------------------------
name: "pm-violin"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_pm_violin -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_pm_violin_H__
#define  __T_pm_violin_H__

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
#define FAUSTCLASS T_pm_violin
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

static float T_pm_violin_faustpower2_f(float value) {
	return value * value;
}

class T_pm_violin : public FaustDsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	float fConst4;
	int IOTA0;
	float fRec1[2048];
	float fConst5;
	float fConst6;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst7;
	float fRec9[2];
	FAUSTFLOAT fHslider1;
	float fSlow1;
	float fRec10[2];
	float fConst8;
	float fRec2[2];
	float fRec3[2];
	float fRec4[4];
	float fRec5[2048];
	float fVec0[2];
	FAUSTFLOAT fHslider2;
	float fSlow2;
	float fRec11[2];
	float fVec1[2048];
	float fRec6[2];
	float fRec7[4];
	float fRec8[2048];
	float fConst9;
	float fConst10;
	float fConst11;
	float fRec0[3];
	
 public:
	T_pm_violin() {
	}
	
	T_pm_violin(const T_pm_violin&) = default;
	
	virtual ~T_pm_violin() = default;
	
	T_pm_violin& operator=(const T_pm_violin&) = default;
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_pm_violin -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/fdelay4:author", "Julius O. Smith III");
		m->declare("delays.lib/fdelayltv:author", "Julius O. Smith III");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("filename", "pm-violin.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/resonbp:author", "Julius O. Smith III");
		m->declare("filters.lib/resonbp:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/resonbp:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/version", "1.9.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "pm-violin");
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
		fConst1 = std::tan(1570.7964f / fConst0);
		fConst2 = 1.0f / fConst1;
		fConst3 = (fConst2 + 0.5f) / fConst1 + 1.0f;
		fConst4 = 1.0f / (fConst1 * fConst3);
		fConst5 = 0.0014705883f * fConst0;
		fConst6 = 44.1f / fConst0;
		fConst7 = 1.0f - fConst6;
		fConst8 = 0.00882353f * fConst0;
		fConst9 = 1.0f / fConst3;
		fConst10 = (fConst2 + -0.5f) / fConst1 + 1.0f;
		fConst11 = 2.0f * (1.0f - 1.0f / T_pm_violin_faustpower2_f(fConst1));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.75f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.7f);
		fHslider2 = static_cast<FAUSTFLOAT>(0.5f);
	}
	
	virtual void instanceClear() {
		IOTA0 = 0;
		for (int l0 = 0; l0 < 2048; l0 = faust_wrap_add(l0, 1)) {
			fRec1[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec9[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = faust_wrap_add(l2, 1)) {
			fRec10[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = faust_wrap_add(l3, 1)) {
			fRec2[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = faust_wrap_add(l4, 1)) {
			fRec3[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 4; l5 = faust_wrap_add(l5, 1)) {
			fRec4[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2048; l6 = faust_wrap_add(l6, 1)) {
			fRec5[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = faust_wrap_add(l7, 1)) {
			fVec0[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = faust_wrap_add(l8, 1)) {
			fRec11[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2048; l9 = faust_wrap_add(l9, 1)) {
			fVec1[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = faust_wrap_add(l10, 1)) {
			fRec6[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 4; l11 = faust_wrap_add(l11, 1)) {
			fRec7[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2048; l12 = faust_wrap_add(l12, 1)) {
			fRec8[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 3; l13 = faust_wrap_add(l13, 1)) {
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
	
	virtual T_pm_violin* clone() {
		return new T_pm_violin(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("pm-violin");
		ui_interface->addHorizontalSlider("bowPosition", &fHslider1, FAUSTFLOAT(0.7f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("bowPressure", &fHslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("stringLength", &fHslider0, FAUSTFLOAT(0.75f), FAUSTFLOAT(0.1f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst6 * static_cast<float>(fHslider0);
		fSlow1 = fConst6 * static_cast<float>(fHslider1);
		fSlow2 = fConst6 * static_cast<float>(fHslider2);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		fRec1[IOTA0 & 2047] = -(0.98926467f * (0.6f * fRec7[2] + 0.2f * (fRec7[1] + fRec7[3])));
		fRec9[0] = fSlow0 + fConst7 * fRec9[1];
		float fTemp0 = -0.08f + fRec9[0];
		fRec10[0] = fSlow1 + fConst7 * fRec10[1];
		float fTemp1 = fConst5 * fTemp0 * fRec10[0];
		float fTemp2 = -1.499995f + fTemp1;
		float fTemp3 = std::floor(fTemp2);
		float fTemp4 = -4.0f - fTemp3 + fTemp1;
		float fTemp5 = -3.0f - fTemp3 + fTemp1;
		float fTemp6 = -2.0f - fTemp3 + fTemp1;
		int iTemp7 = static_cast<int>(fTemp2);
		int iTemp8 = static_cast<int>(std::min<float>(fConst8, static_cast<float>(std::max<int>(0, iTemp7))));
		float fTemp9 = -1.0f - fTemp3 + fTemp1;
		float fTemp10 = fTemp1 - fTemp3;
		int iTemp11 = static_cast<int>(std::min<float>(fConst8, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp7)))));
		float fTemp12 = fTemp10 * fTemp9;
		int iTemp13 = static_cast<int>(std::min<float>(fConst8, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp7)))));
		float fTemp14 = fTemp12 * fTemp6;
		int iTemp15 = static_cast<int>(std::min<float>(fConst8, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp7)))));
		float fTemp16 = fTemp14 * fTemp5;
		int iTemp17 = static_cast<int>(std::min<float>(fConst8, static_cast<float>(std::max<int>(0, faust_wrap_add(4, iTemp7)))));
		fRec2[0] = fTemp4 * (fTemp5 * (fTemp6 * (0.041666668f * fRec8[(faust_wrap_sub(IOTA0, faust_wrap_add(1, iTemp8))) & 2047] * fTemp9 - 0.16666667f * fTemp10 * fRec8[(faust_wrap_sub(IOTA0, faust_wrap_add(1, iTemp11))) & 2047]) + 0.25f * fTemp12 * fRec8[(faust_wrap_sub(IOTA0, faust_wrap_add(1, iTemp13))) & 2047]) - 0.16666667f * fTemp14 * fRec8[(faust_wrap_sub(IOTA0, faust_wrap_add(1, iTemp15))) & 2047]) + 0.041666668f * fTemp16 * fRec8[(faust_wrap_sub(IOTA0, faust_wrap_add(1, iTemp17))) & 2047];
		float fTemp18 = fConst5 * fTemp0 * (1.0f - fRec10[0]);
		float fTemp19 = -1.499995f + fTemp18;
		float fTemp20 = std::floor(fTemp19);
		float fTemp21 = -4.0f - fTemp20 + fTemp18;
		float fTemp22 = -3.0f - fTemp20 + fTemp18;
		float fTemp23 = -2.0f - fTemp20 + fTemp18;
		int iTemp24 = static_cast<int>(fTemp19);
		int iTemp25 = static_cast<int>(std::min<float>(fConst8, static_cast<float>(std::max<int>(0, iTemp24))));
		float fTemp26 = -1.0f - fTemp20 + fTemp18;
		float fTemp27 = fTemp18 - fTemp20;
		int iTemp28 = static_cast<int>(std::min<float>(fConst8, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp24)))));
		float fTemp29 = fTemp27 * fTemp26;
		int iTemp30 = static_cast<int>(std::min<float>(fConst8, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp24)))));
		float fTemp31 = fTemp29 * fTemp23;
		int iTemp32 = static_cast<int>(std::min<float>(fConst8, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp24)))));
		float fTemp33 = fTemp31 * fTemp22;
		int iTemp34 = static_cast<int>(std::min<float>(fConst8, static_cast<float>(std::max<int>(0, faust_wrap_add(4, iTemp24)))));
		fRec3[0] = fTemp21 * (fTemp22 * (fTemp23 * (0.041666668f * fRec1[(faust_wrap_sub(IOTA0, faust_wrap_add(1, iTemp25))) & 2047] * fTemp26 - 0.16666667f * fTemp27 * fRec1[(faust_wrap_sub(IOTA0, faust_wrap_add(1, iTemp28))) & 2047]) + 0.25f * fTemp29 * fRec1[(faust_wrap_sub(IOTA0, faust_wrap_add(1, iTemp30))) & 2047]) - 0.16666667f * fTemp31 * fRec1[(faust_wrap_sub(IOTA0, faust_wrap_add(1, iTemp32))) & 2047]) + 0.041666668f * fTemp33 * fRec1[(faust_wrap_sub(IOTA0, faust_wrap_add(1, iTemp34))) & 2047];
		fRec4[0] = fRec2[1];
		fRec5[IOTA0 & 2047] = -(0.99880147f * (0.8f * fRec4[2] + 0.1f * (fRec4[1] + fRec4[3])));
		fVec0[0] = fTemp4 * (fTemp5 * (fTemp6 * (0.041666668f * fTemp9 * fRec5[(faust_wrap_sub(IOTA0, faust_wrap_add(2, iTemp8))) & 2047] - 0.16666667f * fTemp10 * fRec5[(faust_wrap_sub(IOTA0, faust_wrap_add(2, iTemp11))) & 2047]) + 0.25f * fTemp12 * fRec5[(faust_wrap_sub(IOTA0, faust_wrap_add(2, iTemp13))) & 2047]) - 0.16666667f * fTemp14 * fRec5[(faust_wrap_sub(IOTA0, faust_wrap_add(2, iTemp15))) & 2047]) + 0.041666668f * fTemp16 * fRec5[(faust_wrap_sub(IOTA0, faust_wrap_add(2, iTemp17))) & 2047];
		float fTemp35 = static_cast<float>(inputs[0]) - (fRec3[1] + fVec0[1]);
		fRec11[0] = fSlow2 + fConst7 * fRec11[1];
		float fTemp36 = fTemp35 * std::min<float>(1.0f, std::pow(0.75f + std::fabs(fTemp35 * (5.0f - 4.0f * fRec11[0])), -4.0f));
		float fTemp37 = fVec0[1] + fTemp36;
		fVec1[IOTA0 & 2047] = fTemp37;
		fRec6[0] = fTemp21 * (fTemp22 * (fTemp23 * (0.041666668f * fTemp26 * fVec1[(faust_wrap_sub(IOTA0, iTemp25)) & 2047] - 0.16666667f * fTemp27 * fVec1[(faust_wrap_sub(IOTA0, iTemp28)) & 2047]) + 0.25f * fTemp29 * fVec1[(faust_wrap_sub(IOTA0, iTemp30)) & 2047]) - 0.16666667f * fTemp31 * fVec1[(faust_wrap_sub(IOTA0, iTemp32)) & 2047]) + 0.041666668f * fTemp33 * fVec1[(faust_wrap_sub(IOTA0, iTemp34)) & 2047];
		fRec7[0] = fRec6[1];
		fRec8[IOTA0 & 2047] = fRec3[1] + fTemp36;
		fRec0[0] = fRec7[1] - fConst9 * (fConst10 * fRec0[2] + fConst11 * fRec0[1]);
		outputs[0] = static_cast<FAUSTFLOAT>(fConst4 * (fRec0[0] - fRec0[2]));
		IOTA0 = faust_wrap_add(IOTA0, 1);
		fRec9[1] = fRec9[0];
		fRec10[1] = fRec10[0];
		fRec2[1] = fRec2[0];
		fRec3[1] = fRec3[0];
		for (int j0 = 3; j0 > 0; j0 = faust_wrap_sub(j0, 1)) {
			fRec4[j0] = fRec4[faust_wrap_sub(j0, 1)];
		}
		fVec0[1] = fVec0[0];
		fRec11[1] = fRec11[0];
		fRec6[1] = fRec6[0];
		for (int j1 = 3; j1 > 0; j1 = faust_wrap_sub(j1, 1)) {
			fRec7[j1] = fRec7[faust_wrap_sub(j1, 1)];
		}
		fRec0[2] = fRec0[1];
		fRec0[1] = fRec0[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
