/* ------------------------------------------------------------
name: "pm-clarinet"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_pm_clarinet -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_pm_clarinet_H__
#define  __T_pm_clarinet_H__

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
#define FAUSTCLASS T_pm_clarinet
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


class T_pm_clarinet : public FaustDsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst2;
	float fRec6[2];
	float fRec0[2];
	FAUSTFLOAT fHslider1;
	float fSlow1;
	float fRec7[2];
	int IOTA0;
	float fRec1[2048];
	float fConst3;
	FAUSTFLOAT fHslider2;
	float fSlow2;
	float fRec8[2];
	float fConst4;
	float fRec2[2];
	float fRec3[2048];
	float fRec4[2];
	float fVec0[2];
	float fRec5[2];
	
 public:
	T_pm_clarinet() {
	}
	
	T_pm_clarinet(const T_pm_clarinet&) = default;
	
	virtual ~T_pm_clarinet() = default;
	
	T_pm_clarinet& operator=(const T_pm_clarinet&) = default;
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_pm_clarinet -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/fdelay4:author", "Julius O. Smith III");
		m->declare("delays.lib/fdelayltv:author", "Julius O. Smith III");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("filename", "pm-clarinet.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "pm-clarinet");
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
		fConst3 = 0.0014705883f * fConst0;
		fConst4 = 0.00882353f * fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.5f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.5f);
		fHslider2 = static_cast<FAUSTFLOAT>(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = faust_wrap_add(l0, 1)) {
			fRec6[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = faust_wrap_add(l2, 1)) {
			fRec7[l2] = 0.0f;
		}
		IOTA0 = 0;
		for (int l3 = 0; l3 < 2048; l3 = faust_wrap_add(l3, 1)) {
			fRec1[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = faust_wrap_add(l4, 1)) {
			fRec8[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = faust_wrap_add(l5, 1)) {
			fRec2[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2048; l6 = faust_wrap_add(l6, 1)) {
			fRec3[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = faust_wrap_add(l7, 1)) {
			fRec4[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = faust_wrap_add(l8, 1)) {
			fVec0[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = faust_wrap_add(l9, 1)) {
			fRec5[l9] = 0.0f;
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
	
	virtual T_pm_clarinet* clone() {
		return new T_pm_clarinet(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("pm-clarinet");
		ui_interface->addHorizontalSlider("bellOpening", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("reedStiffness", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("tubeLength", &fHslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.05f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst1 * static_cast<float>(fHslider0);
		fSlow1 = fConst1 * static_cast<float>(fHslider1);
		fSlow2 = fConst1 * static_cast<float>(fHslider2);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		fRec6[0] = fSlow0 + fConst2 * fRec6[1];
		fRec0[0] = (1.0f - fRec6[0]) * fRec5[1] + fRec6[0] * fRec0[1];
		float fTemp0 = std::fabs(static_cast<float>(inputs[0]));
		fRec7[0] = fSlow1 + fConst2 * fRec7[1];
		fRec1[IOTA0 & 2047] = fTemp0 - fRec4[1] * std::max<float>(-1.0f, std::min<float>(1.0f, 0.7f - fRec4[1] * (-0.44f + 0.26f * fRec7[0])));
		fRec8[0] = fSlow2 + fConst2 * fRec8[1];
		float fTemp1 = fConst3 * (-0.05f + 0.5f * fRec8[0]);
		float fTemp2 = -1.499995f + fTemp1;
		float fTemp3 = std::floor(fTemp2);
		float fTemp4 = -4.0f - fTemp3 + fTemp1;
		float fTemp5 = -3.0f - fTemp3 + fTemp1;
		float fTemp6 = -2.0f - fTemp3 + fTemp1;
		int iTemp7 = static_cast<int>(fTemp2);
		int iTemp8 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, iTemp7)))));
		float fTemp9 = -1.0f - fTemp3 + fTemp1;
		float fTemp10 = fTemp1 - fTemp3;
		int iTemp11 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp7))))));
		float fTemp12 = fTemp10 * fTemp9;
		int iTemp13 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp7))))));
		float fTemp14 = fTemp12 * fTemp6;
		int iTemp15 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp7))))));
		float fTemp16 = fTemp14 * fTemp5;
		int iTemp17 = faust_wrap_add(1, static_cast<int>(std::min<float>(fConst4, static_cast<float>(std::max<int>(0, faust_wrap_add(4, iTemp7))))));
		fRec2[0] = fTemp4 * (fTemp5 * (fTemp6 * (0.041666668f * fRec3[(faust_wrap_sub(IOTA0, iTemp8)) & 2047] * fTemp9 - 0.16666667f * fTemp10 * fRec3[(faust_wrap_sub(IOTA0, iTemp11)) & 2047]) + 0.25f * fTemp12 * fRec3[(faust_wrap_sub(IOTA0, iTemp13)) & 2047]) - 0.16666667f * fTemp14 * fRec3[(faust_wrap_sub(IOTA0, iTemp15)) & 2047]) + 0.041666668f * fTemp16 * fRec3[(faust_wrap_sub(IOTA0, iTemp17)) & 2047];
		fRec3[IOTA0 & 2047] = fRec0[0];
		fRec4[0] = fTemp0 + fRec2[1];
		fVec0[0] = fTemp4 * (fTemp5 * (fTemp6 * (0.041666668f * fTemp9 * fRec1[(faust_wrap_sub(IOTA0, iTemp8)) & 2047] - 0.16666667f * fTemp10 * fRec1[(faust_wrap_sub(IOTA0, iTemp11)) & 2047]) + 0.25f * fTemp12 * fRec1[(faust_wrap_sub(IOTA0, iTemp13)) & 2047]) - 0.16666667f * fTemp14 * fRec1[(faust_wrap_sub(IOTA0, iTemp15)) & 2047]) + 0.041666668f * fTemp16 * fRec1[(faust_wrap_sub(IOTA0, iTemp17)) & 2047];
		fRec5[0] = fVec0[1];
		outputs[0] = static_cast<FAUSTFLOAT>(fRec5[0]);
		fRec6[1] = fRec6[0];
		fRec0[1] = fRec0[0];
		fRec7[1] = fRec7[0];
		IOTA0 = faust_wrap_add(IOTA0, 1);
		fRec8[1] = fRec8[0];
		fRec2[1] = fRec2[0];
		fRec4[1] = fRec4[0];
		fVec0[1] = fVec0[0];
		fRec5[1] = fRec5[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
