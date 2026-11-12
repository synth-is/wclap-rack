/* ------------------------------------------------------------
name: "english-bell"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_english_bell -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_english_bell_H__
#define  __T_english_bell_H__

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
#define FAUSTCLASS T_english_bell
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

const static float fT_english_bellSIG0Wave0[350] = {0.694274f,0.471698f,0.17239f,0.585446f,0.397986f,0.919579f,0.531947f,0.100205f,0.639469f,0.672209f,0.144345f,0.416595f,0.124108f,0.380591f,0.256578f,0.646031f,0.8522f,0.0224376f,0.382762f,0.143925f,0.368691f,0.430556f,0.32318f,0.142956f,0.274521f,0.713824f,0.442794f,0.352473f,0.247756f,0.415152f,1.0f,0.401869f,0.197981f,0.27951f,0.210249f,0.36974f,0.369227f,0.155769f,0.272368f,0.335712f,0.31645f,0.714103f,0.285781f,0.22006f,0.827704f,0.206342f,0.180177f,0.311478f,0.197607f,0.575475f,0.473311f,0.587232f,0.50288f,0.337308f,0.304514f,0.429039f,0.351522f,0.341373f,0.175081f,0.561748f,0.439172f,0.323164f,0.540518f,0.536523f,0.0743865f,0.213417f,0.358012f,0.474494f,0.310274f,0.839413f,0.241372f,0.202343f,0.480634f,0.995685f,0.37374f,0.133998f,0.520674f,0.207514f,1.0f,0.101239f,0.279536f,0.185985f,0.436293f,0.62411f,0.334519f,0.283585f,0.179317f,0.353847f,0.449545f,0.574128f,0.135172f,0.538275f,0.476424f,0.832903f,0.164198f,0.188562f,0.135978f,0.390128f,0.131045f,0.312065f,0.142139f,0.0255901f,0.266947f,0.371607f,0.0168435f,0.0249468f,0.508917f,0.35441f,0.283348f,0.628155f,0.292478f,0.35835f,0.342569f,0.441237f,0.886699f,0.0174698f,0.00641843f,0.55532f,0.880129f,0.0306909f,0.290081f,0.248816f,0.981736f,0.324624f,0.213676f,0.432885f,0.0981559f,0.444149f,0.395554f,0.525069f,0.0771308f,0.0488804f,0.591321f,0.108044f,0.443802f,0.740318f,0.599438f,0.293093f,1.0f,0.141662f,0.910031f,0.226126f,0.299702f,0.341472f,0.0568061f,0.222494f,0.918718f,0.199478f,0.21662f,0.107759f,0.692324f,0.556336f,0.281718f,0.430832f,0.341656f,0.608095f,0.342129f,0.311312f,0.229953f,0.695087f,0.0761489f,0.349818f,0.361706f,0.577611f,0.147797f,0.327376f,0.465715f,0.342902f,0.521381f,0.836828f,0.241112f,0.284394f,0.539316f,0.143408f,0.51702f,1.0f,0.236336f,0.480333f,0.676744f,0.807582f,0.468621f,0.236953f,0.411604f,0.579251f,0.425098f,0.37599f,0.461176f,0.27653f,0.462368f,0.613004f,0.666849f,0.954715f,0.161507f,0.170433f,0.290461f,0.117303f,0.365133f,0.233794f,0.194568f,0.338874f,0.523381f,0.39835f,0.447839f,0.652891f,0.15708f,0.340354f,0.44217f,0.338764f,0.184397f,0.771607f,0.160502f,0.324487f,0.477499f,0.831519f,0.0168764f,0.154264f,0.201865f,0.237786f,0.390066f,0.880164f,0.284234f,0.162837f,0.437557f,0.227846f,0.39934f,1.0f,0.158107f,0.396903f,0.513029f,0.676457f,0.204282f,0.0895575f,0.55541f,0.732486f,0.125062f,0.171229f,0.081646f,0.0541394f,0.39616f,0.454288f,0.466863f,0.928842f,0.155393f,0.26285f,0.113453f,0.133163f,0.302021f,0.612616f,0.228392f,0.195617f,0.287236f,0.198762f,0.499884f,0.809139f,0.00929925f,0.0840299f,0.286446f,0.182112f,0.186044f,0.754138f,0.279556f,0.266948f,0.494291f,1.0f,0.321284f,0.0230981f,0.0375537f,0.262531f,0.602204f,0.489925f,0.633077f,0.407409f,0.422256f,0.0910641f,0.357935f,0.550179f,0.106192f,0.132366f,0.376231f,0.351151f,0.0420302f,0.0372183f,0.696335f,0.840821f,0.855235f,0.249248f,0.860011f,0.340481f,0.28558f,0.363039f,0.324122f,0.515699f,0.228131f,0.172592f,0.0188723f,0.168243f,0.995105f,0.741759f,0.107093f,0.070349f,0.136636f,0.0780455f,0.315748f,0.502201f,0.0190422f,0.033914f,0.225724f,0.160236f,0.184101f,0.564203f,0.247317f,0.284225f,0.327153f,0.651443f,0.593471f,0.0163899f,0.0141048f,0.52113f,1.0f,0.105109f,0.530936f,0.363724f,0.924809f,0.25041f,0.69288f,0.414122f,0.0793658f,0.347813f,0.441731f,0.476428f,0.0808834f,0.0581638f,0.557239f,0.556515f,0.746084f,0.582228f,0.177231f,0.158425f,0.850903f,0.755271f,0.673614f,0.31706f,0.0515386f,0.201898f,0.0577938f,0.232031f,0.734988f,0.141594f,0.267062f,0.145807f};
class T_english_bellSIG0 {
	
  private:
	
	int fT_english_bellSIG0Wave0_idx;
	int fSampleRate;
	
  public:
	
	int getNumInputsT_english_bellSIG0() {
		return 0;
	}
	int getNumOutputsT_english_bellSIG0() {
		return 1;
	}
	
	void instanceInitT_english_bellSIG0(int sample_rate) {
		fSampleRate = sample_rate;
		fT_english_bellSIG0Wave0_idx = 0;
	}
	
	void fillT_english_bellSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = faust_wrap_add(i1, 1)) {
			table[i1] = fT_english_bellSIG0Wave0[fT_english_bellSIG0Wave0_idx];
			fT_english_bellSIG0Wave0_idx = (faust_wrap_add(1, fT_english_bellSIG0Wave0_idx)) % 350;
		}
	}

};

static T_english_bellSIG0* newT_english_bellSIG0() { return (T_english_bellSIG0*)new T_english_bellSIG0(); }
static void deleteT_english_bellSIG0(T_english_bellSIG0* dsp) { delete dsp; }

static float T_english_bell_faustpower2_f(float value) {
	return value * value;
}
static float ftbl0T_english_bellSIG0[350];

class T_english_bell : public FaustDsp {
	
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
	T_english_bell() {
	}
	
	T_english_bell(const T_english_bell&) = default;
	
	virtual ~T_english_bell() = default;
	
	T_english_bell& operator=(const T_english_bell&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_english_bell -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "english-bell.dsp");
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
		m->declare("name", "english-bell");
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
		T_english_bellSIG0* sig0 = newT_english_bellSIG0();
		sig0->instanceInitT_english_bellSIG0(sample_rate);
		sig0->fillT_english_bellSIG0(350, ftbl0T_english_bellSIG0);
		deleteT_english_bellSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 1.0f / fConst0;
		fConst2 = 44.1f / fConst0;
		fConst3 = 1.0f - fConst2;
		fConst4 = 2.0f * std::cos(1630.0405f / fConst0);
		fConst5 = 2.0f * std::cos(1645.2834f / fConst0);
		fConst6 = 2.0f * std::cos(3099.6902f / fConst0);
		fConst7 = 2.0f * std::cos(3106.8591f / fConst0);
		fConst8 = 2.0f * std::cos(3789.0623f / fConst0);
		fConst9 = 2.0f * std::cos(3799.706f / fConst0);
		fConst10 = 2.0f * std::cos(4552.5825f / fConst0);
		fConst11 = 2.0f * std::cos(4553.908f / fConst0);
		fConst12 = 2.0f * std::cos(4944.7725f / fConst0);
		fConst13 = 2.0f * std::cos(5855.2563f / fConst0);
		fConst14 = 2.0f * std::cos(5861.244f / fConst0);
		fConst15 = 2.0f * std::cos(5927.7393f / fConst0);
		fConst16 = 2.0f * std::cos(5947.242f / fConst0);
		fConst17 = 2.0f * std::cos(5950.8613f / fConst0);
		fConst18 = 2.0f * std::cos(5986.506f / fConst0);
		fConst19 = 2.0f * std::cos(6435.4897f / fConst0);
		fConst20 = 2.0f * std::cos(6502.2173f / fConst0);
		fConst21 = 2.0f * std::cos(7612.6445f / fConst0);
		fConst22 = 2.0f * std::cos(7616.54f / fConst0);
		fConst23 = 2.0f * std::cos(7767.7764f / fConst0);
		fConst24 = 2.0f * std::cos(8420.788f / fConst0);
		fConst25 = 2.0f * std::cos(8426.694f / fConst0);
		fConst26 = 2.0f * std::cos(9127.144f / fConst0);
		fConst27 = 2.0f * std::cos(9131.855f / fConst0);
		fConst28 = 2.0f * std::cos(9134.307f / fConst0);
		fConst29 = 2.0f * std::cos(9157.429f / fConst0);
		fConst30 = 2.0f * std::cos(9168.612f / fConst0);
		fConst31 = 2.0f * std::cos(9177.849f / fConst0);
		fConst32 = 2.0f * std::cos(9316.77f / fConst0);
		fConst33 = 2.0f * std::cos(9318.215f / fConst0);
		fConst34 = 2.0f * std::cos(9547.614f / fConst0);
		fConst35 = 2.0f * std::cos(9721.784f / fConst0);
		fConst36 = 2.0f * std::cos(9839.154f / fConst0);
		fConst37 = 2.0f * std::cos(9908.27f / fConst0);
		fConst38 = 2.0f * std::cos(10665.519f / fConst0);
		fConst39 = 2.0f * std::cos(11476.929f / fConst0);
		fConst40 = 2.0f * std::cos(11891.306f / fConst0);
		fConst41 = 2.0f * std::cos(11913.484f / fConst0);
		fConst42 = 2.0f * std::cos(11937.173f / fConst0);
		fConst43 = 2.0f * std::cos(11990.14f / fConst0);
		fConst44 = 2.0f * std::cos(12591.189f / fConst0);
		fConst45 = 2.0f * std::cos(12598.227f / fConst0);
		fConst46 = 2.0f * std::cos(12603.379f / fConst0);
		fConst47 = 2.0f * std::cos(12604.007f / fConst0);
		fConst48 = 2.0f * std::cos(12989.983f / fConst0);
		fConst49 = 2.0f * std::cos(13084.23f / fConst0);
		fConst50 = 2.0f * std::cos(13111.813f / fConst0);
		fConst51 = 2.0f * std::cos(13323.871f / fConst0);
		fConst52 = 2.0f * std::cos(13381.865f / fConst0);
		fConst53 = 2.0f * std::cos(13403.4795f / fConst0);
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(1e+01f);
		fHslider1 = static_cast<FAUSTFLOAT>(1.0f);
		fHslider2 = static_cast<FAUSTFLOAT>(2.5f);
		fHslider3 = static_cast<FAUSTFLOAT>(3.0f);
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
	
	virtual T_english_bell* clone() {
		return new T_english_bell(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("english-bell");
		ui_interface->addHorizontalSlider("exPos", &fHslider3, FAUSTFLOAT(3.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(6.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("t60", &fHslider0, FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f), FAUSTFLOAT(6e+01f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("t60DecayRatio", &fHslider1, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f), FAUSTFLOAT(5.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("t60DecaySlope", &fHslider2, FAUSTFLOAT(2.5f), FAUSTFLOAT(0.1f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
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
		float fTemp1 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.121396415f * fRec2[0], fRec3[0])));
		fRec0[0] = fTemp0 - fRec0[2] * T_english_bell_faustpower2_f(fTemp1) + fConst4 * fRec0[1] * fTemp1;
		fRec4[0] = fSlow3 + fConst3 * fRec4[1];
		int iTemp2 = faust_wrap_mul(50, static_cast<int>(fRec4[0]));
		float fTemp3 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.12253163f * fRec2[0], fRec3[0])));
		fRec5[0] = fTemp0 - fRec5[2] * T_english_bell_faustpower2_f(fTemp3) + fConst5 * fRec5[1] * fTemp3;
		float fTemp4 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.2308478f * fRec2[0], fRec3[0])));
		fRec6[0] = fTemp0 - fRec6[2] * T_english_bell_faustpower2_f(fTemp4) + fConst6 * fRec6[1] * fTemp4;
		float fTemp5 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.23138173f * fRec2[0], fRec3[0])));
		fRec7[0] = fTemp0 - fRec7[2] * T_english_bell_faustpower2_f(fTemp5) + fConst7 * fRec7[1] * fTemp5;
		float fTemp6 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.28218845f * fRec2[0], fRec3[0])));
		fRec8[0] = fTemp0 - fRec8[2] * T_english_bell_faustpower2_f(fTemp6) + fConst8 * fRec8[1] * fTemp6;
		float fTemp7 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.28298113f * fRec2[0], fRec3[0])));
		fRec9[0] = fTemp0 - fRec9[2] * T_english_bell_faustpower2_f(fTemp7) + fConst9 * fRec9[1] * fTemp7;
		float fTemp8 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.33905122f * fRec2[0], fRec3[0])));
		fRec10[0] = fTemp0 - fRec10[2] * T_english_bell_faustpower2_f(fTemp8) + fConst10 * fRec10[1] * fTemp8;
		float fTemp9 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.33914995f * fRec2[0], fRec3[0])));
		fRec11[0] = fTemp0 - fRec11[2] * T_english_bell_faustpower2_f(fTemp9) + fConst11 * fRec11[1] * fTemp9;
		float fTemp10 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.36825937f * fRec2[0], fRec3[0])));
		fRec12[0] = fTemp0 - fRec12[2] * T_english_bell_faustpower2_f(fTemp10) + fConst12 * fRec12[1] * fTemp10;
		float fTemp11 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.43606716f * fRec2[0], fRec3[0])));
		fRec13[0] = fTemp0 - fRec13[2] * T_english_bell_faustpower2_f(fTemp11) + fConst13 * fRec13[1] * fTemp11;
		float fTemp12 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.43651313f * fRec2[0], fRec3[0])));
		fRec14[0] = fTemp0 - fRec14[2] * T_english_bell_faustpower2_f(fTemp12) + fConst14 * fRec14[1] * fTemp12;
		float fTemp13 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.4414653f * fRec2[0], fRec3[0])));
		fRec15[0] = fTemp0 - fRec15[2] * T_english_bell_faustpower2_f(fTemp13) + fConst15 * fRec15[1] * fTemp13;
		float fTemp14 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.44291776f * fRec2[0], fRec3[0])));
		fRec16[0] = fTemp0 - fRec16[2] * T_english_bell_faustpower2_f(fTemp14) + fConst16 * fRec16[1] * fTemp14;
		float fTemp15 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.4431873f * fRec2[0], fRec3[0])));
		fRec17[0] = fTemp0 - fRec17[2] * T_english_bell_faustpower2_f(fTemp15) + fConst17 * fRec17[1] * fTemp15;
		float fTemp16 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.4458419f * fRec2[0], fRec3[0])));
		fRec18[0] = fTemp0 - fRec18[2] * T_english_bell_faustpower2_f(fTemp16) + fConst18 * fRec18[1] * fTemp16;
		float fTemp17 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.47927976f * fRec2[0], fRec3[0])));
		fRec19[0] = fTemp0 - fRec19[2] * T_english_bell_faustpower2_f(fTemp17) + fConst19 * fRec19[1] * fTemp17;
		float fTemp18 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.48424923f * fRec2[0], fRec3[0])));
		fRec20[0] = fTemp0 - fRec20[2] * T_english_bell_faustpower2_f(fTemp18) + fConst20 * fRec20[1] * fTemp18;
		float fTemp19 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.56694776f * fRec2[0], fRec3[0])));
		fRec21[0] = fTemp0 - fRec21[2] * T_english_bell_faustpower2_f(fTemp19) + fConst21 * fRec21[1] * fTemp19;
		float fTemp20 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.56723785f * fRec2[0], fRec3[0])));
		fRec22[0] = fTemp0 - fRec22[2] * T_english_bell_faustpower2_f(fTemp20) + fConst22 * fRec22[1] * fTemp20;
		float fTemp21 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.5785011f * fRec2[0], fRec3[0])));
		fRec23[0] = fTemp0 - fRec23[2] * T_english_bell_faustpower2_f(fTemp21) + fConst23 * fRec23[1] * fTemp21;
		float fTemp22 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6271338f * fRec2[0], fRec3[0])));
		fRec24[0] = fTemp0 - fRec24[2] * T_english_bell_faustpower2_f(fTemp22) + fConst24 * fRec24[1] * fTemp22;
		float fTemp23 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.62757367f * fRec2[0], fRec3[0])));
		fRec25[0] = fTemp0 - fRec25[2] * T_english_bell_faustpower2_f(fTemp23) + fConst25 * fRec25[1] * fTemp23;
		float fTemp24 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.67973924f * fRec2[0], fRec3[0])));
		fRec26[0] = fTemp0 - fRec26[2] * T_english_bell_faustpower2_f(fTemp24) + fConst26 * fRec26[1] * fTemp24;
		float fTemp25 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6800902f * fRec2[0], fRec3[0])));
		fRec27[0] = fTemp0 - fRec27[2] * T_english_bell_faustpower2_f(fTemp25) + fConst27 * fRec27[1] * fTemp25;
		float fTemp26 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6802727f * fRec2[0], fRec3[0])));
		fRec28[0] = fTemp0 - fRec28[2] * T_english_bell_faustpower2_f(fTemp26) + fConst28 * fRec28[1] * fTemp26;
		float fTemp27 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.68199474f * fRec2[0], fRec3[0])));
		fRec29[0] = fTemp0 - fRec29[2] * T_english_bell_faustpower2_f(fTemp27) + fConst29 * fRec29[1] * fTemp27;
		float fTemp28 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.68282765f * fRec2[0], fRec3[0])));
		fRec30[0] = fTemp0 - fRec30[2] * T_english_bell_faustpower2_f(fTemp28) + fConst30 * fRec30[1] * fTemp28;
		float fTemp29 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6835155f * fRec2[0], fRec3[0])));
		fRec31[0] = fTemp0 - fRec31[2] * T_english_bell_faustpower2_f(fTemp29) + fConst31 * fRec31[1] * fTemp29;
		float fTemp30 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6938616f * fRec2[0], fRec3[0])));
		fRec32[0] = fTemp0 - fRec32[2] * T_english_bell_faustpower2_f(fTemp30) + fConst32 * fRec32[1] * fTemp30;
		float fTemp31 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.69396925f * fRec2[0], fRec3[0])));
		fRec33[0] = fTemp0 - fRec33[2] * T_english_bell_faustpower2_f(fTemp31) + fConst33 * fRec33[1] * fTemp31;
		float fTemp32 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7110536f * fRec2[0], fRec3[0])));
		fRec34[0] = fTemp0 - fRec34[2] * T_english_bell_faustpower2_f(fTemp32) + fConst34 * fRec34[1] * fTemp32;
		float fTemp33 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.72402483f * fRec2[0], fRec3[0])));
		fRec35[0] = fTemp0 - fRec35[2] * T_english_bell_faustpower2_f(fTemp33) + fConst35 * fRec35[1] * fTemp33;
		float fTemp34 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.73276585f * fRec2[0], fRec3[0])));
		fRec36[0] = fTemp0 - fRec36[2] * T_english_bell_faustpower2_f(fTemp34) + fConst36 * fRec36[1] * fTemp34;
		float fTemp35 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7379132f * fRec2[0], fRec3[0])));
		fRec37[0] = fTemp0 - fRec37[2] * T_english_bell_faustpower2_f(fTemp35) + fConst37 * fRec37[1] * fTemp35;
		float fTemp36 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.79430896f * fRec2[0], fRec3[0])));
		fRec38[0] = fTemp0 - fRec38[2] * T_english_bell_faustpower2_f(fTemp36) + fConst38 * fRec38[1] * fTemp36;
		float fTemp37 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.85473835f * fRec2[0], fRec3[0])));
		fRec39[0] = fTemp0 - fRec39[2] * T_english_bell_faustpower2_f(fTemp37) + fConst39 * fRec39[1] * fTemp37;
		float fTemp38 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8855988f * fRec2[0], fRec3[0])));
		fRec40[0] = fTemp0 - fRec40[2] * T_english_bell_faustpower2_f(fTemp38) + fConst40 * fRec40[1] * fTemp38;
		float fTemp39 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8872506f * fRec2[0], fRec3[0])));
		fRec41[0] = fTemp0 - fRec41[2] * T_english_bell_faustpower2_f(fTemp39) + fConst41 * fRec41[1] * fTemp39;
		float fTemp40 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8890147f * fRec2[0], fRec3[0])));
		fRec42[0] = fTemp0 - fRec42[2] * T_english_bell_faustpower2_f(fTemp40) + fConst42 * fRec42[1] * fTemp40;
		float fTemp41 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8929594f * fRec2[0], fRec3[0])));
		fRec43[0] = fTemp0 - fRec43[2] * T_english_bell_faustpower2_f(fTemp41) + fConst43 * fRec43[1] * fTemp41;
		float fTemp42 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.93772227f * fRec2[0], fRec3[0])));
		fRec44[0] = fTemp0 - fRec44[2] * T_english_bell_faustpower2_f(fTemp42) + fConst44 * fRec44[1] * fTemp42;
		float fTemp43 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.93824637f * fRec2[0], fRec3[0])));
		fRec45[0] = fTemp0 - fRec45[2] * T_english_bell_faustpower2_f(fTemp43) + fConst45 * fRec45[1] * fTemp43;
		float fTemp44 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.93863004f * fRec2[0], fRec3[0])));
		fRec46[0] = fTemp0 - fRec46[2] * T_english_bell_faustpower2_f(fTemp44) + fConst46 * fRec46[1] * fTemp44;
		float fTemp45 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.93867683f * fRec2[0], fRec3[0])));
		fRec47[0] = fTemp0 - fRec47[2] * T_english_bell_faustpower2_f(fTemp45) + fConst47 * fRec47[1] * fTemp45;
		float fTemp46 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.96742225f * fRec2[0], fRec3[0])));
		fRec48[0] = fTemp0 - fRec48[2] * T_english_bell_faustpower2_f(fTemp46) + fConst48 * fRec48[1] * fTemp46;
		float fTemp47 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9744413f * fRec2[0], fRec3[0])));
		fRec49[0] = fTemp0 - fRec49[2] * T_english_bell_faustpower2_f(fTemp47) + fConst49 * fRec49[1] * fTemp47;
		float fTemp48 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9764955f * fRec2[0], fRec3[0])));
		fRec50[0] = fTemp0 - fRec50[2] * T_english_bell_faustpower2_f(fTemp48) + fConst50 * fRec50[1] * fTemp48;
		float fTemp49 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9922884f * fRec2[0], fRec3[0])));
		fRec51[0] = fTemp0 - fRec51[2] * T_english_bell_faustpower2_f(fTemp49) + fConst51 * fRec51[1] * fTemp49;
		float fTemp50 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9966075f * fRec2[0], fRec3[0])));
		fRec52[0] = fTemp0 - fRec52[2] * T_english_bell_faustpower2_f(fTemp50) + fConst52 * fRec52[1] * fTemp50;
		float fTemp51 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.99821717f * fRec2[0], fRec3[0])));
		fRec53[0] = fTemp0 - fRec53[2] * T_english_bell_faustpower2_f(fTemp51) + fConst53 * fRec53[1] * fTemp51;
		outputs[0] = static_cast<FAUSTFLOAT>(0.02f * ((fRec0[0] - fRec0[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(iTemp2, 349))] + (fRec5[0] - fRec5[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(1, iTemp2), 349))] + (fRec6[0] - fRec6[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(2, iTemp2), 349))] + (fRec7[0] - fRec7[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(3, iTemp2), 349))] + (fRec8[0] - fRec8[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(4, iTemp2), 349))] + (fRec9[0] - fRec9[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(5, iTemp2), 349))] + (fRec10[0] - fRec10[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(6, iTemp2), 349))] + (fRec11[0] - fRec11[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(7, iTemp2), 349))] + (fRec12[0] - fRec12[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(8, iTemp2), 349))] + (fRec13[0] - fRec13[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(9, iTemp2), 349))] + (fRec14[0] - fRec14[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(10, iTemp2), 349))] + (fRec15[0] - fRec15[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(11, iTemp2), 349))] + (fRec16[0] - fRec16[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(12, iTemp2), 349))] + (fRec17[0] - fRec17[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(13, iTemp2), 349))] + (fRec18[0] - fRec18[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(14, iTemp2), 349))] + (fRec19[0] - fRec19[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(15, iTemp2), 349))] + (fRec20[0] - fRec20[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(16, iTemp2), 349))] + (fRec21[0] - fRec21[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(17, iTemp2), 349))] + (fRec22[0] - fRec22[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(18, iTemp2), 349))] + (fRec23[0] - fRec23[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(19, iTemp2), 349))] + (fRec24[0] - fRec24[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(20, iTemp2), 349))] + (fRec25[0] - fRec25[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(21, iTemp2), 349))] + (fRec26[0] - fRec26[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(22, iTemp2), 349))] + (fRec27[0] - fRec27[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(23, iTemp2), 349))] + (fRec28[0] - fRec28[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(24, iTemp2), 349))] + (fRec29[0] - fRec29[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(25, iTemp2), 349))] + (fRec30[0] - fRec30[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(26, iTemp2), 349))] + (fRec31[0] - fRec31[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(27, iTemp2), 349))] + (fRec32[0] - fRec32[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(28, iTemp2), 349))] + (fRec33[0] - fRec33[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(29, iTemp2), 349))] + (fRec34[0] - fRec34[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(30, iTemp2), 349))] + (fRec35[0] - fRec35[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(31, iTemp2), 349))] + (fRec36[0] - fRec36[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(32, iTemp2), 349))] + (fRec37[0] - fRec37[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(33, iTemp2), 349))] + (fRec38[0] - fRec38[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(34, iTemp2), 349))] + (fRec39[0] - fRec39[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(35, iTemp2), 349))] + (fRec40[0] - fRec40[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(36, iTemp2), 349))] + (fRec41[0] - fRec41[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(37, iTemp2), 349))] + (fRec42[0] - fRec42[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(38, iTemp2), 349))] + (fRec43[0] - fRec43[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(39, iTemp2), 349))] + (fRec44[0] - fRec44[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(40, iTemp2), 349))] + (fRec45[0] - fRec45[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(41, iTemp2), 349))] + (fRec46[0] - fRec46[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(42, iTemp2), 349))] + (fRec47[0] - fRec47[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(43, iTemp2), 349))] + (fRec48[0] - fRec48[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(44, iTemp2), 349))] + (fRec49[0] - fRec49[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(45, iTemp2), 349))] + (fRec50[0] - fRec50[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(46, iTemp2), 349))] + (fRec51[0] - fRec51[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(47, iTemp2), 349))] + (fRec52[0] - fRec52[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(48, iTemp2), 349))] + (fRec53[0] - fRec53[2]) * ftbl0T_english_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(49, iTemp2), 349))]));
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
