/* ------------------------------------------------------------
name: "springreverb"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_springreverb -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_springreverb_H__
#define  __T_springreverb_H__

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
#define FAUSTCLASS T_springreverb
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

static float T_springreverb_faustpower2_f(float value) {
	return value * value;
}

class T_springreverb : public FaustDsp {
	
 private:
	
	int IOTA0;
	float fVec0[8192];
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst2;
	float fRec0[2];
	float fConst3;
	float fConst4;
	float fConst5;
	float fConst6;
	FAUSTFLOAT fHslider1;
	float fSlow1;
	float fRec18[2];
	FAUSTFLOAT fHslider2;
	float fSlow2;
	float fRec19[2];
	float fConst7;
	float fConst8;
	int iConst9;
	int iConst10;
	int iConst11;
	int iConst12;
	int iConst13;
	int iConst14;
	int iConst15;
	int iConst16;
	float fVec1[2048];
	int iConst17;
	float fVec2[4096];
	int iConst18;
	float fVec3[4096];
	int iConst19;
	float fVec4[8192];
	int iConst20;
	float fVec5[4096];
	int iConst21;
	float fVec6[8192];
	int iConst22;
	float fVec7[4096];
	int iConst23;
	float fVec8[8192];
	int iConst24;
	float fVec9[2048];
	int iConst25;
	float fVec10[4096];
	int iConst26;
	float fVec11[4096];
	int iConst27;
	float fVec12[4096];
	int iConst28;
	float fVec13[4096];
	int iConst29;
	float fVec14[4096];
	int iConst30;
	float fVec15[4096];
	int iConst31;
	float fVec16[8192];
	int iConst32;
	float fVec17[4096];
	int iConst33;
	float fVec18[16384];
	float fConst34;
	FAUSTFLOAT fHslider3;
	float fSlow3;
	float fRec20[2];
	float fVec19[2];
	float fRec2[2];
	float fVec20[8192];
	int iConst35;
	float fVec21[16384];
	float fVec22[2];
	float fRec3[2];
	float fRec4[2];
	float fVec23[4096];
	int iConst36;
	float fVec24[16384];
	float fVec25[2];
	float fRec5[2];
	float fVec26[4096];
	int iConst37;
	float fVec27[16384];
	float fVec28[2];
	float fRec6[2];
	float fVec29[4096];
	float fVec30[16384];
	float fVec31[2];
	float fRec7[2];
	float fRec8[2];
	float fRec9[2];
	float fVec32[8192];
	int iConst38;
	float fVec33[16384];
	float fVec34[2];
	float fRec10[2];
	float fVec35[2048];
	float fVec36[16384];
	float fVec37[2];
	float fRec11[2];
	float fVec38[8192];
	int iConst39;
	float fVec39[16384];
	float fVec40[2];
	float fRec12[2];
	float fRec13[2];
	float fRec14[2];
	float fRec15[2];
	float fRec16[2];
	float fRec17[2];
	float fVec41[2];
	float fRec1[2];
	
 public:
	T_springreverb() {
	}
	
	T_springreverb(const T_springreverb&) = default;
	
	virtual ~T_springreverb() = default;
	
	T_springreverb& operator=(const T_springreverb&) = default;
	
	void metadata(Meta* m) { 
		m->declare("aanl.lib/ADAA1:author", "Dario Sanfilippo");
		m->declare("aanl.lib/ADAA1:copyright", "Copyright (C) 2021 Dario Sanfilippo     <sanfilippo.dario@gmail.com>");
		m->declare("aanl.lib/ADAA1:license", "MIT");
		m->declare("aanl.lib/hardclip:author", "Dario Sanfilippo");
		m->declare("aanl.lib/hardclip:copyright", "Copyright (C) 2021 Dario Sanfilippo     <sanfilippo.dario@gmail.com>");
		m->declare("aanl.lib/hardclip:license", "MIT");
		m->declare("aanl.lib/name", "Faust Antialiased Nonlinearities");
		m->declare("aanl.lib/version", "1.5.0");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_springreverb -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("filename", "springreverb.dsp");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highpass:license", "LicenseRef-STK-4.3");
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
		m->declare("name", "springreverb");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/springreverb:author", "Daniel Leonov");
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
		fConst3 = 1.0f / std::tan(471.2389f / fConst0);
		fConst4 = 1.0f / (fConst3 + 1.0f);
		fConst5 = 1.0f - fConst3;
		fConst6 = 3.1415927f / fConst0;
		fConst7 = std::round(0.035f * fConst0);
		fConst8 = std::max<float>(0.0f, fConst7);
		iConst9 = static_cast<int>(std::min<float>(std::round(0.05f * fConst0), fConst8));
		iConst10 = static_cast<int>(std::min<float>(fConst7, fConst8));
		iConst11 = static_cast<int>(std::min<float>(std::round(0.0425f * fConst0), fConst8));
		iConst12 = static_cast<int>(std::min<float>(std::round(0.0275f * fConst0), fConst8));
		iConst13 = static_cast<int>(std::min<float>(std::round(0.04625f * fConst0), fConst8));
		iConst14 = static_cast<int>(std::min<float>(std::round(0.03125f * fConst0), fConst8));
		iConst15 = static_cast<int>(std::min<float>(std::round(0.03875f * fConst0), fConst8));
		iConst16 = static_cast<int>(std::min<float>(std::round(0.02375f * fConst0), fConst8));
		iConst17 = static_cast<int>(std::min<float>(std::round(0.009f * fConst0), fConst8));
		iConst18 = static_cast<int>(std::min<float>(std::round(0.0195f * fConst0), fConst8));
		iConst19 = static_cast<int>(std::min<float>(std::round(0.01425f * fConst0), fConst8));
		iConst20 = static_cast<int>(std::min<float>(std::round(0.02475f * fConst0), fConst8));
		iConst21 = static_cast<int>(std::min<float>(std::round(0.011625f * fConst0), fConst8));
		iConst22 = static_cast<int>(std::min<float>(std::round(0.022125f * fConst0), fConst8));
		iConst23 = static_cast<int>(std::min<float>(std::round(0.016875f * fConst0), fConst8));
		iConst24 = static_cast<int>(std::min<float>(std::round(0.027375f * fConst0), fConst8));
		iConst25 = static_cast<int>(std::min<float>(std::round(0.01f * fConst0), fConst8));
		iConst26 = static_cast<int>(std::min<float>(std::round(0.0175f * fConst0), fConst8));
		iConst27 = static_cast<int>(std::min<float>(std::round(0.01375f * fConst0), fConst8));
		iConst28 = static_cast<int>(std::min<float>(std::round(0.02125f * fConst0), fConst8));
		iConst29 = static_cast<int>(std::min<float>(std::round(0.011875f * fConst0), fConst8));
		iConst30 = static_cast<int>(std::min<float>(std::round(0.019375f * fConst0), fConst8));
		iConst31 = static_cast<int>(std::min<float>(std::round(0.015625f * fConst0), fConst8));
		iConst32 = static_cast<int>(std::min<float>(std::round(0.023125f * fConst0), fConst8));
		iConst33 = static_cast<int>(std::min<float>(std::round(0.0205f * fConst0), fConst8));
		fConst34 = std::round(0.08f * fConst0);
		iConst35 = static_cast<int>(std::min<float>(std::round(0.023375f * fConst0), fConst8));
		iConst36 = static_cast<int>(std::min<float>(std::round(0.01475f * fConst0), fConst8));
		iConst37 = static_cast<int>(std::min<float>(std::round(0.017625f * fConst0), fConst8));
		iConst38 = static_cast<int>(std::min<float>(std::round(0.02625f * fConst0), fConst8));
		iConst39 = static_cast<int>(std::min<float>(std::round(0.029125f * fConst0), fConst8));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.5f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.5f);
		fHslider2 = static_cast<FAUSTFLOAT>(0.5f);
		fHslider3 = static_cast<FAUSTFLOAT>(0.5f);
	}
	
	virtual void instanceClear() {
		IOTA0 = 0;
		for (int l0 = 0; l0 < 8192; l0 = faust_wrap_add(l0, 1)) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec0[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = faust_wrap_add(l2, 1)) {
			fRec18[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = faust_wrap_add(l3, 1)) {
			fRec19[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2048; l4 = faust_wrap_add(l4, 1)) {
			fVec1[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 4096; l5 = faust_wrap_add(l5, 1)) {
			fVec2[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 4096; l6 = faust_wrap_add(l6, 1)) {
			fVec3[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 8192; l7 = faust_wrap_add(l7, 1)) {
			fVec4[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 4096; l8 = faust_wrap_add(l8, 1)) {
			fVec5[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 8192; l9 = faust_wrap_add(l9, 1)) {
			fVec6[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 4096; l10 = faust_wrap_add(l10, 1)) {
			fVec7[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 8192; l11 = faust_wrap_add(l11, 1)) {
			fVec8[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2048; l12 = faust_wrap_add(l12, 1)) {
			fVec9[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 4096; l13 = faust_wrap_add(l13, 1)) {
			fVec10[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 4096; l14 = faust_wrap_add(l14, 1)) {
			fVec11[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 4096; l15 = faust_wrap_add(l15, 1)) {
			fVec12[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 4096; l16 = faust_wrap_add(l16, 1)) {
			fVec13[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 4096; l17 = faust_wrap_add(l17, 1)) {
			fVec14[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 4096; l18 = faust_wrap_add(l18, 1)) {
			fVec15[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 8192; l19 = faust_wrap_add(l19, 1)) {
			fVec16[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 4096; l20 = faust_wrap_add(l20, 1)) {
			fVec17[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 16384; l21 = faust_wrap_add(l21, 1)) {
			fVec18[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = faust_wrap_add(l22, 1)) {
			fRec20[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = faust_wrap_add(l23, 1)) {
			fVec19[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = faust_wrap_add(l24, 1)) {
			fRec2[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 8192; l25 = faust_wrap_add(l25, 1)) {
			fVec20[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 16384; l26 = faust_wrap_add(l26, 1)) {
			fVec21[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = faust_wrap_add(l27, 1)) {
			fVec22[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = faust_wrap_add(l28, 1)) {
			fRec3[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = faust_wrap_add(l29, 1)) {
			fRec4[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 4096; l30 = faust_wrap_add(l30, 1)) {
			fVec23[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 16384; l31 = faust_wrap_add(l31, 1)) {
			fVec24[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = faust_wrap_add(l32, 1)) {
			fVec25[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = faust_wrap_add(l33, 1)) {
			fRec5[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 4096; l34 = faust_wrap_add(l34, 1)) {
			fVec26[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 16384; l35 = faust_wrap_add(l35, 1)) {
			fVec27[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = faust_wrap_add(l36, 1)) {
			fVec28[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = faust_wrap_add(l37, 1)) {
			fRec6[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 4096; l38 = faust_wrap_add(l38, 1)) {
			fVec29[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 16384; l39 = faust_wrap_add(l39, 1)) {
			fVec30[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = faust_wrap_add(l40, 1)) {
			fVec31[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = faust_wrap_add(l41, 1)) {
			fRec7[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = faust_wrap_add(l42, 1)) {
			fRec8[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = faust_wrap_add(l43, 1)) {
			fRec9[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 8192; l44 = faust_wrap_add(l44, 1)) {
			fVec32[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 16384; l45 = faust_wrap_add(l45, 1)) {
			fVec33[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = faust_wrap_add(l46, 1)) {
			fVec34[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = faust_wrap_add(l47, 1)) {
			fRec10[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 2048; l48 = faust_wrap_add(l48, 1)) {
			fVec35[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 16384; l49 = faust_wrap_add(l49, 1)) {
			fVec36[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = faust_wrap_add(l50, 1)) {
			fVec37[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = faust_wrap_add(l51, 1)) {
			fRec11[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 8192; l52 = faust_wrap_add(l52, 1)) {
			fVec38[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 16384; l53 = faust_wrap_add(l53, 1)) {
			fVec39[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = faust_wrap_add(l54, 1)) {
			fVec40[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = faust_wrap_add(l55, 1)) {
			fRec12[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = faust_wrap_add(l56, 1)) {
			fRec13[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 2; l57 = faust_wrap_add(l57, 1)) {
			fRec14[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = faust_wrap_add(l58, 1)) {
			fRec15[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2; l59 = faust_wrap_add(l59, 1)) {
			fRec16[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = faust_wrap_add(l60, 1)) {
			fRec17[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 2; l61 = faust_wrap_add(l61, 1)) {
			fVec41[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 2; l62 = faust_wrap_add(l62, 1)) {
			fRec1[l62] = 0.0f;
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
	
	virtual T_springreverb* clone() {
		return new T_springreverb(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("springreverb");
		ui_interface->addHorizontalSlider("blend", &fHslider0, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("dwell", &fHslider2, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("tension", &fHslider3, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("tone", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
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
		fVec0[IOTA0 & 8191] = fTemp0;
		fRec0[0] = fSlow0 + fConst2 * fRec0[1];
		fRec18[0] = fSlow1 + fConst2 * fRec18[1];
		float fTemp1 = std::max<float>(0.0f, std::min<float>(1e+01f, 1e+01f * fRec18[0]));
		float fTemp2 = 1.5e+03f + fTemp1 * (5e+01f + 1.9e+02f * fTemp1);
		float fTemp3 = 1.0f / std::tan(fConst6 * fTemp2);
		float fTemp4 = 1.0f - fTemp3;
		float fTemp5 = fRec14[1] + fRec13[1];
		float fTemp6 = fRec8[1] + fRec16[1];
		fRec19[0] = fSlow2 + fConst2 * fRec19[1];
		float fTemp7 = std::max<float>(0.0f, std::min<float>(1e+01f, 1e+01f * fRec19[0]));
		float fTemp8 = 0.26f + fTemp7 * (0.013f - 0.0006f * fTemp7);
		float fTemp9 = fVec0[(faust_wrap_sub(IOTA0, iConst9)) & 8191];
		float fTemp10 = fVec0[(faust_wrap_sub(IOTA0, iConst10)) & 8191];
		float fTemp11 = fTemp9 + fTemp10;
		float fTemp12 = fVec0[(faust_wrap_sub(IOTA0, iConst11)) & 8191];
		float fTemp13 = fVec0[(faust_wrap_sub(IOTA0, iConst12)) & 8191];
		float fTemp14 = fTemp12 + fTemp13;
		float fTemp15 = fTemp11 + fTemp14;
		float fTemp16 = fVec0[(faust_wrap_sub(IOTA0, iConst13)) & 8191];
		float fTemp17 = fVec0[(faust_wrap_sub(IOTA0, iConst14)) & 8191];
		float fTemp18 = fTemp16 + fTemp17;
		float fTemp19 = fVec0[(faust_wrap_sub(IOTA0, iConst15)) & 8191];
		float fTemp20 = fVec0[(faust_wrap_sub(IOTA0, iConst16)) & 8191];
		float fTemp21 = fTemp19 + fTemp20;
		float fTemp22 = fTemp18 + fTemp21;
		fVec1[IOTA0 & 2047] = fTemp15 + fTemp22;
		float fTemp23 = fVec1[(faust_wrap_sub(IOTA0, iConst17)) & 2047];
		float fTemp24 = fTemp9 - fTemp10;
		float fTemp25 = fTemp12 - fTemp13;
		float fTemp26 = fTemp24 + fTemp25;
		float fTemp27 = fTemp16 - fTemp17;
		float fTemp28 = fTemp19 - fTemp20;
		float fTemp29 = fTemp27 + fTemp28;
		fVec2[IOTA0 & 4095] = fTemp26 + fTemp29;
		float fTemp30 = fVec2[(faust_wrap_sub(IOTA0, iConst18)) & 4095];
		float fTemp31 = fTemp23 - fTemp30;
		float fTemp32 = fTemp11 - fTemp14;
		float fTemp33 = fTemp18 - fTemp21;
		fVec3[IOTA0 & 4095] = fTemp32 + fTemp33;
		float fTemp34 = fVec3[(faust_wrap_sub(IOTA0, iConst19)) & 4095];
		float fTemp35 = fTemp24 - fTemp25;
		float fTemp36 = fTemp27 - fTemp28;
		fVec4[IOTA0 & 8191] = fTemp35 + fTemp36;
		float fTemp37 = fVec4[(faust_wrap_sub(IOTA0, iConst20)) & 8191];
		float fTemp38 = fTemp34 - fTemp37;
		float fTemp39 = fTemp31 + fTemp38;
		fVec5[IOTA0 & 4095] = fTemp15 - fTemp22;
		float fTemp40 = fVec5[(faust_wrap_sub(IOTA0, iConst21)) & 4095];
		fVec6[IOTA0 & 8191] = fTemp26 - fTemp29;
		float fTemp41 = fVec6[(faust_wrap_sub(IOTA0, iConst22)) & 8191];
		float fTemp42 = fTemp40 - fTemp41;
		fVec7[IOTA0 & 4095] = fTemp32 - fTemp33;
		float fTemp43 = fVec7[(faust_wrap_sub(IOTA0, iConst23)) & 4095];
		fVec8[IOTA0 & 8191] = fTemp35 - fTemp36;
		float fTemp44 = fVec8[(faust_wrap_sub(IOTA0, iConst24)) & 8191];
		float fTemp45 = fTemp43 - fTemp44;
		float fTemp46 = fTemp42 - fTemp45;
		fVec9[IOTA0 & 2047] = fTemp39 - fTemp46;
		float fTemp47 = fVec9[(faust_wrap_sub(IOTA0, iConst25)) & 2047];
		float fTemp48 = fTemp23 + fTemp30;
		float fTemp49 = fTemp34 + fTemp37;
		float fTemp50 = fTemp48 + fTemp49;
		float fTemp51 = fTemp40 + fTemp41;
		float fTemp52 = fTemp43 + fTemp44;
		float fTemp53 = fTemp51 - fTemp52;
		fVec10[IOTA0 & 4095] = fTemp50 - fTemp53;
		float fTemp54 = fVec10[(faust_wrap_sub(IOTA0, iConst26)) & 4095];
		float fTemp55 = fTemp47 + fTemp54;
		float fTemp56 = fTemp31 - fTemp38;
		float fTemp57 = fTemp42 + fTemp45;
		fVec11[IOTA0 & 4095] = fTemp56 - fTemp57;
		float fTemp58 = fVec11[(faust_wrap_sub(IOTA0, iConst27)) & 4095];
		float fTemp59 = fTemp48 - fTemp49;
		float fTemp60 = fTemp51 + fTemp52;
		fVec12[IOTA0 & 4095] = fTemp59 - fTemp60;
		float fTemp61 = fVec12[(faust_wrap_sub(IOTA0, iConst28)) & 4095];
		float fTemp62 = fTemp58 - fTemp61;
		float fTemp63 = fTemp55 - fTemp62;
		fVec13[IOTA0 & 4095] = fTemp39 + fTemp46;
		float fTemp64 = fVec13[(faust_wrap_sub(IOTA0, iConst29)) & 4095];
		fVec14[IOTA0 & 4095] = fTemp50 + fTemp53;
		float fTemp65 = fVec14[(faust_wrap_sub(IOTA0, iConst30)) & 4095];
		float fTemp66 = fTemp64 - fTemp65;
		fVec15[IOTA0 & 4095] = fTemp56 + fTemp57;
		float fTemp67 = fVec15[(faust_wrap_sub(IOTA0, iConst31)) & 4095];
		fVec16[IOTA0 & 8191] = fTemp59 + fTemp60;
		float fTemp68 = fVec16[(faust_wrap_sub(IOTA0, iConst32)) & 8191];
		float fTemp69 = fTemp67 - fTemp68;
		float fTemp70 = fTemp66 - fTemp69;
		fVec17[IOTA0 & 4095] = fTemp63 - fTemp70;
		fVec18[IOTA0 & 16383] = (fRec9[1] + fTemp5 + fRec15[1] - (fRec17[1] + fTemp6 + fRec4[1])) * fTemp8 - 0.01f * fVec17[(faust_wrap_sub(IOTA0, iConst33)) & 4095];
		fRec20[0] = fSlow3 + fConst2 * fRec20[1];
		float fTemp71 = std::max<float>(0.0f, std::min<float>(1e+01f, 1e+01f * fRec20[0]));
		float fTemp72 = fTemp71 * (-0.0072f + 0.00032f * fTemp71);
		float fTemp73 = fVec18[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst34, std::max<float>(0.0f, std::round(fConst0 * (0.070196f + fTemp72))))))) & 16383];
		fVec19[0] = fTemp73;
		float fTemp74 = 1.0f + fTemp3;
		fRec2[0] = -((fRec2[1] * fTemp4 - (fTemp73 + fVec19[1])) / fTemp74);
		fVec20[IOTA0 & 8191] = fTemp63 + fTemp70;
		fVec21[IOTA0 & 16383] = fTemp8 * (fRec17[1] + fTemp5 + fRec4[1] - (fRec9[1] + fTemp6 + fRec15[1])) - 0.01f * fVec20[(faust_wrap_sub(IOTA0, iConst35)) & 8191];
		float fTemp75 = fVec21[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst34, std::max<float>(0.0f, std::round(fConst0 * (0.070308f + fTemp72))))))) & 16383];
		fVec22[0] = fTemp75;
		fRec3[0] = -((fTemp4 * fRec3[1] - (fTemp75 + fVec22[1])) / fTemp74);
		float fTemp76 = fRec3[0] - fRec3[1];
		fRec4[0] = ((std::fabs(fTemp76) <= 0.001f) ? std::max<float>(-1.0f, std::min<float>(1.0f, 0.5f * (fRec3[1] + fRec3[0]))) : ((((fRec3[0] <= 1.0f) & (fRec3[0] >= -1.0f)) ? 0.5f * T_springreverb_faustpower2_f(fRec3[0]) : -0.5f + fRec3[0] * static_cast<float>((fRec3[0] > 0.0f) - (fRec3[0] < 0.0f))) - (((fRec3[1] <= 1.0f) & (fRec3[1] >= -1.0f)) ? 0.5f * T_springreverb_faustpower2_f(fRec3[1]) : -0.5f + fRec3[1] * static_cast<float>((fRec3[1] > 0.0f) - (fRec3[1] < 0.0f)))) / fTemp76);
		float fTemp77 = fTemp47 - fTemp54;
		float fTemp78 = fTemp58 + fTemp61;
		float fTemp79 = fTemp77 + fTemp78;
		float fTemp80 = fTemp64 + fTemp65;
		float fTemp81 = fTemp67 + fTemp68;
		float fTemp82 = fTemp80 + fTemp81;
		fVec23[IOTA0 & 4095] = fTemp79 - fTemp82;
		float fTemp83 = fRec14[1] + fRec8[1];
		float fTemp84 = fRec13[1] + fRec16[1];
		fVec24[IOTA0 & 16383] = 0.01f * fVec23[(faust_wrap_sub(IOTA0, iConst36)) & 4095] + fTemp8 * (fRec4[1] + fRec15[1] + fTemp83 - (fRec17[1] + fRec9[1] + fTemp84));
		float fTemp85 = fVec24[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst34, std::max<float>(0.0f, std::round(fConst0 * (0.070084f + fTemp72))))))) & 16383];
		fVec25[0] = fTemp85;
		fRec5[0] = -((fTemp4 * fRec5[1] - (fTemp85 + fVec25[1])) / fTemp74);
		fVec26[IOTA0 & 4095] = fTemp79 + fTemp82;
		fVec27[IOTA0 & 16383] = fTemp8 * (fRec17[1] + fRec9[1] + fTemp83 - (fRec4[1] + fRec15[1] + fTemp84)) - 0.01f * fVec26[(faust_wrap_sub(IOTA0, iConst37)) & 4095];
		float fTemp86 = fVec27[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst34, std::max<float>(0.0f, std::round(fConst0 * (0.07014f + fTemp72))))))) & 16383];
		fVec28[0] = fTemp86;
		fRec6[0] = -((fTemp4 * fRec6[1] - (fTemp86 + fVec28[1])) / fTemp74);
		float fTemp87 = fTemp77 - fTemp78;
		float fTemp88 = fTemp80 - fTemp81;
		fVec29[IOTA0 & 4095] = fTemp87 + fTemp88;
		float fTemp89 = fRec16[1] + fRec13[1] + fTemp83;
		fVec30[IOTA0 & 16383] = 0.01f * fVec29[(faust_wrap_sub(IOTA0, iConst29)) & 4095] + fTemp8 * (fTemp89 - (fRec17[1] + fRec9[1] + fRec15[1] + fRec4[1]));
		float fTemp90 = fVec30[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst34, std::max<float>(0.0f, std::round(fConst0 * (0.070056f + fTemp72))))))) & 16383];
		fVec31[0] = fTemp90;
		fRec7[0] = -((fTemp4 * fRec7[1] - (fTemp90 + fVec31[1])) / fTemp74);
		float fTemp91 = fRec2[0] - fRec2[1];
		fRec8[0] = ((std::fabs(fTemp91) <= 0.001f) ? std::max<float>(-1.0f, std::min<float>(1.0f, 0.5f * (fRec2[1] + fRec2[0]))) : ((((fRec2[0] <= 1.0f) & (fRec2[0] >= -1.0f)) ? 0.5f * T_springreverb_faustpower2_f(fRec2[0]) : -0.5f + fRec2[0] * static_cast<float>((fRec2[0] > 0.0f) - (fRec2[0] < 0.0f))) - (((fRec2[1] <= 1.0f) & (fRec2[1] >= -1.0f)) ? 0.5f * T_springreverb_faustpower2_f(fRec2[1]) : -0.5f + fRec2[1] * static_cast<float>((fRec2[1] > 0.0f) - (fRec2[1] < 0.0f)))) / fTemp91);
		float fTemp92 = fRec6[0] - fRec6[1];
		fRec9[0] = ((std::fabs(fTemp92) <= 0.001f) ? std::max<float>(-1.0f, std::min<float>(1.0f, 0.5f * (fRec6[1] + fRec6[0]))) : ((((fRec6[0] <= 1.0f) & (fRec6[0] >= -1.0f)) ? 0.5f * T_springreverb_faustpower2_f(fRec6[0]) : -0.5f + fRec6[0] * static_cast<float>((fRec6[0] > 0.0f) - (fRec6[0] < 0.0f))) - (((fRec6[1] <= 1.0f) & (fRec6[1] >= -1.0f)) ? 0.5f * T_springreverb_faustpower2_f(fRec6[1]) : -0.5f + fRec6[1] * static_cast<float>((fRec6[1] > 0.0f) - (fRec6[1] < 0.0f)))) / fTemp92);
		float fTemp93 = fTemp55 + fTemp62;
		float fTemp94 = fTemp66 + fTemp69;
		fVec32[IOTA0 & 8191] = fTemp93 - fTemp94;
		float fTemp95 = fRec14[1] + fRec16[1];
		float fTemp96 = fRec8[1] + fRec13[1];
		fVec33[IOTA0 & 16383] = 0.01f * fVec32[(faust_wrap_sub(IOTA0, iConst38)) & 8191] + fTemp8 * (fRec17[1] + fRec15[1] + fTemp95 - (fRec9[1] + fRec4[1] + fTemp96));
		float fTemp97 = fVec33[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst34, std::max<float>(0.0f, std::round(fConst0 * (0.070364f + fTemp72))))))) & 16383];
		fVec34[0] = fTemp97;
		fRec10[0] = -((fTemp4 * fRec10[1] - (fTemp97 + fVec34[1])) / fTemp74);
		fVec35[IOTA0 & 2047] = fTemp87 - fTemp88;
		fVec36[IOTA0 & 16383] = fTemp8 * (fRec17[1] + fRec9[1] + fRec4[1] + fRec15[1] + fTemp89) - 0.01f * fVec35[(faust_wrap_sub(IOTA0, iConst17)) & 2047];
		float fTemp98 = fVec36[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst34, std::max<float>(0.0f, std::round(fConst0 * (0.070028f + fTemp72))))))) & 16383];
		fVec37[0] = fTemp98;
		fRec11[0] = -((fTemp4 * fRec11[1] - (fTemp98 + fVec37[1])) / fTemp74);
		fVec38[IOTA0 & 8191] = fTemp93 + fTemp94;
		fVec39[IOTA0 & 16383] = fTemp8 * (fRec9[1] + fRec4[1] + fTemp95 - (fRec17[1] + fRec15[1] + fTemp96)) - 0.01f * fVec38[(faust_wrap_sub(IOTA0, iConst39)) & 8191];
		float fTemp99 = fVec39[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst34, std::max<float>(0.0f, std::round(fConst0 * (0.070476f + fTemp72))))))) & 16383];
		fVec40[0] = fTemp99;
		fRec12[0] = -((fTemp4 * fRec12[1] - (fTemp99 + fVec40[1])) / fTemp74);
		float fTemp100 = fRec5[0] - fRec5[1];
		fRec13[0] = ((std::fabs(fTemp100) <= 0.001f) ? std::max<float>(-1.0f, std::min<float>(1.0f, 0.5f * (fRec5[1] + fRec5[0]))) : ((((fRec5[0] <= 1.0f) & (fRec5[0] >= -1.0f)) ? 0.5f * T_springreverb_faustpower2_f(fRec5[0]) : -0.5f + fRec5[0] * static_cast<float>((fRec5[0] > 0.0f) - (fRec5[0] < 0.0f))) - (((fRec5[1] <= 1.0f) & (fRec5[1] >= -1.0f)) ? 0.5f * T_springreverb_faustpower2_f(fRec5[1]) : -0.5f + fRec5[1] * static_cast<float>((fRec5[1] > 0.0f) - (fRec5[1] < 0.0f)))) / fTemp100);
		float fTemp101 = fRec11[0] - fRec11[1];
		fRec14[0] = ((std::fabs(fTemp101) <= 0.001f) ? std::max<float>(-1.0f, std::min<float>(1.0f, 0.5f * (fRec11[1] + fRec11[0]))) : ((((fRec11[0] <= 1.0f) & (fRec11[0] >= -1.0f)) ? 0.5f * T_springreverb_faustpower2_f(fRec11[0]) : -0.5f + fRec11[0] * static_cast<float>((fRec11[0] > 0.0f) - (fRec11[0] < 0.0f))) - (((fRec11[1] <= 1.0f) & (fRec11[1] >= -1.0f)) ? 0.5f * T_springreverb_faustpower2_f(fRec11[1]) : -0.5f + fRec11[1] * static_cast<float>((fRec11[1] > 0.0f) - (fRec11[1] < 0.0f)))) / fTemp101);
		float fTemp102 = fRec7[0] - fRec7[1];
		fRec15[0] = ((std::fabs(fTemp102) <= 0.001f) ? std::max<float>(-1.0f, std::min<float>(1.0f, 0.5f * (fRec7[1] + fRec7[0]))) : ((((fRec7[0] <= 1.0f) & (fRec7[0] >= -1.0f)) ? 0.5f * T_springreverb_faustpower2_f(fRec7[0]) : -0.5f + fRec7[0] * static_cast<float>((fRec7[0] > 0.0f) - (fRec7[0] < 0.0f))) - (((fRec7[1] <= 1.0f) & (fRec7[1] >= -1.0f)) ? 0.5f * T_springreverb_faustpower2_f(fRec7[1]) : -0.5f + fRec7[1] * static_cast<float>((fRec7[1] > 0.0f) - (fRec7[1] < 0.0f)))) / fTemp102);
		float fTemp103 = fRec10[0] - fRec10[1];
		fRec16[0] = ((std::fabs(fTemp103) <= 0.001f) ? std::max<float>(-1.0f, std::min<float>(1.0f, 0.5f * (fRec10[1] + fRec10[0]))) : ((((fRec10[0] <= 1.0f) & (fRec10[0] >= -1.0f)) ? 0.5f * T_springreverb_faustpower2_f(fRec10[0]) : -0.5f + fRec10[0] * static_cast<float>((fRec10[0] > 0.0f) - (fRec10[0] < 0.0f))) - (((fRec10[1] <= 1.0f) & (fRec10[1] >= -1.0f)) ? 0.5f * T_springreverb_faustpower2_f(fRec10[1]) : -0.5f + fRec10[1] * static_cast<float>((fRec10[1] > 0.0f) - (fRec10[1] < 0.0f)))) / fTemp103);
		float fTemp104 = fRec12[0] - fRec12[1];
		fRec17[0] = ((std::fabs(fTemp104) <= 0.001f) ? std::max<float>(-1.0f, std::min<float>(1.0f, 0.5f * (fRec12[1] + fRec12[0]))) : ((((fRec12[0] <= 1.0f) & (fRec12[0] >= -1.0f)) ? 0.5f * T_springreverb_faustpower2_f(fRec12[0]) : -0.5f + fRec12[0] * static_cast<float>((fRec12[0] > 0.0f) - (fRec12[0] < 0.0f))) - (((fRec12[1] <= 1.0f) & (fRec12[1] >= -1.0f)) ? 0.5f * T_springreverb_faustpower2_f(fRec12[1]) : -0.5f + fRec12[1] * static_cast<float>((fRec12[1] > 0.0f) - (fRec12[1] < 0.0f)))) / fTemp104);
		float fTemp105 = fRec14[0] + fRec15[0] + fRec13[0] + fRec9[0] + fRec8[0] + fRec4[0] + fRec16[0] + fRec17[0];
		fVec41[0] = fTemp105;
		fRec1[0] = -(fConst4 * (fConst5 * fRec1[1] - fConst3 * (fTemp105 - fVec41[1])));
		outputs[0] = static_cast<FAUSTFLOAT>(fTemp0 + 0.04f * std::max<float>(0.0f, std::min<float>(1e+01f, 1e+01f * fRec0[0])) * fRec1[0] * std::min<float>(5.0f, 1.0f + 2e+03f / fTemp2));
		IOTA0 = faust_wrap_add(IOTA0, 1);
		fRec0[1] = fRec0[0];
		fRec18[1] = fRec18[0];
		fRec19[1] = fRec19[0];
		fRec20[1] = fRec20[0];
		fVec19[1] = fVec19[0];
		fRec2[1] = fRec2[0];
		fVec22[1] = fVec22[0];
		fRec3[1] = fRec3[0];
		fRec4[1] = fRec4[0];
		fVec25[1] = fVec25[0];
		fRec5[1] = fRec5[0];
		fVec28[1] = fVec28[0];
		fRec6[1] = fRec6[0];
		fVec31[1] = fVec31[0];
		fRec7[1] = fRec7[0];
		fRec8[1] = fRec8[0];
		fRec9[1] = fRec9[0];
		fVec34[1] = fVec34[0];
		fRec10[1] = fRec10[0];
		fVec37[1] = fVec37[0];
		fRec11[1] = fRec11[0];
		fVec40[1] = fVec40[0];
		fRec12[1] = fRec12[0];
		fRec13[1] = fRec13[0];
		fRec14[1] = fRec14[0];
		fRec15[1] = fRec15[0];
		fRec16[1] = fRec16[0];
		fRec17[1] = fRec17[0];
		fVec41[1] = fVec41[0];
		fRec1[1] = fRec1[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
