/* ------------------------------------------------------------
name: "djembe"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_djembe -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_djembe_H__
#define  __T_djembe_H__

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
#define FAUSTCLASS T_djembe
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

static float T_djembe_faustpower2_f(float value) {
	return value * value;
}

class T_djembe : public FaustDsp {
	
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
	float fRec1[2];
	float fRec0[3];
	float fConst7;
	float fConst8;
	float fConst9;
	float fRec2[3];
	float fConst10;
	float fConst11;
	float fConst12;
	float fRec3[3];
	float fConst13;
	float fConst14;
	float fConst15;
	float fRec4[3];
	float fConst16;
	float fConst17;
	float fConst18;
	float fRec5[3];
	float fConst19;
	float fConst20;
	float fConst21;
	float fRec6[3];
	float fConst22;
	float fConst23;
	float fConst24;
	float fRec7[3];
	float fConst25;
	float fConst26;
	float fConst27;
	float fRec8[3];
	float fConst28;
	float fConst29;
	float fConst30;
	float fRec9[3];
	float fConst31;
	float fConst32;
	float fConst33;
	float fRec10[3];
	float fConst34;
	float fConst35;
	float fConst36;
	float fRec11[3];
	float fConst37;
	float fConst38;
	float fConst39;
	float fRec12[3];
	float fConst40;
	float fConst41;
	float fConst42;
	float fRec13[3];
	float fConst43;
	float fConst44;
	float fConst45;
	float fRec14[3];
	float fConst46;
	float fConst47;
	float fConst48;
	float fRec15[3];
	float fConst49;
	float fConst50;
	float fConst51;
	float fRec16[3];
	float fConst52;
	float fConst53;
	float fConst54;
	float fRec17[3];
	float fConst55;
	float fConst56;
	float fConst57;
	float fRec18[3];
	float fConst58;
	float fConst59;
	float fConst60;
	float fRec19[3];
	float fConst61;
	float fConst62;
	float fConst63;
	float fRec20[3];
	
 public:
	T_djembe() {
	}
	
	T_djembe(const T_djembe&) = default;
	
	virtual ~T_djembe() = default;
	
	T_djembe& operator=(const T_djembe&) = default;
	
	void metadata(Meta* m) { 
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_djembe -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "djembe.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/version", "1.9.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "djembe");
		m->declare("physmodels.lib/name", "Faust Physical Models Library");
		m->declare("physmodels.lib/version", "1.2.0");
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
		fConst1 = std::pow(0.001f, 1.6666666f / fConst0);
		fConst2 = T_djembe_faustpower2_f(fConst1);
		fConst3 = 2.0f * fConst1;
		fConst4 = 6.2831855f / fConst0;
		fConst5 = 44.1f / fConst0;
		fConst6 = 1.0f - fConst5;
		fConst7 = std::pow(0.001f, 1.754386f / fConst0);
		fConst8 = T_djembe_faustpower2_f(fConst7);
		fConst9 = 2.0f * fConst7;
		fConst10 = std::pow(0.001f, 1.8518518f / fConst0);
		fConst11 = T_djembe_faustpower2_f(fConst10);
		fConst12 = 2.0f * fConst10;
		fConst13 = std::pow(0.001f, 1.9607843f / fConst0);
		fConst14 = T_djembe_faustpower2_f(fConst13);
		fConst15 = 2.0f * fConst13;
		fConst16 = std::pow(0.001f, 2.0833333f / fConst0);
		fConst17 = T_djembe_faustpower2_f(fConst16);
		fConst18 = 2.0f * fConst16;
		fConst19 = std::pow(0.001f, 2.2222223f / fConst0);
		fConst20 = T_djembe_faustpower2_f(fConst19);
		fConst21 = 2.0f * fConst19;
		fConst22 = std::pow(0.001f, 2.3809524f / fConst0);
		fConst23 = T_djembe_faustpower2_f(fConst22);
		fConst24 = 2.0f * fConst22;
		fConst25 = std::pow(0.001f, 2.5641026f / fConst0);
		fConst26 = T_djembe_faustpower2_f(fConst25);
		fConst27 = 2.0f * fConst25;
		fConst28 = std::pow(0.001f, 2.7777777f / fConst0);
		fConst29 = T_djembe_faustpower2_f(fConst28);
		fConst30 = 2.0f * fConst28;
		fConst31 = std::pow(0.001f, 3.030303f / fConst0);
		fConst32 = T_djembe_faustpower2_f(fConst31);
		fConst33 = 2.0f * fConst31;
		fConst34 = std::pow(0.001f, 3.3333333f / fConst0);
		fConst35 = T_djembe_faustpower2_f(fConst34);
		fConst36 = 2.0f * fConst34;
		fConst37 = std::pow(0.001f, 3.7037036f / fConst0);
		fConst38 = T_djembe_faustpower2_f(fConst37);
		fConst39 = 2.0f * fConst37;
		fConst40 = std::pow(0.001f, 4.1666665f / fConst0);
		fConst41 = T_djembe_faustpower2_f(fConst40);
		fConst42 = 2.0f * fConst40;
		fConst43 = std::pow(0.001f, 4.7619047f / fConst0);
		fConst44 = T_djembe_faustpower2_f(fConst43);
		fConst45 = 2.0f * fConst43;
		fConst46 = std::pow(0.001f, 5.5555553f / fConst0);
		fConst47 = T_djembe_faustpower2_f(fConst46);
		fConst48 = 2.0f * fConst46;
		fConst49 = std::pow(0.001f, 6.6666665f / fConst0);
		fConst50 = T_djembe_faustpower2_f(fConst49);
		fConst51 = 2.0f * fConst49;
		fConst52 = std::pow(0.001f, 8.333333f / fConst0);
		fConst53 = T_djembe_faustpower2_f(fConst52);
		fConst54 = 2.0f * fConst52;
		fConst55 = std::pow(0.001f, 11.111111f / fConst0);
		fConst56 = T_djembe_faustpower2_f(fConst55);
		fConst57 = 2.0f * fConst55;
		fConst58 = std::pow(0.001f, 16.666666f / fConst0);
		fConst59 = T_djembe_faustpower2_f(fConst58);
		fConst60 = 2.0f * fConst58;
		fConst61 = std::pow(0.001f, 33.333332f / fConst0);
		fConst62 = T_djembe_faustpower2_f(fConst61);
		fConst63 = 2.0f * fConst61;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(6e+01f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = faust_wrap_add(l0, 1)) {
			fRec1[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 3; l1 = faust_wrap_add(l1, 1)) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 3; l2 = faust_wrap_add(l2, 1)) {
			fRec2[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 3; l3 = faust_wrap_add(l3, 1)) {
			fRec3[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 3; l4 = faust_wrap_add(l4, 1)) {
			fRec4[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 3; l5 = faust_wrap_add(l5, 1)) {
			fRec5[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 3; l6 = faust_wrap_add(l6, 1)) {
			fRec6[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 3; l7 = faust_wrap_add(l7, 1)) {
			fRec7[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 3; l8 = faust_wrap_add(l8, 1)) {
			fRec8[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 3; l9 = faust_wrap_add(l9, 1)) {
			fRec9[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 3; l10 = faust_wrap_add(l10, 1)) {
			fRec10[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 3; l11 = faust_wrap_add(l11, 1)) {
			fRec11[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 3; l12 = faust_wrap_add(l12, 1)) {
			fRec12[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 3; l13 = faust_wrap_add(l13, 1)) {
			fRec13[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 3; l14 = faust_wrap_add(l14, 1)) {
			fRec14[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 3; l15 = faust_wrap_add(l15, 1)) {
			fRec15[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 3; l16 = faust_wrap_add(l16, 1)) {
			fRec16[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 3; l17 = faust_wrap_add(l17, 1)) {
			fRec17[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 3; l18 = faust_wrap_add(l18, 1)) {
			fRec18[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 3; l19 = faust_wrap_add(l19, 1)) {
			fRec19[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 3; l20 = faust_wrap_add(l20, 1)) {
			fRec20[l20] = 0.0f;
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
	
	virtual T_djembe* clone() {
		return new T_djembe(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("djembe");
		ui_interface->addHorizontalSlider("freq", &fHslider0, FAUSTFLOAT(6e+01f), FAUSTFLOAT(5e+01f), FAUSTFLOAT(5e+02f), FAUSTFLOAT(1.0f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst5 * static_cast<float>(fHslider0);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		float fTemp0 = static_cast<float>(inputs[0]);
		fRec1[0] = fSlow0 + fConst6 * fRec1[1];
		fRec0[0] = fTemp0 - fConst2 * fRec0[2] + fConst3 * fRec0[1] * std::cos(fConst4 * fRec1[0]);
		fRec2[0] = fTemp0 - fConst8 * fRec2[2] + fConst9 * fRec2[1] * std::cos(fConst4 * (2e+02f + fRec1[0]));
		fRec3[0] = fTemp0 - fConst11 * fRec3[2] + fConst12 * fRec3[1] * std::cos(fConst4 * (4e+02f + fRec1[0]));
		fRec4[0] = fTemp0 - fConst14 * fRec4[2] + fConst15 * fRec4[1] * std::cos(fConst4 * (6e+02f + fRec1[0]));
		fRec5[0] = fTemp0 - fConst17 * fRec5[2] + fConst18 * fRec5[1] * std::cos(fConst4 * (8e+02f + fRec1[0]));
		fRec6[0] = fTemp0 - fConst20 * fRec6[2] + fConst21 * fRec6[1] * std::cos(fConst4 * (1e+03f + fRec1[0]));
		fRec7[0] = fTemp0 - fConst23 * fRec7[2] + fConst24 * fRec7[1] * std::cos(fConst4 * (1.2e+03f + fRec1[0]));
		fRec8[0] = fTemp0 - fConst26 * fRec8[2] + fConst27 * fRec8[1] * std::cos(fConst4 * (1.4e+03f + fRec1[0]));
		fRec9[0] = fTemp0 - fConst29 * fRec9[2] + fConst30 * fRec9[1] * std::cos(fConst4 * (1.6e+03f + fRec1[0]));
		fRec10[0] = fTemp0 - fConst32 * fRec10[2] + fConst33 * fRec10[1] * std::cos(fConst4 * (1.8e+03f + fRec1[0]));
		fRec11[0] = fTemp0 - fConst35 * fRec11[2] + fConst36 * fRec11[1] * std::cos(fConst4 * (2e+03f + fRec1[0]));
		fRec12[0] = fTemp0 - fConst38 * fRec12[2] + fConst39 * fRec12[1] * std::cos(fConst4 * (2.2e+03f + fRec1[0]));
		fRec13[0] = fTemp0 - fConst41 * fRec13[2] + fConst42 * fRec13[1] * std::cos(fConst4 * (2.4e+03f + fRec1[0]));
		fRec14[0] = fTemp0 - fConst44 * fRec14[2] + fConst45 * fRec14[1] * std::cos(fConst4 * (2.6e+03f + fRec1[0]));
		fRec15[0] = fTemp0 - fConst47 * fRec15[2] + fConst48 * fRec15[1] * std::cos(fConst4 * (2.8e+03f + fRec1[0]));
		fRec16[0] = fTemp0 - fConst50 * fRec16[2] + fConst51 * fRec16[1] * std::cos(fConst4 * (3e+03f + fRec1[0]));
		fRec17[0] = fTemp0 - fConst53 * fRec17[2] + fConst54 * fRec17[1] * std::cos(fConst4 * (3.2e+03f + fRec1[0]));
		fRec18[0] = fTemp0 - fConst56 * fRec18[2] + fConst57 * fRec18[1] * std::cos(fConst4 * (3.4e+03f + fRec1[0]));
		fRec19[0] = fTemp0 - fConst59 * fRec19[2] + fConst60 * fRec19[1] * std::cos(fConst4 * (3.6e+03f + fRec1[0]));
		fRec20[0] = fTemp0 - fConst62 * fRec20[2] + fConst63 * fRec20[1] * std::cos(fConst4 * (3.8e+03f + fRec1[0]));
		outputs[0] = static_cast<FAUSTFLOAT>(0.05f * (fRec0[0] + 0.25f * (fRec2[0] - fRec2[2]) + 0.11111111f * (fRec3[0] - fRec3[2]) + 0.0625f * (fRec4[0] - fRec4[2]) + 0.04f * (fRec5[0] - fRec5[2]) + 0.027777778f * (fRec6[0] - fRec6[2]) + 0.020408163f * (fRec7[0] - fRec7[2]) + 0.015625f * (fRec8[0] - fRec8[2]) + 0.012345679f * (fRec9[0] - fRec9[2]) + 0.01f * (fRec10[0] - fRec10[2]) + 0.008264462f * (fRec11[0] - fRec11[2]) + 0.0069444445f * (fRec12[0] - fRec12[2]) + 0.00591716f * (fRec13[0] - fRec13[2]) + 0.0051020407f * (fRec14[0] - fRec14[2]) + 0.0044444446f * (fRec15[0] - fRec15[2]) + 0.00390625f * (fRec16[0] - fRec16[2]) + 0.0034602077f * (fRec17[0] - fRec17[2]) + 0.0030864198f * (fRec18[0] - fRec18[2]) + 0.002770083f * (fRec19[0] - fRec19[2]) + 0.0025f * (fRec20[0] - fRec20[2]) - fRec0[2]));
		fRec1[1] = fRec1[0];
		fRec0[2] = fRec0[1];
		fRec0[1] = fRec0[0];
		fRec2[2] = fRec2[1];
		fRec2[1] = fRec2[0];
		fRec3[2] = fRec3[1];
		fRec3[1] = fRec3[0];
		fRec4[2] = fRec4[1];
		fRec4[1] = fRec4[0];
		fRec5[2] = fRec5[1];
		fRec5[1] = fRec5[0];
		fRec6[2] = fRec6[1];
		fRec6[1] = fRec6[0];
		fRec7[2] = fRec7[1];
		fRec7[1] = fRec7[0];
		fRec8[2] = fRec8[1];
		fRec8[1] = fRec8[0];
		fRec9[2] = fRec9[1];
		fRec9[1] = fRec9[0];
		fRec10[2] = fRec10[1];
		fRec10[1] = fRec10[0];
		fRec11[2] = fRec11[1];
		fRec11[1] = fRec11[0];
		fRec12[2] = fRec12[1];
		fRec12[1] = fRec12[0];
		fRec13[2] = fRec13[1];
		fRec13[1] = fRec13[0];
		fRec14[2] = fRec14[1];
		fRec14[1] = fRec14[0];
		fRec15[2] = fRec15[1];
		fRec15[1] = fRec15[0];
		fRec16[2] = fRec16[1];
		fRec16[1] = fRec16[0];
		fRec17[2] = fRec17[1];
		fRec17[1] = fRec17[0];
		fRec18[2] = fRec18[1];
		fRec18[1] = fRec18[0];
		fRec19[2] = fRec19[1];
		fRec19[1] = fRec19[0];
		fRec20[2] = fRec20[1];
		fRec20[1] = fRec20[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
