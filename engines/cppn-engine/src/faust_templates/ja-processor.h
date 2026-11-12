/* ------------------------------------------------------------
name: "ja-processor"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_ja_processor -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_ja_processor_H__
#define  __T_ja_processor_H__

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif 

/* link with : "" */
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
#define FAUSTCLASS T_ja_processor
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

static float T_ja_processor_faustpower2_f(float value) {
	return value * value;
}

class T_ja_processor : public FaustDsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	float fConst3;
	float fConst4;
	float fConst5;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst6;
	float fRec0[2];
	FAUSTFLOAT fHslider1;
	float fSlow1;
	float fRec2[2];
	float fVec0[3];
	FAUSTFLOAT fHslider2;
	float fSlow2;
	float fRec3[2];
	FAUSTFLOAT fHslider3;
	float fSlow3;
	float fRec4[2];
	FAUSTFLOAT fHslider4;
	float fSlow4;
	float fRec5[2];
	FAUSTFLOAT fHslider5;
	float fSlow5;
	float fRec6[2];
	float fRec1[2];
	float fConst7;
	float fConst8;
	float fRec7[2];
	float fConst9;
	float fRec8[2];
	FAUSTFLOAT fHslider6;
	float fSlow6;
	float fRec9[2];
	
 public:
	T_ja_processor() {
	}
	
	T_ja_processor(const T_ja_processor&) = default;
	
	virtual ~T_ja_processor() = default;
	
	T_ja_processor& operator=(const T_ja_processor&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_ja_processor -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "ja-processor.dsp");
		m->declare("filters.lib/SVFTPT:author", "Dario Sanfilippo");
		m->declare("filters.lib/SVFTPT:copyright", "Copyright (C) 2024 Dario Sanfilippo <sanfilippo.dario@gmail.com>");
		m->declare("filters.lib/SVFTPT:license", "MIT");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/version", "1.9.0");
		m->declare("hysteresis.lib/author", "Thomas Mandolini");
		m->declare("hysteresis.lib/contributor", "Bart Brouns a.k.a magnetophon");
		m->declare("hysteresis.lib/ja_hysteresis:author", "Thomas Mandolini");
		m->declare("hysteresis.lib/ja_hysteresis:license", "LGPL-2.1-only");
		m->declare("hysteresis.lib/ja_processor:author", "Thomas Mandolini");
		m->declare("hysteresis.lib/ja_processor:license", "LGPL-2.1-only");
		m->declare("hysteresis.lib/name", "Faust Hysteresis Library");
		m->declare("hysteresis.lib/version", "1.0.1");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "ja-processor");
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
		fConst1 = std::tan(21.991148f / fConst0);
		fConst2 = fConst1 + 1.4142271f;
		fConst3 = fConst1 * fConst2 + 1.0f;
		fConst4 = 316.22775f / fConst3;
		fConst5 = 44.1f / fConst0;
		fConst6 = 1.0f - fConst5;
		fConst7 = fConst1 / fConst3;
		fConst8 = 2.0f * fConst7;
		fConst9 = 2.0f * fConst1;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(7.2e+02f);
		fHslider1 = static_cast<FAUSTFLOAT>(2.5f);
		fHslider2 = static_cast<FAUSTFLOAT>(0.5f);
		fHslider3 = static_cast<FAUSTFLOAT>(0.015f);
		fHslider4 = static_cast<FAUSTFLOAT>(3.8e+02f);
		fHslider5 = static_cast<FAUSTFLOAT>(3.8e+02f);
		fHslider6 = static_cast<FAUSTFLOAT>(1.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = faust_wrap_add(l0, 1)) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec2[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 3; l2 = faust_wrap_add(l2, 1)) {
			fVec0[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = faust_wrap_add(l3, 1)) {
			fRec3[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = faust_wrap_add(l4, 1)) {
			fRec4[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = faust_wrap_add(l5, 1)) {
			fRec5[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = faust_wrap_add(l6, 1)) {
			fRec6[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = faust_wrap_add(l7, 1)) {
			fRec1[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = faust_wrap_add(l8, 1)) {
			fRec7[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = faust_wrap_add(l9, 1)) {
			fRec8[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = faust_wrap_add(l10, 1)) {
			fRec9[l10] = 0.0f;
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
	
	virtual T_ja_processor* clone() {
		return new T_ja_processor(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("ja-processor");
		ui_interface->addHorizontalSlider("Ms", &fHslider4, FAUSTFLOAT(3.8e+02f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("a", &fHslider0, FAUSTFLOAT(7.2e+02f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(2e+03f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("alpha", &fHslider3, FAUSTFLOAT(0.015f), FAUSTFLOAT(0.001f), FAUSTFLOAT(0.1f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("c", &fHslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("drive", &fHslider1, FAUSTFLOAT(2.5f), FAUSTFLOAT(0.5f), FAUSTFLOAT(5.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("k", &fHslider5, FAUSTFLOAT(3.8e+02f), FAUSTFLOAT(5e+01f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("trim", &fHslider6, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.5f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst5 * static_cast<float>(fHslider0);
		fSlow1 = fConst5 * static_cast<float>(fHslider1);
		fSlow2 = fConst5 * static_cast<float>(fHslider2);
		fSlow3 = fConst5 * static_cast<float>(fHslider3);
		fSlow4 = fConst5 * static_cast<float>(fHslider4);
		fSlow5 = fConst5 * static_cast<float>(fHslider5);
		fSlow6 = fConst5 * static_cast<float>(fHslider6);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		fRec0[0] = fSlow0 + fConst6 * fRec0[1];
		fRec2[0] = fSlow1 + fConst6 * fRec2[1];
		float fTemp0 = static_cast<float>(inputs[0]) * fRec2[0];
		fVec0[0] = fTemp0;
		float fTemp1 = fVec0[1] - fVec0[2];
		float fTemp2 = 0.0004446953f * fTemp1;
		float fTemp3 = 0.00049410586f * fTemp0;
		float fTemp4 = fTemp0 - fVec0[1];
		float fTemp5 = 0.00014823176f * fTemp4;
		float fTemp6 = fTemp2 + fTemp3 - (fTemp5 + 0.00049410586f * fVec0[1]);
		fRec3[0] = fSlow2 + fConst6 * fRec3[1];
		float fTemp7 = fTemp2 + 0.002668172f * fVec0[1] + fTemp3;
		fRec4[0] = fSlow3 + fConst6 * fRec4[1];
		fRec5[0] = fSlow4 + fConst6 * fRec5[1];
		float fTemp8 = std::max<float>(fRec5[0], 1e-06f);
		float fTemp9 = std::max<float>(fRec0[0] / fTemp8, 1e-09f);
		float fTemp10 = tanhf((fTemp7 + fRec1[1] * fRec4[0] - fTemp5) / fTemp9);
		float fTemp11 = 1.0f - T_ja_processor_faustpower2_f(fTemp10);
		float fTemp12 = fTemp10 - fRec1[1];
		float fTemp13 = 1.0f + 3.0f * std::fabs(fTemp12);
		fRec6[0] = fSlow5 + fConst6 * fRec6[1];
		float fTemp14 = fRec3[0] * fRec4[0];
		float fTemp15 = std::max<float>(-1.0f, std::min<float>(1.0f, fRec1[1] + fTemp6 * (fRec3[0] * fTemp11 / fTemp9 + fTemp12 / (fTemp13 * (0.001f - fRec4[0] * fTemp12 / fTemp13 + ((fTemp6 >= 0.0f) ? 1.0f : -1.0f) * fRec6[0] / fTemp8))) / (1.0f - fTemp14 * fTemp11 / fTemp9)));
		float fTemp16 = 0.0003952847f * (fTemp1 - fTemp4);
		float fTemp17 = 0.0015811388f * (fTemp0 + fVec0[1]);
		float fTemp18 = fTemp16 + fTemp5 + fTemp17 - fTemp7;
		float fTemp19 = tanhf((fTemp17 + fTemp16 + fRec4[0] * fTemp15) / fTemp9);
		float fTemp20 = 1.0f - T_ja_processor_faustpower2_f(fTemp19);
		float fTemp21 = fTemp19 - fTemp15;
		float fTemp22 = 1.0f + 3.0f * std::fabs(fTemp21);
		float fTemp23 = std::max<float>(-1.0f, std::min<float>(1.0f, fTemp15 + fTemp18 * (fRec3[0] * fTemp20 / fTemp9 + fTemp21 / (fTemp22 * (0.001f - fRec4[0] * fTemp21 / fTemp22 + fRec6[0] * ((fTemp18 >= 0.0f) ? 1.0f : -1.0f) / fTemp8))) / (1.0f - fTemp14 * fTemp20 / fTemp9)));
		float fTemp24 = 0.00014823176f * fTemp1;
		float fTemp25 = 0.00049410586f * fVec0[1];
		float fTemp26 = fTemp24 + fTemp25 + 0.002668172f * fTemp0;
		float fTemp27 = 0.0004446953f * fTemp4;
		float fTemp28 = fTemp26 - (fTemp16 + fTemp17 + fTemp27);
		float fTemp29 = tanhf((fTemp26 + fRec4[0] * fTemp23 - fTemp27) / fTemp9);
		float fTemp30 = 1.0f - T_ja_processor_faustpower2_f(fTemp29);
		float fTemp31 = fTemp29 - fTemp23;
		float fTemp32 = 1.0f + 3.0f * std::fabs(fTemp31);
		float fTemp33 = std::max<float>(-1.0f, std::min<float>(1.0f, fTemp23 + fTemp28 * (fRec3[0] * fTemp30 / fTemp9 + fTemp31 / (fTemp32 * (0.001f - fRec4[0] * fTemp31 / fTemp32 + fRec6[0] * ((fTemp28 >= 0.0f) ? 1.0f : -1.0f) / fTemp8))) / (1.0f - fTemp14 * fTemp30 / fTemp9)));
		float fTemp34 = fTemp27 + 0.00049410586f * fTemp0 - (fTemp25 + fTemp24);
		float fTemp35 = tanhf((0.0031622776f * fTemp0 + fRec4[0] * fTemp33) / fTemp9);
		float fTemp36 = 1.0f - T_ja_processor_faustpower2_f(fTemp35);
		float fTemp37 = fTemp35 - fTemp33;
		float fTemp38 = 1.0f + 3.0f * std::fabs(fTemp37);
		fRec1[0] = std::max<float>(-1.0f, std::min<float>(1.0f, fTemp33 + fTemp34 * (fRec3[0] * fTemp36 / fTemp9 + fTemp37 / (fTemp38 * (0.001f - fRec4[0] * fTemp37 / fTemp38 + fRec6[0] * ((fTemp34 >= 0.0f) ? 1.0f : -1.0f) / fTemp8))) / (1.0f - fTemp14 * fTemp36 / fTemp9)));
		float fTemp39 = fRec0[0] * fRec1[0] / std::max<float>(0.01f, fRec3[0] * fRec5[0]) - (fConst2 * fRec7[1] + fRec8[1]);
		fRec7[0] = fRec7[1] + fConst8 * fTemp39;
		fRec8[0] = fRec8[1] + fConst9 * (fRec7[1] + fConst7 * fTemp39);
		fRec9[0] = fSlow6 + fConst6 * fRec9[1];
		outputs[0] = static_cast<FAUSTFLOAT>(fConst4 * (fTemp39 * fRec9[0] / std::max<float>(0.001f, fRec2[0])));
		fRec0[1] = fRec0[0];
		fRec2[1] = fRec2[0];
		fVec0[2] = fVec0[1];
		fVec0[1] = fVec0[0];
		fRec3[1] = fRec3[0];
		fRec4[1] = fRec4[0];
		fRec5[1] = fRec5[0];
		fRec6[1] = fRec6[0];
		fRec1[1] = fRec1[0];
		fRec7[1] = fRec7[0];
		fRec8[1] = fRec8[0];
		fRec9[1] = fRec9[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
