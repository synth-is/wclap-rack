/* ------------------------------------------------------------
name: "dattorro-default"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_dattorro_default -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_dattorro_default_H__
#define  __T_dattorro_default_H__

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
#define FAUSTCLASS T_dattorro_default
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


class T_dattorro_default : public FaustDsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst1;
	float fRec0[2];
	int IOTA0;
	float fVec0[1024];
	float fRec2[2];
	float fVec1[1024];
	float fVec2[8192];
	float fRec4[2];
	float fRec5[2];
	float fVec3[2048];
	float fRec7[2];
	float fVec4[8192];
	float fRec8[2];
	float fVec5[4096];
	float fRec16[3];
	float fVec6[256];
	float fRec15[2];
	float fVec7[128];
	float fRec14[2];
	float fVec8[512];
	float fRec13[2];
	float fVec9[512];
	float fRec17[2];
	float fVec10[2048];
	float fRec10[2];
	float fRec11[2];
	float fVec11[4096];
	float fRec12[2];
	
 public:
	T_dattorro_default() {
	}
	
	T_dattorro_default(const T_dattorro_default&) = default;
	
	virtual ~T_dattorro_default() = default;
	
	T_dattorro_default& operator=(const T_dattorro_default&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_dattorro_default -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "dattorro-default.dsp");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "dattorro-default");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("reverbs.lib/dattorro_rev:author", "Jakob Zerbian");
		m->declare("reverbs.lib/dattorro_rev:licence", "LicenseRef-STK-4.3");
		m->declare("reverbs.lib/dattorro_rev_default:author", "Jakob Zerbian");
		m->declare("reverbs.lib/dattorro_rev_default:license", "LicenseRef-STK-4.3");
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
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = faust_wrap_add(l0, 1)) {
			fRec0[l0] = 0.0f;
		}
		IOTA0 = 0;
		for (int l1 = 0; l1 < 1024; l1 = faust_wrap_add(l1, 1)) {
			fVec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = faust_wrap_add(l2, 1)) {
			fRec2[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 1024; l3 = faust_wrap_add(l3, 1)) {
			fVec1[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 8192; l4 = faust_wrap_add(l4, 1)) {
			fVec2[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = faust_wrap_add(l5, 1)) {
			fRec4[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = faust_wrap_add(l6, 1)) {
			fRec5[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2048; l7 = faust_wrap_add(l7, 1)) {
			fVec3[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = faust_wrap_add(l8, 1)) {
			fRec7[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 8192; l9 = faust_wrap_add(l9, 1)) {
			fVec4[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = faust_wrap_add(l10, 1)) {
			fRec8[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 4096; l11 = faust_wrap_add(l11, 1)) {
			fVec5[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 3; l12 = faust_wrap_add(l12, 1)) {
			fRec16[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 256; l13 = faust_wrap_add(l13, 1)) {
			fVec6[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = faust_wrap_add(l14, 1)) {
			fRec15[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 128; l15 = faust_wrap_add(l15, 1)) {
			fVec7[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = faust_wrap_add(l16, 1)) {
			fRec14[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 512; l17 = faust_wrap_add(l17, 1)) {
			fVec8[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = faust_wrap_add(l18, 1)) {
			fRec13[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 512; l19 = faust_wrap_add(l19, 1)) {
			fVec9[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = faust_wrap_add(l20, 1)) {
			fRec17[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2048; l21 = faust_wrap_add(l21, 1)) {
			fVec10[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = faust_wrap_add(l22, 1)) {
			fRec10[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = faust_wrap_add(l23, 1)) {
			fRec11[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 4096; l24 = faust_wrap_add(l24, 1)) {
			fVec11[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = faust_wrap_add(l25, 1)) {
			fRec12[l25] = 0.0f;
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
	
	virtual T_dattorro_default* clone() {
		return new T_dattorro_default(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("dattorro-default");
		ui_interface->addHorizontalSlider("mix", &fHslider0, FAUSTFLOAT(0.4f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst0 * static_cast<float>(fHslider0);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		float fTemp0 = static_cast<float>(inputs[0]);
		fRec0[0] = fSlow0 + fConst1 * fRec0[1];
		float fTemp1 = 0.7f * fRec2[1] + fRec12[1];
		fVec0[IOTA0 & 1023] = fTemp1;
		float fRec1 = -(0.7f * fTemp1);
		fRec2[0] = fVec0[(faust_wrap_sub(IOTA0, 672)) & 1023];
		float fTemp2 = 0.7f * fRec5[1] + fRec10[1];
		fVec1[IOTA0 & 1023] = fTemp2;
		float fRec3 = -(0.7f * fTemp2);
		fVec2[IOTA0 & 8191] = fRec2[1] + fRec1;
		fRec4[0] = 0.0005f * fRec4[1] + 0.9995f * fVec2[(faust_wrap_sub(IOTA0, 4453)) & 8191];
		fRec5[0] = fVec1[(faust_wrap_sub(IOTA0, 908)) & 1023];
		float fTemp3 = fRec7[1] - fRec4[0];
		fVec3[IOTA0 & 2047] = fTemp3;
		float fRec6 = -(0.25f * fTemp3);
		fRec7[0] = -(0.5f * fVec3[(faust_wrap_sub(IOTA0, 1800)) & 2047]);
		fVec4[IOTA0 & 8191] = fRec5[1] + fRec3;
		fRec8[0] = 0.0005f * fRec8[1] + 0.9995f * fVec4[(faust_wrap_sub(IOTA0, 4217)) & 8191];
		float fTemp4 = fRec11[1] - fRec8[0];
		fVec5[IOTA0 & 4095] = fTemp4;
		float fRec9 = -(0.25f * fTemp4);
		fRec16[0] = 0.49975f * fTemp0 + 0.0005f * fRec16[2];
		float fTemp5 = fRec16[0] - 0.75f * fRec15[1];
		fVec6[IOTA0 & 255] = fTemp5;
		fRec15[0] = fVec6[(faust_wrap_sub(IOTA0, 142)) & 255];
		float fTemp6 = fRec15[1] - 0.75f * (fRec14[1] - fTemp5);
		fVec7[IOTA0 & 127] = fTemp6;
		fRec14[0] = fVec7[(faust_wrap_sub(IOTA0, 107)) & 127];
		float fTemp7 = fRec14[1] + 0.75f * fTemp6 - 0.625f * fRec13[1];
		fVec8[IOTA0 & 511] = fTemp7;
		fRec13[0] = fVec8[(faust_wrap_sub(IOTA0, 379)) & 511];
		float fTemp8 = fRec13[1] - 0.625f * (fRec17[1] - fTemp7);
		fVec9[IOTA0 & 511] = fTemp8;
		fRec17[0] = fVec9[(faust_wrap_sub(IOTA0, 277)) & 511];
		float fTemp9 = 0.625f * fTemp8;
		fVec10[IOTA0 & 2047] = fRec7[1] + fRec6;
		fRec10[0] = fTemp9 + 0.5f * fVec10[(faust_wrap_sub(IOTA0, 1800)) & 2047] + fRec17[1];
		fRec11[0] = -(0.5f * fVec5[(faust_wrap_sub(IOTA0, 2656)) & 4095]);
		fVec11[IOTA0 & 4095] = fRec11[1] + fRec9;
		fRec12[0] = fRec17[1] + fTemp9 + 0.5f * fVec11[(faust_wrap_sub(IOTA0, 2656)) & 4095];
		outputs[0] = static_cast<FAUSTFLOAT>(fTemp0 * (1.0f - fRec0[0]) + 0.5f * fRec0[0] * (fRec12[0] + fRec10[0]));
		fRec0[1] = fRec0[0];
		IOTA0 = faust_wrap_add(IOTA0, 1);
		fRec2[1] = fRec2[0];
		fRec4[1] = fRec4[0];
		fRec5[1] = fRec5[0];
		fRec7[1] = fRec7[0];
		fRec8[1] = fRec8[0];
		fRec16[2] = fRec16[1];
		fRec16[1] = fRec16[0];
		fRec15[1] = fRec15[0];
		fRec14[1] = fRec14[0];
		fRec13[1] = fRec13[0];
		fRec17[1] = fRec17[0];
		fRec10[1] = fRec10[0];
		fRec11[1] = fRec11[0];
		fRec12[1] = fRec12[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
