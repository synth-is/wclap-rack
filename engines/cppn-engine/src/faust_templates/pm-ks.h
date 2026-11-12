/* ------------------------------------------------------------
name: "pm-ks"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_pm_ks -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_pm_ks_H__
#define  __T_pm_ks_H__

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
#define FAUSTCLASS T_pm_ks
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


class T_pm_ks : public FaustDsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst3;
	float fRec0[2];
	FAUSTFLOAT fHslider1;
	float fSlow1;
	float fRec5[2];
	int IOTA0;
	float fRec1[2048];
	float fConst4;
	float fRec2[2];
	float fVec0[2048];
	float fRec3[3];
	float fRec4[3];
	
 public:
	T_pm_ks() {
	}
	
	T_pm_ks(const T_pm_ks&) = default;
	
	virtual ~T_pm_ks() = default;
	
	T_pm_ks& operator=(const T_pm_ks&) = default;
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_pm_ks -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/fdelay4:author", "Julius O. Smith III");
		m->declare("delays.lib/fdelayltv:author", "Julius O. Smith III");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("filename", "pm-ks.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "pm-ks");
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
		fHslider0 = static_cast<FAUSTFLOAT>(0.5f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.1f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = faust_wrap_add(l0, 1)) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec5[l1] = 0.0f;
		}
		IOTA0 = 0;
		for (int l2 = 0; l2 < 2048; l2 = faust_wrap_add(l2, 1)) {
			fRec1[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = faust_wrap_add(l3, 1)) {
			fRec2[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2048; l4 = faust_wrap_add(l4, 1)) {
			fVec0[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 3; l5 = faust_wrap_add(l5, 1)) {
			fRec3[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 3; l6 = faust_wrap_add(l6, 1)) {
			fRec4[l6] = 0.0f;
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
	
	virtual T_pm_ks* clone() {
		return new T_pm_ks(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("pm-ks");
		ui_interface->addHorizontalSlider("damping", &fHslider1, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("length", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.1f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst2 * static_cast<float>(fHslider0);
		fSlow1 = fConst2 * static_cast<float>(fHslider1);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		fRec0[0] = fSlow0 + fConst3 * fRec0[1];
		float fTemp0 = fConst1 * (-0.05f + fRec0[0]);
		float fTemp1 = -1.499995f + fTemp0;
		float fTemp2 = std::floor(fTemp1);
		float fTemp3 = fTemp0 - fTemp2;
		float fTemp4 = -1.0f - fTemp2 + fTemp0;
		float fTemp5 = fTemp3 * fTemp4;
		float fTemp6 = -2.0f - fTemp2 + fTemp0;
		float fTemp7 = fTemp5 * fTemp6;
		float fTemp8 = -3.0f - fTemp2 + fTemp0;
		float fTemp9 = fTemp7 * fTemp8;
		float fTemp10 = 0.5f * static_cast<float>(inputs[0]);
		fRec5[0] = fSlow1 + fConst3 * fRec5[1];
		float fTemp11 = 0.5f * (fRec3[1] + fRec3[2]) * (0.8f + 0.2f * (1.0f - fRec5[0]));
		fRec1[IOTA0 & 2047] = fTemp11;
		float fTemp12 = -4.0f - fTemp2 + fTemp0;
		int iTemp13 = static_cast<int>(fTemp1);
		int iTemp14 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, iTemp13)))));
		int iTemp15 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp13))))));
		int iTemp16 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp13))))));
		int iTemp17 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp13))))));
		int iTemp18 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, faust_wrap_add(4, iTemp13))))));
		fRec2[0] = fTemp12 * (fTemp8 * (fTemp6 * (0.041666668f * fRec1[(faust_wrap_sub(IOTA0, iTemp14)) & 2047] * fTemp4 - 0.16666667f * fTemp3 * fRec1[(faust_wrap_sub(IOTA0, iTemp15)) & 2047]) + 0.25f * fTemp5 * fRec1[(faust_wrap_sub(IOTA0, iTemp16)) & 2047]) - 0.16666667f * fTemp7 * fRec1[(faust_wrap_sub(IOTA0, iTemp17)) & 2047]) + 0.041666668f * fTemp9 * fRec1[(faust_wrap_sub(IOTA0, iTemp18)) & 2047];
		float fTemp19 = fTemp10 + fRec4[2];
		fVec0[IOTA0 & 2047] = fTemp19;
		float fTemp20 = fTemp12 * (fTemp8 * (fTemp6 * (0.041666668f * fTemp4 * fVec0[(faust_wrap_sub(IOTA0, iTemp14)) & 2047] - 0.16666667f * fTemp3 * fVec0[(faust_wrap_sub(IOTA0, iTemp15)) & 2047]) + 0.25f * fTemp5 * fVec0[(faust_wrap_sub(IOTA0, iTemp16)) & 2047]) - 0.16666667f * fTemp7 * fVec0[(faust_wrap_sub(IOTA0, iTemp17)) & 2047]);
		float fTemp21 = 0.041666668f * fTemp9 * fVec0[(faust_wrap_sub(IOTA0, iTemp18)) & 2047];
		fRec3[0] = fTemp20 + fTemp21;
		fRec4[0] = fTemp10 + fRec2[1];
		outputs[0] = static_cast<FAUSTFLOAT>(fTemp21 + fTemp11 + fTemp20);
		fRec0[1] = fRec0[0];
		fRec5[1] = fRec5[0];
		IOTA0 = faust_wrap_add(IOTA0, 1);
		fRec2[1] = fRec2[0];
		fRec3[2] = fRec3[1];
		fRec3[1] = fRec3[0];
		fRec4[2] = fRec4[1];
		fRec4[1] = fRec4[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
