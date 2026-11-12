/* ------------------------------------------------------------
name: "compressor-mono"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_compressor_mono -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_compressor_mono_H__
#define  __T_compressor_mono_H__

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
#define FAUSTCLASS T_compressor_mono
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


class T_compressor_mono : public FaustDsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst2;
	float fRec1[2];
	float fConst3;
	FAUSTFLOAT fHslider1;
	float fSlow1;
	float fRec3[2];
	float fRec2[2];
	FAUSTFLOAT fHslider2;
	float fSlow2;
	float fRec4[2];
	FAUSTFLOAT fHslider3;
	float fSlow3;
	float fRec5[2];
	float fRec0[2];
	
 public:
	T_compressor_mono() {
	}
	
	T_compressor_mono(const T_compressor_mono&) = default;
	
	virtual ~T_compressor_mono() = default;
	
	T_compressor_mono& operator=(const T_compressor_mono&) = default;
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/amp_follower_ar:author", "Jonatan Liljedahl, revised by Romain Michon");
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "1.4.0");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_compressor_mono -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("compressors.lib/compression_gain_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compression_gain_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compression_gain_mono:license", "LicenseRef-STK-4.3");
		m->declare("compressors.lib/compressor_lad_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compressor_lad_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compressor_lad_mono:license", "LicenseRef-STK-4.3");
		m->declare("compressors.lib/compressor_mono:author", "Julius O. Smith III");
		m->declare("compressors.lib/compressor_mono:copyright", "Copyright (C) 2014-2020 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("compressors.lib/compressor_mono:license", "LicenseRef-STK-4.3");
		m->declare("compressors.lib/name", "Faust Compressor Effect Library");
		m->declare("compressors.lib/version", "1.6.0");
		m->declare("filename", "compressor-mono.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "compressor-mono");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("signals.lib/name", "Faust Routing Library");
		m->declare("signals.lib/onePoleSwitching:author", "Jonatan Liljedahl, revised by Dario Sanfilippo");
		m->declare("signals.lib/onePoleSwitching:licence", "LicenseRef-STK-4.3");
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
		fConst3 = 1.0f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.005f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.1f);
		fHslider2 = static_cast<FAUSTFLOAT>(-24.0f);
		fHslider3 = static_cast<FAUSTFLOAT>(8.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = faust_wrap_add(l0, 1)) {
			fRec1[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec3[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = faust_wrap_add(l2, 1)) {
			fRec2[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = faust_wrap_add(l3, 1)) {
			fRec4[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = faust_wrap_add(l4, 1)) {
			fRec5[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = faust_wrap_add(l5, 1)) {
			fRec0[l5] = 0.0f;
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
	
	virtual T_compressor_mono* clone() {
		return new T_compressor_mono(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("compressor-mono");
		ui_interface->addHorizontalSlider("att", &fHslider0, FAUSTFLOAT(0.005f), FAUSTFLOAT(0.0001f), FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addHorizontalSlider("ratio", &fHslider3, FAUSTFLOAT(8.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("rel", &fHslider1, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.001f), FAUSTFLOAT(3.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("thresh", &fHslider2, FAUSTFLOAT(-24.0f), FAUSTFLOAT(-6e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.1f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst1 * static_cast<float>(fHslider0);
		fSlow1 = fConst1 * static_cast<float>(fHslider1);
		fSlow2 = fConst1 * static_cast<float>(fHslider2);
		fSlow3 = fConst1 * static_cast<float>(fHslider3);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		float fTemp0 = static_cast<float>(inputs[0]);
		fRec1[0] = fSlow0 + fConst2 * fRec1[1];
		float fTemp1 = 0.5f * fRec1[0];
		int iTemp2 = std::fabs(fTemp1) < 1.1920929e-07f;
		float fTemp3 = ((iTemp2) ? 0.0f : std::exp(-(fConst3 / ((iTemp2) ? 1.0f : fTemp1))));
		float fTemp4 = std::fabs(fTemp0);
		fRec3[0] = fSlow1 + fConst2 * fRec3[1];
		int iTemp5 = std::fabs(fRec3[0]) < 1.1920929e-07f;
		int iTemp6 = std::fabs(fRec1[0]) < 1.1920929e-07f;
		float fTemp7 = ((fTemp4 > fRec2[1]) ? ((iTemp6) ? 0.0f : std::exp(-(fConst3 / ((iTemp6) ? 1.0f : fRec1[0])))) : ((iTemp5) ? 0.0f : std::exp(-(fConst3 / ((iTemp5) ? 1.0f : fRec3[0])))));
		fRec2[0] = fTemp4 * (1.0f - fTemp7) + fRec2[1] * fTemp7;
		fRec4[0] = fSlow2 + fConst2 * fRec4[1];
		fRec5[0] = fSlow3 + fConst2 * fRec5[1];
		fRec0[0] = (1.0f - fTemp3) * std::max<float>(2e+01f * std::log10(std::max<float>(1.1754944e-38f, fRec2[0])) - fRec4[0], 0.0f) * (-1.0f + 1.0f / std::max<float>(1.1920929e-07f, fRec5[0])) + fTemp3 * fRec0[1];
		outputs[0] = static_cast<FAUSTFLOAT>(fTemp0 * std::pow(1e+01f, 0.05f * fRec0[0]));
		fRec1[1] = fRec1[0];
		fRec3[1] = fRec3[0];
		fRec2[1] = fRec2[0];
		fRec4[1] = fRec4[0];
		fRec5[1] = fRec5[0];
		fRec0[1] = fRec0[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
