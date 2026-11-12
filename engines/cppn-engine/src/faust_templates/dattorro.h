/* ------------------------------------------------------------
name: "dattorro"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_dattorro -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_dattorro_H__
#define  __T_dattorro_H__

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
#define FAUSTCLASS T_dattorro
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


class T_dattorro : public FaustDsp {
	
 private:
	
	int IOTA0;
	float fVec0[256];
	int fSampleRate;
	float fConst0;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst1;
	float fRec0[2];
	float fVec1[1024];
	float fRec1[2];
	float fVec2[1024];
	FAUSTFLOAT fHslider1;
	float fSlow1;
	float fRec13[2];
	float fVec3[8192];
	float fRec4[2];
	float fRec5[2];
	FAUSTFLOAT fHslider2;
	float fSlow2;
	float fRec14[2];
	float fVec4[2048];
	float fRec6[2];
	FAUSTFLOAT fHslider3;
	float fSlow3;
	float fRec19[2];
	float fRec18[3];
	float fVec5[256];
	float fRec17[2];
	float fVec6[128];
	float fRec16[2];
	float fVec7[512];
	float fRec15[2];
	float fVec8[512];
	float fRec20[2];
	float fVec9[2048];
	float fRec8[2];
	float fVec10[8192];
	float fRec9[2];
	float fVec11[4096];
	float fRec11[2];
	float fVec12[4096];
	float fRec12[2];
	
 public:
	T_dattorro() {
	}
	
	T_dattorro(const T_dattorro&) = default;
	
	virtual ~T_dattorro() = default;
	
	T_dattorro& operator=(const T_dattorro&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_dattorro -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "dattorro.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "dattorro");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("reverbs.lib/dattorro_rev:author", "Jakob Zerbian");
		m->declare("reverbs.lib/dattorro_rev:licence", "LicenseRef-STK-4.3");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "1.5.1");
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
		fConst0 = 44.1f / std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 1.0f - fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.4f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.2f);
		fHslider2 = static_cast<FAUSTFLOAT>(0.4f);
		fHslider3 = static_cast<FAUSTFLOAT>(0.5f);
	}
	
	virtual void instanceClear() {
		IOTA0 = 0;
		for (int l0 = 0; l0 < 256; l0 = faust_wrap_add(l0, 1)) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 1024; l2 = faust_wrap_add(l2, 1)) {
			fVec1[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = faust_wrap_add(l3, 1)) {
			fRec1[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 1024; l4 = faust_wrap_add(l4, 1)) {
			fVec2[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = faust_wrap_add(l5, 1)) {
			fRec13[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 8192; l6 = faust_wrap_add(l6, 1)) {
			fVec3[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = faust_wrap_add(l7, 1)) {
			fRec4[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = faust_wrap_add(l8, 1)) {
			fRec5[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = faust_wrap_add(l9, 1)) {
			fRec14[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2048; l10 = faust_wrap_add(l10, 1)) {
			fVec4[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = faust_wrap_add(l11, 1)) {
			fRec6[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = faust_wrap_add(l12, 1)) {
			fRec19[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 3; l13 = faust_wrap_add(l13, 1)) {
			fRec18[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 256; l14 = faust_wrap_add(l14, 1)) {
			fVec5[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = faust_wrap_add(l15, 1)) {
			fRec17[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 128; l16 = faust_wrap_add(l16, 1)) {
			fVec6[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = faust_wrap_add(l17, 1)) {
			fRec16[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 512; l18 = faust_wrap_add(l18, 1)) {
			fVec7[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = faust_wrap_add(l19, 1)) {
			fRec15[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 512; l20 = faust_wrap_add(l20, 1)) {
			fVec8[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = faust_wrap_add(l21, 1)) {
			fRec20[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2048; l22 = faust_wrap_add(l22, 1)) {
			fVec9[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = faust_wrap_add(l23, 1)) {
			fRec8[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 8192; l24 = faust_wrap_add(l24, 1)) {
			fVec10[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = faust_wrap_add(l25, 1)) {
			fRec9[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 4096; l26 = faust_wrap_add(l26, 1)) {
			fVec11[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = faust_wrap_add(l27, 1)) {
			fRec11[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 4096; l28 = faust_wrap_add(l28, 1)) {
			fVec12[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = faust_wrap_add(l29, 1)) {
			fRec12[l29] = 0.0f;
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
	
	virtual T_dattorro* clone() {
		return new T_dattorro(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("dattorro");
		ui_interface->addHorizontalSlider("bw", &fHslider3, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
		ui_interface->addHorizontalSlider("damping", &fHslider1, FAUSTFLOAT(0.2f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("decay", &fHslider2, FAUSTFLOAT(0.4f), FAUSTFLOAT(0.0f), FAUSTFLOAT(0.85f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("mix", &fHslider0, FAUSTFLOAT(0.4f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst0 * static_cast<float>(fHslider0);
		fSlow1 = fConst0 * static_cast<float>(fHslider1);
		fSlow2 = fConst0 * static_cast<float>(fHslider2);
		fSlow3 = fConst0 * static_cast<float>(fHslider3);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		float fTemp0 = static_cast<float>(inputs[0]);
		fVec0[IOTA0 & 255] = fTemp0;
		fRec0[0] = fSlow0 + fConst1 * fRec0[1];
		float fTemp1 = 0.7f * fRec1[1] + fRec12[1];
		fVec1[IOTA0 & 1023] = fTemp1;
		fRec1[0] = fVec1[(faust_wrap_sub(IOTA0, 672)) & 1023];
		float fRec2 = -(0.7f * fTemp1);
		float fTemp2 = 0.7f * fRec5[1] + fRec8[1];
		fVec2[IOTA0 & 1023] = fTemp2;
		float fRec3 = -(0.7f * fTemp2);
		fRec13[0] = fSlow1 + fConst1 * fRec13[1];
		fVec3[IOTA0 & 8191] = fRec1[1] + fRec2;
		float fTemp3 = 1.0f - fRec13[0];
		fRec4[0] = fRec4[1] * fRec13[0] + fVec3[(faust_wrap_sub(IOTA0, 4453)) & 8191] * fTemp3;
		fRec5[0] = fVec2[(faust_wrap_sub(IOTA0, 908)) & 1023];
		fRec14[0] = fSlow2 + fConst1 * fRec14[1];
		float fTemp4 = fRec4[0] * fRec14[0] - 0.5f * fRec6[1];
		fVec4[IOTA0 & 2047] = fTemp4;
		fRec6[0] = fVec4[(faust_wrap_sub(IOTA0, 1800)) & 2047];
		float fRec7 = 0.5f * fTemp4;
		fRec19[0] = fSlow3 + fConst1 * fRec19[1];
		fRec18[0] = fRec18[2] * (1.0f - fRec19[0]) + 0.5f * fRec19[0] * fVec0[(faust_wrap_sub(IOTA0, 200)) & 255];
		float fTemp5 = fRec18[0] - 0.7f * fRec17[1];
		fVec5[IOTA0 & 255] = fTemp5;
		fRec17[0] = fVec5[(faust_wrap_sub(IOTA0, 142)) & 255];
		float fTemp6 = fRec17[1] - 0.7f * (fRec16[1] - fTemp5);
		fVec6[IOTA0 & 127] = fTemp6;
		fRec16[0] = fVec6[(faust_wrap_sub(IOTA0, 107)) & 127];
		float fTemp7 = fRec16[1] + 0.7f * fTemp6 - 0.6f * fRec15[1];
		fVec7[IOTA0 & 511] = fTemp7;
		fRec15[0] = fVec7[(faust_wrap_sub(IOTA0, 379)) & 511];
		float fTemp8 = fRec15[1] - 0.6f * (fRec20[1] - fTemp7);
		fVec8[IOTA0 & 511] = fTemp8;
		fRec20[0] = fVec8[(faust_wrap_sub(IOTA0, 277)) & 511];
		float fTemp9 = 0.6f * fTemp8;
		fVec9[IOTA0 & 2047] = fRec6[1] + fRec7;
		fRec8[0] = fTemp9 + fRec14[0] * fVec9[(faust_wrap_sub(IOTA0, 1800)) & 2047] + fRec20[1];
		fVec10[IOTA0 & 8191] = fRec5[1] + fRec3;
		fRec9[0] = fRec13[0] * fRec9[1] + fTemp3 * fVec10[(faust_wrap_sub(IOTA0, 4217)) & 8191];
		float fTemp10 = fRec14[0] * fRec9[0] - 0.5f * fRec11[1];
		fVec11[IOTA0 & 4095] = fTemp10;
		float fRec10 = 0.5f * fTemp10;
		fRec11[0] = fVec11[(faust_wrap_sub(IOTA0, 2656)) & 4095];
		fVec12[IOTA0 & 4095] = fRec11[1] + fRec10;
		fRec12[0] = fRec20[1] + fTemp9 + fRec14[0] * fVec12[(faust_wrap_sub(IOTA0, 2656)) & 4095];
		outputs[0] = static_cast<FAUSTFLOAT>(fTemp0 * (1.0f - fRec0[0]) + 0.5f * fRec0[0] * (fRec12[0] + fRec8[0]));
		IOTA0 = faust_wrap_add(IOTA0, 1);
		fRec0[1] = fRec0[0];
		fRec1[1] = fRec1[0];
		fRec13[1] = fRec13[0];
		fRec4[1] = fRec4[0];
		fRec5[1] = fRec5[0];
		fRec14[1] = fRec14[0];
		fRec6[1] = fRec6[0];
		fRec19[1] = fRec19[0];
		fRec18[2] = fRec18[1];
		fRec18[1] = fRec18[0];
		fRec17[1] = fRec17[0];
		fRec16[1] = fRec16[0];
		fRec15[1] = fRec15[0];
		fRec20[1] = fRec20[0];
		fRec8[1] = fRec8[0];
		fRec9[1] = fRec9[0];
		fRec11[1] = fRec11[0];
		fRec12[1] = fRec12[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
