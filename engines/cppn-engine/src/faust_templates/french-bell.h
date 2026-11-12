/* ------------------------------------------------------------
name: "french-bell"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_french_bell -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_french_bell_H__
#define  __T_french_bell_H__

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
#define FAUSTCLASS T_french_bell
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

const static float fT_french_bellSIG0Wave0[350] = {0.97289f,0.542393f,0.495832f,0.897966f,0.552367f,0.557895f,0.614213f,0.353694f,0.436039f,0.441024f,0.674913f,0.566754f,0.755008f,0.69903f,0.164398f,0.91004f,0.628373f,0.201718f,0.429517f,0.503715f,0.871174f,0.106886f,0.761173f,0.673602f,0.291937f,0.58859f,0.31528f,0.413081f,0.274464f,0.494062f,0.696121f,0.61201f,0.382757f,0.995113f,0.228806f,0.198449f,0.595847f,0.306263f,0.252397f,0.0883567f,0.236086f,1.0f,0.245278f,0.379388f,0.198824f,0.548892f,0.492764f,0.420871f,0.794637f,0.605634f,1.0f,0.604159f,0.399841f,0.799347f,0.507187f,0.50981f,0.477383f,0.310226f,0.426976f,0.437623f,0.735712f,0.630728f,0.625785f,0.651168f,0.277738f,0.850858f,0.578079f,0.645354f,0.3837f,0.370598f,0.782542f,0.181325f,0.614391f,0.740684f,0.342441f,0.586186f,0.286909f,0.405197f,0.259215f,0.566983f,0.748219f,0.655898f,0.36826f,0.940814f,0.336157f,0.413702f,0.561557f,0.402176f,0.117698f,0.329369f,0.254571f,0.870706f,0.260981f,0.274122f,0.206247f,0.6453f,0.400758f,0.363622f,0.636834f,0.584566f,0.975603f,0.616937f,0.295401f,0.650447f,0.464221f,0.465057f,0.312467f,0.238358f,0.383695f,0.399674f,0.753912f,0.662012f,0.504268f,0.599248f,0.378665f,0.725363f,0.493214f,1.0f,0.332836f,0.265889f,0.674073f,0.320401f,0.440018f,0.769782f,0.316419f,0.529968f,0.380509f,0.578676f,0.249013f,0.591384f,0.761717f,0.687057f,0.324437f,0.818864f,0.505369f,0.672485f,0.461783f,0.426198f,0.0678875f,0.435329f,0.347955f,0.708394f,0.293322f,0.328986f,0.258254f,0.893512f,0.320131f,0.433554f,0.459302f,0.542213f,0.817241f,0.51623f,0.205302f,0.467354f,0.388683f,0.388216f,0.171262f,0.150865f,0.29172f,0.311414f,0.658876f,0.570647f,0.383619f,0.502455f,0.364114f,0.532313f,0.352989f,1.0f,0.261984f,0.219591f,0.544031f,0.337199f,0.279173f,0.668303f,0.208439f,0.39923f,0.418674f,0.648618f,0.234133f,0.504729f,0.645347f,0.572851f,0.232828f,0.614292f,0.485272f,0.666264f,0.31657f,0.320355f,0.191421f,0.340131f,0.342069f,0.538371f,0.281131f,0.393115f,0.251394f,0.890725f,0.310644f,0.5037f,0.29909f,0.442478f,0.733128f,0.455217f,0.199322f,0.315699f,0.375856f,0.37649f,0.029145f,0.0200283f,0.279578f,0.3168f,0.655957f,0.546843f,0.349666f,0.470249f,0.353765f,0.286794f,0.180185f,1.0f,0.210831f,0.280133f,0.535853f,0.376488f,0.15367f,0.634745f,0.0510449f,0.485575f,0.593111f,0.917884f,0.380477f,0.422925f,0.599373f,0.311421f,0.135654f,0.359954f,0.295748f,0.474439f,0.353339f,0.116743f,0.454313f,0.112858f,0.35931f,0.483897f,0.301608f,0.577342f,0.262663f,0.794986f,0.54958f,0.808086f,0.152511f,0.439591f,0.535941f,0.308018f,0.419837f,0.579191f,0.250428f,0.25212f,0.10286f,0.288332f,0.599679f,0.665108f,0.636285f,0.495234f,0.251613f,0.208527f,0.0939072f,0.458415f,0.318952f,0.23535f,0.215189f,0.313412f,0.0472787f,0.0386893f,0.0863359f,0.222346f,0.361511f,0.997037f,0.238509f,0.38214f,1.0f,0.203554f,0.472087f,0.509015f,0.206371f,0.441013f,0.794008f,0.971623f,0.796384f,0.55043f,0.68778f,0.554298f,0.0436545f,0.0595577f,0.214685f,0.58147f,0.27732f,0.368466f,0.121382f,0.152739f,0.0782244f,0.190497f,0.316269f,0.180456f,0.405196f,0.518716f,0.159365f,0.165808f,0.194372f,0.614764f,0.553415f,0.644794f,0.44178f,0.44149f,0.221588f,1.0f,0.579371f,0.606339f,0.529167f,0.0214475f,0.417046f,0.663152f,0.894841f,0.475752f,0.168289f,0.46859f,0.304604f,0.666369f,0.308337f,0.295091f,0.768945f,0.350911f,0.49068f,0.516584f,0.400117f,0.820687f,0.545486f,0.709357f,0.509759f,0.472006f,0.152455f,0.981265f,0.459134f,0.698666f,0.247154f,0.623253f,0.255508f,0.830815f,0.088709f,0.126082f,0.0770623f,0.263328f};
class T_french_bellSIG0 {
	
  private:
	
	int fT_french_bellSIG0Wave0_idx;
	int fSampleRate;
	
  public:
	
	int getNumInputsT_french_bellSIG0() {
		return 0;
	}
	int getNumOutputsT_french_bellSIG0() {
		return 1;
	}
	
	void instanceInitT_french_bellSIG0(int sample_rate) {
		fSampleRate = sample_rate;
		fT_french_bellSIG0Wave0_idx = 0;
	}
	
	void fillT_french_bellSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = faust_wrap_add(i1, 1)) {
			table[i1] = fT_french_bellSIG0Wave0[fT_french_bellSIG0Wave0_idx];
			fT_french_bellSIG0Wave0_idx = (faust_wrap_add(1, fT_french_bellSIG0Wave0_idx)) % 350;
		}
	}

};

static T_french_bellSIG0* newT_french_bellSIG0() { return (T_french_bellSIG0*)new T_french_bellSIG0(); }
static void deleteT_french_bellSIG0(T_french_bellSIG0* dsp) { delete dsp; }

static float T_french_bell_faustpower2_f(float value) {
	return value * value;
}
static float ftbl0T_french_bellSIG0[350];

class T_french_bell : public FaustDsp {
	
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
	T_french_bell() {
	}
	
	T_french_bell(const T_french_bell&) = default;
	
	virtual ~T_french_bell() = default;
	
	T_french_bell& operator=(const T_french_bell&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_french_bell -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "french-bell.dsp");
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
		m->declare("name", "french-bell");
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
		T_french_bellSIG0* sig0 = newT_french_bellSIG0();
		sig0->instanceInitT_french_bellSIG0(sample_rate);
		sig0->fillT_french_bellSIG0(350, ftbl0T_french_bellSIG0);
		deleteT_french_bellSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 1.0f / fConst0;
		fConst2 = 44.1f / fConst0;
		fConst3 = 1.0f - fConst2;
		fConst4 = 2.0f * std::cos(2758.8022f / fConst0);
		fConst5 = 2.0f * std::cos(2766.5178f / fConst0);
		fConst6 = 2.0f * std::cos(3810.4502f / fConst0);
		fConst7 = 2.0f * std::cos(3843.9019f / fConst0);
		fConst8 = 2.0f * std::cos(4458.6553f / fConst0);
		fConst9 = 2.0f * std::cos(4460.2886f / fConst0);
		fConst10 = 2.0f * std::cos(5384.476f / fConst0);
		fConst11 = 2.0f * std::cos(5648.904f / fConst0);
		fConst12 = 2.0f * std::cos(6687.634f / fConst0);
		fConst13 = 2.0f * std::cos(6690.0215f / fConst0);
		fConst14 = 2.0f * std::cos(6742.0464f / fConst0);
		fConst15 = 2.0f * std::cos(6750.843f / fConst0);
		fConst16 = 2.0f * std::cos(6764.3516f / fConst0);
		fConst17 = 2.0f * std::cos(7196.886f / fConst0);
		fConst18 = 2.0f * std::cos(7218.7515f / fConst0);
		fConst19 = 2.0f * std::cos(7521.538f / fConst0);
		fConst20 = 2.0f * std::cos(7539.257f / fConst0);
		fConst21 = 2.0f * std::cos(8719.176f / fConst0);
		fConst22 = 2.0f * std::cos(8797.465f / fConst0);
		fConst23 = 2.0f * std::cos(8811.351f / fConst0);
		fConst24 = 2.0f * std::cos(9500.553f / fConst0);
		fConst25 = 2.0f * std::cos(9506.836f / fConst0);
		fConst26 = 2.0f * std::cos(10005.784f / fConst0);
		fConst27 = 2.0f * std::cos(10011.313f / fConst0);
		fConst28 = 2.0f * std::cos(10451.765f / fConst0);
		fConst29 = 2.0f * std::cos(10470.74f / fConst0);
		fConst30 = 2.0f * std::cos(10626.877f / fConst0);
		fConst31 = 2.0f * std::cos(10642.2705f / fConst0);
		fConst32 = 2.0f * std::cos(11340.081f / fConst0);
		fConst33 = 2.0f * std::cos(11365.277f / fConst0);
		fConst34 = 2.0f * std::cos(11435.837f / fConst0);
		fConst35 = 2.0f * std::cos(11605.923f / fConst0);
		fConst36 = 2.0f * std::cos(12005.471f / fConst0);
		fConst37 = 2.0f * std::cos(12112.097f / fConst0);
		fConst38 = 2.0f * std::cos(12786.722f / fConst0);
		fConst39 = 2.0f * std::cos(12809.719f / fConst0);
		fConst40 = 2.0f * std::cos(12909.81f / fConst0);
		fConst41 = 2.0f * std::cos(12916.155f / fConst0);
		fConst42 = 2.0f * std::cos(13060.795f / fConst0);
		fConst43 = 2.0f * std::cos(13142.916f / fConst0);
		fConst44 = 2.0f * std::cos(13293.838f / fConst0);
		fConst45 = 2.0f * std::cos(13364.398f / fConst0);
		fConst46 = 2.0f * std::cos(13884.96f / fConst0);
		fConst47 = 2.0f * std::cos(13911.286f / fConst0);
		fConst48 = 2.0f * std::cos(14427.136f / fConst0);
		fConst49 = 2.0f * std::cos(14439.263f / fConst0);
		fConst50 = 2.0f * std::cos(14444.8545f / fConst0);
		fConst51 = 2.0f * std::cos(14499.204f / fConst0);
		fConst52 = 2.0f * std::cos(15107.542f / fConst0);
		fConst53 = 2.0f * std::cos(15136.131f / fConst0);
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
	
	virtual T_french_bell* clone() {
		return new T_french_bell(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("french-bell");
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
		float fTemp1 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.17182992f * fRec2[0], fRec3[0])));
		fRec0[0] = fTemp0 - fRec0[2] * T_french_bell_faustpower2_f(fTemp1) + fConst4 * fRec0[1] * fTemp1;
		fRec4[0] = fSlow3 + fConst3 * fRec4[1];
		int iTemp2 = faust_wrap_mul(50, static_cast<int>(fRec4[0]));
		float fTemp3 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.17231049f * fRec2[0], fRec3[0])));
		fRec5[0] = fTemp0 - fRec5[2] * T_french_bell_faustpower2_f(fTemp3) + fConst5 * fRec5[1] * fTemp3;
		float fTemp4 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.23733103f * fRec2[0], fRec3[0])));
		fRec6[0] = fTemp0 - fRec6[2] * T_french_bell_faustpower2_f(fTemp4) + fConst6 * fRec6[1] * fTemp4;
		float fTemp5 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.23941454f * fRec2[0], fRec3[0])));
		fRec7[0] = fTemp0 - fRec7[2] * T_french_bell_faustpower2_f(fTemp5) + fConst7 * fRec7[1] * fTemp5;
		float fTemp6 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.277704f * fRec2[0], fRec3[0])));
		fRec8[0] = fTemp0 - fRec8[2] * T_french_bell_faustpower2_f(fTemp6) + fConst8 * fRec8[1] * fTemp6;
		float fTemp7 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.27780575f * fRec2[0], fRec3[0])));
		fRec9[0] = fTemp0 - fRec9[2] * T_french_bell_faustpower2_f(fTemp7) + fConst9 * fRec9[1] * fTemp7;
		float fTemp8 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.33536807f * fRec2[0], fRec3[0])));
		fRec10[0] = fTemp0 - fRec10[2] * T_french_bell_faustpower2_f(fTemp8) + fConst10 * fRec10[1] * fTemp8;
		float fTemp9 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.35183775f * fRec2[0], fRec3[0])));
		fRec11[0] = fTemp0 - fRec11[2] * T_french_bell_faustpower2_f(fTemp9) + fConst11 * fRec11[1] * fTemp9;
		float fTemp10 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.41653427f * fRec2[0], fRec3[0])));
		fRec12[0] = fTemp0 - fRec12[2] * T_french_bell_faustpower2_f(fTemp10) + fConst12 * fRec12[1] * fTemp10;
		float fTemp11 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.41668296f * fRec2[0], fRec3[0])));
		fRec13[0] = fTemp0 - fRec13[2] * T_french_bell_faustpower2_f(fTemp11) + fConst13 * fRec13[1] * fTemp11;
		float fTemp12 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.4199233f * fRec2[0], fRec3[0])));
		fRec14[0] = fTemp0 - fRec14[2] * T_french_bell_faustpower2_f(fTemp12) + fConst14 * fRec14[1] * fTemp12;
		float fTemp13 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.4204712f * fRec2[0], fRec3[0])));
		fRec15[0] = fTemp0 - fRec15[2] * T_french_bell_faustpower2_f(fTemp13) + fConst15 * fRec15[1] * fTemp13;
		float fTemp14 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.42131257f * fRec2[0], fRec3[0])));
		fRec16[0] = fTemp0 - fRec16[2] * T_french_bell_faustpower2_f(fTemp14) + fConst16 * fRec16[1] * fTemp14;
		float fTemp15 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.44825265f * fRec2[0], fRec3[0])));
		fRec17[0] = fTemp0 - fRec17[2] * T_french_bell_faustpower2_f(fTemp15) + fConst17 * fRec17[1] * fTemp15;
		float fTemp16 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.44961452f * fRec2[0], fRec3[0])));
		fRec18[0] = fTemp0 - fRec18[2] * T_french_bell_faustpower2_f(fTemp16) + fConst18 * fRec18[1] * fTemp16;
		float fTemp17 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.46847337f * fRec2[0], fRec3[0])));
		fRec19[0] = fTemp0 - fRec19[2] * T_french_bell_faustpower2_f(fTemp17) + fConst19 * fRec19[1] * fTemp17;
		float fTemp18 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.46957695f * fRec2[0], fRec3[0])));
		fRec20[0] = fTemp0 - fRec20[2] * T_french_bell_faustpower2_f(fTemp18) + fConst20 * fRec20[1] * fTemp18;
		float fTemp19 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.54306734f * fRec2[0], fRec3[0])));
		fRec21[0] = fTemp0 - fRec21[2] * T_french_bell_faustpower2_f(fTemp19) + fConst21 * fRec21[1] * fTemp19;
		float fTemp20 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.5479435f * fRec2[0], fRec3[0])));
		fRec22[0] = fTemp0 - fRec22[2] * T_french_bell_faustpower2_f(fTemp20) + fConst22 * fRec22[1] * fTemp20;
		float fTemp21 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.54880834f * fRec2[0], fRec3[0])));
		fRec23[0] = fTemp0 - fRec23[2] * T_french_bell_faustpower2_f(fTemp21) + fConst23 * fRec23[1] * fTemp21;
		float fTemp22 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.5917348f * fRec2[0], fRec3[0])));
		fRec24[0] = fTemp0 - fRec24[2] * T_french_bell_faustpower2_f(fTemp22) + fConst24 * fRec24[1] * fTemp22;
		float fTemp23 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.5921262f * fRec2[0], fRec3[0])));
		fRec25[0] = fTemp0 - fRec25[2] * T_french_bell_faustpower2_f(fTemp23) + fConst25 * fRec25[1] * fTemp23;
		float fTemp24 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.62320274f * fRec2[0], fRec3[0])));
		fRec26[0] = fTemp0 - fRec26[2] * T_french_bell_faustpower2_f(fTemp24) + fConst26 * fRec26[1] * fTemp24;
		float fTemp25 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.62354714f * fRec2[0], fRec3[0])));
		fRec27[0] = fTemp0 - fRec27[2] * T_french_bell_faustpower2_f(fTemp25) + fConst27 * fRec27[1] * fTemp25;
		float fTemp26 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6509803f * fRec2[0], fRec3[0])));
		fRec28[0] = fTemp0 - fRec28[2] * T_french_bell_faustpower2_f(fTemp26) + fConst28 * fRec28[1] * fTemp26;
		float fTemp27 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6521622f * fRec2[0], fRec3[0])));
		fRec29[0] = fTemp0 - fRec29[2] * T_french_bell_faustpower2_f(fTemp27) + fConst29 * fRec29[1] * fTemp27;
		float fTemp28 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.66188705f * fRec2[0], fRec3[0])));
		fRec30[0] = fTemp0 - fRec30[2] * T_french_bell_faustpower2_f(fTemp28) + fConst30 * fRec30[1] * fTemp28;
		float fTemp29 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.66284585f * fRec2[0], fRec3[0])));
		fRec31[0] = fTemp0 - fRec31[2] * T_french_bell_faustpower2_f(fTemp29) + fConst31 * fRec31[1] * fTemp29;
		float fTemp30 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7063085f * fRec2[0], fRec3[0])));
		fRec32[0] = fTemp0 - fRec32[2] * T_french_bell_faustpower2_f(fTemp30) + fConst32 * fRec32[1] * fTemp30;
		float fTemp31 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.70787776f * fRec2[0], fRec3[0])));
		fRec33[0] = fTemp0 - fRec33[2] * T_french_bell_faustpower2_f(fTemp31) + fConst33 * fRec33[1] * fTemp31;
		float fTemp32 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7122725f * fRec2[0], fRec3[0])));
		fRec34[0] = fTemp0 - fRec34[2] * T_french_bell_faustpower2_f(fTemp32) + fConst34 * fRec34[1] * fTemp32;
		float fTemp33 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7228662f * fRec2[0], fRec3[0])));
		fRec35[0] = fTemp0 - fRec35[2] * T_french_bell_faustpower2_f(fTemp33) + fConst35 * fRec35[1] * fTemp33;
		float fTemp34 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7477517f * fRec2[0], fRec3[0])));
		fRec36[0] = fTemp0 - fRec36[2] * T_french_bell_faustpower2_f(fTemp34) + fConst36 * fRec36[1] * fTemp34;
		float fTemp35 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7543928f * fRec2[0], fRec3[0])));
		fRec37[0] = fTemp0 - fRec37[2] * T_french_bell_faustpower2_f(fTemp35) + fConst37 * fRec37[1] * fTemp35;
		float fTemp36 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7964114f * fRec2[0], fRec3[0])));
		fRec38[0] = fTemp0 - fRec38[2] * T_french_bell_faustpower2_f(fTemp36) + fConst38 * fRec38[1] * fTemp36;
		float fTemp37 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7978437f * fRec2[0], fRec3[0])));
		fRec39[0] = fTemp0 - fRec39[2] * T_french_bell_faustpower2_f(fTemp37) + fConst39 * fRec39[1] * fTemp37;
		float fTemp38 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8040778f * fRec2[0], fRec3[0])));
		fRec40[0] = fTemp0 - fRec40[2] * T_french_bell_faustpower2_f(fTemp38) + fConst40 * fRec40[1] * fTemp38;
		float fTemp39 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.80447304f * fRec2[0], fRec3[0])));
		fRec41[0] = fTemp0 - fRec41[2] * T_french_bell_faustpower2_f(fTemp39) + fConst41 * fRec41[1] * fTemp39;
		float fTemp40 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8134818f * fRec2[0], fRec3[0])));
		fRec42[0] = fTemp0 - fRec42[2] * T_french_bell_faustpower2_f(fTemp40) + fConst42 * fRec42[1] * fTemp40;
		float fTemp41 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.81859666f * fRec2[0], fRec3[0])));
		fRec43[0] = fTemp0 - fRec43[2] * T_french_bell_faustpower2_f(fTemp41) + fConst43 * fRec43[1] * fTemp41;
		float fTemp42 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.82799673f * fRec2[0], fRec3[0])));
		fRec44[0] = fTemp0 - fRec44[2] * T_french_bell_faustpower2_f(fTemp42) + fConst44 * fRec44[1] * fTemp42;
		float fTemp43 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8323915f * fRec2[0], fRec3[0])));
		fRec45[0] = fTemp0 - fRec45[2] * T_french_bell_faustpower2_f(fTemp43) + fConst45 * fRec45[1] * fTemp43;
		float fTemp44 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8648143f * fRec2[0], fRec3[0])));
		fRec46[0] = fTemp0 - fRec46[2] * T_french_bell_faustpower2_f(fTemp44) + fConst46 * fRec46[1] * fTemp44;
		float fTemp45 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.86645406f * fRec2[0], fRec3[0])));
		fRec47[0] = fTemp0 - fRec47[2] * T_french_bell_faustpower2_f(fTemp45) + fConst47 * fRec47[1] * fTemp45;
		float fTemp46 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.89858335f * fRec2[0], fRec3[0])));
		fRec48[0] = fTemp0 - fRec48[2] * T_french_bell_faustpower2_f(fTemp46) + fConst48 * fRec48[1] * fTemp46;
		float fTemp47 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8993386f * fRec2[0], fRec3[0])));
		fRec49[0] = fTemp0 - fRec49[2] * T_french_bell_faustpower2_f(fTemp47) + fConst49 * fRec49[1] * fTemp47;
		float fTemp48 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.89968693f * fRec2[0], fRec3[0])));
		fRec50[0] = fTemp0 - fRec50[2] * T_french_bell_faustpower2_f(fTemp48) + fConst50 * fRec50[1] * fTemp48;
		float fTemp49 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.90307206f * fRec2[0], fRec3[0])));
		fRec51[0] = fTemp0 - fRec51[2] * T_french_bell_faustpower2_f(fTemp49) + fConst51 * fRec51[1] * fTemp49;
		float fTemp50 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9409619f * fRec2[0], fRec3[0])));
		fRec52[0] = fTemp0 - fRec52[2] * T_french_bell_faustpower2_f(fTemp50) + fConst52 * fRec52[1] * fTemp50;
		float fTemp51 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9427425f * fRec2[0], fRec3[0])));
		fRec53[0] = fTemp0 - fRec53[2] * T_french_bell_faustpower2_f(fTemp51) + fConst53 * fRec53[1] * fTemp51;
		outputs[0] = static_cast<FAUSTFLOAT>(0.02f * ((fRec0[0] - fRec0[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(iTemp2, 349))] + (fRec5[0] - fRec5[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(1, iTemp2), 349))] + (fRec6[0] - fRec6[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(2, iTemp2), 349))] + (fRec7[0] - fRec7[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(3, iTemp2), 349))] + (fRec8[0] - fRec8[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(4, iTemp2), 349))] + (fRec9[0] - fRec9[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(5, iTemp2), 349))] + (fRec10[0] - fRec10[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(6, iTemp2), 349))] + (fRec11[0] - fRec11[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(7, iTemp2), 349))] + (fRec12[0] - fRec12[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(8, iTemp2), 349))] + (fRec13[0] - fRec13[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(9, iTemp2), 349))] + (fRec14[0] - fRec14[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(10, iTemp2), 349))] + (fRec15[0] - fRec15[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(11, iTemp2), 349))] + (fRec16[0] - fRec16[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(12, iTemp2), 349))] + (fRec17[0] - fRec17[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(13, iTemp2), 349))] + (fRec18[0] - fRec18[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(14, iTemp2), 349))] + (fRec19[0] - fRec19[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(15, iTemp2), 349))] + (fRec20[0] - fRec20[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(16, iTemp2), 349))] + (fRec21[0] - fRec21[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(17, iTemp2), 349))] + (fRec22[0] - fRec22[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(18, iTemp2), 349))] + (fRec23[0] - fRec23[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(19, iTemp2), 349))] + (fRec24[0] - fRec24[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(20, iTemp2), 349))] + (fRec25[0] - fRec25[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(21, iTemp2), 349))] + (fRec26[0] - fRec26[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(22, iTemp2), 349))] + (fRec27[0] - fRec27[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(23, iTemp2), 349))] + (fRec28[0] - fRec28[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(24, iTemp2), 349))] + (fRec29[0] - fRec29[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(25, iTemp2), 349))] + (fRec30[0] - fRec30[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(26, iTemp2), 349))] + (fRec31[0] - fRec31[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(27, iTemp2), 349))] + (fRec32[0] - fRec32[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(28, iTemp2), 349))] + (fRec33[0] - fRec33[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(29, iTemp2), 349))] + (fRec34[0] - fRec34[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(30, iTemp2), 349))] + (fRec35[0] - fRec35[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(31, iTemp2), 349))] + (fRec36[0] - fRec36[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(32, iTemp2), 349))] + (fRec37[0] - fRec37[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(33, iTemp2), 349))] + (fRec38[0] - fRec38[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(34, iTemp2), 349))] + (fRec39[0] - fRec39[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(35, iTemp2), 349))] + (fRec40[0] - fRec40[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(36, iTemp2), 349))] + (fRec41[0] - fRec41[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(37, iTemp2), 349))] + (fRec42[0] - fRec42[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(38, iTemp2), 349))] + (fRec43[0] - fRec43[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(39, iTemp2), 349))] + (fRec44[0] - fRec44[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(40, iTemp2), 349))] + (fRec45[0] - fRec45[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(41, iTemp2), 349))] + (fRec46[0] - fRec46[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(42, iTemp2), 349))] + (fRec47[0] - fRec47[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(43, iTemp2), 349))] + (fRec48[0] - fRec48[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(44, iTemp2), 349))] + (fRec49[0] - fRec49[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(45, iTemp2), 349))] + (fRec50[0] - fRec50[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(46, iTemp2), 349))] + (fRec51[0] - fRec51[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(47, iTemp2), 349))] + (fRec52[0] - fRec52[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(48, iTemp2), 349))] + (fRec53[0] - fRec53[2]) * ftbl0T_french_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(49, iTemp2), 349))]));
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
