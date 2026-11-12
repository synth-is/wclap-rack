/* ------------------------------------------------------------
name: "ef-transpose"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_ef_transpose -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_ef_transpose_H__
#define  __T_ef_transpose_H__

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
#define FAUSTCLASS T_ef_transpose
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


class T_ef_transpose : public FaustDsp {
	
 private:
	
	int IOTA0;
	float fVec0[4096];
	int fSampleRate;
	float fConst0;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst1;
	float fRec1[2];
	float fRec0[2];
	
 public:
	T_ef_transpose() {
	}
	
	T_ef_transpose(const T_ef_transpose&) = default;
	
	virtual ~T_ef_transpose() = default;
	
	T_ef_transpose& operator=(const T_ef_transpose&) = default;
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_ef_transpose -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("filename", "ef-transpose.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("misceffects.lib/name", "Misc Effects Library");
		m->declare("misceffects.lib/version", "2.6.0");
		m->declare("name", "ef-transpose");
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
		fConst0 = 44.1f / std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 1.0f - fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(12.0f);
	}
	
	virtual void instanceClear() {
		IOTA0 = 0;
		for (int l0 = 0; l0 < 4096; l0 = faust_wrap_add(l0, 1)) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec1[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = faust_wrap_add(l2, 1)) {
			fRec0[l2] = 0.0f;
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
	
	virtual T_ef_transpose* clone() {
		return new T_ef_transpose(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("ef-transpose");
		ui_interface->addHorizontalSlider("semitones", &fHslider0, FAUSTFLOAT(12.0f), FAUSTFLOAT(-24.0f), FAUSTFLOAT(24.0f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst0 * static_cast<float>(fHslider0);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		float fTemp0 = static_cast<float>(inputs[0]);
		fVec0[IOTA0 & 4095] = fTemp0;
		fRec1[0] = fSlow0 + fConst1 * fRec1[1];
		fRec0[0] = std::fmod(1025.0f - std::pow(2.0f, 0.083333336f * fRec1[0]) + fRec0[1], 1024.0f);
		int iTemp1 = static_cast<int>(fRec0[0]);
		float fTemp2 = std::floor(fRec0[0]);
		float fTemp3 = std::min<float>(0.001953125f * fRec0[0], 1.0f);
		float fTemp4 = 1024.0f + fRec0[0];
		int iTemp5 = static_cast<int>(fTemp4);
		float fTemp6 = std::floor(fTemp4);
		outputs[0] = static_cast<FAUSTFLOAT>((fVec0[(faust_wrap_sub(IOTA0, std::min<int>(65537, std::max<int>(0, iTemp1)))) & 4095] * (1.0f - fRec0[0] + fTemp2) + (fRec0[0] - fTemp2) * fVec0[(faust_wrap_sub(IOTA0, std::min<int>(65537, std::max<int>(0, faust_wrap_add(1, iTemp1))))) & 4095]) * fTemp3 + (fVec0[(faust_wrap_sub(IOTA0, std::min<int>(65537, std::max<int>(0, iTemp5)))) & 4095] * (-1023.0f - fRec0[0] + fTemp6) + (1024.0f - fTemp6 + fRec0[0]) * fVec0[(faust_wrap_sub(IOTA0, std::min<int>(65537, std::max<int>(0, faust_wrap_add(1, iTemp5))))) & 4095]) * (1.0f - fTemp3));
		IOTA0 = faust_wrap_add(IOTA0, 1);
		fRec1[1] = fRec1[0];
		fRec0[1] = fRec0[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
