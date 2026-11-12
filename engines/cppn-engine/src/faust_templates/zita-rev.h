/* ------------------------------------------------------------
name: "zita-rev"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_zita_rev -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_zita_rev_H__
#define  __T_zita_rev_H__

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
#define FAUSTCLASS T_zita_rev
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

static float T_zita_rev_faustpower2_f(float value) {
	return value * value;
}

class T_zita_rev : public FaustDsp {
	
 private:
	
	int IOTA0;
	float fVec0[16384];
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst2;
	float fRec0[2];
	float fConst3;
	FAUSTFLOAT fHslider1;
	float fSlow1;
	float fRec41[2];
	float fRec1[2];
	float fRec2[2];
	float fRec3[2];
	float fRec4[2];
	float fRec5[2];
	float fRec6[2];
	float fRec7[2];
	float fConst4;
	float fConst5;
	FAUSTFLOAT fHslider2;
	float fSlow2;
	float fRec42[2];
	float fConst6;
	FAUSTFLOAT fHslider3;
	float fSlow3;
	float fRec43[2];
	FAUSTFLOAT fHslider4;
	float fSlow4;
	float fRec44[2];
	float fRec8[2];
	float fConst7;
	float fConst8;
	float fRec9[2];
	float fConst9;
	float fConst10;
	float fRec10[2];
	float fRec11[2];
	float fVec1[32768];
	float fConst11;
	int iConst12;
	float fConst13;
	FAUSTFLOAT fHslider5;
	float fSlow5;
	float fRec45[2];
	float fVec2[2048];
	int iConst14;
	float fRec12[2];
	float fVec3[32768];
	float fConst15;
	int iConst16;
	float fVec4[4096];
	int iConst17;
	float fRec13[2];
	float fConst18;
	float fConst19;
	float fRec15[2];
	float fConst20;
	float fConst21;
	float fRec16[2];
	float fConst22;
	float fConst23;
	float fRec17[2];
	float fConst24;
	float fConst25;
	float fRec18[2];
	float fVec5[16384];
	float fConst26;
	int iConst27;
	float fVec6[4096];
	int iConst28;
	float fRec19[2];
	float fVec7[16384];
	float fConst29;
	int iConst30;
	float fVec8[2048];
	float fVec9[16384];
	float fConst31;
	int iConst32;
	float fVec10[4096];
	float fVec11[16384];
	float fConst33;
	int iConst34;
	float fVec12[2048];
	int iConst35;
	float fRec22[2];
	float fConst36;
	float fConst37;
	float fRec23[2];
	float fVec13[32768];
	float fConst38;
	int iConst39;
	float fVec14[4096];
	int iConst40;
	float fRec24[2];
	int iConst41;
	float fRec27[2];
	float fVec15[16384];
	float fConst42;
	int iConst43;
	float fVec16[4096];
	int iConst44;
	float fRec28[2];
	int iConst45;
	float fRec31[2];
	float fRec33[3];
	float fRec34[3];
	float fRec35[3];
	float fRec36[3];
	float fRec37[3];
	float fRec38[3];
	float fRec39[3];
	float fRec40[3];
	
 public:
	T_zita_rev() {
	}
	
	T_zita_rev(const T_zita_rev&) = default;
	
	virtual ~T_zita_rev() = default;
	
	T_zita_rev& operator=(const T_zita_rev&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_zita_rev -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("filename", "zita-rev.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass0_highpass1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/version", "1.9.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "zita-rev");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "1.5.1");
		m->declare("routes.lib/hadamard:author", "Remy Muller, revised by Romain Michon");
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
		fConst3 = 3.1415927f / fConst0;
		fConst4 = std::floor(0.210389f * fConst0 + 0.5f);
		fConst5 = 6.9077554f * (fConst4 / fConst0);
		fConst6 = 6.2831855f / fConst0;
		fConst7 = std::floor(0.219991f * fConst0 + 0.5f);
		fConst8 = 6.9077554f * (fConst7 / fConst0);
		fConst9 = std::floor(0.174713f * fConst0 + 0.5f);
		fConst10 = 6.9077554f * (fConst9 / fConst0);
		fConst11 = std::floor(0.019123f * fConst0 + 0.5f);
		iConst12 = static_cast<int>(std::min<float>(16384.0f, std::max<float>(0.0f, fConst7 - fConst11)));
		fConst13 = 0.001f * fConst0;
		iConst14 = static_cast<int>(std::min<float>(1024.0f, std::max<float>(0.0f, fConst11 + -1.0f)));
		fConst15 = std::floor(0.024421f * fConst0 + 0.5f);
		iConst16 = static_cast<int>(std::min<float>(16384.0f, std::max<float>(0.0f, fConst4 - fConst15)));
		iConst17 = static_cast<int>(std::min<float>(2048.0f, std::max<float>(0.0f, fConst15 + -1.0f)));
		fConst18 = std::floor(0.192303f * fConst0 + 0.5f);
		fConst19 = 6.9077554f * (fConst18 / fConst0);
		fConst20 = std::floor(0.125f * fConst0 + 0.5f);
		fConst21 = 6.9077554f * (fConst20 / fConst0);
		fConst22 = std::floor(0.153129f * fConst0 + 0.5f);
		fConst23 = 6.9077554f * (fConst22 / fConst0);
		fConst24 = std::floor(0.256891f * fConst0 + 0.5f);
		fConst25 = 6.9077554f * (fConst24 / fConst0);
		fConst26 = std::floor(0.029291f * fConst0 + 0.5f);
		iConst27 = static_cast<int>(std::min<float>(8192.0f, std::max<float>(0.0f, fConst18 - fConst26)));
		iConst28 = static_cast<int>(std::min<float>(2048.0f, std::max<float>(0.0f, fConst26 + -1.0f)));
		fConst29 = std::floor(0.013458f * fConst0 + 0.5f);
		iConst30 = static_cast<int>(std::min<float>(8192.0f, std::max<float>(0.0f, fConst20 - fConst29)));
		fConst31 = std::floor(0.022904f * fConst0 + 0.5f);
		iConst32 = static_cast<int>(std::min<float>(8192.0f, std::max<float>(0.0f, fConst9 - fConst31)));
		fConst33 = std::floor(0.020346f * fConst0 + 0.5f);
		iConst34 = static_cast<int>(std::min<float>(8192.0f, std::max<float>(0.0f, fConst22 - fConst33)));
		iConst35 = static_cast<int>(std::min<float>(1024.0f, std::max<float>(0.0f, fConst33 + -1.0f)));
		fConst36 = std::floor(0.127837f * fConst0 + 0.5f);
		fConst37 = 6.9077554f * (fConst36 / fConst0);
		fConst38 = std::floor(0.027333f * fConst0 + 0.5f);
		iConst39 = static_cast<int>(std::min<float>(16384.0f, std::max<float>(0.0f, fConst24 - fConst38)));
		iConst40 = static_cast<int>(std::min<float>(2048.0f, std::max<float>(0.0f, fConst38 + -1.0f)));
		iConst41 = static_cast<int>(std::min<float>(2048.0f, std::max<float>(0.0f, fConst31 + -1.0f)));
		fConst42 = std::floor(0.031604f * fConst0 + 0.5f);
		iConst43 = static_cast<int>(std::min<float>(8192.0f, std::max<float>(0.0f, fConst36 - fConst42)));
		iConst44 = static_cast<int>(std::min<float>(2048.0f, std::max<float>(0.0f, fConst42 + -1.0f)));
		iConst45 = static_cast<int>(std::min<float>(1024.0f, std::max<float>(0.0f, fConst29 + -1.0f)));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.4f);
		fHslider1 = static_cast<FAUSTFLOAT>(2e+02f);
		fHslider2 = static_cast<FAUSTFLOAT>(2.0f);
		fHslider3 = static_cast<FAUSTFLOAT>(6e+03f);
		fHslider4 = static_cast<FAUSTFLOAT>(3.0f);
		fHslider5 = static_cast<FAUSTFLOAT>(4e+01f);
	}
	
	virtual void instanceClear() {
		IOTA0 = 0;
		for (int l0 = 0; l0 < 16384; l0 = faust_wrap_add(l0, 1)) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = faust_wrap_add(l2, 1)) {
			fRec41[l2] = 0.0f;
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
		for (int l6 = 0; l6 < 2; l6 = faust_wrap_add(l6, 1)) {
			fRec4[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 2; l7 = faust_wrap_add(l7, 1)) {
			fRec5[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = faust_wrap_add(l8, 1)) {
			fRec6[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = faust_wrap_add(l9, 1)) {
			fRec7[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = faust_wrap_add(l10, 1)) {
			fRec42[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = faust_wrap_add(l11, 1)) {
			fRec43[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = faust_wrap_add(l12, 1)) {
			fRec44[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = faust_wrap_add(l13, 1)) {
			fRec8[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = faust_wrap_add(l14, 1)) {
			fRec9[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = faust_wrap_add(l15, 1)) {
			fRec10[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = faust_wrap_add(l16, 1)) {
			fRec11[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 32768; l17 = faust_wrap_add(l17, 1)) {
			fVec1[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = faust_wrap_add(l18, 1)) {
			fRec45[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2048; l19 = faust_wrap_add(l19, 1)) {
			fVec2[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = faust_wrap_add(l20, 1)) {
			fRec12[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 32768; l21 = faust_wrap_add(l21, 1)) {
			fVec3[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 4096; l22 = faust_wrap_add(l22, 1)) {
			fVec4[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = faust_wrap_add(l23, 1)) {
			fRec13[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = faust_wrap_add(l24, 1)) {
			fRec15[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = faust_wrap_add(l25, 1)) {
			fRec16[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = faust_wrap_add(l26, 1)) {
			fRec17[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = faust_wrap_add(l27, 1)) {
			fRec18[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 16384; l28 = faust_wrap_add(l28, 1)) {
			fVec5[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 4096; l29 = faust_wrap_add(l29, 1)) {
			fVec6[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = faust_wrap_add(l30, 1)) {
			fRec19[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 16384; l31 = faust_wrap_add(l31, 1)) {
			fVec7[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2048; l32 = faust_wrap_add(l32, 1)) {
			fVec8[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 16384; l33 = faust_wrap_add(l33, 1)) {
			fVec9[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 4096; l34 = faust_wrap_add(l34, 1)) {
			fVec10[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 16384; l35 = faust_wrap_add(l35, 1)) {
			fVec11[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2048; l36 = faust_wrap_add(l36, 1)) {
			fVec12[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = faust_wrap_add(l37, 1)) {
			fRec22[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = faust_wrap_add(l38, 1)) {
			fRec23[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 32768; l39 = faust_wrap_add(l39, 1)) {
			fVec13[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 4096; l40 = faust_wrap_add(l40, 1)) {
			fVec14[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = faust_wrap_add(l41, 1)) {
			fRec24[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = faust_wrap_add(l42, 1)) {
			fRec27[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 16384; l43 = faust_wrap_add(l43, 1)) {
			fVec15[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 4096; l44 = faust_wrap_add(l44, 1)) {
			fVec16[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = faust_wrap_add(l45, 1)) {
			fRec28[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = faust_wrap_add(l46, 1)) {
			fRec31[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 3; l47 = faust_wrap_add(l47, 1)) {
			fRec33[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 3; l48 = faust_wrap_add(l48, 1)) {
			fRec34[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 3; l49 = faust_wrap_add(l49, 1)) {
			fRec35[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 3; l50 = faust_wrap_add(l50, 1)) {
			fRec36[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 3; l51 = faust_wrap_add(l51, 1)) {
			fRec37[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 3; l52 = faust_wrap_add(l52, 1)) {
			fRec38[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 3; l53 = faust_wrap_add(l53, 1)) {
			fRec39[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 3; l54 = faust_wrap_add(l54, 1)) {
			fRec40[l54] = 0.0f;
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
	
	virtual T_zita_rev* clone() {
		return new T_zita_rev(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("zita-rev");
		ui_interface->addHorizontalSlider("f1", &fHslider1, FAUSTFLOAT(2e+02f), FAUSTFLOAT(2e+01f), FAUSTFLOAT(1e+03f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("f2", &fHslider3, FAUSTFLOAT(6e+03f), FAUSTFLOAT(1.5e+03f), FAUSTFLOAT(1.9e+04f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("mix", &fHslider0, FAUSTFLOAT(0.4f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("rdel", &fHslider5, FAUSTFLOAT(4e+01f), FAUSTFLOAT(0.0f), FAUSTFLOAT(2e+02f), FAUSTFLOAT(0.1f));
		ui_interface->addHorizontalSlider("t60dc", &fHslider4, FAUSTFLOAT(3.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("t60m", &fHslider2, FAUSTFLOAT(2.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(8.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst1 * static_cast<float>(fHslider0);
		fSlow1 = fConst1 * static_cast<float>(fHslider1);
		fSlow2 = fConst1 * static_cast<float>(fHslider2);
		fSlow3 = fConst1 * static_cast<float>(fHslider3);
		fSlow4 = fConst1 * static_cast<float>(fHslider4);
		fSlow5 = fConst1 * static_cast<float>(fHslider5);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		float fTemp0 = static_cast<float>(inputs[0]);
		fVec0[IOTA0 & 16383] = fTemp0;
		fRec0[0] = fSlow0 + fConst2 * fRec0[1];
		fRec41[0] = fSlow1 + fConst2 * fRec41[1];
		float fTemp1 = 1.0f / std::tan(fConst3 * fRec41[0]);
		float fTemp2 = 1.0f - fTemp1;
		float fTemp3 = 1.0f + fTemp1;
		fRec1[0] = -((fRec1[1] * fTemp2 - (fRec40[1] + fRec40[2])) / fTemp3);
		fRec2[0] = -((fTemp2 * fRec2[1] - (fRec34[1] + fRec34[2])) / fTemp3);
		fRec3[0] = -((fTemp2 * fRec3[1] - (fRec33[1] + fRec33[2])) / fTemp3);
		fRec4[0] = -((fTemp2 * fRec4[1] - (fRec39[1] + fRec39[2])) / fTemp3);
		fRec5[0] = -((fTemp2 * fRec5[1] - (fRec36[1] + fRec36[2])) / fTemp3);
		fRec6[0] = -((fTemp2 * fRec6[1] - (fRec38[1] + fRec38[2])) / fTemp3);
		fRec7[0] = -((fTemp2 * fRec7[1] - (fRec37[1] + fRec37[2])) / fTemp3);
		fRec42[0] = fSlow2 + fConst2 * fRec42[1];
		float fTemp4 = std::exp(-(fConst5 / fRec42[0]));
		float fTemp5 = T_zita_rev_faustpower2_f(fTemp4);
		fRec43[0] = fSlow3 + fConst2 * fRec43[1];
		float fTemp6 = std::cos(fConst6 * fRec43[0]);
		float fTemp7 = 1.0f - fTemp5 * fTemp6;
		float fTemp8 = 1.0f - fTemp5;
		float fTemp9 = fTemp7 / fTemp8;
		float fTemp10 = std::sqrt(std::max<float>(0.0f, -1.0f + T_zita_rev_faustpower2_f(fTemp7) / T_zita_rev_faustpower2_f(fTemp8)));
		fRec44[0] = fSlow4 + fConst2 * fRec44[1];
		fRec8[0] = fTemp4 * (1.0f - fTemp9 + fTemp10) * (fRec40[1] + (-1.0f + std::exp(-(fConst5 / fRec44[0])) / fTemp4) * fRec1[0]) + (fTemp9 - fTemp10) * fRec8[1];
		float fTemp11 = std::exp(-(fConst8 / fRec42[0]));
		float fTemp12 = T_zita_rev_faustpower2_f(fTemp11);
		float fTemp13 = 1.0f - fTemp6 * fTemp12;
		float fTemp14 = 1.0f - fTemp12;
		float fTemp15 = fTemp13 / fTemp14;
		float fTemp16 = std::sqrt(std::max<float>(0.0f, -1.0f + T_zita_rev_faustpower2_f(fTemp13) / T_zita_rev_faustpower2_f(fTemp14)));
		fRec9[0] = fTemp11 * (1.0f - fTemp15 + fTemp16) * (fRec36[1] + (-1.0f + std::exp(-(fConst8 / fRec44[0])) / fTemp11) * fRec5[0]) + (fTemp15 - fTemp16) * fRec9[1];
		float fTemp17 = std::exp(-(fConst10 / fRec42[0]));
		float fTemp18 = T_zita_rev_faustpower2_f(fTemp17);
		float fTemp19 = 1.0f - fTemp6 * fTemp18;
		float fTemp20 = 1.0f - fTemp18;
		float fTemp21 = fTemp19 / fTemp20;
		float fTemp22 = std::sqrt(std::max<float>(0.0f, -1.0f + T_zita_rev_faustpower2_f(fTemp19) / T_zita_rev_faustpower2_f(fTemp20)));
		fRec10[0] = fTemp17 * (1.0f - fTemp21 + fTemp22) * (fRec34[1] + (-1.0f + std::exp(-(fConst10 / fRec44[0])) / fTemp17) * fRec2[0]) + (fTemp21 - fTemp22) * fRec10[1];
		fRec11[0] = -((fTemp2 * fRec11[1] - (fRec35[1] + fRec35[2])) / fTemp3);
		fVec1[IOTA0 & 32767] = 1e-20f + 0.35355338f * fRec9[0];
		float fTemp23 = 0.6f * fRec12[1] + fVec1[(faust_wrap_sub(IOTA0, iConst12)) & 32767];
		fRec45[0] = fSlow5 + fConst2 * fRec45[1];
		float fTemp24 = 0.15f * fVec0[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(8192.0f, std::max<float>(0.0f, fConst13 * fRec45[0]))))) & 16383];
		fVec2[IOTA0 & 2047] = fTemp23 - fTemp24;
		fRec12[0] = fVec2[(faust_wrap_sub(IOTA0, iConst14)) & 2047];
		fVec3[IOTA0 & 32767] = 1e-20f + 0.35355338f * fRec8[0];
		float fTemp25 = fVec3[(faust_wrap_sub(IOTA0, iConst16)) & 32767] + fTemp24 + 0.6f * fRec13[1];
		fVec4[IOTA0 & 4095] = fTemp25;
		fRec13[0] = fVec4[(faust_wrap_sub(IOTA0, iConst17)) & 4095];
		float fRec14 = 0.6f * (fTemp24 - fTemp23);
		float fTemp26 = std::exp(-(fConst19 / fRec42[0]));
		float fTemp27 = T_zita_rev_faustpower2_f(fTemp26);
		float fTemp28 = 1.0f - fTemp6 * fTemp27;
		float fTemp29 = 1.0f - fTemp27;
		float fTemp30 = fTemp28 / fTemp29;
		float fTemp31 = std::sqrt(std::max<float>(0.0f, -1.0f + T_zita_rev_faustpower2_f(fTemp28) / T_zita_rev_faustpower2_f(fTemp29)));
		fRec15[0] = fTemp26 * (1.0f - fTemp30 + fTemp31) * (fRec33[1] + (-1.0f + std::exp(-(fConst19 / fRec44[0])) / fTemp26) * fRec3[0]) + (fTemp30 - fTemp31) * fRec15[1];
		float fTemp32 = std::exp(-(fConst21 / fRec42[0]));
		float fTemp33 = T_zita_rev_faustpower2_f(fTemp32);
		float fTemp34 = 1.0f - fTemp6 * fTemp33;
		float fTemp35 = 1.0f - fTemp33;
		float fTemp36 = fTemp34 / fTemp35;
		float fTemp37 = std::sqrt(std::max<float>(0.0f, -1.0f + T_zita_rev_faustpower2_f(fTemp34) / T_zita_rev_faustpower2_f(fTemp35)));
		fRec16[0] = fTemp32 * (1.0f - fTemp36 + fTemp37) * (fRec35[1] + (-1.0f + std::exp(-(fConst21 / fRec44[0])) / fTemp32) * fRec11[0]) + (fTemp36 - fTemp37) * fRec16[1];
		float fTemp38 = std::exp(-(fConst23 / fRec42[0]));
		float fTemp39 = T_zita_rev_faustpower2_f(fTemp38);
		float fTemp40 = 1.0f - fTemp6 * fTemp39;
		float fTemp41 = 1.0f - fTemp39;
		float fTemp42 = fTemp40 / fTemp41;
		float fTemp43 = std::sqrt(std::max<float>(0.0f, -1.0f + T_zita_rev_faustpower2_f(fTemp40) / T_zita_rev_faustpower2_f(fTemp41)));
		fRec17[0] = fTemp38 * (1.0f - fTemp42 + fTemp43) * (fRec39[1] + (-1.0f + std::exp(-(fConst23 / fRec44[0])) / fTemp38) * fRec4[0]) + (fTemp42 - fTemp43) * fRec17[1];
		float fTemp44 = std::exp(-(fConst25 / fRec42[0]));
		float fTemp45 = T_zita_rev_faustpower2_f(fTemp44);
		float fTemp46 = 1.0f - fTemp6 * fTemp45;
		float fTemp47 = 1.0f - fTemp45;
		float fTemp48 = fTemp46 / fTemp47;
		float fTemp49 = std::sqrt(std::max<float>(0.0f, -1.0f + T_zita_rev_faustpower2_f(fTemp46) / T_zita_rev_faustpower2_f(fTemp47)));
		fRec18[0] = fTemp44 * (1.0f - fTemp48 + fTemp49) * (fRec38[1] + (-1.0f + std::exp(-(fConst25 / fRec44[0])) / fTemp44) * fRec6[0]) + (fTemp48 - fTemp49) * fRec18[1];
		fVec5[IOTA0 & 16383] = 1e-20f + 0.35355338f * fRec15[0];
		float fTemp50 = fVec5[(faust_wrap_sub(IOTA0, iConst27)) & 16383] + fTemp24 + 0.6f * fRec19[1];
		fVec6[IOTA0 & 4095] = fTemp50;
		fRec19[0] = fVec6[(faust_wrap_sub(IOTA0, iConst28)) & 4095];
		fVec7[IOTA0 & 16383] = 1e-20f + 0.35355338f * fRec16[0];
		float fTemp51 = fVec7[(faust_wrap_sub(IOTA0, iConst30)) & 16383] - (fTemp24 + 0.6f * fRec31[1]);
		fVec8[IOTA0 & 2047] = fTemp51;
		float fRec20 = 0.6f * fTemp51;
		fVec9[IOTA0 & 16383] = 1e-20f + 0.35355338f * fRec10[0];
		float fTemp52 = fTemp24 + fVec9[(faust_wrap_sub(IOTA0, iConst32)) & 16383] - 0.6f * fRec27[1];
		fVec10[IOTA0 & 4095] = fTemp52;
		float fRec21 = 0.6f * fTemp52;
		fVec11[IOTA0 & 16383] = 1e-20f + 0.35355338f * fRec17[0];
		float fTemp53 = fTemp24 + fVec11[(faust_wrap_sub(IOTA0, iConst34)) & 16383] - 0.6f * fRec22[1];
		fVec12[IOTA0 & 2047] = fTemp53;
		fRec22[0] = fVec12[(faust_wrap_sub(IOTA0, iConst35)) & 2047];
		float fTemp54 = std::exp(-(fConst37 / fRec42[0]));
		float fTemp55 = T_zita_rev_faustpower2_f(fTemp54);
		float fTemp56 = 1.0f - fTemp6 * fTemp55;
		float fTemp57 = 1.0f - fTemp55;
		float fTemp58 = fTemp56 / fTemp57;
		float fTemp59 = std::sqrt(std::max<float>(0.0f, -1.0f + T_zita_rev_faustpower2_f(fTemp56) / T_zita_rev_faustpower2_f(fTemp57)));
		fRec23[0] = fTemp54 * (1.0f - fTemp58 + fTemp59) * (fRec37[1] + (-1.0f + std::exp(-(fConst37 / fRec44[0])) / fTemp54) * fRec7[0]) + (fTemp58 - fTemp59) * fRec23[1];
		fVec13[IOTA0 & 32767] = 1e-20f + 0.35355338f * fRec18[0];
		float fTemp60 = 0.6f * fRec24[1] + fVec13[(faust_wrap_sub(IOTA0, iConst39)) & 32767];
		fVec14[IOTA0 & 4095] = fTemp60 - fTemp24;
		fRec24[0] = fVec14[(faust_wrap_sub(IOTA0, iConst40)) & 4095];
		float fRec25 = 0.6f * (fTemp24 - fTemp60);
		float fRec26 = -(0.6f * fTemp25);
		fRec27[0] = fVec10[(faust_wrap_sub(IOTA0, iConst41)) & 4095];
		fVec15[IOTA0 & 16383] = 1e-20f + 0.35355338f * fRec23[0];
		float fTemp61 = fVec15[(faust_wrap_sub(IOTA0, iConst43)) & 16383] - (fTemp24 + 0.6f * fRec28[1]);
		fVec16[IOTA0 & 4095] = fTemp61;
		fRec28[0] = fVec16[(faust_wrap_sub(IOTA0, iConst44)) & 4095];
		float fRec29 = 0.6f * fTemp61;
		float fRec30 = -(0.6f * fTemp50);
		fRec31[0] = fVec8[(faust_wrap_sub(IOTA0, iConst45)) & 2047];
		float fRec32 = 0.6f * fTemp53;
		float fTemp62 = fRec12[1] + fRec19[1];
		fRec33[0] = fRec14 + fRec30 + fRec29 + fRec32 + fRec28[1] + fRec22[1] + fTemp62 - (fRec25 + fRec26 + fRec20 + fRec21 + fRec24[1] + fRec27[1] + fRec13[1] + fRec31[1]);
		fRec34[0] = fRec25 + fRec26 + fRec29 + fRec32 + fRec28[1] + fRec24[1] + fRec13[1] + fRec22[1] - (fRec14 + fRec30 + fRec20 + fRec21 + fRec27[1] + fRec31[1] + fTemp62);
		float fTemp63 = fRec12[1] + fRec13[1];
		fRec35[0] = fRec14 + fRec26 + fRec20 + fRec32 + fRec22[1] + fRec31[1] + fTemp63 - (fRec25 + fRec30 + fRec29 + fRec21 + fRec28[1] + fRec24[1] + fRec19[1] + fRec27[1]);
		fRec36[0] = fRec25 + fRec30 + fRec20 + fRec32 + fRec24[1] + fRec22[1] + fRec19[1] + fRec31[1] - (fRec14 + fRec26 + fRec29 + fRec21 + fRec28[1] + fRec27[1] + fTemp63);
		float fTemp64 = fRec13[1] + fRec19[1];
		fRec37[0] = fRec30 + fRec26 + fRec21 + fRec32 + fRec22[1] + fRec27[1] + fTemp64 - (fRec14 + fRec25 + fRec20 + fRec29 + fRec28[1] + fRec24[1] + fRec12[1] + fRec31[1]);
		fRec38[0] = fRec14 + fRec25 + fRec21 + fRec32 + fRec24[1] + fRec22[1] + fRec12[1] + fRec27[1] - (fRec30 + fRec26 + fRec20 + fRec29 + fRec28[1] + fRec31[1] + fTemp64);
		float fTemp65 = fRec19[1] + fTemp63;
		fRec39[0] = fRec14 + fRec25 + fRec30 + fRec26 + fRec20 + fRec29 + fRec21 + fRec32 + fRec28[1] + fRec24[1] + fRec22[1] + fRec27[1] + fRec31[1] + fTemp65;
		fRec40[0] = fRec20 + fRec29 + fRec21 + fRec32 + fRec28[1] + fRec22[1] + fRec31[1] + fRec27[1] - (fRec14 + fRec25 + fRec30 + fRec26 + fRec24[1] + fTemp65);
		outputs[0] = static_cast<FAUSTFLOAT>(fTemp0 * (1.0f - fRec0[0]) + 0.185f * fRec0[0] * (fRec40[0] + fRec37[0] + (fRec40[0] - fRec37[0])));
		IOTA0 = faust_wrap_add(IOTA0, 1);
		fRec0[1] = fRec0[0];
		fRec41[1] = fRec41[0];
		fRec1[1] = fRec1[0];
		fRec2[1] = fRec2[0];
		fRec3[1] = fRec3[0];
		fRec4[1] = fRec4[0];
		fRec5[1] = fRec5[0];
		fRec6[1] = fRec6[0];
		fRec7[1] = fRec7[0];
		fRec42[1] = fRec42[0];
		fRec43[1] = fRec43[0];
		fRec44[1] = fRec44[0];
		fRec8[1] = fRec8[0];
		fRec9[1] = fRec9[0];
		fRec10[1] = fRec10[0];
		fRec11[1] = fRec11[0];
		fRec45[1] = fRec45[0];
		fRec12[1] = fRec12[0];
		fRec13[1] = fRec13[0];
		fRec15[1] = fRec15[0];
		fRec16[1] = fRec16[0];
		fRec17[1] = fRec17[0];
		fRec18[1] = fRec18[0];
		fRec19[1] = fRec19[0];
		fRec22[1] = fRec22[0];
		fRec23[1] = fRec23[0];
		fRec24[1] = fRec24[0];
		fRec27[1] = fRec27[0];
		fRec28[1] = fRec28[0];
		fRec31[1] = fRec31[0];
		fRec33[2] = fRec33[1];
		fRec33[1] = fRec33[0];
		fRec34[2] = fRec34[1];
		fRec34[1] = fRec34[0];
		fRec35[2] = fRec35[1];
		fRec35[1] = fRec35[0];
		fRec36[2] = fRec36[1];
		fRec36[1] = fRec36[0];
		fRec37[2] = fRec37[1];
		fRec37[1] = fRec37[0];
		fRec38[2] = fRec38[1];
		fRec38[1] = fRec38[0];
		fRec39[2] = fRec39[1];
		fRec39[1] = fRec39[0];
		fRec40[2] = fRec40[1];
		fRec40[1] = fRec40[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
