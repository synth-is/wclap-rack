/* ------------------------------------------------------------
name: "moog-ladder"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_moog_ladder -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_moog_ladder_H__
#define  __T_moog_ladder_H__

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
#define FAUSTCLASS T_moog_ladder
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

static float T_moog_ladder_faustpower4_f(float value) {
	return value * value * value * value;
}

class T_moog_ladder : public FaustDsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst3;
	float fRec4[2];
	FAUSTFLOAT fHslider1;
	float fSlow1;
	float fRec5[2];
	float fRec0[2];
	float fRec1[2];
	float fRec2[2];
	float fRec3[2];
	
 public:
	T_moog_ladder() {
	}
	
	T_moog_ladder(const T_moog_ladder&) = default;
	
	virtual ~T_moog_ladder() = default;
	
	T_moog_ladder& operator=(const T_moog_ladder&) = default;
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_moog_ladder -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "moog-ladder.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "moog-ladder");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Routing Library");
		m->declare("signals.lib/version", "1.7.0");
		m->declare("vaeffects.lib/moogLadder:author", "Dario Sanfilippo");
		m->declare("vaeffects.lib/moogLadder:license", "LicenseRef-STK-4.3");
		m->declare("vaeffects.lib/name", "Faust Virtual Analog Filter Effect Library");
		m->declare("vaeffects.lib/version", "1.6.0");
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
		fConst2 = 44.1f / fConst0;
		fConst3 = 1.0f - fConst2;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.3f);
		fHslider1 = static_cast<FAUSTFLOAT>(12.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = faust_wrap_add(l0, 1)) {
			fRec4[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec5[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = faust_wrap_add(l2, 1)) {
			fRec0[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = faust_wrap_add(l3, 1)) {
			fRec1[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = faust_wrap_add(l4, 1)) {
			fRec2[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = faust_wrap_add(l5, 1)) {
			fRec3[l5] = 0.0f;
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
	
	virtual T_moog_ladder* clone() {
		return new T_moog_ladder(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("moog-ladder");
		ui_interface->addHorizontalSlider("Q", &fHslider1, FAUSTFLOAT(12.0f), FAUSTFLOAT(0.707f), FAUSTFLOAT(24.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("normFreq", &fHslider0, FAUSTFLOAT(0.3f), FAUSTFLOAT(0.01f), FAUSTFLOAT(0.95f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst2 * static_cast<float>(fHslider0);
		fSlow1 = fConst2 * static_cast<float>(fHslider1);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		fRec4[0] = fSlow0 + fConst3 * fRec4[1];
		float fTemp0 = std::tan(fConst1 * std::pow(1e+01f, 1.0f + 3.0f * fRec4[0]));
		fRec5[0] = fSlow1 + fConst3 * fRec5[1];
		float fTemp1 = -0.70710677f + fRec5[0];
		float fTemp2 = 1.0f + fTemp0;
		float fTemp3 = fTemp0 * ((static_cast<float>(inputs[0]) - 0.1646572f * fTemp1 * (1.0f - fTemp0 / fTemp2) * (fRec3[1] + fTemp0 * (fRec2[1] + fTemp0 * (fRec0[1] + fTemp0 * fRec1[1] / fTemp2) / fTemp2) / fTemp2)) / (1.0f + 0.1646572f * (T_moog_ladder_faustpower4_f(fTemp0) * fTemp1 / T_moog_ladder_faustpower4_f(fTemp2))) - fRec1[1]) / fTemp2;
		float fTemp4 = fTemp0 * (fRec1[1] + fTemp3 - fRec0[1]) / fTemp2;
		fRec0[0] = fRec0[1] + 2.0f * fTemp4;
		fRec1[0] = fRec1[1] + 2.0f * fTemp3;
		float fTemp5 = fTemp0 * (fRec0[1] + fTemp4 - fRec2[1]) / fTemp2;
		fRec2[0] = fRec2[1] + 2.0f * fTemp5;
		float fTemp6 = fTemp0 * (fRec2[1] + fTemp5 - fRec3[1]) / fTemp2;
		fRec3[0] = fRec3[1] + 2.0f * fTemp6;
		outputs[0] = static_cast<FAUSTFLOAT>(fRec3[1] + fTemp6);
		fRec4[1] = fRec4[0];
		fRec5[1] = fRec5[0];
		fRec0[1] = fRec0[0];
		fRec1[1] = fRec1[0];
		fRec2[1] = fRec2[0];
		fRec3[1] = fRec3[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
