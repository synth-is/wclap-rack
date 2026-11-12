/* ------------------------------------------------------------
name: "pm-formant-bp"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_pm_formant_bp -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_pm_formant_bp_H__
#define  __T_pm_formant_bp_H__

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
#define FAUSTCLASS T_pm_formant_bp
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

static float T_pm_formant_bp_faustpower2_f(float value) {
	return value * value;
}

class T_pm_formant_bp : public FaustDsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst2;
	float fRec0[2];
	FAUSTFLOAT fHslider1;
	float fSlow1;
	float fRec1[2];
	float fConst3;
	float fRec3[2];
	FAUSTFLOAT fHslider2;
	float fSlow2;
	float fRec4[2];
	int iRec5[2];
	float fConst4;
	FAUSTFLOAT fHslider3;
	float fSlow3;
	float fRec6[2];
	float fRec2[3];
	float fRec7[3];
	float fRec8[3];
	float fRec9[3];
	float fRec10[3];
	
 public:
	T_pm_formant_bp() {
	}
	
	T_pm_formant_bp(const T_pm_formant_bp&) = default;
	
	virtual ~T_pm_formant_bp() = default;
	
	T_pm_formant_bp& operator=(const T_pm_formant_bp&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/listInterp:author", "Romain Michon");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_pm_formant_bp -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "pm-formant-bp.dsp");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/resonbp:author", "Julius O. Smith III");
		m->declare("filters.lib/resonbp:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/resonbp:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/tf2:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf2s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf2s:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/version", "1.9.0");
		m->declare("maths.lib/author", "GRAME");
		m->declare("maths.lib/copyright", "GRAME");
		m->declare("maths.lib/license", "LicenseRef-LGPL-2.1-or-later-with-Faust-exception");
		m->declare("maths.lib/name", "Faust Math Library");
		m->declare("maths.lib/version", "2.9.0");
		m->declare("name", "pm-formant-bp");
		m->declare("noises.lib/name", "Faust Noise Generator Library");
		m->declare("noises.lib/version", "1.6.0");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/saw2ptr:author", "Julius O. Smith III");
		m->declare("oscillators.lib/saw2ptr:license", "LicenseRef-STK-4.3");
		m->declare("oscillators.lib/version", "1.8.0");
		m->declare("physmodels.lib/SFFormantModel:author", "Mike Olsen");
		m->declare("physmodels.lib/autobendFreq:author", "Mike Olsen");
		m->declare("physmodels.lib/formantFilterbank:author", "Mike Olsen");
		m->declare("physmodels.lib/name", "Faust Physical Models Library");
		m->declare("physmodels.lib/version", "1.2.0");
		m->declare("physmodels.lib/vocalEffort:author", "Mike Olsen");
		m->declare("physmodels.lib/voiceGender:author", "Mike Olsen");
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
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 1.0f / fConst0;
		fConst4 = 3.1415927f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(2.0f);
		fHslider1 = static_cast<FAUSTFLOAT>(2.2e+02f);
		fHslider2 = static_cast<FAUSTFLOAT>(0.0f);
		fHslider3 = static_cast<FAUSTFLOAT>(0.0f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = faust_wrap_add(l0, 1)) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec1[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = faust_wrap_add(l2, 1)) {
			fRec3[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = faust_wrap_add(l3, 1)) {
			fRec4[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = faust_wrap_add(l4, 1)) {
			iRec5[l4] = 0;
		}
		for (int l5 = 0; l5 < 2; l5 = faust_wrap_add(l5, 1)) {
			fRec6[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 3; l6 = faust_wrap_add(l6, 1)) {
			fRec2[l6] = 0.0f;
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
	
	virtual T_pm_formant_bp* clone() {
		return new T_pm_formant_bp(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("pm-formant-bp");
		ui_interface->addHorizontalSlider("exType", &fHslider2, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("freq", &fHslider1, FAUSTFLOAT(2.2e+02f), FAUSTFLOAT(5e+01f), FAUSTFLOAT(8e+02f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("voiceType", &fHslider0, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("vowel", &fHslider3, FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst1 * static_cast<float>(fHslider0);
		fSlow1 = fConst1 * static_cast<float>(fHslider1);
		fSlow2 = fConst1 * static_cast<float>(fHslider2);
		fSlow3 = fConst1 * static_cast<float>(fHslider3);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		fRec0[0] = fSlow0 + fConst2 * fRec0[1];
		int iTemp0 = ((fRec0[0] == 0.0f) ? 1 : ((fRec0[0] == 3.0f) ? 1 : 0)) == 0;
		fRec1[0] = fSlow1 + fConst2 * fRec1[1];
		float fTemp1 = 0.8f + 0.00084f * (1e+03f - fRec1[0]);
		float fTemp2 = 3.0f + 0.0036666666f * (4e+02f - fRec1[0]);
		float fTemp3 = std::max<float>(1.1920929e-07f, std::fabs(fRec1[0]));
		float fTemp4 = fRec3[1] + fConst3 * fTemp3;
		float fTemp5 = -1.0f + fTemp4;
		int iTemp6 = fTemp5 < 0.0f;
		fRec3[0] = ((iTemp6) ? fTemp4 : fTemp5);
		fRec4[0] = fSlow2 + fConst2 * fRec4[1];
		iRec5[0] = faust_wrap_add(12345, faust_wrap_mul(1103515245, iRec5[1]));
		float fTemp7 = std::fabs(static_cast<float>(inputs[0])) * ((-1.0f + 2.0f * ((iTemp6) ? fTemp4 : fTemp4 + fTemp5 * (1.0f - fConst0 / fTemp3))) * (1.0f - fRec4[0]) + 4.656613e-10f * fRec4[0] * static_cast<float>(iRec5[0]));
		fRec6[0] = fSlow3 + fConst2 * fRec6[1];
		float fTemp8 = 5.0f * fRec0[0] + fRec6[0];
		int iTemp9 = fTemp8 < 23.0f;
		int iTemp10 = fTemp8 < 24.0f;
		float fTemp11 = -23.0f + fTemp8;
		int iTemp12 = fTemp8 < 22.0f;
		float fTemp13 = -22.0f + fTemp8;
		int iTemp14 = fTemp8 < 21.0f;
		float fTemp15 = -21.0f + fTemp8;
		int iTemp16 = fTemp8 < 2e+01f;
		float fTemp17 = -2e+01f + fTemp8;
		int iTemp18 = fTemp8 < 19.0f;
		float fTemp19 = -19.0f + fTemp8;
		int iTemp20 = fTemp8 < 18.0f;
		float fTemp21 = -18.0f + fTemp8;
		int iTemp22 = fTemp8 < 17.0f;
		float fTemp23 = -17.0f + fTemp8;
		int iTemp24 = fTemp8 < 16.0f;
		float fTemp25 = -16.0f + fTemp8;
		float fTemp26 = 8e+01f * fTemp25;
		int iTemp27 = fTemp8 < 15.0f;
		float fTemp28 = -15.0f + fTemp8;
		int iTemp29 = fTemp8 < 14.0f;
		float fTemp30 = -14.0f + fTemp8;
		int iTemp31 = fTemp8 < 13.0f;
		float fTemp32 = -13.0f + fTemp8;
		int iTemp33 = fTemp8 < 12.0f;
		float fTemp34 = -12.0f + fTemp8;
		int iTemp35 = fTemp8 < 11.0f;
		float fTemp36 = -11.0f + fTemp8;
		int iTemp37 = fTemp8 < 1e+01f;
		float fTemp38 = -1e+01f + fTemp8;
		int iTemp39 = fTemp8 < 9.0f;
		float fTemp40 = -9.0f + fTemp8;
		int iTemp41 = fTemp8 < 8.0f;
		float fTemp42 = -8.0f + fTemp8;
		float fTemp43 = 5e+01f * fTemp42;
		int iTemp44 = fTemp8 < 7.0f;
		float fTemp45 = -7.0f + fTemp8;
		int iTemp46 = fTemp8 < 6.0f;
		float fTemp47 = -6.0f + fTemp8;
		int iTemp48 = fTemp8 < 5.0f;
		float fTemp49 = 5.0f * (1.0f - fRec0[0]);
		float fTemp50 = fTemp49 - fRec6[0];
		int iTemp51 = fTemp8 < 4.0f;
		float fTemp52 = -4.0f + fTemp8;
		int iTemp53 = fTemp8 < 3.0f;
		float fTemp54 = -3.0f + fTemp8;
		int iTemp55 = fTemp8 < 2.0f;
		float fTemp56 = -2.0f + fTemp8;
		int iTemp57 = fTemp8 < 1.0f;
		float fTemp58 = -1.0f + fTemp8;
		int iTemp59 = fTemp8 < 0.0f;
		float fTemp60 = ((iTemp9) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp33) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? ((iTemp57) ? ((iTemp59) ? 8e+02f : ((iTemp57) ? 8e+02f - 4e+02f * fTemp8 : 4e+02f)) : ((iTemp55) ? 4e+02f - 5e+01f * fTemp58 : 3.5e+02f)) : ((iTemp53) ? 3.5e+02f + 1e+02f * fTemp56 : 4.5e+02f)) : ((iTemp51) ? 4.5e+02f - 125.0f * fTemp54 : 325.0f)) : ((iTemp48) ? 325.0f + 275.0f * fTemp52 : 6e+02f)) : ((iTemp46) ? 6e+02f + 2e+02f * fTemp50 : 4e+02f)) : ((iTemp44) ? 4e+02f - 1.5e+02f * fTemp47 : 2.5e+02f)) : ((iTemp41) ? 2.5e+02f + 1.5e+02f * fTemp45 : 4e+02f)) : ((iTemp39) ? 4e+02f - fTemp43 : 3.5e+02f)) : ((iTemp37) ? 3.5e+02f + 3.1e+02f * fTemp40 : 6.6e+02f)) : ((iTemp35) ? 6.6e+02f - 2.2e+02f * fTemp38 : 4.4e+02f)) : ((iTemp33) ? 4.4e+02f - 1.7e+02f * fTemp36 : 2.7e+02f)) : ((iTemp31) ? 2.7e+02f + 1.6e+02f * fTemp34 : 4.3e+02f)) : ((iTemp29) ? 4.3e+02f - 6e+01f * fTemp32 : 3.7e+02f)) : ((iTemp27) ? 3.7e+02f + 4.3e+02f * fTemp30 : 8e+02f)) : ((iTemp24) ? 8e+02f - 4.5e+02f * fTemp28 : 3.5e+02f)) : ((iTemp22) ? 3.5e+02f - fTemp26 : 2.7e+02f)) : ((iTemp20) ? 2.7e+02f + 1.8e+02f * fTemp23 : 4.5e+02f)) : ((iTemp18) ? 4.5e+02f - 125.0f * fTemp21 : 325.0f)) : ((iTemp16) ? 325.0f * (1.0f + fTemp19) : 6.5e+02f)) : ((iTemp14) ? 6.5e+02f - 2.5e+02f * fTemp17 : 4e+02f)) : ((iTemp12) ? 4e+02f - 1.1e+02f * fTemp15 : 2.9e+02f)) : ((iTemp9) ? 2.9e+02f + 1.1e+02f * fTemp13 : 4e+02f)) : ((iTemp10) ? 4e+02f - 5e+01f * fTemp11 : 3.5e+02f));
		float fTemp61 = ((fTemp60 <= fRec1[0]) ? fRec1[0] : fTemp60);
		float fTemp62 = std::tan(fConst4 * fTemp61);
		float fTemp63 = 1.0f / fTemp62;
		float fTemp64 = 2e+01f * fTemp17;
		float fTemp65 = 2e+01f * fTemp28;
		float fTemp66 = 1e+01f * fTemp38;
		float fTemp67 = 1e+01f * fTemp52;
		float fTemp68 = 2e+01f * fTemp54;
		float fTemp69 = 2e+01f * fTemp56;
		float fTemp70 = 2e+01f * fTemp8;
		float fTemp71 = ((iTemp9) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp33) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? ((iTemp57) ? ((iTemp59) ? 8e+01f : ((iTemp57) ? 8e+01f - fTemp70 : 6e+01f)) : ((iTemp55) ? 6e+01f - 1e+01f * fTemp58 : 5e+01f)) : ((iTemp53) ? 5e+01f + fTemp69 : 7e+01f)) : ((iTemp51) ? 7e+01f - fTemp68 : 5e+01f)) : ((iTemp48) ? 5e+01f + fTemp67 : 6e+01f)) : ((iTemp46) ? 6e+01f + 2e+01f * fTemp50 : 4e+01f)) : ((iTemp44) ? 4e+01f + 2e+01f * fTemp47 : 6e+01f)) : ((iTemp41) ? 6e+01f - 2e+01f * fTemp45 : 4e+01f)) : 4e+01f) : ((iTemp37) ? 4e+01f * (1.0f + fTemp40) : 8e+01f)) : ((iTemp35) ? 8e+01f - fTemp66 : 7e+01f)) : ((iTemp33) ? 7e+01f - 3e+01f * fTemp36 : 4e+01f)) : 4e+01f) : 4e+01f) : ((iTemp27) ? 4e+01f * (1.0f + fTemp30) : 8e+01f)) : ((iTemp24) ? 8e+01f - fTemp65 : 6e+01f)) : 6e+01f) : ((iTemp20) ? 6e+01f - 2e+01f * fTemp23 : 4e+01f)) : ((iTemp18) ? 4e+01f + 1e+01f * fTemp21 : 5e+01f)) : 5e+01f) : ((iTemp14) ? 5e+01f + fTemp64 : 7e+01f)) : ((iTemp12) ? 7e+01f - 3e+01f * fTemp15 : 4e+01f)) : ((iTemp9) ? 4e+01f + 3e+01f * fTemp13 : 7e+01f)) : ((iTemp10) ? 7e+01f - 3e+01f * fTemp11 : 4e+01f)) / fTemp61;
		float fTemp72 = 1.0f + (fTemp63 + fTemp71) / fTemp62;
		fRec2[0] = fTemp7 - (fRec2[2] * (1.0f + (fTemp63 - fTemp71) / fTemp62) + 2.0f * fRec2[1] * (1.0f - 1.0f / T_pm_formant_bp_faustpower2_f(fTemp62))) / fTemp72;
		float fTemp73 = ((iTemp9) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp33) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? ((iTemp57) ? ((iTemp59) ? 0.630957f : ((iTemp57) ? 0.630957f - 0.567861f * fTemp8 : 0.063096f)) : ((iTemp55) ? 0.063096f + 0.036904f * fTemp58 : 0.1f)) : ((iTemp53) ? 0.1f + 0.254813f * fTemp56 : 0.354813f)) : ((iTemp51) ? 0.354813f - 0.103624f * fTemp54 : 0.251189f)) : ((iTemp48) ? 0.251189f + 0.195495f * fTemp52 : 0.446684f)) : ((iTemp46) ? 0.446684f + 0.195495f * fTemp50 : 0.251189f)) : ((iTemp44) ? 0.251189f - 0.219566f * fTemp47 : 0.031623f)) : ((iTemp41) ? 0.031623f + 0.250215f * fTemp45 : 0.281838f)) : ((iTemp39) ? 0.281838f - 0.181838f * fTemp42 : 0.1f)) : ((iTemp37) ? 0.1f + 0.401187f * fTemp40 : 0.501187f)) : ((iTemp35) ? 0.501187f - 0.301661f * fTemp38 : 0.199526f)) : ((iTemp33) ? 0.199526f - 0.13643f * fTemp36 : 0.063096f)) : ((iTemp31) ? 0.063096f + 0.253132f * fTemp34 : 0.316228f)) : ((iTemp29) ? 0.316228f - 0.216228f * fTemp32 : 0.1f)) : ((iTemp27) ? 0.1f + 0.401187f * fTemp30 : 0.501187f)) : ((iTemp24) ? 0.501187f - 0.401187f * fTemp28 : 0.1f)) : ((iTemp22) ? 0.1f + 0.151189f * fTemp25 : 0.251189f)) : ((iTemp20) ? 0.251189f + 0.030649f * fTemp23 : 0.281838f)) : ((iTemp18) ? 0.281838f - 0.123349f * fTemp21 : 0.158489f)) : ((iTemp16) ? 0.158489f + 0.342698f * fTemp19 : 0.501187f)) : ((iTemp14) ? 0.501187f - 0.301661f * fTemp17 : 0.199526f)) : ((iTemp12) ? 0.199526f - 0.021698f * fTemp15 : 0.177828f)) : ((iTemp9) ? 0.177828f + 0.1384f * fTemp13 : 0.316228f)) : ((iTemp10) ? 0.316228f - 0.216228f * fTemp11 : 0.1f));
		float fTemp74 = fRec6[0] - fTemp49;
		float fTemp75 = ((iTemp9) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp33) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? ((iTemp57) ? ((iTemp59) ? 1.15e+03f : ((iTemp57) ? 1.15e+03f + 4.5e+02f * fTemp8 : 1.6e+03f)) : ((iTemp55) ? 1.6e+03f + 1e+02f * fTemp58 : 1.7e+03f)) : ((iTemp53) ? 1.7e+03f - 9e+02f * fTemp56 : 8e+02f)) : ((iTemp51) ? 8e+02f - 1e+02f * fTemp54 : 7e+02f)) : ((iTemp48) ? 7e+02f + 3.4e+02f * fTemp52 : 1.04e+03f)) : ((iTemp46) ? 1.04e+03f + 5.8e+02f * fTemp74 : 1.62e+03f)) : ((iTemp44) ? 1.62e+03f + 1.3e+02f * fTemp47 : 1.75e+03f)) : ((iTemp41) ? 1.75e+03f - 1e+03f * fTemp45 : 7.5e+02f)) : ((iTemp39) ? 7.5e+02f - 1.5e+02f * fTemp42 : 6e+02f)) : ((iTemp37) ? 6e+02f + 5.2e+02f * fTemp40 : 1.12e+03f)) : ((iTemp35) ? 1.12e+03f + 6.8e+02f * fTemp38 : 1.8e+03f)) : ((iTemp33) ? 1.8e+03f + 5e+01f * fTemp36 : 1.85e+03f)) : ((iTemp31) ? 1.85e+03f - 1.03e+03f * fTemp34 : 8.2e+02f)) : ((iTemp29) ? 8.2e+02f - 1.9e+02f * fTemp32 : 6.3e+02f)) : ((iTemp27) ? 6.3e+02f + 5.2e+02f * fTemp30 : 1.15e+03f)) : ((iTemp24) ? 1.15e+03f + 8.5e+02f * fTemp28 : 2e+03f)) : ((iTemp22) ? 2e+03f + 1.4e+02f * fTemp25 : 2.14e+03f)) : ((iTemp20) ? 2.14e+03f - 1.34e+03f * fTemp23 : 8e+02f)) : ((iTemp18) ? 8e+02f - 1e+02f * fTemp21 : 7e+02f)) : ((iTemp16) ? 7e+02f + 3.8e+02f * fTemp19 : 1.08e+03f)) : ((iTemp14) ? 1.08e+03f + 6.2e+02f * fTemp17 : 1.7e+03f)) : ((iTemp12) ? 1.7e+03f + 1.7e+02f * fTemp15 : 1.87e+03f)) : ((iTemp9) ? 1.87e+03f - 1.07e+03f * fTemp13 : 8e+02f)) : ((iTemp10) ? 8e+02f - 2e+02f * fTemp11 : 6e+02f));
		float fTemp76 = 3e+01f + 2.0f * fRec1[0];
		float fTemp77 = ((fRec0[0] != 2.0f) ? (((fTemp75 >= 1.3e+03f) & (fRec1[0] >= 2e+02f)) ? fTemp75 - 0.00095238094f * (-2e+02f + fRec1[0]) * (-1.3e+03f + fTemp75) : ((fTemp75 <= fTemp76) ? fTemp76 : fTemp75)) : fTemp75);
		float fTemp78 = std::tan(fConst4 * fTemp77);
		float fTemp79 = 1.0f / fTemp78;
		float fTemp80 = 1e+01f * fTemp13;
		float fTemp81 = 1e+01f * fTemp17;
		float fTemp82 = 1e+01f * fTemp40;
		float fTemp83 = ((iTemp9) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp33) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? ((iTemp57) ? ((iTemp59) ? 9e+01f : ((iTemp57) ? 9e+01f - 1e+01f * fTemp8 : 8e+01f)) : ((iTemp55) ? 8e+01f + 2e+01f * fTemp58 : 1e+02f)) : ((iTemp53) ? 1e+02f - fTemp69 : 8e+01f)) : ((iTemp51) ? 8e+01f - fTemp68 : 6e+01f)) : ((iTemp48) ? 6e+01f + fTemp67 : 7e+01f)) : ((iTemp46) ? 7e+01f + 1e+01f * fTemp74 : 8e+01f)) : ((iTemp44) ? 8e+01f + 1e+01f * fTemp47 : 9e+01f)) : ((iTemp41) ? 9e+01f - 1e+01f * fTemp45 : 8e+01f)) : 8e+01f) : ((iTemp37) ? 8e+01f + fTemp82 : 9e+01f)) : ((iTemp35) ? 9e+01f - fTemp66 : 8e+01f)) : ((iTemp33) ? 8e+01f + 1e+01f * fTemp36 : 9e+01f)) : ((iTemp31) ? 9e+01f - 1e+01f * fTemp34 : 8e+01f)) : ((iTemp29) ? 8e+01f - 2e+01f * fTemp32 : 6e+01f)) : ((iTemp27) ? 6e+01f + 3e+01f * fTemp30 : 9e+01f)) : ((iTemp24) ? 9e+01f + 1e+01f * fTemp28 : 1e+02f)) : ((iTemp22) ? 1e+02f - 1e+01f * fTemp25 : 9e+01f)) : ((iTemp20) ? 9e+01f - 1e+01f * fTemp23 : 8e+01f)) : ((iTemp18) ? 8e+01f - 2e+01f * fTemp21 : 6e+01f)) : ((iTemp16) ? 6e+01f + 3e+01f * fTemp19 : 9e+01f)) : ((iTemp14) ? 9e+01f - fTemp81 : 8e+01f)) : ((iTemp12) ? 8e+01f + 1e+01f * fTemp15 : 9e+01f)) : ((iTemp9) ? 9e+01f - fTemp80 : 8e+01f)) : ((iTemp10) ? 8e+01f - 2e+01f * fTemp11 : 6e+01f)) / fTemp77;
		float fTemp84 = 1.0f + (fTemp79 + fTemp83) / fTemp78;
		fRec7[0] = fTemp7 - (fRec7[2] * (1.0f + (fTemp79 - fTemp83) / fTemp78) + 2.0f * fRec7[1] * (1.0f - 1.0f / T_pm_formant_bp_faustpower2_f(fTemp78))) / fTemp84;
		float fTemp85 = ((iTemp20) ? 0.050119f + 0.029314f * fTemp23 : 0.079433f);
		float fTemp86 = ((iTemp9) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp33) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? ((iTemp57) ? ((iTemp59) ? 0.1f : ((iTemp57) ? 0.1f - 0.068377f * fTemp8 : 0.031623f)) : 0.031623f) : ((iTemp53) ? 0.031623f + 0.126866f * fTemp56 : 0.158489f)) : ((iTemp51) ? 0.158489f - 0.126866f * fTemp54 : 0.031623f)) : ((iTemp48) ? 0.031623f + 0.32319f * fTemp52 : 0.354813f)) : 0.354813f) : ((iTemp44) ? 0.354813f - 0.196324f * fTemp47 : 0.158489f)) : ((iTemp41) ? 0.158489f - 0.069364f * fTemp45 : 0.089125f)) : ((iTemp39) ? 0.089125f - 0.064006f * fTemp42 : 0.025119f)) : ((iTemp37) ? 0.025119f + 0.045676f * fTemp40 : 0.070795f)) : ((iTemp35) ? 0.070795f + 0.055098f * fTemp38 : 0.125893f)) : ((iTemp33) ? 0.125893f - 0.062797f * fTemp36 : 0.063096f)) : ((iTemp31) ? 0.063096f - 0.012977f * fTemp34 : 0.050119f)) : ((iTemp29) ? 0.050119f + 0.020676f * fTemp32 : 0.070795f)) : ((iTemp27) ? 0.070795f - 0.045676f * fTemp30 : 0.025119f)) : ((iTemp24) ? 0.025119f + 0.152709f * fTemp28 : 0.177828f)) : ((iTemp22) ? 0.177828f - 0.127709f * fTemp25 : 0.050119f)) : fTemp85) : ((iTemp18) ? 0.079433f - 0.06165f * fTemp21 : 0.017783f)) : ((iTemp16) ? 0.017783f + 0.428901f * fTemp19 : 0.446684f)) : ((iTemp14) ? 0.446684f - 0.195495f * fTemp17 : 0.251189f)) : ((iTemp12) ? 0.251189f - 0.125296f * fTemp15 : 0.125893f)) : ((iTemp9) ? 0.125893f + 0.125296f * fTemp13 : 0.251189f)) : ((iTemp10) ? 0.251189f - 0.109935f * fTemp11 : 0.141254f));
		float fTemp87 = 1e+02f * fTemp11;
		float fTemp88 = 5e+01f * fTemp19;
		float fTemp89 = 5e+01f * fTemp38;
		float fTemp90 = ((iTemp9) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp33) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? ((iTemp57) ? ((iTemp59) ? 2.8e+03f : ((iTemp57) ? 2.8e+03f - 1e+02f * fTemp8 : 2.7e+03f)) : 2.7e+03f) : ((iTemp53) ? 2.7e+03f + 1.3e+02f * fTemp56 : 2.83e+03f)) : ((iTemp51) ? 2.83e+03f - 3e+02f * fTemp54 : 2.53e+03f)) : ((iTemp48) ? 2.53e+03f - 2.8e+02f * fTemp52 : 2.25e+03f)) : ((iTemp46) ? 2.25e+03f + 1.5e+02f * fTemp74 : 2.4e+03f)) : ((iTemp44) ? 2.4e+03f + 2e+02f * fTemp47 : 2.6e+03f)) : ((iTemp41) ? 2.6e+03f - 2e+02f * fTemp45 : 2.4e+03f)) : 2.4e+03f) : ((iTemp37) ? 2.4e+03f + 3.5e+02f * fTemp40 : 2.75e+03f)) : ((iTemp35) ? 2.75e+03f - fTemp89 : 2.7e+03f)) : ((iTemp33) ? 2.7e+03f + 2e+02f * fTemp36 : 2.9e+03f)) : ((iTemp31) ? 2.9e+03f - 2e+02f * fTemp34 : 2.7e+03f)) : ((iTemp29) ? 2.7e+03f + 5e+01f * fTemp32 : 2.75e+03f)) : ((iTemp27) ? 2.75e+03f + 1.5e+02f * fTemp30 : 2.9e+03f)) : ((iTemp24) ? 2.9e+03f - 1e+02f * fTemp28 : 2.8e+03f)) : ((iTemp22) ? 2.8e+03f + 1.5e+02f * fTemp25 : 2.95e+03f)) : ((iTemp20) ? 2.95e+03f - 1.2e+02f * fTemp23 : 2.83e+03f)) : ((iTemp18) ? 2.83e+03f - 1.3e+02f * fTemp21 : 2.7e+03f)) : ((iTemp16) ? 2.7e+03f - fTemp88 : 2.65e+03f)) : ((iTemp14) ? 2.65e+03f - 5e+01f * fTemp17 : 2.6e+03f)) : ((iTemp12) ? 2.6e+03f + 2e+02f * fTemp15 : 2.8e+03f)) : ((iTemp9) ? 2.8e+03f - 2e+02f * fTemp13 : 2.6e+03f)) : ((iTemp10) ? 2.6e+03f + fTemp87 : 2.7e+03f));
		float fTemp91 = std::tan(fConst4 * fTemp90);
		float fTemp92 = 1.0f / fTemp91;
		float fTemp93 = 2e+01f * fTemp30;
		float fTemp94 = 2e+01f * fTemp38;
		float fTemp95 = 2e+01f * fTemp40;
		float fTemp96 = 1e+01f * fTemp50;
		float fTemp97 = 6e+01f * fTemp52;
		float fTemp98 = ((iTemp9) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp33) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? 1.2e+02f : ((iTemp53) ? 1.2e+02f - fTemp69 : 1e+02f)) : ((iTemp51) ? 1e+02f + 7e+01f * fTemp54 : 1.7e+02f)) : ((iTemp48) ? 1.7e+02f - fTemp97 : 1.1e+02f)) : ((iTemp46) ? 1.1e+02f + fTemp96 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f) : ((iTemp37) ? 1e+02f + fTemp95 : 1.2e+02f)) : ((iTemp35) ? 1.2e+02f - fTemp94 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f) : ((iTemp27) ? 1e+02f + fTemp93 : 1.2e+02f)) : 1.2e+02f) : ((iTemp22) ? 1.2e+02f - 2e+01f * fTemp25 : 1e+02f)) : 1e+02f) : ((iTemp18) ? 1e+02f + 7e+01f * fTemp21 : 1.7e+02f)) : ((iTemp16) ? 1.7e+02f - fTemp88 : 1.2e+02f)) : ((iTemp14) ? 1.2e+02f - fTemp64 : 1e+02f)) : 1e+02f) : 1e+02f) : 1e+02f) / fTemp90;
		float fTemp99 = 1.0f + (fTemp92 + fTemp98) / fTemp91;
		fRec8[0] = fTemp7 - (fRec8[2] * (1.0f + (fTemp92 - fTemp98) / fTemp91) + 2.0f * fRec8[1] * (1.0f - 1.0f / T_pm_formant_bp_faustpower2_f(fTemp91))) / fTemp99;
		float fTemp100 = ((iTemp33) ? 0.1f - 0.084151f * fTemp36 : 0.015849f);
		float fTemp101 = ((iTemp9) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp33) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? ((iTemp57) ? ((iTemp59) ? 0.015849f : ((iTemp57) ? 0.015849f + 0.001934f * fTemp8 : 0.017783f)) : ((iTemp55) ? 0.017783f - 0.001934f * fTemp58 : 0.015849f)) : ((iTemp53) ? 0.015849f + 0.023962f * fTemp56 : 0.039811f)) : ((iTemp51) ? 0.039811f - 0.029811f * fTemp54 : 0.01f)) : ((iTemp48) ? 0.01f + 0.344813f * fTemp52 : 0.354813f)) : ((iTemp46) ? 0.354813f + 0.103624f * fTemp50 : 0.251189f)) : ((iTemp44) ? 0.251189f - 0.171756f * fTemp47 : 0.079433f)) : ((iTemp41) ? 0.079433f + 0.020567f * fTemp45 : 0.1f)) : ((iTemp39) ? 0.1f - 0.060189f * fTemp42 : 0.039811f)) : ((iTemp37) ? 0.039811f + 0.023285f * fTemp40 : 0.063096f)) : ((iTemp35) ? 0.063096f + 0.036904f * fTemp38 : 0.1f)) : fTemp100) : ((iTemp31) ? 0.015849f + 0.063584f * fTemp34 : 0.079433f)) : ((iTemp29) ? 0.079433f - 0.04781f * fTemp32 : 0.031623f)) : ((iTemp27) ? 0.031623f + 0.068377f * fTemp30 : 0.1f)) : ((iTemp24) ? 0.1f - 0.09f * fTemp28 : 0.01f)) : ((iTemp22) ? 0.01f + 0.040119f * fTemp25 : 0.050119f)) : fTemp85) : ((iTemp18) ? 0.079433f - 0.069433f * fTemp21 : 0.01f)) : ((iTemp16) ? 0.01f + 0.388107f * fTemp19 : 0.398107f)) : ((iTemp14) ? 0.398107f - 0.198581f * fTemp17 : 0.199526f)) : ((iTemp12) ? 0.199526f - 0.099526f * fTemp15 : 0.1f)) : ((iTemp9) ? 0.1f + 0.151189f * fTemp13 : 0.251189f)) : ((iTemp10) ? 0.251189f - 0.051663f * fTemp11 : 0.199526f));
		float fTemp102 = 3.5e+02f * fTemp74;
		float fTemp103 = ((iTemp9) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp33) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? ((iTemp57) ? ((iTemp59) ? 3.5e+03f : ((iTemp57) ? 3.5e+03f - 2e+02f * fTemp8 : 3.3e+03f)) : ((iTemp55) ? 3.3e+03f + 4e+02f * fTemp58 : 3.7e+03f)) : ((iTemp53) ? 3.7e+03f - 2e+02f * fTemp56 : 3.5e+03f)) : 3.5e+03f) : ((iTemp48) ? 3.5e+03f - 1.05e+03f * fTemp52 : 2.45e+03f)) : ((iTemp46) ? 2.45e+03f + fTemp102 : 2.8e+03f)) : ((iTemp44) ? 2.8e+03f + 2.5e+02f * fTemp47 : 3.05e+03f)) : ((iTemp41) ? 3.05e+03f - 4.5e+02f * fTemp45 : 2.6e+03f)) : ((iTemp39) ? 2.6e+03f + 75.0f * fTemp42 : 2675.0f)) : ((iTemp37) ? 2675.0f + 325.0f * fTemp40 : 3e+03f)) : 3e+03f) : ((iTemp33) ? 3e+03f + 3.5e+02f * fTemp36 : 3.35e+03f)) : ((iTemp31) ? 3.35e+03f - 3.5e+02f * fTemp34 : 3e+03f)) : 3e+03f) : ((iTemp27) ? 3e+03f + 9e+02f * fTemp30 : 3.9e+03f)) : ((iTemp24) ? 3.9e+03f - 3e+02f * fTemp28 : 3.6e+03f)) : ((iTemp22) ? 3.6e+03f + 3e+02f * fTemp25 : 3.9e+03f)) : ((iTemp20) ? 3.9e+03f - 1e+02f * fTemp23 : 3.8e+03f)) : 3.8e+03f) : ((iTemp16) ? 3.8e+03f - 9e+02f * fTemp19 : 2.9e+03f)) : ((iTemp14) ? 2.9e+03f + 3e+02f * fTemp17 : 3.2e+03f)) : ((iTemp12) ? 3.2e+03f + 5e+01f * fTemp15 : 3.25e+03f)) : ((iTemp9) ? 3.25e+03f - 4.5e+02f * fTemp13 : 2.8e+03f)) : ((iTemp10) ? 2.8e+03f + fTemp87 : 2.9e+03f));
		float fTemp104 = std::tan(fConst4 * fTemp103);
		float fTemp105 = 1.0f / fTemp104;
		float fTemp106 = ((iTemp9) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp33) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? ((iTemp57) ? ((iTemp59) ? 1.3e+02f : ((iTemp57) ? 1.3e+02f + fTemp70 : 1.5e+02f)) : 1.5e+02f) : ((iTemp53) ? 1.5e+02f - fTemp69 : 1.3e+02f)) : ((iTemp51) ? 1.3e+02f + 5e+01f * fTemp54 : 1.8e+02f)) : ((iTemp48) ? 1.8e+02f - fTemp97 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp37) ? 1.2e+02f + fTemp82 : 1.3e+02f)) : ((iTemp35) ? 1.3e+02f - fTemp66 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp27) ? 1.2e+02f + 1e+01f * fTemp30 : 1.3e+02f)) : ((iTemp24) ? 1.3e+02f + fTemp65 : 1.5e+02f)) : ((iTemp22) ? 1.5e+02f - 3e+01f * fTemp25 : 1.2e+02f)) : 1.2e+02f) : ((iTemp18) ? 1.2e+02f + 6e+01f * fTemp21 : 1.8e+02f)) : ((iTemp16) ? 1.8e+02f - fTemp88 : 1.3e+02f)) : ((iTemp14) ? 1.3e+02f - fTemp81 : 1.2e+02f)) : 1.2e+02f) : ((iTemp9) ? 1.2e+02f + fTemp80 : 1.3e+02f)) : ((iTemp10) ? 1.3e+02f - 1e+01f * fTemp11 : 1.2e+02f)) / fTemp103;
		float fTemp107 = 1.0f + (fTemp105 + fTemp106) / fTemp104;
		fRec9[0] = fTemp7 - (fRec9[2] * (1.0f + (fTemp105 - fTemp106) / fTemp104) + 2.0f * fRec9[1] * (1.0f - 1.0f / T_pm_formant_bp_faustpower2_f(fTemp104))) / fTemp107;
		float fTemp108 = ((iTemp9) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp33) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? 0.001f : ((iTemp53) ? 0.001f + 0.000778f * fTemp56 : 0.001778f)) : ((iTemp51) ? 0.001778f - 0.001147f * fTemp54 : 0.000631f)) : ((iTemp48) ? 0.000631f + 0.099369f * fTemp52 : 0.1f)) : ((iTemp46) ? 0.1f + 0.025893f * fTemp74 : 0.125893f)) : ((iTemp44) ? 0.125893f - 0.086082f * fTemp47 : 0.039811f)) : ((iTemp41) ? 0.039811f - 0.029811f * fTemp45 : 0.01f)) : ((iTemp39) ? 0.01f + 0.005849f * fTemp42 : 0.015849f)) : ((iTemp37) ? 0.015849f - 0.00326f * fTemp40 : 0.012589f)) : ((iTemp35) ? 0.012589f + 0.087411f * fTemp38 : 0.1f)) : fTemp100) : ((iTemp31) ? 0.015849f + 0.004104f * fTemp34 : 0.019953f)) : 0.019953f) : ((iTemp27) ? 0.019953f - 0.016791f * fTemp30 : 0.003162f)) : ((iTemp24) ? 0.003162f - 0.001577f * fTemp28 : 0.001585f)) : ((iTemp22) ? 0.001585f + 0.004725f * fTemp25 : 0.00631f)) : ((iTemp20) ? 0.00631f - 0.003148f * fTemp23 : 0.003162f)) : ((iTemp18) ? 0.003162f - 0.002162f * fTemp21 : 0.001f)) : ((iTemp16) ? 0.001f + 0.078433f * fTemp19 : 0.079433f)) : ((iTemp14) ? 0.079433f + 0.020567f * fTemp17 : 0.1f)) : ((iTemp12) ? 0.1f - 0.068377f * fTemp15 : 0.031623f)) : ((iTemp9) ? 0.031623f + 0.018496f * fTemp13 : 0.050119f)) : 0.050119f);
		float fTemp109 = ((iTemp9) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp33) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp51) ? 4.95e+03f : ((iTemp48) ? 4.95e+03f - 2.2e+03f * fTemp52 : 2.75e+03f)) : ((iTemp46) ? 2.75e+03f + fTemp102 : 3.1e+03f)) : ((iTemp44) ? 3.1e+03f + 2.4e+02f * fTemp47 : 3.34e+03f)) : ((iTemp41) ? 3.34e+03f - 4.4e+02f * fTemp45 : 2.9e+03f)) : ((iTemp39) ? 2.9e+03f + fTemp43 : 2.95e+03f)) : ((iTemp37) ? 2.95e+03f + 4e+02f * fTemp40 : 3.35e+03f)) : ((iTemp35) ? 3.35e+03f - fTemp89 : 3.3e+03f)) : ((iTemp33) ? 3.3e+03f + 2.9e+02f * fTemp36 : 3.59e+03f)) : ((iTemp31) ? 3.59e+03f - 2.9e+02f * fTemp34 : 3.3e+03f)) : ((iTemp29) ? 3.3e+03f + 1e+02f * fTemp32 : 3.4e+03f)) : ((iTemp27) ? 3.4e+03f + 1.55e+03f * fTemp30 : 4.95e+03f)) : 4.95e+03f) : 4.95e+03f) : 4.95e+03f) : 4.95e+03f) : ((iTemp16) ? 4.95e+03f - 1.7e+03f * fTemp19 : 3.25e+03f)) : ((iTemp14) ? 3.25e+03f + 3.3e+02f * fTemp17 : 3.58e+03f)) : ((iTemp12) ? 3.58e+03f - 4e+01f * fTemp15 : 3.54e+03f)) : ((iTemp9) ? 3.54e+03f - 5.4e+02f * fTemp13 : 3e+03f)) : ((iTemp10) ? 3e+03f + 3e+02f * fTemp11 : 3.3e+03f));
		float fTemp110 = std::tan(fConst4 * fTemp109);
		float fTemp111 = 1.0f / fTemp110;
		float fTemp112 = ((iTemp9) ? ((iTemp12) ? ((iTemp14) ? ((iTemp16) ? ((iTemp18) ? ((iTemp20) ? ((iTemp22) ? ((iTemp24) ? ((iTemp27) ? ((iTemp29) ? ((iTemp31) ? ((iTemp33) ? ((iTemp35) ? ((iTemp37) ? ((iTemp39) ? ((iTemp41) ? ((iTemp44) ? ((iTemp46) ? ((iTemp48) ? ((iTemp51) ? ((iTemp53) ? ((iTemp55) ? ((iTemp57) ? ((iTemp59) ? 1.4e+02f : ((iTemp57) ? 1.4e+02f + 6e+01f * fTemp8 : 2e+02f)) : 2e+02f) : ((iTemp53) ? 2e+02f - 65.0f * fTemp56 : 135.0f)) : ((iTemp51) ? 135.0f + 65.0f * fTemp54 : 2e+02f)) : ((iTemp48) ? 2e+02f - 7e+01f * fTemp52 : 1.3e+02f)) : ((iTemp46) ? 1.3e+02f + fTemp96 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp37) ? 1.2e+02f + fTemp95 : 1.4e+02f)) : ((iTemp35) ? 1.4e+02f - fTemp94 : 1.2e+02f)) : 1.2e+02f) : 1.2e+02f) : 1.2e+02f) : ((iTemp27) ? 1.2e+02f + fTemp93 : 1.4e+02f)) : ((iTemp24) ? 1.4e+02f + 6e+01f * fTemp28 : 2e+02f)) : ((iTemp22) ? 2e+02f - fTemp26 : 1.2e+02f)) : 1.2e+02f) : ((iTemp18) ? 1.2e+02f + 8e+01f * fTemp21 : 2e+02f)) : ((iTemp16) ? 2e+02f - 6e+01f * fTemp19 : 1.4e+02f)) : ((iTemp14) ? 1.4e+02f - fTemp64 : 1.2e+02f)) : 1.2e+02f) : ((iTemp9) ? 1.2e+02f + 15.0f * fTemp13 : 135.0f)) : ((iTemp10) ? 135.0f - 15.0f * fTemp11 : 1.2e+02f)) / fTemp109;
		float fTemp113 = 1.0f + (fTemp111 + fTemp112) / fTemp110;
		fRec10[0] = fTemp7 - (fRec10[2] * (1.0f + (fTemp111 - fTemp112) / fTemp110) + 2.0f * fRec10[1] * (1.0f - 1.0f / T_pm_formant_bp_faustpower2_f(fTemp110))) / fTemp113;
		outputs[0] = static_cast<FAUSTFLOAT>(((iTemp0) ? fTemp2 : fTemp1) * (fRec2[0] - fRec2[2]) / (fTemp62 * fTemp72) + ((iTemp0) ? fTemp2 * fTemp73 : fTemp1 * fTemp73) * (fRec7[0] - fRec7[2]) / (fTemp78 * fTemp84) + ((iTemp0) ? fTemp2 * fTemp86 : fTemp1 * fTemp86) * (fRec8[0] - fRec8[2]) / (fTemp91 * fTemp99) + ((iTemp0) ? fTemp2 * fTemp101 : fTemp1 * fTemp101) * (fRec9[0] - fRec9[2]) / (fTemp104 * fTemp107) + ((iTemp0) ? fTemp2 * fTemp108 : fTemp1 * fTemp108) * (fRec10[0] - fRec10[2]) / (fTemp110 * fTemp113));
		fRec0[1] = fRec0[0];
		fRec1[1] = fRec1[0];
		fRec3[1] = fRec3[0];
		fRec4[1] = fRec4[0];
		iRec5[1] = iRec5[0];
		fRec6[1] = fRec6[0];
		fRec2[2] = fRec2[1];
		fRec2[1] = fRec2[0];
		fRec7[2] = fRec7[1];
		fRec7[1] = fRec7[0];
		fRec8[2] = fRec8[1];
		fRec8[1] = fRec8[0];
		fRec9[2] = fRec9[1];
		fRec9[1] = fRec9[0];
		fRec10[2] = fRec10[1];
		fRec10[1] = fRec10[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
