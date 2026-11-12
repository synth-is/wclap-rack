/* ------------------------------------------------------------
name: "ef-wavefold"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_ef_wavefold -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_ef_wavefold_H__
#define  __T_ef_wavefold_H__

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
#define FAUSTCLASS T_ef_wavefold
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


class T_ef_wavefold : public FaustDsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst1;
	float fRec0[2];
	FAUSTFLOAT fHslider1;
	float fSlow1;
	float fRec1[2];
	
 public:
	T_ef_wavefold() {
	}
	
	T_ef_wavefold(const T_ef_wavefold&) = default;
	
	virtual ~T_ef_wavefold() = default;
	
	T_ef_wavefold& operator=(const T_ef_wavefold&) = default;
	
	void metadata(Meta* m) { 
		m->declare("aanl.lib/clip:deprecated", "internal helper, renamed _clip");
		m->declare("aanl.lib/name", "Faust Antialiased Nonlinearities");
		m->declare("aanl.lib/version", "1.5.0");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_ef_wavefold -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "ef-wavefold.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("misceffects.lib/name", "Misc Effects Library");
		m->declare("misceffects.lib/version", "2.6.0");
		m->declare("misceffects.lib/wavefold:author", "David Braun");
		m->declare("name", "ef-wavefold");
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
		fHslider0 = static_cast<FAUSTFLOAT>(0.5f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = faust_wrap_add(l0, 1)) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec1[l1] = 0.0f;
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
	
	virtual T_ef_wavefold* clone() {
		return new T_ef_wavefold(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("ef-wavefold");
		ui_interface->addHorizontalSlider("output", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("threshold", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.05f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst0 * static_cast<float>(fHslider0);
		fSlow1 = fConst0 * static_cast<float>(fHslider1);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		fRec0[0] = fSlow0 + fConst1 * fRec0[1];
		int iTemp0 = fRec0[0] < 0.0f;
		float fTemp1 = ((iTemp0) ? -fRec0[0] : fRec0[0]);
		float fTemp2 = std::max<float>(0.0f, std::min<float>(1.0f, static_cast<float>(inputs[0])));
		float fTemp3 = 0.8f * fTemp2;
		float fTemp4 = 1.0f - fTemp3;
		float fTemp5 = 1.25f * ((fTemp3 + -1.0f + fTemp1) / fTemp2);
		float fTemp6 = ((fTemp1 > fTemp4) ? 1.0f + fTemp2 * (-1.0f + 0.4f * std::fabs(-0.5f - std::floor(fTemp5) + fTemp5)) : fTemp1) / fTemp4;
		fRec1[0] = fSlow1 + fConst1 * fRec1[1];
		outputs[0] = static_cast<FAUSTFLOAT>(((iTemp0) ? -fTemp6 : fTemp6) * fRec1[0]);
		fRec0[1] = fRec0[0];
		fRec1[1] = fRec1[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
