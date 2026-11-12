/* ------------------------------------------------------------
name: "vital-rev"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_vital_rev -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_vital_rev_H__
#define  __T_vital_rev_H__

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
#define FAUSTCLASS T_vital_rev
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

static float T_vital_rev_faustpower2_f(float value) {
	return value * value;
}

class T_vital_rev : public FaustDsp {
	
 private:
	
	int IOTA0;
	float fVec0[8192];
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst2;
	int iVec1[2];
	float fRec0[2];
	float fConst3;
	float fConst4;
	float fConst5;
	float fRec1[2];
	float fRec2[2];
	float fRec3[2];
	float fRec4[2];
	float fRec5[2];
	float fRec6[2];
	float fRec7[2];
	float fRec8[2];
	float fRec9[2];
	float fRec10[2];
	float fRec11[2];
	float fRec12[2];
	float fConst6;
	float fConst7;
	float fConst8;
	float fVec2[2];
	float fRec13[2];
	float fRec14[2];
	float fRec15[2];
	float fRec16[2];
	float fRec17[2];
	float fRec18[2];
	float fRec19[2];
	float fRec20[2];
	float fVec3[2];
	float fRec21[2];
	float fRec22[2];
	float fRec23[2];
	float fRec24[2];
	float fRec25[2];
	float fVec4[2];
	float fRec26[2];
	float fRec27[2];
	float fRec28[2];
	float fRec29[2];
	float fRec30[2];
	float fRec31[2];
	float fRec32[2];
	float fVec5[2];
	float fRec33[2];
	float fVec6[2];
	float fRec34[2];
	float fRec35[2];
	float fRec36[2];
	float fRec37[2];
	float fRec38[2];
	float fRec39[2];
	float fVec7[2];
	float fRec40[2];
	float fVec8[2];
	float fRec41[2];
	float fRec42[2];
	FAUSTFLOAT fHslider1;
	float fSlow1;
	float fRec113[2];
	float fConst9;
	FAUSTFLOAT fHslider2;
	float fSlow2;
	float fRec115[2];
	float fRec114[2];
	FAUSTFLOAT fHslider3;
	float fSlow3;
	float fRec116[2];
	FAUSTFLOAT fHslider4;
	float fSlow4;
	float fRec117[2];
	float fVec9[65536];
	float fConst10;
	float fConst11;
	float fConst12;
	float fConst13;
	float fConst14;
	float fConst15;
	float fConst16;
	float fConst17;
	float fConst18;
	float fConst19;
	float fConst20;
	float fConst21;
	float fConst22;
	float fConst23;
	float fConst24;
	int iConst25;
	int iConst26;
	float fConst27;
	float fConst28;
	int iConst29;
	float fConst30;
	float fConst31;
	int iConst32;
	float fConst33;
	int iConst34;
	float fVec10[2];
	float fRec119[2];
	float fRec118[2];
	float fVec11[1024];
	float fRec43[2];
	float fVec12[2];
	float fRec44[2];
	float fVec13[2];
	float fRec45[2];
	float fRec46[2];
	float fRec120[2];
	float fVec14[131072];
	float fVec15[1024];
	float fRec48[2];
	float fRec49[2];
	float fVec16[2];
	float fRec50[2];
	float fVec17[2];
	float fRec51[2];
	float fRec121[2];
	float fVec18[65536];
	float fVec19[1024];
	float fRec52[2];
	float fRec53[2];
	float fVec20[131072];
	float fVec21[1024];
	float fRec54[2];
	float fVec22[2];
	float fRec55[2];
	float fRec122[2];
	float fVec23[131072];
	float fVec24[1024];
	float fRec56[2];
	float fRec57[2];
	float fRec123[2];
	float fVec25[131072];
	float fVec26[1024];
	float fVec27[2];
	float fRec59[2];
	float fVec28[2];
	float fRec60[2];
	float fVec29[2];
	float fRec61[2];
	float fVec30[2];
	float fRec62[2];
	float fRec63[2];
	float fRec64[2];
	float fRec66[2];
	float fRec67[2];
	float fRec124[2];
	float fVec31[131072];
	float fVec32[1024];
	float fRec69[2];
	float fVec33[65536];
	float fVec34[1024];
	float fRec70[2];
	float fRec71[2];
	float fVec35[131072];
	float fVec36[1024];
	float fRec73[2];
	float fVec37[131072];
	float fVec38[1024];
	float fRec74[2];
	float fRec125[2];
	float fVec39[131072];
	float fVec40[1024];
	float fRec77[2];
	float fRec78[2];
	float fRec79[2];
	float fRec80[2];
	float fRec84[2];
	float fRec126[2];
	float fVec41[131072];
	float fVec42[1024];
	float fRec85[2];
	float fRec86[2];
	float fVec43[131072];
	float fVec44[1024];
	float fRec88[2];
	float fVec45[131072];
	float fVec46[1024];
	float fVec47[131072];
	float fVec48[1024];
	float fRec91[2];
	float fVec49[65536];
	float fVec50[1024];
	float fRec93[2];
	float fRec94[2];
	float fRec97[3];
	float fRec98[3];
	float fRec99[3];
	float fRec100[3];
	float fRec101[3];
	float fRec102[3];
	float fRec103[3];
	float fRec104[3];
	float fRec105[3];
	float fRec106[3];
	float fRec107[3];
	float fRec108[3];
	float fRec109[3];
	float fRec110[3];
	float fRec111[3];
	float fRec112[3];
	
 public:
	T_vital_rev() {
	}
	
	T_vital_rev(const T_vital_rev&) = default;
	
	virtual ~T_vital_rev() = default;
	
	T_vital_rev& operator=(const T_vital_rev&) = default;
	
	void metadata(Meta* m) { 
		m->declare("aanl.lib/clip:deprecated", "internal helper, renamed _clip");
		m->declare("aanl.lib/name", "Faust Antialiased Nonlinearities");
		m->declare("aanl.lib/version", "1.5.0");
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "1.4.0");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_vital_rev -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/fdelayltv:author", "Julius O. Smith III");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("filename", "vital-rev.dsp");
		m->declare("filters.lib/allpass_comb:author", "Julius O. Smith III");
		m->declare("filters.lib/allpass_comb:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/allpass_comb:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/filterbank:author", "Julius O. Smith III");
		m->declare("filters.lib/filterbank:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/filterbank:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highpass:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/highshelf:author", "Julius O. Smith III");
		m->declare("filters.lib/highshelf:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highshelf:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass0_highpass1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/lowshelf:author", "Julius O. Smith III");
		m->declare("filters.lib/lowshelf:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowshelf:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/version", "1.9.0");
		m->declare("interpolators.lib/interpolate_linear:author", "Stéphane Letz");
		m->declare("interpolators.lib/interpolate_linear:licence", "MIT");
		m->declare("interpolators.lib/name", "Faust Interpolator Library");
		m->declare("interpolators.lib/remap:author", "David Braun");
		m->declare("interpolators.lib/version", "1.6.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("misceffects.lib/dryWetMixerConstantPower:author", "David Braun, revised by Stéphane Letz");
		m->declare("misceffects.lib/name", "Misc Effects Library");
		m->declare("misceffects.lib/version", "2.6.0");
		m->declare("name", "vital-rev");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "1.5.1");
		m->declare("reverbs.lib/vital_rev:author", "David Braun");
		m->declare("reverbs.lib/vital_rev:license", "GPL-3.0-only");
		m->declare("routes.lib/name", "Faust Signal Routing Library");
		m->declare("routes.lib/version", "1.4.0");
		m->declare("signals.lib/name", "Faust Routing Library");
		m->declare("signals.lib/version", "1.7.0");
		m->declare("spats.lib/name", "Faust Spatialization Library");
		m->declare("spats.lib/version", "1.3.0");
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
		fConst3 = 1.0f / std::tan(2012.1505f / fConst0);
		fConst4 = 1.0f / (fConst3 + 1.0f);
		fConst5 = 1.0f - fConst3;
		fConst6 = 1.0f / std::tan(7956.156f / fConst0);
		fConst7 = 1.0f / (fConst6 + 1.0f);
		fConst8 = 1.0f - fConst6;
		fConst9 = 1.0f / fConst0;
		fConst10 = 0.62716556f * fConst0;
		fConst11 = 1.0f / std::tan(15820.664f / fConst0);
		fConst12 = 1.0f / (fConst11 + 1.0f);
		fConst13 = 1.0f - fConst11;
		fConst14 = 1.0f / std::tan(255.91528f / fConst0);
		fConst15 = 1.0f / (fConst14 + 1.0f);
		fConst16 = 1.0f - fConst14;
		fConst17 = std::max<float>(1.0f, 0.03f * fConst0);
		fConst18 = fConst17 + -0.999995f;
		fConst19 = std::floor(fConst18);
		fConst20 = fConst17 + (-3.0f - fConst19);
		fConst21 = fConst17 + (-2.0f - fConst19);
		fConst22 = fConst17 - fConst19;
		fConst23 = 0.5f * fConst22;
		fConst24 = 0.3f * fConst0;
		iConst25 = static_cast<int>(fConst18);
		iConst26 = static_cast<int>(std::min<float>(fConst24, static_cast<float>(std::max<int>(0, faust_wrap_add(iConst25, 1)))));
		fConst27 = fConst17 + (-1.0f - fConst19);
		fConst28 = 0.16666667f * fConst27;
		iConst29 = static_cast<int>(std::min<float>(fConst24, static_cast<float>(std::max<int>(0, iConst25))));
		fConst30 = fConst22 * fConst27;
		fConst31 = 0.5f * fConst30;
		iConst32 = static_cast<int>(std::min<float>(fConst24, static_cast<float>(std::max<int>(0, faust_wrap_add(iConst25, 2)))));
		fConst33 = 0.16666667f * fConst30 * fConst21;
		iConst34 = static_cast<int>(std::min<float>(fConst24, static_cast<float>(std::max<int>(0, faust_wrap_add(iConst25, 3)))));
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.4f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.5f);
		fHslider2 = static_cast<FAUSTFLOAT>(0.2f);
		fHslider3 = static_cast<FAUSTFLOAT>(0.3f);
		fHslider4 = static_cast<FAUSTFLOAT>(0.6f);
	}
	
	virtual void instanceClear() {
		IOTA0 = 0;
		for (int l0 = 0; l0 < 8192; l0 = faust_wrap_add(l0, 1)) {
			fVec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			iVec1[l1] = 0;
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
			fRec8[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 2; l11 = faust_wrap_add(l11, 1)) {
			fRec9[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = faust_wrap_add(l12, 1)) {
			fRec10[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = faust_wrap_add(l13, 1)) {
			fRec11[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = faust_wrap_add(l14, 1)) {
			fRec12[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 2; l15 = faust_wrap_add(l15, 1)) {
			fVec2[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = faust_wrap_add(l16, 1)) {
			fRec13[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = faust_wrap_add(l17, 1)) {
			fRec14[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = faust_wrap_add(l18, 1)) {
			fRec15[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 2; l19 = faust_wrap_add(l19, 1)) {
			fRec16[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = faust_wrap_add(l20, 1)) {
			fRec17[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = faust_wrap_add(l21, 1)) {
			fRec18[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = faust_wrap_add(l22, 1)) {
			fRec19[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 2; l23 = faust_wrap_add(l23, 1)) {
			fRec20[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = faust_wrap_add(l24, 1)) {
			fVec3[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = faust_wrap_add(l25, 1)) {
			fRec21[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = faust_wrap_add(l26, 1)) {
			fRec22[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 2; l27 = faust_wrap_add(l27, 1)) {
			fRec23[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = faust_wrap_add(l28, 1)) {
			fRec24[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = faust_wrap_add(l29, 1)) {
			fRec25[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = faust_wrap_add(l30, 1)) {
			fVec4[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 2; l31 = faust_wrap_add(l31, 1)) {
			fRec26[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = faust_wrap_add(l32, 1)) {
			fRec27[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = faust_wrap_add(l33, 1)) {
			fRec28[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = faust_wrap_add(l34, 1)) {
			fRec29[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 2; l35 = faust_wrap_add(l35, 1)) {
			fRec30[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = faust_wrap_add(l36, 1)) {
			fRec31[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = faust_wrap_add(l37, 1)) {
			fRec32[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = faust_wrap_add(l38, 1)) {
			fVec5[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = faust_wrap_add(l39, 1)) {
			fRec33[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = faust_wrap_add(l40, 1)) {
			fVec6[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = faust_wrap_add(l41, 1)) {
			fRec34[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = faust_wrap_add(l42, 1)) {
			fRec35[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 2; l43 = faust_wrap_add(l43, 1)) {
			fRec36[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 2; l44 = faust_wrap_add(l44, 1)) {
			fRec37[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = faust_wrap_add(l45, 1)) {
			fRec38[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = faust_wrap_add(l46, 1)) {
			fRec39[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = faust_wrap_add(l47, 1)) {
			fVec7[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 2; l48 = faust_wrap_add(l48, 1)) {
			fRec40[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = faust_wrap_add(l49, 1)) {
			fVec8[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = faust_wrap_add(l50, 1)) {
			fRec41[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = faust_wrap_add(l51, 1)) {
			fRec42[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 2; l52 = faust_wrap_add(l52, 1)) {
			fRec113[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 2; l53 = faust_wrap_add(l53, 1)) {
			fRec115[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = faust_wrap_add(l54, 1)) {
			fRec114[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = faust_wrap_add(l55, 1)) {
			fRec116[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = faust_wrap_add(l56, 1)) {
			fRec117[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 65536; l57 = faust_wrap_add(l57, 1)) {
			fVec9[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = faust_wrap_add(l58, 1)) {
			fVec10[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2; l59 = faust_wrap_add(l59, 1)) {
			fRec119[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = faust_wrap_add(l60, 1)) {
			fRec118[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 1024; l61 = faust_wrap_add(l61, 1)) {
			fVec11[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 2; l62 = faust_wrap_add(l62, 1)) {
			fRec43[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 2; l63 = faust_wrap_add(l63, 1)) {
			fVec12[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2; l64 = faust_wrap_add(l64, 1)) {
			fRec44[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 2; l65 = faust_wrap_add(l65, 1)) {
			fVec13[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 2; l66 = faust_wrap_add(l66, 1)) {
			fRec45[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 2; l67 = faust_wrap_add(l67, 1)) {
			fRec46[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 2; l68 = faust_wrap_add(l68, 1)) {
			fRec120[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 131072; l69 = faust_wrap_add(l69, 1)) {
			fVec14[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 1024; l70 = faust_wrap_add(l70, 1)) {
			fVec15[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 2; l71 = faust_wrap_add(l71, 1)) {
			fRec48[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 2; l72 = faust_wrap_add(l72, 1)) {
			fRec49[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 2; l73 = faust_wrap_add(l73, 1)) {
			fVec16[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 2; l74 = faust_wrap_add(l74, 1)) {
			fRec50[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 2; l75 = faust_wrap_add(l75, 1)) {
			fVec17[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 2; l76 = faust_wrap_add(l76, 1)) {
			fRec51[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 2; l77 = faust_wrap_add(l77, 1)) {
			fRec121[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 65536; l78 = faust_wrap_add(l78, 1)) {
			fVec18[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 1024; l79 = faust_wrap_add(l79, 1)) {
			fVec19[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 2; l80 = faust_wrap_add(l80, 1)) {
			fRec52[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 2; l81 = faust_wrap_add(l81, 1)) {
			fRec53[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 131072; l82 = faust_wrap_add(l82, 1)) {
			fVec20[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 1024; l83 = faust_wrap_add(l83, 1)) {
			fVec21[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 2; l84 = faust_wrap_add(l84, 1)) {
			fRec54[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 2; l85 = faust_wrap_add(l85, 1)) {
			fVec22[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 2; l86 = faust_wrap_add(l86, 1)) {
			fRec55[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 2; l87 = faust_wrap_add(l87, 1)) {
			fRec122[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 131072; l88 = faust_wrap_add(l88, 1)) {
			fVec23[l88] = 0.0f;
		}
		for (int l89 = 0; l89 < 1024; l89 = faust_wrap_add(l89, 1)) {
			fVec24[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 2; l90 = faust_wrap_add(l90, 1)) {
			fRec56[l90] = 0.0f;
		}
		for (int l91 = 0; l91 < 2; l91 = faust_wrap_add(l91, 1)) {
			fRec57[l91] = 0.0f;
		}
		for (int l92 = 0; l92 < 2; l92 = faust_wrap_add(l92, 1)) {
			fRec123[l92] = 0.0f;
		}
		for (int l93 = 0; l93 < 131072; l93 = faust_wrap_add(l93, 1)) {
			fVec25[l93] = 0.0f;
		}
		for (int l94 = 0; l94 < 1024; l94 = faust_wrap_add(l94, 1)) {
			fVec26[l94] = 0.0f;
		}
		for (int l95 = 0; l95 < 2; l95 = faust_wrap_add(l95, 1)) {
			fVec27[l95] = 0.0f;
		}
		for (int l96 = 0; l96 < 2; l96 = faust_wrap_add(l96, 1)) {
			fRec59[l96] = 0.0f;
		}
		for (int l97 = 0; l97 < 2; l97 = faust_wrap_add(l97, 1)) {
			fVec28[l97] = 0.0f;
		}
		for (int l98 = 0; l98 < 2; l98 = faust_wrap_add(l98, 1)) {
			fRec60[l98] = 0.0f;
		}
		for (int l99 = 0; l99 < 2; l99 = faust_wrap_add(l99, 1)) {
			fVec29[l99] = 0.0f;
		}
		for (int l100 = 0; l100 < 2; l100 = faust_wrap_add(l100, 1)) {
			fRec61[l100] = 0.0f;
		}
		for (int l101 = 0; l101 < 2; l101 = faust_wrap_add(l101, 1)) {
			fVec30[l101] = 0.0f;
		}
		for (int l102 = 0; l102 < 2; l102 = faust_wrap_add(l102, 1)) {
			fRec62[l102] = 0.0f;
		}
		for (int l103 = 0; l103 < 2; l103 = faust_wrap_add(l103, 1)) {
			fRec63[l103] = 0.0f;
		}
		for (int l104 = 0; l104 < 2; l104 = faust_wrap_add(l104, 1)) {
			fRec64[l104] = 0.0f;
		}
		for (int l105 = 0; l105 < 2; l105 = faust_wrap_add(l105, 1)) {
			fRec66[l105] = 0.0f;
		}
		for (int l106 = 0; l106 < 2; l106 = faust_wrap_add(l106, 1)) {
			fRec67[l106] = 0.0f;
		}
		for (int l107 = 0; l107 < 2; l107 = faust_wrap_add(l107, 1)) {
			fRec124[l107] = 0.0f;
		}
		for (int l108 = 0; l108 < 131072; l108 = faust_wrap_add(l108, 1)) {
			fVec31[l108] = 0.0f;
		}
		for (int l109 = 0; l109 < 1024; l109 = faust_wrap_add(l109, 1)) {
			fVec32[l109] = 0.0f;
		}
		for (int l110 = 0; l110 < 2; l110 = faust_wrap_add(l110, 1)) {
			fRec69[l110] = 0.0f;
		}
		for (int l111 = 0; l111 < 65536; l111 = faust_wrap_add(l111, 1)) {
			fVec33[l111] = 0.0f;
		}
		for (int l112 = 0; l112 < 1024; l112 = faust_wrap_add(l112, 1)) {
			fVec34[l112] = 0.0f;
		}
		for (int l113 = 0; l113 < 2; l113 = faust_wrap_add(l113, 1)) {
			fRec70[l113] = 0.0f;
		}
		for (int l114 = 0; l114 < 2; l114 = faust_wrap_add(l114, 1)) {
			fRec71[l114] = 0.0f;
		}
		for (int l115 = 0; l115 < 131072; l115 = faust_wrap_add(l115, 1)) {
			fVec35[l115] = 0.0f;
		}
		for (int l116 = 0; l116 < 1024; l116 = faust_wrap_add(l116, 1)) {
			fVec36[l116] = 0.0f;
		}
		for (int l117 = 0; l117 < 2; l117 = faust_wrap_add(l117, 1)) {
			fRec73[l117] = 0.0f;
		}
		for (int l118 = 0; l118 < 131072; l118 = faust_wrap_add(l118, 1)) {
			fVec37[l118] = 0.0f;
		}
		for (int l119 = 0; l119 < 1024; l119 = faust_wrap_add(l119, 1)) {
			fVec38[l119] = 0.0f;
		}
		for (int l120 = 0; l120 < 2; l120 = faust_wrap_add(l120, 1)) {
			fRec74[l120] = 0.0f;
		}
		for (int l121 = 0; l121 < 2; l121 = faust_wrap_add(l121, 1)) {
			fRec125[l121] = 0.0f;
		}
		for (int l122 = 0; l122 < 131072; l122 = faust_wrap_add(l122, 1)) {
			fVec39[l122] = 0.0f;
		}
		for (int l123 = 0; l123 < 1024; l123 = faust_wrap_add(l123, 1)) {
			fVec40[l123] = 0.0f;
		}
		for (int l124 = 0; l124 < 2; l124 = faust_wrap_add(l124, 1)) {
			fRec77[l124] = 0.0f;
		}
		for (int l125 = 0; l125 < 2; l125 = faust_wrap_add(l125, 1)) {
			fRec78[l125] = 0.0f;
		}
		for (int l126 = 0; l126 < 2; l126 = faust_wrap_add(l126, 1)) {
			fRec79[l126] = 0.0f;
		}
		for (int l127 = 0; l127 < 2; l127 = faust_wrap_add(l127, 1)) {
			fRec80[l127] = 0.0f;
		}
		for (int l128 = 0; l128 < 2; l128 = faust_wrap_add(l128, 1)) {
			fRec84[l128] = 0.0f;
		}
		for (int l129 = 0; l129 < 2; l129 = faust_wrap_add(l129, 1)) {
			fRec126[l129] = 0.0f;
		}
		for (int l130 = 0; l130 < 131072; l130 = faust_wrap_add(l130, 1)) {
			fVec41[l130] = 0.0f;
		}
		for (int l131 = 0; l131 < 1024; l131 = faust_wrap_add(l131, 1)) {
			fVec42[l131] = 0.0f;
		}
		for (int l132 = 0; l132 < 2; l132 = faust_wrap_add(l132, 1)) {
			fRec85[l132] = 0.0f;
		}
		for (int l133 = 0; l133 < 2; l133 = faust_wrap_add(l133, 1)) {
			fRec86[l133] = 0.0f;
		}
		for (int l134 = 0; l134 < 131072; l134 = faust_wrap_add(l134, 1)) {
			fVec43[l134] = 0.0f;
		}
		for (int l135 = 0; l135 < 1024; l135 = faust_wrap_add(l135, 1)) {
			fVec44[l135] = 0.0f;
		}
		for (int l136 = 0; l136 < 2; l136 = faust_wrap_add(l136, 1)) {
			fRec88[l136] = 0.0f;
		}
		for (int l137 = 0; l137 < 131072; l137 = faust_wrap_add(l137, 1)) {
			fVec45[l137] = 0.0f;
		}
		for (int l138 = 0; l138 < 1024; l138 = faust_wrap_add(l138, 1)) {
			fVec46[l138] = 0.0f;
		}
		for (int l139 = 0; l139 < 131072; l139 = faust_wrap_add(l139, 1)) {
			fVec47[l139] = 0.0f;
		}
		for (int l140 = 0; l140 < 1024; l140 = faust_wrap_add(l140, 1)) {
			fVec48[l140] = 0.0f;
		}
		for (int l141 = 0; l141 < 2; l141 = faust_wrap_add(l141, 1)) {
			fRec91[l141] = 0.0f;
		}
		for (int l142 = 0; l142 < 65536; l142 = faust_wrap_add(l142, 1)) {
			fVec49[l142] = 0.0f;
		}
		for (int l143 = 0; l143 < 1024; l143 = faust_wrap_add(l143, 1)) {
			fVec50[l143] = 0.0f;
		}
		for (int l144 = 0; l144 < 2; l144 = faust_wrap_add(l144, 1)) {
			fRec93[l144] = 0.0f;
		}
		for (int l145 = 0; l145 < 2; l145 = faust_wrap_add(l145, 1)) {
			fRec94[l145] = 0.0f;
		}
		for (int l146 = 0; l146 < 3; l146 = faust_wrap_add(l146, 1)) {
			fRec97[l146] = 0.0f;
		}
		for (int l147 = 0; l147 < 3; l147 = faust_wrap_add(l147, 1)) {
			fRec98[l147] = 0.0f;
		}
		for (int l148 = 0; l148 < 3; l148 = faust_wrap_add(l148, 1)) {
			fRec99[l148] = 0.0f;
		}
		for (int l149 = 0; l149 < 3; l149 = faust_wrap_add(l149, 1)) {
			fRec100[l149] = 0.0f;
		}
		for (int l150 = 0; l150 < 3; l150 = faust_wrap_add(l150, 1)) {
			fRec101[l150] = 0.0f;
		}
		for (int l151 = 0; l151 < 3; l151 = faust_wrap_add(l151, 1)) {
			fRec102[l151] = 0.0f;
		}
		for (int l152 = 0; l152 < 3; l152 = faust_wrap_add(l152, 1)) {
			fRec103[l152] = 0.0f;
		}
		for (int l153 = 0; l153 < 3; l153 = faust_wrap_add(l153, 1)) {
			fRec104[l153] = 0.0f;
		}
		for (int l154 = 0; l154 < 3; l154 = faust_wrap_add(l154, 1)) {
			fRec105[l154] = 0.0f;
		}
		for (int l155 = 0; l155 < 3; l155 = faust_wrap_add(l155, 1)) {
			fRec106[l155] = 0.0f;
		}
		for (int l156 = 0; l156 < 3; l156 = faust_wrap_add(l156, 1)) {
			fRec107[l156] = 0.0f;
		}
		for (int l157 = 0; l157 < 3; l157 = faust_wrap_add(l157, 1)) {
			fRec108[l157] = 0.0f;
		}
		for (int l158 = 0; l158 < 3; l158 = faust_wrap_add(l158, 1)) {
			fRec109[l158] = 0.0f;
		}
		for (int l159 = 0; l159 < 3; l159 = faust_wrap_add(l159, 1)) {
			fRec110[l159] = 0.0f;
		}
		for (int l160 = 0; l160 < 3; l160 = faust_wrap_add(l160, 1)) {
			fRec111[l160] = 0.0f;
		}
		for (int l161 = 0; l161 < 3; l161 = faust_wrap_add(l161, 1)) {
			fRec112[l161] = 0.0f;
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
	
	virtual T_vital_rev* clone() {
		return new T_vital_rev(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("vital-rev");
		ui_interface->addHorizontalSlider("chorus_amt", &fHslider3, FAUSTFLOAT(0.3f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("chorus_freq", &fHslider2, FAUSTFLOAT(0.2f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("mix", &fHslider0, FAUSTFLOAT(0.4f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("size", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("time", &fHslider4, FAUSTFLOAT(0.6f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst1 * static_cast<float>(fHslider0);
		fSlow1 = fConst1 * static_cast<float>(fHslider1);
		fSlow2 = fConst1 * static_cast<float>(fHslider2);
		fSlow3 = fConst1 * static_cast<float>(fHslider3);
		fSlow4 = fConst1 * static_cast<float>(fHslider4);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		float fTemp0 = static_cast<float>(inputs[0]);
		fVec0[IOTA0 & 8191] = fTemp0;
		iVec1[0] = 1;
		fRec0[0] = fSlow0 + fConst2 * fRec0[1];
		fRec1[0] = -(fConst4 * (fConst5 * fRec1[1] - fConst3 * (fRec97[1] - fRec97[2])));
		fRec2[0] = -(fConst4 * (fConst5 * fRec2[1] - fConst3 * (fRec112[1] - fRec112[2])));
		fRec3[0] = -(fConst4 * (fConst5 * fRec3[1] - fConst3 * (fRec102[1] - fRec102[2])));
		fRec4[0] = -(fConst4 * (fConst5 * fRec4[1] - fConst3 * (fRec99[1] - fRec99[2])));
		fRec5[0] = -(fConst4 * (fConst5 * fRec5[1] - (fRec111[1] + fRec111[2])));
		fRec6[0] = -(fConst4 * (fConst5 * fRec6[1] - (fRec104[1] + fRec104[2])));
		fRec7[0] = -(fConst4 * (fConst5 * fRec7[1] - (fRec107[1] + fRec107[2])));
		fRec8[0] = -(fConst4 * (fConst5 * fRec8[1] - fConst3 * (fRec100[1] - fRec100[2])));
		fRec9[0] = -(fConst4 * (fConst5 * fRec9[1] - (fRec98[1] + fRec98[2])));
		fRec10[0] = -(fConst4 * (fConst5 * fRec10[1] - (fRec100[1] + fRec100[2])));
		fRec11[0] = -(fConst4 * (fConst5 * fRec11[1] - (fRec99[1] + fRec99[2])));
		fRec12[0] = -(fConst4 * (fConst5 * fRec12[1] - (fRec108[1] + fRec108[2])));
		float fTemp1 = fRec8[0] + 0.19054607f * fRec10[0];
		fVec2[0] = fTemp1;
		fRec13[0] = -(fConst7 * (fConst8 * fRec13[1] - (fTemp1 + fVec2[1])));
		fRec14[0] = -(fConst4 * (fConst5 * fRec14[1] - fConst3 * (fRec98[1] - fRec98[2])));
		fRec15[0] = -(fConst4 * (fConst5 * fRec15[1] - (fRec110[1] + fRec110[2])));
		fRec16[0] = -(fConst4 * (fConst5 * fRec16[1] - fConst3 * (fRec107[1] - fRec107[2])));
		fRec17[0] = -(fConst4 * (fConst5 * fRec17[1] - fConst3 * (fRec111[1] - fRec111[2])));
		fRec18[0] = -(fConst4 * (fConst5 * fRec18[1] - fConst3 * (fRec110[1] - fRec110[2])));
		fRec19[0] = -(fConst4 * (fConst5 * fRec19[1] - (fRec112[1] + fRec112[2])));
		fRec20[0] = -(fConst4 * (fConst5 * fRec20[1] - (fRec105[1] + fRec105[2])));
		float fTemp2 = fRec16[0] + 0.19054607f * fRec7[0];
		fVec3[0] = fTemp2;
		fRec21[0] = -(fConst7 * (fConst8 * fRec21[1] - fConst6 * (fTemp2 - fVec3[1])));
		fRec22[0] = -(fConst4 * (fConst5 * fRec22[1] - fConst3 * (fRec106[1] - fRec106[2])));
		fRec23[0] = -(fConst4 * (fConst5 * fRec23[1] - fConst3 * (fRec109[1] - fRec109[2])));
		fRec24[0] = -(fConst4 * (fConst5 * fRec24[1] - fConst3 * (fRec103[1] - fRec103[2])));
		fRec25[0] = -(fConst7 * (fConst8 * fRec25[1] - fConst6 * (fTemp1 - fVec2[1])));
		float fTemp3 = fRec18[0] + 0.19054607f * fRec15[0];
		fVec4[0] = fTemp3;
		fRec26[0] = -(fConst7 * (fConst8 * fRec26[1] - fConst6 * (fTemp3 - fVec4[1])));
		fRec27[0] = -(fConst4 * (fConst5 * fRec27[1] - fConst3 * (fRec105[1] - fRec105[2])));
		fRec28[0] = -(fConst4 * (fConst5 * fRec28[1] - (fRec103[1] + fRec103[2])));
		fRec29[0] = -(fConst4 * (fConst5 * fRec29[1] - (fRec109[1] + fRec109[2])));
		fRec30[0] = -(fConst4 * (fConst5 * fRec30[1] - fConst3 * (fRec108[1] - fRec108[2])));
		fRec31[0] = -(fConst4 * (fConst5 * fRec31[1] - (fRec106[1] + fRec106[2])));
		fRec32[0] = -(fConst4 * (fConst5 * fRec32[1] - fConst3 * (fRec101[1] - fRec101[2])));
		float fTemp4 = fRec4[0] + 0.19054607f * fRec11[0];
		fVec5[0] = fTemp4;
		fRec33[0] = -(fConst7 * (fConst8 * fRec33[1] - (fTemp4 + fVec5[1])));
		float fTemp5 = fRec17[0] + 0.19054607f * fRec5[0];
		fVec6[0] = fTemp5;
		fRec34[0] = -(fConst7 * (fConst8 * fRec34[1] - (fTemp5 + fVec6[1])));
		fRec35[0] = -(fConst4 * (fConst5 * fRec35[1] - (fRec101[1] + fRec101[2])));
		fRec36[0] = -(fConst7 * (fConst8 * fRec36[1] - fConst6 * (fTemp4 - fVec5[1])));
		fRec37[0] = -(fConst4 * (fConst5 * fRec37[1] - (fRec102[1] + fRec102[2])));
		fRec38[0] = -(fConst4 * (fConst5 * fRec38[1] - (fRec97[1] + fRec97[2])));
		fRec39[0] = -(fConst4 * (fConst5 * fRec39[1] - fConst3 * (fRec104[1] - fRec104[2])));
		float fTemp6 = fRec2[0] + 0.19054607f * fRec19[0];
		fVec7[0] = fTemp6;
		fRec40[0] = -(fConst7 * (fConst8 * fRec40[1] - (fTemp6 + fVec7[1])));
		float fTemp7 = fRec1[0] + 0.19054607f * fRec38[0];
		fVec8[0] = fTemp7;
		fRec41[0] = -(fConst7 * (fConst8 * fRec41[1] - fConst6 * (fTemp7 - fVec8[1])));
		fRec42[0] = -(fConst7 * (fConst8 * fRec42[1] - fConst6 * (fTemp6 - fVec7[1])));
		fRec113[0] = fSlow1 + fConst2 * fRec113[1];
		float fTemp8 = std::pow(2.0f, -3.0f + 4.0f * std::max<float>(0.0f, std::min<float>(1.0f, fRec113[0])));
		int iTemp9 = faust_wrap_sub(1, iVec1[1]);
		fRec115[0] = fSlow2 + fConst2 * fRec115[1];
		float fTemp10 = fConst9 * std::min<float>(16.0f, std::exp(-8.0f + 11.0f * fRec115[0]));
		float fTemp11 = ((iTemp9) ? 0.0f : fRec114[1] + fTemp10);
		fRec114[0] = fTemp11 - std::floor(fTemp11);
		fRec116[0] = fSlow3 + fConst2 * fRec116[1];
		float fTemp12 = T_vital_rev_faustpower2_f(std::max<float>(0.0f, std::min<float>(1.0f, fRec116[0])));
		float fTemp13 = 0.05668934f * std::sin(6.2831855f * fRec114[0]) * fTemp12;
		float fTemp14 = std::max<float>(1.0f, fConst0 * fTemp8 * (0.070764855f + fTemp13));
		float fTemp15 = -0.999995f + fTemp14;
		float fTemp16 = std::floor(fTemp15);
		float fTemp17 = -2.0f - fTemp16 + fTemp14;
		float fTemp18 = fTemp14 - fTemp16;
		fRec117[0] = fSlow4 + fConst2 * fRec117[1];
		float fTemp19 = fTemp8 / std::max<float>(0.1f, std::min<float>(1e+02f, std::exp(-6.0f + 12.0f * fRec117[0])));
		float fTemp20 = (0.33113113f * fRec42[0] + fRec40[0]) * std::pow(0.001f, 0.070764855f * fTemp19);
		fVec9[IOTA0 & 65535] = fTemp20;
		int iTemp21 = static_cast<int>(fTemp15);
		float fTemp22 = -1.0f - fTemp16 + fTemp14;
		float fTemp23 = fTemp18 * fTemp22;
		float fTemp24 = fConst20 * (fConst21 * (fConst23 * fVec0[(faust_wrap_sub(IOTA0, iConst26)) & 8191] - fConst28 * fVec0[(faust_wrap_sub(IOTA0, iConst29)) & 8191]) - fConst31 * fVec0[(faust_wrap_sub(IOTA0, iConst32)) & 8191]) + fConst33 * fVec0[(faust_wrap_sub(IOTA0, iConst34)) & 8191];
		fVec10[0] = fTemp24;
		fRec119[0] = -(fConst15 * (fConst16 * fRec119[1] - fConst14 * (fTemp24 - fVec10[1])));
		fRec118[0] = -(fConst12 * (fConst13 * fRec118[1] - (fRec119[1] + fRec119[0])));
		float fTemp25 = 0.25f * fRec118[0];
		float fTemp26 = (-3.0f - fTemp16 + fTemp14) * (fTemp17 * (0.5f * fTemp18 * fVec9[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp21))))))) & 65535] - 0.16666667f * fVec9[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, iTemp21)))))) & 65535] * fTemp22) - 0.5f * fTemp23 * fVec9[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp21))))))) & 65535]) + 0.16666667f * fTemp23 * fTemp17 * fVec9[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp21))))))) & 65535] + fTemp25 - 0.6f * fRec43[1];
		fVec11[IOTA0 & 1023] = fTemp26;
		fRec43[0] = fVec11[(faust_wrap_sub(IOTA0, 956)) & 1023];
		float fTemp27 = fRec27[0] + 0.19054607f * fRec20[0];
		fVec12[0] = fTemp27;
		fRec44[0] = -(fConst7 * (fConst8 * fRec44[1] - fConst6 * (fTemp27 - fVec12[1])));
		float fTemp28 = fRec14[0] + 0.19054607f * fRec9[0];
		fVec13[0] = fTemp28;
		fRec45[0] = -(fConst7 * (fConst8 * fRec45[1] - fConst6 * (fTemp28 - fVec13[1])));
		fRec46[0] = -(fConst7 * (fConst8 * fRec46[1] - fConst6 * (fTemp5 - fVec6[1])));
		float fTemp29 = ((iTemp9) ? 0.3125f : fTemp10 + fRec120[1]);
		fRec120[0] = fTemp29 - std::floor(fTemp29);
		float fTemp30 = 0.05668934f * fTemp12 * std::sin(6.2831855f * fRec120[0]);
		float fTemp31 = std::max<float>(1.0f, fConst0 * fTemp8 * (0.125f - fTemp30));
		float fTemp32 = -0.999995f + fTemp31;
		float fTemp33 = std::floor(fTemp32);
		float fTemp34 = fTemp31 - fTemp33;
		float fTemp35 = -1.0f - fTemp33 + fTemp31;
		float fTemp36 = fTemp34 * fTemp35;
		float fTemp37 = -2.0f - fTemp33 + fTemp31;
		float fTemp38 = (0.33113113f * fRec46[0] + fRec34[0]) * std::pow(0.001f, 0.125f * fTemp19);
		fVec14[IOTA0 & 131071] = fTemp38;
		int iTemp39 = static_cast<int>(fTemp32);
		float fTemp40 = 0.16666667f * fTemp36 * fTemp37 * fVec14[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp39))))))) & 131071] + fTemp25 + (-3.0f - fTemp33 + fTemp31) * (fTemp37 * (0.5f * fTemp34 * fVec14[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp39))))))) & 131071] - 0.16666667f * fVec14[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, iTemp39)))))) & 131071] * fTemp35) - 0.5f * fTemp36 * fVec14[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp39))))))) & 131071]) - 0.6f * fRec84[1];
		fVec15[IOTA0 & 1023] = fTemp40;
		float fRec47 = 0.6f * fTemp40;
		fRec48[0] = -(fConst7 * (fConst8 * fRec48[1] - (fTemp3 + fVec4[1])));
		fRec49[0] = -(fConst7 * (fConst8 * fRec49[1] - (fTemp28 + fVec13[1])));
		float fTemp41 = fRec23[0] + 0.19054607f * fRec29[0];
		fVec16[0] = fTemp41;
		fRec50[0] = -(fConst7 * (fConst8 * fRec50[1] - fConst6 * (fTemp41 - fVec16[1])));
		float fTemp42 = fRec22[0] + 0.19054607f * fRec31[0];
		fVec17[0] = fTemp42;
		fRec51[0] = -(fConst7 * (fConst8 * fRec51[1] - (fTemp42 + fVec17[1])));
		float fTemp43 = ((iTemp9) ? 0.0625f : fTemp10 + fRec121[1]);
		fRec121[0] = fTemp43 - std::floor(fTemp43);
		float fTemp44 = 0.05668934f * fTemp12 * std::sin(6.2831855f * fRec121[0]);
		float fTemp45 = std::max<float>(1.0f, fConst0 * fTemp8 * (0.07776644f + fTemp44));
		float fTemp46 = -0.999995f + fTemp45;
		float fTemp47 = std::floor(fTemp46);
		float fTemp48 = fTemp45 - fTemp47;
		float fTemp49 = -1.0f - fTemp47 + fTemp45;
		float fTemp50 = fTemp48 * fTemp49;
		float fTemp51 = -2.0f - fTemp47 + fTemp45;
		float fTemp52 = (0.33113113f * fRec36[0] + fRec33[0]) * std::pow(0.001f, 0.07776644f * fTemp19);
		fVec18[IOTA0 & 65535] = fTemp52;
		int iTemp53 = static_cast<int>(fTemp46);
		float fTemp54 = 0.16666667f * fTemp50 * fTemp51 * fVec18[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp53))))))) & 65535] + fTemp25 + (-3.0f - fTemp47 + fTemp45) * (fTemp51 * (0.5f * fTemp48 * fVec18[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp53))))))) & 65535] - 0.16666667f * fVec18[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, iTemp53)))))) & 65535] * fTemp49) - 0.5f * fTemp50 * fVec18[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp53))))))) & 65535]) - 0.6f * fRec52[1];
		fVec19[IOTA0 & 1023] = fTemp54;
		fRec52[0] = fVec19[(faust_wrap_sub(IOTA0, 1001)) & 1023];
		fRec53[0] = -(fConst7 * (fConst8 * fRec53[1] - (fTemp2 + fVec3[1])));
		float fTemp55 = std::max<float>(1.0f, fConst0 * fTemp8 * (0.21039456f + fTemp30));
		float fTemp56 = -0.999995f + fTemp55;
		float fTemp57 = std::floor(fTemp56);
		float fTemp58 = fTemp55 - fTemp57;
		float fTemp59 = -1.0f - fTemp57 + fTemp55;
		float fTemp60 = fTemp58 * fTemp59;
		float fTemp61 = -2.0f - fTemp57 + fTemp55;
		float fTemp62 = (0.33113113f * fRec25[0] + fRec13[0]) * std::pow(0.001f, 0.21039456f * fTemp19);
		fVec20[IOTA0 & 131071] = fTemp62;
		int iTemp63 = static_cast<int>(fTemp56);
		float fTemp64 = 0.16666667f * fTemp60 * fTemp61 * fVec20[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp63))))))) & 131071] + fTemp25 + (-3.0f - fTemp57 + fTemp55) * (fTemp61 * (0.5f * fTemp58 * fVec20[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp63))))))) & 131071] - 0.16666667f * fVec20[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, iTemp63)))))) & 131071] * fTemp59) - 0.5f * fTemp60 * fVec20[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp63))))))) & 131071]) - 0.6f * fRec54[1];
		fVec21[IOTA0 & 1023] = fTemp64;
		fRec54[0] = fVec21[(faust_wrap_sub(IOTA0, 798)) & 1023];
		float fTemp65 = fRec32[0] + 0.19054607f * fRec35[0];
		fVec22[0] = fTemp65;
		fRec55[0] = -(fConst7 * (fConst8 * fRec55[1] - (fTemp65 + fVec22[1])));
		float fTemp66 = ((iTemp9) ? 0.25f : fTemp10 + fRec122[1]);
		fRec122[0] = fTemp66 - std::floor(fTemp66);
		float fTemp67 = 0.05668934f * fTemp12 * std::sin(6.2831855f * fRec122[0]);
		float fTemp68 = std::max<float>(1.0f, fConst0 * fTemp8 * (0.21998005f - fTemp67));
		float fTemp69 = -0.999995f + fTemp68;
		float fTemp70 = std::floor(fTemp69);
		float fTemp71 = fTemp68 - fTemp70;
		float fTemp72 = -1.0f - fTemp70 + fTemp68;
		float fTemp73 = fTemp71 * fTemp72;
		float fTemp74 = -2.0f - fTemp70 + fTemp68;
		float fTemp75 = (0.33113113f * fRec26[0] + fRec48[0]) * std::pow(0.001f, 0.21998005f * fTemp19);
		fVec23[IOTA0 & 131071] = fTemp75;
		int iTemp76 = static_cast<int>(fTemp69);
		float fTemp77 = 0.16666667f * fTemp73 * fTemp74 * fVec23[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp76))))))) & 131071] + fTemp25 + (-3.0f - fTemp70 + fTemp68) * (fTemp74 * (0.5f * fTemp71 * fVec23[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp76))))))) & 131071] - 0.16666667f * fVec23[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, iTemp76)))))) & 131071] * fTemp72) - 0.5f * fTemp73 * fVec23[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp76))))))) & 131071]) - 0.6f * fRec56[1];
		fVec24[IOTA0 & 1023] = fTemp77;
		fRec56[0] = fVec24[(faust_wrap_sub(IOTA0, 894)) & 1023];
		fRec57[0] = -(fConst7 * (fConst8 * fRec57[1] - (fTemp27 + fVec12[1])));
		float fTemp78 = ((iTemp9) ? 0.1875f : fTemp10 + fRec123[1]);
		fRec123[0] = fTemp78 - std::floor(fTemp78);
		float fTemp79 = 0.05668934f * fTemp12 * std::sin(6.2831855f * fRec123[0]);
		float fTemp80 = std::max<float>(1.0f, fConst0 * fTemp8 * (0.12767006f - fTemp79));
		float fTemp81 = -0.999995f + fTemp80;
		float fTemp82 = std::floor(fTemp81);
		float fTemp83 = fTemp80 - fTemp82;
		float fTemp84 = -1.0f - fTemp82 + fTemp80;
		float fTemp85 = fTemp83 * fTemp84;
		float fTemp86 = -2.0f - fTemp82 + fTemp80;
		float fTemp87 = (0.33113113f * fRec44[0] + fRec57[0]) * std::pow(0.001f, 0.12767006f * fTemp19);
		fVec25[IOTA0 & 131071] = fTemp87;
		int iTemp88 = static_cast<int>(fTemp81);
		float fTemp89 = 0.16666667f * fTemp85 * fTemp86 * fVec25[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp88))))))) & 131071] + fTemp25 + (-3.0f - fTemp82 + fTemp80) * (fTemp86 * (0.5f * fTemp83 * fVec25[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp88))))))) & 131071] - 0.16666667f * fVec25[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, iTemp88)))))) & 131071] * fTemp84) - 0.5f * fTemp85 * fVec25[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp88))))))) & 131071]) - 0.6f * fRec63[1];
		fVec26[IOTA0 & 1023] = fTemp89;
		float fRec58 = 0.6f * fTemp89;
		float fTemp90 = fRec39[0] + 0.19054607f * fRec6[0];
		fVec27[0] = fTemp90;
		fRec59[0] = -(fConst7 * (fConst8 * fRec59[1] - fConst6 * (fTemp90 - fVec27[1])));
		float fTemp91 = fRec3[0] + 0.19054607f * fRec37[0];
		fVec28[0] = fTemp91;
		fRec60[0] = -(fConst7 * (fConst8 * fRec60[1] - (fTemp91 + fVec28[1])));
		float fTemp92 = fRec30[0] + 0.19054607f * fRec12[0];
		fVec29[0] = fTemp92;
		fRec61[0] = -(fConst7 * (fConst8 * fRec61[1] - fConst6 * (fTemp92 - fVec29[1])));
		float fTemp93 = fRec24[0] + 0.19054607f * fRec28[0];
		fVec30[0] = fTemp93;
		fRec62[0] = -(fConst7 * (fConst8 * fRec62[1] - (fTemp93 + fVec30[1])));
		fRec63[0] = fVec26[(faust_wrap_sub(IOTA0, 996)) & 1023];
		fRec64[0] = -(fConst7 * (fConst8 * fRec64[1] - (fTemp41 + fVec16[1])));
		float fRec65 = 0.6f * fTemp64;
		fRec66[0] = -(fConst7 * (fConst8 * fRec66[1] - fConst6 * (fTemp93 - fVec30[1])));
		fRec67[0] = -(fConst7 * (fConst8 * fRec67[1] - (fTemp92 + fVec29[1])));
		float fTemp94 = ((iTemp9) ? 0.125f : fTemp10 + fRec124[1]);
		fRec124[0] = fTemp94 - std::floor(fTemp94);
		float fTemp95 = 0.05668934f * fTemp12 * std::sin(6.2831855f * fRec124[0]);
		float fTemp96 = std::max<float>(1.0f, fConst0 * fTemp8 * (0.11940046f - fTemp95));
		float fTemp97 = -0.999995f + fTemp96;
		float fTemp98 = std::floor(fTemp97);
		float fTemp99 = fTemp96 - fTemp98;
		float fTemp100 = -1.0f - fTemp98 + fTemp96;
		float fTemp101 = fTemp99 * fTemp100;
		float fTemp102 = -2.0f - fTemp98 + fTemp96;
		float fTemp103 = (0.33113113f * fRec66[0] + fRec62[0]) * std::pow(0.001f, 0.11940046f * fTemp19);
		fVec31[IOTA0 & 131071] = fTemp103;
		int iTemp104 = static_cast<int>(fTemp97);
		float fTemp105 = 0.16666667f * fTemp101 * fTemp102 * fVec31[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp104))))))) & 131071] + fTemp25 + (-3.0f - fTemp98 + fTemp96) * (fTemp102 * (0.5f * fTemp99 * fVec31[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp104))))))) & 131071] - 0.16666667f * fVec31[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, iTemp104)))))) & 131071] * fTemp100) - 0.5f * fTemp101 * fVec31[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp104))))))) & 131071]) - 0.6f * fRec86[1];
		fVec32[IOTA0 & 1023] = fTemp105;
		float fRec68 = 0.6f * fTemp105;
		fRec69[0] = -(fConst7 * (fConst8 * fRec69[1] - (fTemp7 + fVec8[1])));
		float fTemp106 = std::max<float>(1.0f, fConst0 * fTemp8 * (0.08223061f + fTemp95));
		float fTemp107 = -0.999995f + fTemp106;
		float fTemp108 = std::floor(fTemp107);
		float fTemp109 = fTemp106 - fTemp108;
		float fTemp110 = -1.0f - fTemp108 + fTemp106;
		float fTemp111 = fTemp109 * fTemp110;
		float fTemp112 = -2.0f - fTemp108 + fTemp106;
		float fTemp113 = (0.33113113f * fRec21[0] + fRec53[0]) * std::pow(0.001f, 0.08223061f * fTemp19);
		fVec33[IOTA0 & 65535] = fTemp113;
		int iTemp114 = static_cast<int>(fTemp107);
		float fTemp115 = 0.16666667f * fTemp111 * fTemp112 * fVec33[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp114))))))) & 65535] + fTemp25 + (-3.0f - fTemp108 + fTemp106) * (fTemp112 * (0.5f * fTemp109 * fVec33[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp114))))))) & 65535] - 0.16666667f * fVec33[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, iTemp114)))))) & 65535] * fTemp110) - 0.5f * fTemp111 * fVec33[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp114))))))) & 65535]) - 0.6f * fRec70[1];
		fVec34[IOTA0 & 1023] = fTemp115;
		fRec70[0] = fVec34[(faust_wrap_sub(IOTA0, 710)) & 1023];
		fRec71[0] = -(fConst7 * (fConst8 * fRec71[1] - fConst6 * (fTemp65 - fVec22[1])));
		float fTemp116 = std::max<float>(1.0f, fConst0 * fTemp8 * (0.14782245f - fTemp44));
		float fTemp117 = -0.999995f + fTemp116;
		float fTemp118 = std::floor(fTemp117);
		float fTemp119 = fTemp116 - fTemp118;
		float fTemp120 = -1.0f - fTemp118 + fTemp116;
		float fTemp121 = fTemp119 * fTemp120;
		float fTemp122 = -2.0f - fTemp118 + fTemp116;
		float fTemp123 = (0.33113113f * fRec45[0] + fRec49[0]) * std::pow(0.001f, 0.14782245f * fTemp19);
		fVec35[IOTA0 & 131071] = fTemp123;
		int iTemp124 = static_cast<int>(fTemp117);
		float fTemp125 = 0.16666667f * fTemp121 * fTemp122 * fVec35[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp124))))))) & 131071] + fTemp25 + (-3.0f - fTemp118 + fTemp116) * (fTemp122 * (0.5f * fTemp119 * fVec35[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp124))))))) & 131071] - 0.16666667f * fVec35[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, iTemp124)))))) & 131071] * fTemp120) - 0.5f * fTemp121 * fVec35[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp124))))))) & 131071]) - 0.6f * fRec79[1];
		fVec36[IOTA0 & 1023] = fTemp125;
		float fRec72 = 0.6f * fTemp125;
		fRec73[0] = -(fConst7 * (fConst8 * fRec73[1] - fConst6 * (fTemp42 - fVec17[1])));
		float fTemp126 = std::max<float>(1.0f, fConst0 * fTemp8 * (0.17490976f + fTemp79));
		float fTemp127 = -0.999995f + fTemp126;
		float fTemp128 = std::floor(fTemp127);
		float fTemp129 = fTemp126 - fTemp128;
		float fTemp130 = -1.0f - fTemp128 + fTemp126;
		float fTemp131 = fTemp129 * fTemp130;
		float fTemp132 = -2.0f - fTemp128 + fTemp126;
		float fTemp133 = (0.33113113f * fRec71[0] + fRec55[0]) * std::pow(0.001f, 0.17490976f * fTemp19);
		fVec37[IOTA0 & 131071] = fTemp133;
		int iTemp134 = static_cast<int>(fTemp127);
		float fTemp135 = 0.16666667f * fTemp131 * fTemp132 * fVec37[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp134))))))) & 131071] + fTemp25 + (-3.0f - fTemp128 + fTemp126) * (fTemp132 * (0.5f * fTemp129 * fVec37[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp134))))))) & 131071] - 0.16666667f * fVec37[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, iTemp134)))))) & 131071] * fTemp130) - 0.5f * fTemp131 * fVec37[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp134))))))) & 131071]) - 0.6f * fRec74[1];
		fVec38[IOTA0 & 1023] = fTemp135;
		fRec74[0] = fVec38[(faust_wrap_sub(IOTA0, 566)) & 1023];
		float fRec75 = 0.6f * fTemp26;
		float fTemp136 = ((iTemp9) ? 0.375f : fTemp10 + fRec125[1]);
		fRec125[0] = fTemp136 - std::floor(fTemp136);
		float fTemp137 = 0.05668934f * fTemp12 * std::sin(6.2831855f * fRec125[0]);
		float fTemp138 = std::max<float>(1.0f, fConst0 * fTemp8 * (0.17470522f + fTemp137));
		float fTemp139 = -0.999995f + fTemp138;
		float fTemp140 = std::floor(fTemp139);
		float fTemp141 = fTemp138 - fTemp140;
		float fTemp142 = -1.0f - fTemp140 + fTemp138;
		float fTemp143 = fTemp141 * fTemp142;
		float fTemp144 = -2.0f - fTemp140 + fTemp138;
		float fTemp145 = (0.33113113f * fRec61[0] + fRec67[0]) * std::pow(0.001f, 0.17470522f * fTemp19);
		fVec39[IOTA0 & 131071] = fTemp145;
		int iTemp146 = static_cast<int>(fTemp139);
		float fTemp147 = 0.16666667f * fTemp143 * fTemp144 * fVec39[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp146))))))) & 131071] + fTemp25 + (-3.0f - fTemp140 + fTemp138) * (fTemp144 * (0.5f * fTemp141 * fVec39[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp146))))))) & 131071] - 0.16666667f * fVec39[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, iTemp146)))))) & 131071] * fTemp142) - 0.5f * fTemp143 * fVec39[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp146))))))) & 131071]) - 0.6f * fRec80[1];
		fVec40[IOTA0 & 1023] = fTemp147;
		float fRec76 = 0.6f * fTemp147;
		fRec77[0] = -(fConst7 * (fConst8 * fRec77[1] - fConst6 * (fTemp91 - fVec28[1])));
		fRec78[0] = -(fConst7 * (fConst8 * fRec78[1] - (fTemp90 + fVec27[1])));
		fRec79[0] = fVec36[(faust_wrap_sub(IOTA0, 778)) & 1023];
		fRec80[0] = fVec40[(faust_wrap_sub(IOTA0, 932)) & 1023];
		float fRec81 = 0.6f * fTemp54;
		float fRec82 = 0.6f * fTemp77;
		float fRec83 = 0.6f * fTemp135;
		fRec84[0] = fVec15[(faust_wrap_sub(IOTA0, 806)) & 1023];
		float fTemp148 = ((iTemp9) ? 0.4375f : fTemp10 + fRec126[1]);
		fRec126[0] = fTemp148 - std::floor(fTemp148);
		float fTemp149 = 0.05668934f * fTemp12 * std::sin(6.2831855f * fRec126[0]);
		float fTemp150 = std::max<float>(1.0f, fConst0 * fTemp8 * (0.12786055f - fTemp149));
		float fTemp151 = -0.999995f + fTemp150;
		float fTemp152 = std::floor(fTemp151);
		float fTemp153 = fTemp150 - fTemp152;
		float fTemp154 = -1.0f - fTemp152 + fTemp150;
		float fTemp155 = fTemp153 * fTemp154;
		float fTemp156 = -2.0f - fTemp152 + fTemp150;
		float fTemp157 = (0.33113113f * fRec73[0] + fRec51[0]) * std::pow(0.001f, 0.12786055f * fTemp19);
		fVec41[IOTA0 & 131071] = fTemp157;
		int iTemp158 = static_cast<int>(fTemp151);
		float fTemp159 = 0.16666667f * fTemp155 * fTemp156 * fVec41[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp158))))))) & 131071] + fTemp25 + (-3.0f - fTemp152 + fTemp150) * (fTemp156 * (0.5f * fTemp153 * fVec41[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp158))))))) & 131071] - 0.16666667f * fVec41[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, iTemp158)))))) & 131071] * fTemp154) - 0.5f * fTemp155 * fVec41[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp158))))))) & 131071]) - 0.6f * fRec85[1];
		fVec42[IOTA0 & 1023] = fTemp159;
		fRec85[0] = fVec42[(faust_wrap_sub(IOTA0, 852)) & 1023];
		fRec86[0] = fVec32[(faust_wrap_sub(IOTA0, 662)) & 1023];
		float fRec87 = 0.6f * fTemp115;
		float fTemp160 = std::max<float>(1.0f, fConst0 * fTemp8 * (0.15313378f + fTemp67));
		float fTemp161 = -0.999995f + fTemp160;
		float fTemp162 = std::floor(fTemp161);
		float fTemp163 = fTemp160 - fTemp162;
		float fTemp164 = -1.0f - fTemp162 + fTemp160;
		float fTemp165 = fTemp163 * fTemp164;
		float fTemp166 = -2.0f - fTemp162 + fTemp160;
		float fTemp167 = (0.33113113f * fRec77[0] + fRec60[0]) * std::pow(0.001f, 0.15313378f * fTemp19);
		fVec43[IOTA0 & 131071] = fTemp167;
		int iTemp168 = static_cast<int>(fTemp161);
		float fTemp169 = 0.16666667f * fTemp165 * fTemp166 * fVec43[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp168))))))) & 131071] + fTemp25 + (-3.0f - fTemp162 + fTemp160) * (fTemp166 * (0.5f * fTemp163 * fVec43[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp168))))))) & 131071] - 0.16666667f * fVec43[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, iTemp168)))))) & 131071] * fTemp164) - 0.5f * fTemp165 * fVec43[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp168))))))) & 131071]) - 0.6f * fRec88[1];
		fVec44[IOTA0 & 1023] = fTemp169;
		fRec88[0] = fVec44[(faust_wrap_sub(IOTA0, 1000)) & 1023];
		float fTemp170 = std::max<float>(1.0f, fConst0 * fTemp8 * (0.25688207f + fTemp149));
		float fTemp171 = -0.999995f + fTemp170;
		float fTemp172 = std::floor(fTemp171);
		float fTemp173 = fTemp170 - fTemp172;
		float fTemp174 = -1.0f - fTemp172 + fTemp170;
		float fTemp175 = fTemp173 * fTemp174;
		float fTemp176 = -2.0f - fTemp172 + fTemp170;
		float fTemp177 = (0.33113113f * fRec50[0] + fRec64[0]) * std::pow(0.001f, 0.25688207f * fTemp19);
		fVec45[IOTA0 & 131071] = fTemp177;
		int iTemp178 = static_cast<int>(fTemp171);
		float fTemp179 = 0.16666667f * fTemp175 * fTemp176 * fVec45[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp178))))))) & 131071] + fTemp25 + (-3.0f - fTemp172 + fTemp170) * (fTemp176 * (0.5f * fTemp173 * fVec45[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp178))))))) & 131071] - 0.16666667f * fVec45[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, iTemp178)))))) & 131071] * fTemp174) - 0.5f * fTemp175 * fVec45[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp178))))))) & 131071]) - 0.6f * fRec91[1];
		fVec46[IOTA0 & 1023] = fTemp179;
		float fRec89 = 0.6f * fTemp179;
		float fTemp180 = std::max<float>(1.0f, fConst0 * fTemp8 * (0.19230045f - fTemp137));
		float fTemp181 = -0.999995f + fTemp180;
		float fTemp182 = std::floor(fTemp181);
		float fTemp183 = fTemp180 - fTemp182;
		float fTemp184 = -1.0f - fTemp182 + fTemp180;
		float fTemp185 = fTemp183 * fTemp184;
		float fTemp186 = -2.0f - fTemp182 + fTemp180;
		float fTemp187 = (0.33113113f * fRec41[0] + fRec69[0]) * std::pow(0.001f, 0.19230045f * fTemp19);
		fVec47[IOTA0 & 131071] = fTemp187;
		int iTemp188 = static_cast<int>(fTemp181);
		float fTemp189 = 0.16666667f * fTemp185 * fTemp186 * fVec47[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp188))))))) & 131071] + fTemp25 + (-3.0f - fTemp182 + fTemp180) * (fTemp186 * (0.5f * fTemp183 * fVec47[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp188))))))) & 131071] - 0.16666667f * fVec47[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, iTemp188)))))) & 131071] * fTemp184) - 0.5f * fTemp185 * fVec47[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp188))))))) & 131071]) - 0.6f * fRec94[1];
		fVec48[IOTA0 & 1023] = fTemp189;
		float fRec90 = 0.6f * fTemp189;
		fRec91[0] = fVec46[(faust_wrap_sub(IOTA0, 875)) & 1023];
		float fRec92 = 0.6f * fTemp159;
		float fTemp190 = std::max<float>(1.0f, fConst0 * fTemp8 * (0.10252925f - fTemp13));
		float fTemp191 = -0.999995f + fTemp190;
		float fTemp192 = std::floor(fTemp191);
		float fTemp193 = fTemp190 - fTemp192;
		float fTemp194 = -1.0f - fTemp192 + fTemp190;
		float fTemp195 = fTemp193 * fTemp194;
		float fTemp196 = -2.0f - fTemp192 + fTemp190;
		float fTemp197 = (0.33113113f * fRec59[0] + fRec78[0]) * std::pow(0.001f, 0.10252925f * fTemp19);
		fVec49[IOTA0 & 65535] = fTemp197;
		int iTemp198 = static_cast<int>(fTemp191);
		float fTemp199 = 0.16666667f * fTemp195 * fTemp196 * fVec49[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp198))))))) & 65535] + fTemp25 + (-3.0f - fTemp192 + fTemp190) * (fTemp196 * (0.5f * fTemp193 * fVec49[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp198))))))) & 65535] - 0.16666667f * fVec49[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, iTemp198)))))) & 65535] * fTemp194) - 0.5f * fTemp195 * fVec49[(faust_wrap_sub(IOTA0, static_cast<int>(std::min<float>(fConst10, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp198))))))) & 65535]) - 0.6f * fRec93[1];
		fVec50[IOTA0 & 1023] = fTemp199;
		fRec93[0] = fVec50[(faust_wrap_sub(IOTA0, 832)) & 1023];
		fRec94[0] = fVec48[(faust_wrap_sub(IOTA0, 906)) & 1023];
		float fRec95 = 0.6f * fTemp169;
		float fRec96 = 0.6f * fTemp199;
		float fTemp200 = 0.25f * (fRec58 + fRec83 + fRec92 + fRec89 + fRec68 + fRec87 + fRec76 + fRec72 + fRec81 + fRec47 + fRec65 + fRec96 + fRec75 + fRec82 + fRec95 + fRec90 + fRec93[1] + fRec94[1] + fRec91[1] + fRec88[1] + fRec85[1] + fRec80[1] + fRec74[1] + fRec79[1] + fRec70[1] + fRec86[1] + fRec63[1] + fRec56[1] + fRec54[1] + fRec52[1] + fRec43[1] + fRec84[1]);
		float fTemp201 = fRec68 + fRec87 + fRec76 + fRec90 + fRec94[1] + fRec80[1] + fRec86[1] + fRec70[1];
		float fTemp202 = fRec92 + fRec47 + fRec82 + fRec90 + fRec94[1] + fRec85[1] + fRec84[1] + fRec56[1];
		fRec97[0] = fRec94[1] + fRec90 + fTemp200 - 0.5f * (fTemp201 + fTemp202);
		float fTemp203 = fRec72 + fRec81 + fRec47 + fRec65 + fRec79[1] + fRec54[1] + fRec84[1] + fRec52[1];
		float fTemp204 = fRec58 + fRec68 + fRec72 + fRec96 + fRec93[1] + fRec79[1] + fRec63[1] + fRec86[1];
		fRec98[0] = fRec79[1] + fRec72 + fTemp200 - 0.5f * (fTemp203 + fTemp204);
		float fTemp205 = fRec83 + fRec87 + fRec81 + fRec75 + fRec74[1] + fRec70[1] + fRec43[1] + fRec52[1];
		fRec99[0] = fRec52[1] + fRec81 + fTemp200 - 0.5f * (fTemp203 + fTemp205);
		float fTemp206 = fRec89 + fRec76 + fRec65 + fRec95 + fRec91[1] + fRec88[1] + fRec54[1] + fRec80[1];
		fRec100[0] = fRec54[1] + fRec65 + fTemp200 - 0.5f * (fTemp203 + fTemp206);
		float fTemp207 = fRec58 + fRec83 + fRec92 + fRec89 + fRec91[1] + fRec85[1] + fRec63[1] + fRec74[1];
		fRec101[0] = fRec74[1] + fRec83 + fTemp200 - 0.5f * (fTemp207 + fTemp205);
		float fTemp208 = fRec96 + fRec75 + fRec82 + fRec95 + fRec93[1] + fRec88[1] + fRec43[1] + fRec56[1];
		fRec102[0] = fRec88[1] + fRec95 + fTemp200 - 0.5f * (fTemp208 + fTemp206);
		fRec103[0] = fRec86[1] + fRec68 + fTemp200 - 0.5f * (fTemp201 + fTemp204);
		fRec104[0] = fRec93[1] + fRec96 + fTemp200 - 0.5f * (fTemp208 + fTemp204);
		fRec105[0] = fRec63[1] + fRec58 + fTemp200 - 0.5f * (fTemp207 + fTemp204);
		fRec106[0] = fRec85[1] + fRec92 + fTemp200 - 0.5f * (fTemp207 + fTemp202);
		fRec107[0] = fRec70[1] + fRec87 + fTemp200 - 0.5f * (fTemp201 + fTemp205);
		fRec108[0] = fRec80[1] + fRec76 + fTemp200 - 0.5f * (fTemp201 + fTemp206);
		fRec109[0] = fRec91[1] + fRec89 + fTemp200 - 0.5f * (fTemp207 + fTemp206);
		fRec110[0] = fRec56[1] + fRec82 + fTemp200 - 0.5f * (fTemp208 + fTemp202);
		fRec111[0] = fRec84[1] + fRec47 + fTemp200 - 0.5f * (fTemp203 + fTemp202);
		fRec112[0] = fRec43[1] + fRec75 + fTemp200 - 0.5f * (fTemp208 + fTemp205);
		outputs[0] = static_cast<FAUSTFLOAT>(fTemp0 * (1.0f - fRec0[0]) + 0.25f * fRec0[0] * (fRec100[0] + fRec109[0] + fRec111[0] + fRec106[0] + fRec99[0] + fRec101[0] + fRec98[0] + fRec105[0] + fRec102[0] + fRec108[0] + fRec110[0] + fRec97[0] + fRec112[0] + fRec107[0] + fRec104[0] + fRec103[0]));
		IOTA0 = faust_wrap_add(IOTA0, 1);
		iVec1[1] = iVec1[0];
		fRec0[1] = fRec0[0];
		fRec1[1] = fRec1[0];
		fRec2[1] = fRec2[0];
		fRec3[1] = fRec3[0];
		fRec4[1] = fRec4[0];
		fRec5[1] = fRec5[0];
		fRec6[1] = fRec6[0];
		fRec7[1] = fRec7[0];
		fRec8[1] = fRec8[0];
		fRec9[1] = fRec9[0];
		fRec10[1] = fRec10[0];
		fRec11[1] = fRec11[0];
		fRec12[1] = fRec12[0];
		fVec2[1] = fVec2[0];
		fRec13[1] = fRec13[0];
		fRec14[1] = fRec14[0];
		fRec15[1] = fRec15[0];
		fRec16[1] = fRec16[0];
		fRec17[1] = fRec17[0];
		fRec18[1] = fRec18[0];
		fRec19[1] = fRec19[0];
		fRec20[1] = fRec20[0];
		fVec3[1] = fVec3[0];
		fRec21[1] = fRec21[0];
		fRec22[1] = fRec22[0];
		fRec23[1] = fRec23[0];
		fRec24[1] = fRec24[0];
		fRec25[1] = fRec25[0];
		fVec4[1] = fVec4[0];
		fRec26[1] = fRec26[0];
		fRec27[1] = fRec27[0];
		fRec28[1] = fRec28[0];
		fRec29[1] = fRec29[0];
		fRec30[1] = fRec30[0];
		fRec31[1] = fRec31[0];
		fRec32[1] = fRec32[0];
		fVec5[1] = fVec5[0];
		fRec33[1] = fRec33[0];
		fVec6[1] = fVec6[0];
		fRec34[1] = fRec34[0];
		fRec35[1] = fRec35[0];
		fRec36[1] = fRec36[0];
		fRec37[1] = fRec37[0];
		fRec38[1] = fRec38[0];
		fRec39[1] = fRec39[0];
		fVec7[1] = fVec7[0];
		fRec40[1] = fRec40[0];
		fVec8[1] = fVec8[0];
		fRec41[1] = fRec41[0];
		fRec42[1] = fRec42[0];
		fRec113[1] = fRec113[0];
		fRec115[1] = fRec115[0];
		fRec114[1] = fRec114[0];
		fRec116[1] = fRec116[0];
		fRec117[1] = fRec117[0];
		fVec10[1] = fVec10[0];
		fRec119[1] = fRec119[0];
		fRec118[1] = fRec118[0];
		fRec43[1] = fRec43[0];
		fVec12[1] = fVec12[0];
		fRec44[1] = fRec44[0];
		fVec13[1] = fVec13[0];
		fRec45[1] = fRec45[0];
		fRec46[1] = fRec46[0];
		fRec120[1] = fRec120[0];
		fRec48[1] = fRec48[0];
		fRec49[1] = fRec49[0];
		fVec16[1] = fVec16[0];
		fRec50[1] = fRec50[0];
		fVec17[1] = fVec17[0];
		fRec51[1] = fRec51[0];
		fRec121[1] = fRec121[0];
		fRec52[1] = fRec52[0];
		fRec53[1] = fRec53[0];
		fRec54[1] = fRec54[0];
		fVec22[1] = fVec22[0];
		fRec55[1] = fRec55[0];
		fRec122[1] = fRec122[0];
		fRec56[1] = fRec56[0];
		fRec57[1] = fRec57[0];
		fRec123[1] = fRec123[0];
		fVec27[1] = fVec27[0];
		fRec59[1] = fRec59[0];
		fVec28[1] = fVec28[0];
		fRec60[1] = fRec60[0];
		fVec29[1] = fVec29[0];
		fRec61[1] = fRec61[0];
		fVec30[1] = fVec30[0];
		fRec62[1] = fRec62[0];
		fRec63[1] = fRec63[0];
		fRec64[1] = fRec64[0];
		fRec66[1] = fRec66[0];
		fRec67[1] = fRec67[0];
		fRec124[1] = fRec124[0];
		fRec69[1] = fRec69[0];
		fRec70[1] = fRec70[0];
		fRec71[1] = fRec71[0];
		fRec73[1] = fRec73[0];
		fRec74[1] = fRec74[0];
		fRec125[1] = fRec125[0];
		fRec77[1] = fRec77[0];
		fRec78[1] = fRec78[0];
		fRec79[1] = fRec79[0];
		fRec80[1] = fRec80[0];
		fRec84[1] = fRec84[0];
		fRec126[1] = fRec126[0];
		fRec85[1] = fRec85[0];
		fRec86[1] = fRec86[0];
		fRec88[1] = fRec88[0];
		fRec91[1] = fRec91[0];
		fRec93[1] = fRec93[0];
		fRec94[1] = fRec94[0];
		fRec97[2] = fRec97[1];
		fRec97[1] = fRec97[0];
		fRec98[2] = fRec98[1];
		fRec98[1] = fRec98[0];
		fRec99[2] = fRec99[1];
		fRec99[1] = fRec99[0];
		fRec100[2] = fRec100[1];
		fRec100[1] = fRec100[0];
		fRec101[2] = fRec101[1];
		fRec101[1] = fRec101[0];
		fRec102[2] = fRec102[1];
		fRec102[1] = fRec102[0];
		fRec103[2] = fRec103[1];
		fRec103[1] = fRec103[0];
		fRec104[2] = fRec104[1];
		fRec104[1] = fRec104[0];
		fRec105[2] = fRec105[1];
		fRec105[1] = fRec105[0];
		fRec106[2] = fRec106[1];
		fRec106[1] = fRec106[0];
		fRec107[2] = fRec107[1];
		fRec107[1] = fRec107[0];
		fRec108[2] = fRec108[1];
		fRec108[1] = fRec108[0];
		fRec109[2] = fRec109[1];
		fRec109[1] = fRec109[0];
		fRec110[2] = fRec110[1];
		fRec110[1] = fRec110[0];
		fRec111[2] = fRec111[1];
		fRec111[1] = fRec111[0];
		fRec112[2] = fRec112[1];
		fRec112[1] = fRec112[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
