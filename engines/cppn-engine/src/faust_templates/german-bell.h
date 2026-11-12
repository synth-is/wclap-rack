/* ------------------------------------------------------------
name: "german-bell"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_german_bell -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_german_bell_H__
#define  __T_german_bell_H__

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
#define FAUSTCLASS T_german_bell
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

const static float fT_german_bellSIG0Wave0[350] = {0.761411f,0.797803f,0.586057f,0.225039f,0.926354f,0.813875f,0.529871f,0.158915f,0.662516f,0.30841f,0.0454785f,0.556082f,0.784713f,0.213035f,0.956227f,0.795165f,0.313508f,0.158531f,0.301586f,0.16832f,0.281267f,0.154237f,0.217235f,0.247239f,0.522688f,0.170303f,0.320614f,0.154857f,0.414941f,0.788364f,0.713299f,0.381592f,0.707011f,1.0f,0.0106946f,0.359763f,0.0746156f,0.431124f,0.118053f,0.52729f,0.255303f,0.646855f,0.201145f,0.919908f,0.359389f,0.253494f,0.865695f,0.0829263f,0.222589f,0.0415736f,0.735774f,0.769678f,0.381416f,0.197543f,0.682389f,0.614766f,0.249525f,0.289883f,0.300319f,0.443691f,0.275272f,0.370218f,0.74879f,0.161247f,0.634353f,0.498498f,0.221988f,0.350637f,0.436817f,0.436463f,0.52508f,0.842646f,0.195324f,0.224755f,0.545681f,0.353778f,0.269044f,0.327814f,0.448952f,0.852305f,0.380503f,1.0f,0.458063f,0.565058f,0.354635f,0.659529f,0.449485f,0.802014f,0.283446f,0.521563f,0.374639f,0.652112f,0.181042f,0.468394f,0.430116f,0.157207f,0.414995f,0.292737f,0.487785f,0.47768f,0.481445f,0.556988f,0.561132f,0.378265f,0.291462f,0.32325f,0.482602f,0.328248f,0.196792f,0.712254f,0.389806f,0.307751f,0.792876f,0.199098f,0.288838f,0.146811f,0.0178444f,0.394017f,0.588212f,0.489226f,0.701486f,0.940303f,0.213191f,0.0778845f,0.474463f,0.609972f,0.16338f,0.408376f,0.330864f,0.88999f,0.0798101f,1.0f,0.167172f,0.208727f,0.227549f,0.825903f,0.531897f,0.580946f,0.215339f,0.247319f,0.268148f,0.504088f,0.160955f,0.170316f,0.230769f,0.151191f,0.0825031f,0.141328f,0.288079f,0.499676f,0.420355f,0.487764f,0.75371f,0.492248f,0.181364f,0.20594f,0.52497f,0.325641f,0.236827f,0.931842f,0.472981f,0.312162f,0.901032f,0.270478f,0.167112f,0.0709698f,0.166004f,0.290745f,0.425893f,0.403633f,0.581772f,0.855694f,0.0325587f,0.0568359f,0.241923f,0.79931f,0.181962f,0.530283f,0.41561f,1.0f,0.0291501f,0.831155f,0.119755f,0.102188f,0.132037f,0.76603f,0.442221f,0.1749f,0.142175f,0.0140794f,0.4375f,0.85186f,0.196836f,0.0907522f,0.551673f,0.256528f,0.0424377f,0.490363f,0.108178f,0.503276f,0.226584f,0.312758f,0.824079f,0.48412f,0.0347338f,0.0619428f,0.255097f,0.145273f,0.190359f,0.995297f,0.50342f,0.217575f,0.793975f,0.357179f,0.0409728f,0.0187931f,0.414458f,0.15844f,0.346233f,0.0771673f,0.175111f,0.380567f,0.497409f,0.316164f,0.488669f,1.0f,0.420657f,0.442333f,0.24706f,0.796407f,0.0104146f,0.227079f,0.10684f,0.0289873f,0.76757f,0.0643122f,0.0423098f,0.715522f,0.472117f,0.392577f,0.660433f,0.803575f,0.188653f,0.0191653f,0.873212f,0.448719f,0.0147128f,0.618967f,0.30718f,0.345899f,0.131394f,0.193225f,0.715283f,0.40562f,0.040637f,0.0503336f,0.0544331f,0.0452023f,0.152302f,0.894549f,0.443343f,0.172071f,0.647667f,0.328993f,0.0416014f,0.0229488f,0.551071f,0.423544f,0.862362f,0.363526f,0.769929f,0.123452f,0.710702f,0.394895f,0.556638f,1.0f,0.450925f,0.422849f,0.092187f,0.413892f,0.00995372f,0.321146f,0.160624f,0.0300681f,0.915385f,0.4755f,0.506951f,0.387549f,0.275591f,0.468281f,0.184776f,0.186837f,0.188195f,0.0202785f,0.697136f,0.408862f,0.0116935f,0.349823f,0.202301f,0.461379f,0.0559023f,0.0582571f,0.260608f,0.181875f,0.0271739f,0.0249578f,0.685089f,0.470384f,0.283194f,0.33105f,0.138349f,0.338068f,0.481992f,0.178242f,0.0155942f,0.0110435f,0.783771f,0.442707f,0.616478f,0.381542f,0.510892f,0.045985f,0.303119f,0.0731909f,0.547715f,0.348941f,0.149981f,0.302158f,0.284482f,0.398177f,0.00413049f,0.180739f,0.062839f,0.0133459f,0.347088f,0.57637f,0.240764f,0.978481f,0.452755f,0.529742f,0.340471f,0.662282f,0.444305f,0.0429901f,1.0f,0.36194f,0.0183372f,0.626893f,0.55285f,0.384936f};
class T_german_bellSIG0 {
	
  private:
	
	int fT_german_bellSIG0Wave0_idx;
	int fSampleRate;
	
  public:
	
	int getNumInputsT_german_bellSIG0() {
		return 0;
	}
	int getNumOutputsT_german_bellSIG0() {
		return 1;
	}
	
	void instanceInitT_german_bellSIG0(int sample_rate) {
		fSampleRate = sample_rate;
		fT_german_bellSIG0Wave0_idx = 0;
	}
	
	void fillT_german_bellSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = faust_wrap_add(i1, 1)) {
			table[i1] = fT_german_bellSIG0Wave0[fT_german_bellSIG0Wave0_idx];
			fT_german_bellSIG0Wave0_idx = (faust_wrap_add(1, fT_german_bellSIG0Wave0_idx)) % 350;
		}
	}

};

static T_german_bellSIG0* newT_german_bellSIG0() { return (T_german_bellSIG0*)new T_german_bellSIG0(); }
static void deleteT_german_bellSIG0(T_german_bellSIG0* dsp) { delete dsp; }

static float T_german_bell_faustpower2_f(float value) {
	return value * value;
}
static float ftbl0T_german_bellSIG0[350];

class T_german_bell : public FaustDsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst3;
	float fRec1[2];
	FAUSTFLOAT fHslider1;
	float fSlow1;
	float fRec2[2];
	FAUSTFLOAT fHslider2;
	float fSlow2;
	float fRec3[2];
	float fConst4;
	float fRec0[3];
	FAUSTFLOAT fHslider3;
	float fSlow3;
	float fRec4[2];
	float fConst5;
	float fRec5[3];
	float fConst6;
	float fRec6[3];
	float fConst7;
	float fRec7[3];
	float fConst8;
	float fRec8[3];
	float fConst9;
	float fRec9[3];
	float fConst10;
	float fRec10[3];
	float fConst11;
	float fRec11[3];
	float fConst12;
	float fRec12[3];
	float fConst13;
	float fRec13[3];
	float fConst14;
	float fRec14[3];
	float fConst15;
	float fRec15[3];
	float fConst16;
	float fRec16[3];
	float fConst17;
	float fRec17[3];
	float fConst18;
	float fRec18[3];
	float fConst19;
	float fRec19[3];
	float fConst20;
	float fRec20[3];
	float fConst21;
	float fRec21[3];
	float fConst22;
	float fRec22[3];
	float fConst23;
	float fRec23[3];
	float fConst24;
	float fRec24[3];
	float fConst25;
	float fRec25[3];
	float fConst26;
	float fRec26[3];
	float fConst27;
	float fRec27[3];
	float fConst28;
	float fRec28[3];
	float fConst29;
	float fRec29[3];
	float fConst30;
	float fRec30[3];
	float fConst31;
	float fRec31[3];
	float fConst32;
	float fRec32[3];
	float fConst33;
	float fRec33[3];
	float fConst34;
	float fRec34[3];
	float fConst35;
	float fRec35[3];
	float fConst36;
	float fRec36[3];
	float fConst37;
	float fRec37[3];
	float fConst38;
	float fRec38[3];
	float fConst39;
	float fRec39[3];
	float fConst40;
	float fRec40[3];
	float fConst41;
	float fRec41[3];
	float fConst42;
	float fRec42[3];
	float fConst43;
	float fRec43[3];
	float fConst44;
	float fRec44[3];
	float fConst45;
	float fRec45[3];
	float fConst46;
	float fRec46[3];
	float fConst47;
	float fRec47[3];
	float fConst48;
	float fRec48[3];
	float fConst49;
	float fRec49[3];
	float fConst50;
	float fRec50[3];
	float fConst51;
	float fRec51[3];
	float fConst52;
	float fRec52[3];
	float fConst53;
	float fRec53[3];
	
 public:
	T_german_bell() {
	}
	
	T_german_bell(const T_german_bell&) = default;
	
	virtual ~T_german_bell() = default;
	
	T_german_bell& operator=(const T_german_bell&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_german_bell -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "german-bell.dsp");
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
		m->declare("name", "german-bell");
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
		T_german_bellSIG0* sig0 = newT_german_bellSIG0();
		sig0->instanceInitT_german_bellSIG0(sample_rate);
		sig0->fillT_german_bellSIG0(350, ftbl0T_german_bellSIG0);
		deleteT_german_bellSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 1.0f / fConst0;
		fConst2 = 44.1f / fConst0;
		fConst3 = 1.0f - fConst2;
		fConst4 = 2.0f * std::cos(1501.1095f / fConst0);
		fConst5 = 2.0f * std::cos(1509.441f / fConst0);
		fConst6 = 2.0f * std::cos(2552.544f / fConst0);
		fConst7 = 2.0f * std::cos(2564.8843f / fConst0);
		fConst8 = 2.0f * std::cos(3525.5266f / fConst0);
		fConst9 = 2.0f * std::cos(3536.9495f / fConst0);
		fConst10 = 2.0f * std::cos(4710.0767f / fConst0);
		fConst11 = 2.0f * std::cos(4711.635f / fConst0);
		fConst12 = 2.0f * std::cos(4909.612f / fConst0);
		fConst13 = 2.0f * std::cos(5030.821f / fConst0);
		fConst14 = 2.0f * std::cos(5036.2686f / fConst0);
		fConst15 = 2.0f * std::cos(5557.3896f / fConst0);
		fConst16 = 2.0f * std::cos(5922.028f / fConst0);
		fConst17 = 2.0f * std::cos(5923.253f / fConst0);
		fConst18 = 2.0f * std::cos(6022.8794f / fConst0);
		fConst19 = 2.0f * std::cos(6035.5083f / fConst0);
		fConst20 = 2.0f * std::cos(6389.1826f / fConst0);
		fConst21 = 2.0f * std::cos(6398.105f / fConst0);
		fConst22 = 2.0f * std::cos(6423.2373f / fConst0);
		fConst23 = 2.0f * std::cos(7578.7783f / fConst0);
		fConst24 = 2.0f * std::cos(7582.988f / fConst0);
		fConst25 = 2.0f * std::cos(7627.347f / fConst0);
		fConst26 = 2.0f * std::cos(7682.8906f / fConst0);
		fConst27 = 2.0f * std::cos(7738.8735f / fConst0);
		fConst28 = 2.0f * std::cos(8454.969f / fConst0);
		fConst29 = 2.0f * std::cos(8514.847f / fConst0);
		fConst30 = 2.0f * std::cos(8523.455f / fConst0);
		fConst31 = 2.0f * std::cos(8528.167f / fConst0);
		fConst32 = 2.0f * std::cos(8729.167f / fConst0);
		fConst33 = 2.0f * std::cos(8740.602f / fConst0);
		fConst34 = 2.0f * std::cos(8923.191f / fConst0);
		fConst35 = 2.0f * std::cos(8947.57f / fConst0);
		fConst36 = 2.0f * std::cos(8949.204f / fConst0);
		fConst37 = 2.0f * std::cos(8963.215f / fConst0);
		fConst38 = 2.0f * std::cos(9941.884f / fConst0);
		fConst39 = 2.0f * std::cos(10248.315f / fConst0);
		fConst40 = 2.0f * std::cos(10292.8f / fConst0);
		fConst41 = 2.0f * std::cos(10808.838f / fConst0);
		fConst42 = 2.0f * std::cos(10849.176f / fConst0);
		fConst43 = 2.0f * std::cos(11333.547f / fConst0);
		fConst44 = 2.0f * std::cos(11479.882f / fConst0);
		fConst45 = 2.0f * std::cos(11494.711f / fConst0);
		fConst46 = 2.0f * std::cos(12081.748f / fConst0);
		fConst47 = 2.0f * std::cos(12107.321f / fConst0);
		fConst48 = 2.0f * std::cos(12195.411f / fConst0);
		fConst49 = 2.0f * std::cos(12217.466f / fConst0);
		fConst50 = 2.0f * std::cos(12240.901f / fConst0);
		fConst51 = 2.0f * std::cos(12376.995f / fConst0);
		fConst52 = 2.0f * std::cos(12517.613f / fConst0);
		fConst53 = 2.0f * std::cos(12640.826f / fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(5.0f);
		fHslider1 = static_cast<FAUSTFLOAT>(0.5f);
		fHslider2 = static_cast<FAUSTFLOAT>(1.0f);
		fHslider3 = static_cast<FAUSTFLOAT>(0.5f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = faust_wrap_add(l0, 1)) {
			fRec1[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec2[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = faust_wrap_add(l2, 1)) {
			fRec3[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 3; l3 = faust_wrap_add(l3, 1)) {
			fRec0[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = faust_wrap_add(l4, 1)) {
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
		for (int l21 = 0; l21 < 3; l21 = faust_wrap_add(l21, 1)) {
			fRec21[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 3; l22 = faust_wrap_add(l22, 1)) {
			fRec22[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 3; l23 = faust_wrap_add(l23, 1)) {
			fRec23[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 3; l24 = faust_wrap_add(l24, 1)) {
			fRec24[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 3; l25 = faust_wrap_add(l25, 1)) {
			fRec25[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 3; l26 = faust_wrap_add(l26, 1)) {
			fRec26[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 3; l27 = faust_wrap_add(l27, 1)) {
			fRec27[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 3; l28 = faust_wrap_add(l28, 1)) {
			fRec28[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 3; l29 = faust_wrap_add(l29, 1)) {
			fRec29[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 3; l30 = faust_wrap_add(l30, 1)) {
			fRec30[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 3; l31 = faust_wrap_add(l31, 1)) {
			fRec31[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 3; l32 = faust_wrap_add(l32, 1)) {
			fRec32[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 3; l33 = faust_wrap_add(l33, 1)) {
			fRec33[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 3; l34 = faust_wrap_add(l34, 1)) {
			fRec34[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 3; l35 = faust_wrap_add(l35, 1)) {
			fRec35[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 3; l36 = faust_wrap_add(l36, 1)) {
			fRec36[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 3; l37 = faust_wrap_add(l37, 1)) {
			fRec37[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 3; l38 = faust_wrap_add(l38, 1)) {
			fRec38[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 3; l39 = faust_wrap_add(l39, 1)) {
			fRec39[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 3; l40 = faust_wrap_add(l40, 1)) {
			fRec40[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 3; l41 = faust_wrap_add(l41, 1)) {
			fRec41[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 3; l42 = faust_wrap_add(l42, 1)) {
			fRec42[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 3; l43 = faust_wrap_add(l43, 1)) {
			fRec43[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 3; l44 = faust_wrap_add(l44, 1)) {
			fRec44[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 3; l45 = faust_wrap_add(l45, 1)) {
			fRec45[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 3; l46 = faust_wrap_add(l46, 1)) {
			fRec46[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 3; l47 = faust_wrap_add(l47, 1)) {
			fRec47[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 3; l48 = faust_wrap_add(l48, 1)) {
			fRec48[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 3; l49 = faust_wrap_add(l49, 1)) {
			fRec49[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 3; l50 = faust_wrap_add(l50, 1)) {
			fRec50[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 3; l51 = faust_wrap_add(l51, 1)) {
			fRec51[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 3; l52 = faust_wrap_add(l52, 1)) {
			fRec52[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 3; l53 = faust_wrap_add(l53, 1)) {
			fRec53[l53] = 0.0f;
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
	
	virtual T_german_bell* clone() {
		return new T_german_bell(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("german-bell");
		ui_interface->addHorizontalSlider("exPos", &fHslider3, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("t60", &fHslider0, FAUSTFLOAT(5.0f), FAUSTFLOAT(0.1f), FAUSTFLOAT(6e+01f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("t60DecayRatio", &fHslider1, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("t60DecaySlope", &fHslider2, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst2 * static_cast<float>(fHslider0);
		fSlow1 = fConst2 * static_cast<float>(fHslider1);
		fSlow2 = fConst2 * static_cast<float>(fHslider2);
		fSlow3 = fConst2 * static_cast<float>(fHslider3);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		float fTemp0 = static_cast<float>(inputs[0]);
		fRec1[0] = fSlow0 + fConst3 * fRec1[1];
		fRec2[0] = fSlow1 + fConst3 * fRec2[1];
		fRec3[0] = fSlow2 + fConst3 * fRec3[1];
		float fTemp1 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.118451215f * fRec2[0], fRec3[0])));
		fRec0[0] = fTemp0 - fRec0[2] * T_german_bell_faustpower2_f(fTemp1) + fConst4 * fRec0[1] * fTemp1;
		fRec4[0] = fSlow3 + fConst3 * fRec4[1];
		int iTemp2 = faust_wrap_mul(50, static_cast<int>(fRec4[0]));
		float fTemp3 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.11910865f * fRec2[0], fRec3[0])));
		fRec5[0] = fTemp0 - fRec5[2] * T_german_bell_faustpower2_f(fTemp3) + fConst5 * fRec5[1] * fTemp3;
		float fTemp4 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.20141898f * fRec2[0], fRec3[0])));
		fRec6[0] = fTemp0 - fRec6[2] * T_german_bell_faustpower2_f(fTemp4) + fConst6 * fRec6[1] * fTemp4;
		float fTemp5 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.20239273f * fRec2[0], fRec3[0])));
		fRec7[0] = fTemp0 - fRec7[2] * T_german_bell_faustpower2_f(fTemp5) + fConst7 * fRec7[1] * fTemp5;
		float fTemp6 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.2781962f * fRec2[0], fRec3[0])));
		fRec8[0] = fTemp0 - fRec8[2] * T_german_bell_faustpower2_f(fTemp6) + fConst8 * fRec8[1] * fTemp6;
		float fTemp7 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.27909756f * fRec2[0], fRec3[0])));
		fRec9[0] = fTemp0 - fRec9[2] * T_german_bell_faustpower2_f(fTemp7) + fConst9 * fRec9[1] * fTemp7;
		float fTemp8 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.37166798f * fRec2[0], fRec3[0])));
		fRec10[0] = fTemp0 - fRec10[2] * T_german_bell_faustpower2_f(fTemp8) + fConst10 * fRec10[1] * fTemp8;
		float fTemp9 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.37179095f * fRec2[0], fRec3[0])));
		fRec11[0] = fTemp0 - fRec11[2] * T_german_bell_faustpower2_f(fTemp9) + fConst11 * fRec11[1] * fTemp9;
		float fTemp10 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.3874131f * fRec2[0], fRec3[0])));
		fRec12[0] = fTemp0 - fRec12[2] * T_german_bell_faustpower2_f(fTemp10) + fConst12 * fRec12[1] * fTemp10;
		float fTemp11 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.3969776f * fRec2[0], fRec3[0])));
		fRec13[0] = fTemp0 - fRec13[2] * T_german_bell_faustpower2_f(fTemp11) + fConst13 * fRec13[1] * fTemp11;
		float fTemp12 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.39740747f * fRec2[0], fRec3[0])));
		fRec14[0] = fTemp0 - fRec14[2] * T_german_bell_faustpower2_f(fTemp12) + fConst14 * fRec14[1] * fTemp12;
		float fTemp13 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.43852866f * fRec2[0], fRec3[0])));
		fRec15[0] = fTemp0 - fRec15[2] * T_german_bell_faustpower2_f(fTemp13) + fConst15 * fRec15[1] * fTemp13;
		float fTemp14 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.46730196f * fRec2[0], fRec3[0])));
		fRec16[0] = fTemp0 - fRec16[2] * T_german_bell_faustpower2_f(fTemp14) + fConst16 * fRec16[1] * fTemp14;
		float fTemp15 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.46739864f * fRec2[0], fRec3[0])));
		fRec17[0] = fTemp0 - fRec17[2] * T_german_bell_faustpower2_f(fTemp15) + fConst17 * fRec17[1] * fTemp15;
		float fTemp16 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.47526005f * fRec2[0], fRec3[0])));
		fRec18[0] = fTemp0 - fRec18[2] * T_german_bell_faustpower2_f(fTemp16) + fConst18 * fRec18[1] * fTemp16;
		float fTemp17 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.4762566f * fRec2[0], fRec3[0])));
		fRec19[0] = fTemp0 - fRec19[2] * T_german_bell_faustpower2_f(fTemp17) + fConst19 * fRec19[1] * fTemp17;
		float fTemp18 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.5041647f * fRec2[0], fRec3[0])));
		fRec20[0] = fTemp0 - fRec20[2] * T_german_bell_faustpower2_f(fTemp18) + fConst20 * fRec20[1] * fTemp18;
		float fTemp19 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.50486875f * fRec2[0], fRec3[0])));
		fRec21[0] = fTemp0 - fRec21[2] * T_german_bell_faustpower2_f(fTemp19) + fConst21 * fRec21[1] * fTemp19;
		float fTemp20 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.506852f * fRec2[0], fRec3[0])));
		fRec22[0] = fTemp0 - fRec22[2] * T_german_bell_faustpower2_f(fTemp20) + fConst22 * fRec22[1] * fTemp20;
		float fTemp21 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.5980346f * fRec2[0], fRec3[0])));
		fRec23[0] = fTemp0 - fRec23[2] * T_german_bell_faustpower2_f(fTemp21) + fConst23 * fRec23[1] * fTemp21;
		float fTemp22 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.59836686f * fRec2[0], fRec3[0])));
		fRec24[0] = fTemp0 - fRec24[2] * T_german_bell_faustpower2_f(fTemp22) + fConst24 * fRec24[1] * fTemp22;
		float fTemp23 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6018672f * fRec2[0], fRec3[0])));
		fRec25[0] = fTemp0 - fRec25[2] * T_german_bell_faustpower2_f(fTemp23) + fConst25 * fRec25[1] * fTemp23;
		float fTemp24 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.60625005f * fRec2[0], fRec3[0])));
		fRec26[0] = fTemp0 - fRec26[2] * T_german_bell_faustpower2_f(fTemp24) + fConst26 * fRec26[1] * fTemp24;
		float fTemp25 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.61066765f * fRec2[0], fRec3[0])));
		fRec27[0] = fTemp0 - fRec27[2] * T_german_bell_faustpower2_f(fTemp25) + fConst27 * fRec27[1] * fTemp25;
		float fTemp26 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.66717404f * fRec2[0], fRec3[0])));
		fRec28[0] = fTemp0 - fRec28[2] * T_german_bell_faustpower2_f(fTemp26) + fConst28 * fRec28[1] * fTemp26;
		float fTemp27 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.671899f * fRec2[0], fRec3[0])));
		fRec29[0] = fTemp0 - fRec29[2] * T_german_bell_faustpower2_f(fTemp27) + fConst29 * fRec29[1] * fTemp27;
		float fTemp28 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6725783f * fRec2[0], fRec3[0])));
		fRec30[0] = fTemp0 - fRec30[2] * T_german_bell_faustpower2_f(fTemp28) + fConst30 * fRec30[1] * fTemp28;
		float fTemp29 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6729501f * fRec2[0], fRec3[0])));
		fRec31[0] = fTemp0 - fRec31[2] * T_german_bell_faustpower2_f(fTemp29) + fConst31 * fRec31[1] * fTemp29;
		float fTemp30 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.68881077f * fRec2[0], fRec3[0])));
		fRec32[0] = fTemp0 - fRec32[2] * T_german_bell_faustpower2_f(fTemp30) + fConst32 * fRec32[1] * fTemp30;
		float fTemp31 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6897131f * fRec2[0], fRec3[0])));
		fRec33[0] = fTemp0 - fRec33[2] * T_german_bell_faustpower2_f(fTemp31) + fConst33 * fRec33[1] * fTemp31;
		float fTemp32 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7041211f * fRec2[0], fRec3[0])));
		fRec34[0] = fTemp0 - fRec34[2] * T_german_bell_faustpower2_f(fTemp32) + fConst34 * fRec34[1] * fTemp32;
		float fTemp33 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7060448f * fRec2[0], fRec3[0])));
		fRec35[0] = fTemp0 - fRec35[2] * T_german_bell_faustpower2_f(fTemp33) + fConst35 * fRec35[1] * fTemp33;
		float fTemp34 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7061737f * fRec2[0], fRec3[0])));
		fRec36[0] = fTemp0 - fRec36[2] * T_german_bell_faustpower2_f(fTemp34) + fConst36 * fRec36[1] * fTemp34;
		float fTemp35 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7072793f * fRec2[0], fRec3[0])));
		fRec37[0] = fTemp0 - fRec37[2] * T_german_bell_faustpower2_f(fTemp35) + fConst37 * fRec37[1] * fTemp35;
		float fTemp36 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.78450525f * fRec2[0], fRec3[0])));
		fRec38[0] = fTemp0 - fRec38[2] * T_german_bell_faustpower2_f(fTemp36) + fConst38 * fRec38[1] * fTemp36;
		float fTemp37 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8086854f * fRec2[0], fRec3[0])));
		fRec39[0] = fTemp0 - fRec39[2] * T_german_bell_faustpower2_f(fTemp37) + fConst39 * fRec39[1] * fTemp37;
		float fTemp38 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8121957f * fRec2[0], fRec3[0])));
		fRec40[0] = fTemp0 - fRec40[2] * T_german_bell_faustpower2_f(fTemp38) + fConst40 * fRec40[1] * fTemp38;
		float fTemp39 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8529158f * fRec2[0], fRec3[0])));
		fRec41[0] = fTemp0 - fRec41[2] * T_german_bell_faustpower2_f(fTemp39) + fConst41 * fRec41[1] * fTemp39;
		float fTemp40 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.85609883f * fRec2[0], fRec3[0])));
		fRec42[0] = fTemp0 - fRec42[2] * T_german_bell_faustpower2_f(fTemp40) + fConst42 * fRec42[1] * fTemp40;
		float fTemp41 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.89432013f * fRec2[0], fRec3[0])));
		fRec43[0] = fTemp0 - fRec43[2] * T_german_bell_faustpower2_f(fTemp41) + fConst43 * fRec43[1] * fTemp41;
		float fTemp42 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9058673f * fRec2[0], fRec3[0])));
		fRec44[0] = fTemp0 - fRec44[2] * T_german_bell_faustpower2_f(fTemp42) + fConst44 * fRec44[1] * fTemp42;
		float fTemp43 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9070374f * fRec2[0], fRec3[0])));
		fRec45[0] = fTemp0 - fRec45[2] * T_german_bell_faustpower2_f(fTemp43) + fConst45 * fRec45[1] * fTemp43;
		float fTemp44 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.95336f * fRec2[0], fRec3[0])));
		fRec46[0] = fTemp0 - fRec46[2] * T_german_bell_faustpower2_f(fTemp44) + fConst46 * fRec46[1] * fTemp44;
		float fTemp45 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.95537794f * fRec2[0], fRec3[0])));
		fRec47[0] = fTemp0 - fRec47[2] * T_german_bell_faustpower2_f(fTemp45) + fConst47 * fRec47[1] * fTemp45;
		float fTemp46 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9623291f * fRec2[0], fRec3[0])));
		fRec48[0] = fTemp0 - fRec48[2] * T_german_bell_faustpower2_f(fTemp46) + fConst48 * fRec48[1] * fTemp46;
		float fTemp47 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9640693f * fRec2[0], fRec3[0])));
		fRec49[0] = fTemp0 - fRec49[2] * T_german_bell_faustpower2_f(fTemp47) + fConst49 * fRec49[1] * fTemp47;
		float fTemp48 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.96591866f * fRec2[0], fRec3[0])));
		fRec50[0] = fTemp0 - fRec50[2] * T_german_bell_faustpower2_f(fTemp48) + fConst50 * fRec50[1] * fTemp48;
		float fTemp49 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9766577f * fRec2[0], fRec3[0])));
		fRec51[0] = fTemp0 - fRec51[2] * T_german_bell_faustpower2_f(fTemp49) + fConst51 * fRec51[1] * fTemp49;
		float fTemp50 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.98775375f * fRec2[0], fRec3[0])));
		fRec52[0] = fTemp0 - fRec52[2] * T_german_bell_faustpower2_f(fTemp50) + fConst52 * fRec52[1] * fTemp50;
		float fTemp51 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9974764f * fRec2[0], fRec3[0])));
		fRec53[0] = fTemp0 - fRec53[2] * T_german_bell_faustpower2_f(fTemp51) + fConst53 * fRec53[1] * fTemp51;
		outputs[0] = static_cast<FAUSTFLOAT>(0.02f * ((fRec0[0] - fRec0[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(iTemp2, 349))] + (fRec5[0] - fRec5[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(1, iTemp2), 349))] + (fRec6[0] - fRec6[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(2, iTemp2), 349))] + (fRec7[0] - fRec7[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(3, iTemp2), 349))] + (fRec8[0] - fRec8[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(4, iTemp2), 349))] + (fRec9[0] - fRec9[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(5, iTemp2), 349))] + (fRec10[0] - fRec10[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(6, iTemp2), 349))] + (fRec11[0] - fRec11[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(7, iTemp2), 349))] + (fRec12[0] - fRec12[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(8, iTemp2), 349))] + (fRec13[0] - fRec13[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(9, iTemp2), 349))] + (fRec14[0] - fRec14[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(10, iTemp2), 349))] + (fRec15[0] - fRec15[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(11, iTemp2), 349))] + (fRec16[0] - fRec16[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(12, iTemp2), 349))] + (fRec17[0] - fRec17[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(13, iTemp2), 349))] + (fRec18[0] - fRec18[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(14, iTemp2), 349))] + (fRec19[0] - fRec19[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(15, iTemp2), 349))] + (fRec20[0] - fRec20[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(16, iTemp2), 349))] + (fRec21[0] - fRec21[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(17, iTemp2), 349))] + (fRec22[0] - fRec22[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(18, iTemp2), 349))] + (fRec23[0] - fRec23[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(19, iTemp2), 349))] + (fRec24[0] - fRec24[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(20, iTemp2), 349))] + (fRec25[0] - fRec25[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(21, iTemp2), 349))] + (fRec26[0] - fRec26[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(22, iTemp2), 349))] + (fRec27[0] - fRec27[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(23, iTemp2), 349))] + (fRec28[0] - fRec28[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(24, iTemp2), 349))] + (fRec29[0] - fRec29[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(25, iTemp2), 349))] + (fRec30[0] - fRec30[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(26, iTemp2), 349))] + (fRec31[0] - fRec31[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(27, iTemp2), 349))] + (fRec32[0] - fRec32[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(28, iTemp2), 349))] + (fRec33[0] - fRec33[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(29, iTemp2), 349))] + (fRec34[0] - fRec34[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(30, iTemp2), 349))] + (fRec35[0] - fRec35[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(31, iTemp2), 349))] + (fRec36[0] - fRec36[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(32, iTemp2), 349))] + (fRec37[0] - fRec37[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(33, iTemp2), 349))] + (fRec38[0] - fRec38[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(34, iTemp2), 349))] + (fRec39[0] - fRec39[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(35, iTemp2), 349))] + (fRec40[0] - fRec40[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(36, iTemp2), 349))] + (fRec41[0] - fRec41[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(37, iTemp2), 349))] + (fRec42[0] - fRec42[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(38, iTemp2), 349))] + (fRec43[0] - fRec43[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(39, iTemp2), 349))] + (fRec44[0] - fRec44[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(40, iTemp2), 349))] + (fRec45[0] - fRec45[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(41, iTemp2), 349))] + (fRec46[0] - fRec46[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(42, iTemp2), 349))] + (fRec47[0] - fRec47[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(43, iTemp2), 349))] + (fRec48[0] - fRec48[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(44, iTemp2), 349))] + (fRec49[0] - fRec49[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(45, iTemp2), 349))] + (fRec50[0] - fRec50[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(46, iTemp2), 349))] + (fRec51[0] - fRec51[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(47, iTemp2), 349))] + (fRec52[0] - fRec52[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(48, iTemp2), 349))] + (fRec53[0] - fRec53[2]) * ftbl0T_german_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(49, iTemp2), 349))]));
		fRec1[1] = fRec1[0];
		fRec2[1] = fRec2[0];
		fRec3[1] = fRec3[0];
		fRec0[2] = fRec0[1];
		fRec0[1] = fRec0[0];
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
		fRec21[2] = fRec21[1];
		fRec21[1] = fRec21[0];
		fRec22[2] = fRec22[1];
		fRec22[1] = fRec22[0];
		fRec23[2] = fRec23[1];
		fRec23[1] = fRec23[0];
		fRec24[2] = fRec24[1];
		fRec24[1] = fRec24[0];
		fRec25[2] = fRec25[1];
		fRec25[1] = fRec25[0];
		fRec26[2] = fRec26[1];
		fRec26[1] = fRec26[0];
		fRec27[2] = fRec27[1];
		fRec27[1] = fRec27[0];
		fRec28[2] = fRec28[1];
		fRec28[1] = fRec28[0];
		fRec29[2] = fRec29[1];
		fRec29[1] = fRec29[0];
		fRec30[2] = fRec30[1];
		fRec30[1] = fRec30[0];
		fRec31[2] = fRec31[1];
		fRec31[1] = fRec31[0];
		fRec32[2] = fRec32[1];
		fRec32[1] = fRec32[0];
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
		fRec41[2] = fRec41[1];
		fRec41[1] = fRec41[0];
		fRec42[2] = fRec42[1];
		fRec42[1] = fRec42[0];
		fRec43[2] = fRec43[1];
		fRec43[1] = fRec43[0];
		fRec44[2] = fRec44[1];
		fRec44[1] = fRec44[0];
		fRec45[2] = fRec45[1];
		fRec45[1] = fRec45[0];
		fRec46[2] = fRec46[1];
		fRec46[1] = fRec46[0];
		fRec47[2] = fRec47[1];
		fRec47[1] = fRec47[0];
		fRec48[2] = fRec48[1];
		fRec48[1] = fRec48[0];
		fRec49[2] = fRec49[1];
		fRec49[1] = fRec49[0];
		fRec50[2] = fRec50[1];
		fRec50[1] = fRec50[0];
		fRec51[2] = fRec51[1];
		fRec51[1] = fRec51[0];
		fRec52[2] = fRec52[1];
		fRec52[1] = fRec52[0];
		fRec53[2] = fRec53[1];
		fRec53[1] = fRec53[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
