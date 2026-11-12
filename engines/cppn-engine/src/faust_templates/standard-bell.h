/* ------------------------------------------------------------
name: "standard-bell"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_standard_bell -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_standard_bell_H__
#define  __T_standard_bell_H__

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
#define FAUSTCLASS T_standard_bell
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

const static float fT_standard_bellSIG0Wave0[350] = {0.691911f,0.622333f,0.548651f,0.463306f,0.826946f,0.749513f,0.2242f,0.642678f,0.760442f,0.326054f,0.276463f,0.359344f,0.18258f,0.686765f,0.457159f,0.839015f,0.845338f,0.372377f,0.306417f,0.147381f,0.359707f,0.653537f,0.27553f,0.401233f,0.435417f,0.251481f,0.190062f,0.773372f,0.315014f,0.228812f,0.521512f,0.411542f,0.720762f,1.0f,0.286502f,0.338938f,0.119995f,0.432289f,0.409677f,0.156272f,0.298871f,0.250786f,0.640776f,0.209431f,0.17001f,0.390014f,0.301698f,0.799413f,0.980581f,0.385f,0.82544f,0.818894f,0.349616f,0.235396f,0.783164f,0.821914f,0.28411f,0.430286f,0.507671f,0.326254f,0.260488f,0.273364f,0.20518f,0.714852f,0.47995f,0.803637f,0.683943f,0.355371f,0.406924f,0.656257f,0.423025f,0.413515f,0.38636f,0.384787f,0.389448f,0.813367f,0.234988f,1.0f,0.311268f,0.350245f,0.403856f,0.646143f,0.500485f,0.833553f,0.431768f,0.467064f,0.298979f,0.487413f,0.514907f,0.369383f,0.106197f,0.494224f,0.816079f,0.535807f,0.379873f,0.380201f,0.606306f,0.516117f,0.748449f,0.556948f,0.587066f,0.584423f,0.394866f,0.341121f,0.433458f,0.455987f,0.361237f,0.42939f,0.122969f,0.133175f,0.505176f,0.513985f,0.0554619f,0.604942f,0.372074f,0.381126f,0.314354f,0.499636f,0.518711f,0.923792f,0.259544f,0.576517f,0.553915f,0.585444f,0.245369f,1.0f,0.117757f,0.977318f,0.652862f,0.509314f,0.14855f,0.506402f,0.180059f,0.356005f,0.38681f,0.279354f,0.205792f,0.551055f,0.689107f,0.445724f,0.306857f,0.324747f,0.603621f,0.394466f,0.288613f,0.264697f,0.60612f,0.20274f,0.267271f,0.925656f,0.439228f,0.425884f,0.626633f,0.547204f,0.230022f,0.225654f,0.392697f,0.493474f,0.149857f,0.0604048f,0.693889f,0.740271f,0.175485f,0.704998f,0.329732f,0.153026f,0.125744f,0.286995f,0.278878f,0.812372f,0.0562174f,0.241479f,0.294525f,0.358834f,0.171047f,0.847604f,0.17228f,0.97521f,0.892073f,0.613987f,0.0659213f,0.301583f,0.0610847f,0.125438f,0.145151f,0.180086f,0.124231f,0.260161f,0.337573f,0.203743f,0.655798f,0.425893f,0.902347f,0.500686f,0.311173f,0.215561f,0.349591f,0.0854218f,0.0805062f,1.0f,0.338652f,0.295396f,0.698314f,0.664972f,0.118983f,0.0881905f,0.31158f,0.391136f,0.151915f,0.239504f,0.685742f,0.884332f,0.288516f,0.768688f,0.274851f,0.0490311f,0.0357865f,0.293303f,0.249461f,0.493771f,0.340984f,0.467623f,0.216631f,0.255235f,0.0988695f,0.46198f,0.147247f,0.640196f,1.0f,0.551938f,0.0453732f,0.189907f,0.0197542f,0.0309217f,0.769837f,0.360418f,0.384041f,0.867434f,0.398948f,0.171848f,0.748652f,0.301957f,0.860611f,0.958674f,0.54903f,0.272753f,0.372753f,0.0180728f,0.0292353f,0.8502f,0.224583f,0.214805f,0.670319f,0.586433f,0.0435142f,0.0388574f,0.144811f,0.157061f,0.155569f,0.418334f,0.673656f,0.749573f,0.337354f,0.747254f,0.255997f,0.0239656f,0.0310719f,0.721087f,0.700616f,0.199051f,0.511844f,0.849485f,0.700682f,0.778658f,0.171289f,0.261973f,0.129228f,0.328597f,0.781821f,0.583813f,0.0806713f,0.416876f,0.0118202f,0.00868563f,1.0f,0.461884f,0.186882f,0.641364f,0.994705f,0.501902f,0.566449f,0.0678845f,0.139737f,0.462582f,0.318656f,0.233947f,0.495941f,0.0314028f,0.0146478f,0.70432f,0.124953f,0.132549f,0.457126f,0.378636f,0.0169362f,0.0195494f,0.204155f,0.294401f,0.271367f,0.730857f,0.459322f,0.433078f,0.325171f,0.734536f,0.416205f,0.012873f,0.0388489f,0.821567f,0.863683f,0.0920531f,0.393972f,0.539544f,0.832052f,0.842732f,0.241144f,0.479558f,0.283092f,0.477845f,0.385473f,0.436587f,0.144308f,0.642395f,0.0215791f,0.00779029f,0.563714f,0.838279f,0.410004f,0.829086f,1.0f,0.630598f,0.0233729f,0.496217f,0.711042f,0.914266f,0.695042f,0.331894f,0.898442f,0.028568f,0.0174966f,0.482846f};
class T_standard_bellSIG0 {
	
  private:
	
	int fT_standard_bellSIG0Wave0_idx;
	int fSampleRate;
	
  public:
	
	int getNumInputsT_standard_bellSIG0() {
		return 0;
	}
	int getNumOutputsT_standard_bellSIG0() {
		return 1;
	}
	
	void instanceInitT_standard_bellSIG0(int sample_rate) {
		fSampleRate = sample_rate;
		fT_standard_bellSIG0Wave0_idx = 0;
	}
	
	void fillT_standard_bellSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = faust_wrap_add(i1, 1)) {
			table[i1] = fT_standard_bellSIG0Wave0[fT_standard_bellSIG0Wave0_idx];
			fT_standard_bellSIG0Wave0_idx = (faust_wrap_add(1, fT_standard_bellSIG0Wave0_idx)) % 350;
		}
	}

};

static T_standard_bellSIG0* newT_standard_bellSIG0() { return (T_standard_bellSIG0*)new T_standard_bellSIG0(); }
static void deleteT_standard_bellSIG0(T_standard_bellSIG0* dsp) { delete dsp; }

static float T_standard_bell_faustpower2_f(float value) {
	return value * value;
}
static float ftbl0T_standard_bellSIG0[350];

class T_standard_bell : public FaustDsp {
	
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
	T_standard_bell() {
	}
	
	T_standard_bell(const T_standard_bell&) = default;
	
	virtual ~T_standard_bell() = default;
	
	T_standard_bell& operator=(const T_standard_bell&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_standard_bell -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "standard-bell.dsp");
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
		m->declare("name", "standard-bell");
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
		T_standard_bellSIG0* sig0 = newT_standard_bellSIG0();
		sig0->instanceInitT_standard_bellSIG0(sample_rate);
		sig0->fillT_standard_bellSIG0(350, ftbl0T_standard_bellSIG0);
		deleteT_standard_bellSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 1.0f / fConst0;
		fConst2 = 44.1f / fConst0;
		fConst3 = 1.0f - fConst2;
		fConst4 = 2.0f * std::cos(3080.3315f / fConst0);
		fConst5 = 2.0f * std::cos(3101.6692f / fConst0);
		fConst6 = 2.0f * std::cos(5810.9287f / fConst0);
		fConst7 = 2.0f * std::cos(5829.407f / fConst0);
		fConst8 = 2.0f * std::cos(7421.761f / fConst0);
		fConst9 = 2.0f * std::cos(7457.764f / fConst0);
		fConst10 = 2.0f * std::cos(8475.012f / fConst0);
		fConst11 = 2.0f * std::cos(8479.158f / fConst0);
		fConst12 = 2.0f * std::cos(9803.843f / fConst0);
		fConst13 = 2.0f * std::cos(10279.103f / fConst0);
		fConst14 = 2.0f * std::cos(10723.701f / fConst0);
		fConst15 = 2.0f * std::cos(10762.405f / fConst0);
		fConst16 = 2.0f * std::cos(10964.473f / fConst0);
		fConst17 = 2.0f * std::cos(10965.7295f / fConst0);
		fConst18 = 2.0f * std::cos(12600.991f / fConst0);
		fConst19 = 2.0f * std::cos(12726.403f / fConst0);
		fConst20 = 2.0f * std::cos(12904.908f / fConst0);
		fConst21 = 2.0f * std::cos(13460.907f / fConst0);
		fConst22 = 2.0f * std::cos(13517.645f / fConst0);
		fConst23 = 2.0f * std::cos(15130.916f / fConst0);
		fConst24 = 2.0f * std::cos(15922.283f / fConst0);
		fConst25 = 2.0f * std::cos(15936.797f / fConst0);
		fConst26 = 2.0f * std::cos(16482.68f / fConst0);
		fConst27 = 2.0f * std::cos(16514.725f / fConst0);
		fConst28 = 2.0f * std::cos(17037.297f / fConst0);
		fConst29 = 2.0f * std::cos(17042.889f / fConst0);
		fConst30 = 2.0f * std::cos(17738.877f / fConst0);
		fConst31 = 2.0f * std::cos(17763.947f / fConst0);
		fConst32 = 2.0f * std::cos(17991.398f / fConst0);
		fConst33 = 2.0f * std::cos(18059.068f / fConst0);
		fConst34 = 2.0f * std::cos(18365.75f / fConst0);
		fConst35 = 2.0f * std::cos(18382.652f / fConst0);
		fConst36 = 2.0f * std::cos(19053.885f / fConst0);
		fConst37 = 2.0f * std::cos(19114.393f / fConst0);
		fConst38 = 2.0f * std::cos(20160.04f / fConst0);
		fConst39 = 2.0f * std::cos(21315.832f / fConst0);
		fConst40 = 2.0f * std::cos(21902.682f / fConst0);
		fConst41 = 2.0f * std::cos(21951.25f / fConst0);
		fConst42 = 2.0f * std::cos(22241.219f / fConst0);
		fConst43 = 2.0f * std::cos(22308.826f / fConst0);
		fConst44 = 2.0f * std::cos(23114.018f / fConst0);
		fConst45 = 2.0f * std::cos(23367.418f / fConst0);
		fConst46 = 2.0f * std::cos(23389.723f / fConst0);
		fConst47 = 2.0f * std::cos(23789.898f / fConst0);
		fConst48 = 2.0f * std::cos(23809.377f / fConst0);
		fConst49 = 2.0f * std::cos(25092.467f / fConst0);
		fConst50 = 2.0f * std::cos(25122.877f / fConst0);
		fConst51 = 2.0f * std::cos(25908.148f / fConst0);
		fConst52 = 2.0f * std::cos(26168.398f / fConst0);
		fConst53 = 2.0f * std::cos(26313.854f / fConst0);
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
	
	virtual T_standard_bell* clone() {
		return new T_standard_bell(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("standard-bell");
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
		float fTemp1 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.11695035f * fRec2[0], fRec3[0])));
		fRec0[0] = fTemp0 - fRec0[2] * T_standard_bell_faustpower2_f(fTemp1) + fConst4 * fRec0[1] * fTemp1;
		fRec4[0] = fSlow3 + fConst3 * fRec4[1];
		int iTemp2 = faust_wrap_mul(50, static_cast<int>(fRec4[0]));
		float fTemp3 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.11776047f * fRec2[0], fRec3[0])));
		fRec5[0] = fTemp0 - fRec5[2] * T_standard_bell_faustpower2_f(fTemp3) + fConst5 * fRec5[1] * fTemp3;
		float fTemp4 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.22062239f * fRec2[0], fRec3[0])));
		fRec6[0] = fTemp0 - fRec6[2] * T_standard_bell_faustpower2_f(fTemp4) + fConst6 * fRec6[1] * fTemp4;
		float fTemp5 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.22132397f * fRec2[0], fRec3[0])));
		fRec7[0] = fTemp0 - fRec7[2] * T_standard_bell_faustpower2_f(fTemp5) + fConst7 * fRec7[1] * fTemp5;
		float fTemp6 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.28178054f * fRec2[0], fRec3[0])));
		fRec8[0] = fTemp0 - fRec8[2] * T_standard_bell_faustpower2_f(fTemp6) + fConst8 * fRec8[1] * fTemp6;
		float fTemp7 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.28314745f * fRec2[0], fRec3[0])));
		fRec9[0] = fTemp0 - fRec9[2] * T_standard_bell_faustpower2_f(fTemp7) + fConst9 * fRec9[1] * fTemp7;
		float fTemp8 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.32176912f * fRec2[0], fRec3[0])));
		fRec10[0] = fTemp0 - fRec10[2] * T_standard_bell_faustpower2_f(fTemp8) + fConst10 * fRec10[1] * fTemp8;
		float fTemp9 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.32192656f * fRec2[0], fRec3[0])));
		fRec11[0] = fTemp0 - fRec11[2] * T_standard_bell_faustpower2_f(fTemp9) + fConst11 * fRec11[1] * fTemp9;
		float fTemp10 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.37222058f * fRec2[0], fRec3[0])));
		fRec12[0] = fTemp0 - fRec12[2] * T_standard_bell_faustpower2_f(fTemp10) + fConst12 * fRec12[1] * fTemp10;
		float fTemp11 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.39026466f * fRec2[0], fRec3[0])));
		fRec13[0] = fTemp0 - fRec13[2] * T_standard_bell_faustpower2_f(fTemp11) + fConst13 * fRec13[1] * fTemp11;
		float fTemp12 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.40714464f * fRec2[0], fRec3[0])));
		fRec14[0] = fTemp0 - fRec14[2] * T_standard_bell_faustpower2_f(fTemp12) + fConst14 * fRec14[1] * fTemp12;
		float fTemp13 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.40861413f * fRec2[0], fRec3[0])));
		fRec15[0] = fTemp0 - fRec15[2] * T_standard_bell_faustpower2_f(fTemp13) + fConst15 * fRec15[1] * fTemp13;
		float fTemp14 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.41628596f * fRec2[0], fRec3[0])));
		fRec16[0] = fTemp0 - fRec16[2] * T_standard_bell_faustpower2_f(fTemp14) + fConst16 * fRec16[1] * fTemp14;
		float fTemp15 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.41633368f * fRec2[0], fRec3[0])));
		fRec17[0] = fTemp0 - fRec17[2] * T_standard_bell_faustpower2_f(fTemp15) + fConst17 * fRec17[1] * fTemp15;
		float fTemp16 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.47841936f * fRec2[0], fRec3[0])));
		fRec18[0] = fTemp0 - fRec18[2] * T_standard_bell_faustpower2_f(fTemp16) + fConst18 * fRec18[1] * fTemp16;
		float fTemp17 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.48318085f * fRec2[0], fRec3[0])));
		fRec19[0] = fTemp0 - fRec19[2] * T_standard_bell_faustpower2_f(fTemp17) + fConst19 * fRec19[1] * fTemp17;
		float fTemp18 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.48995814f * fRec2[0], fRec3[0])));
		fRec20[0] = fTemp0 - fRec20[2] * T_standard_bell_faustpower2_f(fTemp18) + fConst20 * fRec20[1] * fTemp18;
		float fTemp19 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.5110676f * fRec2[0], fRec3[0])));
		fRec21[0] = fTemp0 - fRec21[2] * T_standard_bell_faustpower2_f(fTemp19) + fConst21 * fRec21[1] * fTemp19;
		float fTemp20 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.51322174f * fRec2[0], fRec3[0])));
		fRec22[0] = fTemp0 - fRec22[2] * T_standard_bell_faustpower2_f(fTemp20) + fConst22 * fRec22[1] * fTemp20;
		float fTemp21 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.5744725f * fRec2[0], fRec3[0])));
		fRec23[0] = fTemp0 - fRec23[2] * T_standard_bell_faustpower2_f(fTemp21) + fConst23 * fRec23[1] * fTemp21;
		float fTemp22 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6045182f * fRec2[0], fRec3[0])));
		fRec24[0] = fTemp0 - fRec24[2] * T_standard_bell_faustpower2_f(fTemp22) + fConst24 * fRec24[1] * fTemp22;
		float fTemp23 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6050692f * fRec2[0], fRec3[0])));
		fRec25[0] = fTemp0 - fRec25[2] * T_standard_bell_faustpower2_f(fTemp23) + fConst25 * fRec25[1] * fTemp23;
		float fTemp24 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.62579465f * fRec2[0], fRec3[0])));
		fRec26[0] = fTemp0 - fRec26[2] * T_standard_bell_faustpower2_f(fTemp24) + fConst26 * fRec26[1] * fTemp24;
		float fTemp25 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6270113f * fRec2[0], fRec3[0])));
		fRec27[0] = fTemp0 - fRec27[2] * T_standard_bell_faustpower2_f(fTemp25) + fConst27 * fRec27[1] * fTemp25;
		float fTemp26 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6468517f * fRec2[0], fRec3[0])));
		fRec28[0] = fTemp0 - fRec28[2] * T_standard_bell_faustpower2_f(fTemp26) + fConst28 * fRec28[1] * fTemp26;
		float fTemp27 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.64706403f * fRec2[0], fRec3[0])));
		fRec29[0] = fTemp0 - fRec29[2] * T_standard_bell_faustpower2_f(fTemp27) + fConst29 * fRec29[1] * fTemp27;
		float fTemp28 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6734885f * fRec2[0], fRec3[0])));
		fRec30[0] = fTemp0 - fRec30[2] * T_standard_bell_faustpower2_f(fTemp28) + fConst30 * fRec30[1] * fTemp28;
		float fTemp29 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6744403f * fRec2[0], fRec3[0])));
		fRec31[0] = fTemp0 - fRec31[2] * T_standard_bell_faustpower2_f(fTemp29) + fConst31 * fRec31[1] * fTemp29;
		float fTemp30 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6830759f * fRec2[0], fRec3[0])));
		fRec32[0] = fTemp0 - fRec32[2] * T_standard_bell_faustpower2_f(fTemp30) + fConst32 * fRec32[1] * fTemp30;
		float fTemp31 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6856451f * fRec2[0], fRec3[0])));
		fRec33[0] = fTemp0 - fRec33[2] * T_standard_bell_faustpower2_f(fTemp31) + fConst33 * fRec33[1] * fTemp31;
		float fTemp32 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6972889f * fRec2[0], fRec3[0])));
		fRec34[0] = fTemp0 - fRec34[2] * T_standard_bell_faustpower2_f(fTemp32) + fConst34 * fRec34[1] * fTemp32;
		float fTemp33 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6979306f * fRec2[0], fRec3[0])));
		fRec35[0] = fTemp0 - fRec35[2] * T_standard_bell_faustpower2_f(fTemp33) + fConst35 * fRec35[1] * fTemp33;
		float fTemp34 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.72341514f * fRec2[0], fRec3[0])));
		fRec36[0] = fTemp0 - fRec36[2] * T_standard_bell_faustpower2_f(fTemp34) + fConst36 * fRec36[1] * fTemp34;
		float fTemp35 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.72571236f * fRec2[0], fRec3[0])));
		fRec37[0] = fTemp0 - fRec37[2] * T_standard_bell_faustpower2_f(fTemp35) + fConst37 * fRec37[1] * fTemp35;
		float fTemp36 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7654123f * fRec2[0], fRec3[0])));
		fRec38[0] = fTemp0 - fRec38[2] * T_standard_bell_faustpower2_f(fTemp36) + fConst38 * fRec38[1] * fTemp36;
		float fTemp37 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.809294f * fRec2[0], fRec3[0])));
		fRec39[0] = fTemp0 - fRec39[2] * T_standard_bell_faustpower2_f(fTemp37) + fConst39 * fRec39[1] * fTemp37;
		float fTemp38 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8315748f * fRec2[0], fRec3[0])));
		fRec40[0] = fTemp0 - fRec40[2] * T_standard_bell_faustpower2_f(fTemp38) + fConst40 * fRec40[1] * fTemp38;
		float fTemp39 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8334188f * fRec2[0], fRec3[0])));
		fRec41[0] = fTemp0 - fRec41[2] * T_standard_bell_faustpower2_f(fTemp39) + fConst41 * fRec41[1] * fTemp39;
		float fTemp40 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.844428f * fRec2[0], fRec3[0])));
		fRec42[0] = fTemp0 - fRec42[2] * T_standard_bell_faustpower2_f(fTemp40) + fConst42 * fRec42[1] * fTemp40;
		float fTemp41 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8469948f * fRec2[0], fRec3[0])));
		fRec43[0] = fTemp0 - fRec43[2] * T_standard_bell_faustpower2_f(fTemp41) + fConst43 * fRec43[1] * fTemp41;
		float fTemp42 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8775653f * fRec2[0], fRec3[0])));
		fRec44[0] = fTemp0 - fRec44[2] * T_standard_bell_faustpower2_f(fTemp42) + fConst44 * fRec44[1] * fTemp42;
		float fTemp43 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.88718617f * fRec2[0], fRec3[0])));
		fRec45[0] = fTemp0 - fRec45[2] * T_standard_bell_faustpower2_f(fTemp43) + fConst45 * fRec45[1] * fTemp43;
		float fTemp44 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.88803303f * fRec2[0], fRec3[0])));
		fRec46[0] = fTemp0 - fRec46[2] * T_standard_bell_faustpower2_f(fTemp44) + fConst46 * fRec46[1] * fTemp44;
		float fTemp45 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.90322644f * fRec2[0], fRec3[0])));
		fRec47[0] = fTemp0 - fRec47[2] * T_standard_bell_faustpower2_f(fTemp45) + fConst47 * fRec47[1] * fTemp45;
		float fTemp46 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.90396595f * fRec2[0], fRec3[0])));
		fRec48[0] = fTemp0 - fRec48[2] * T_standard_bell_faustpower2_f(fTemp46) + fConst48 * fRec48[1] * fTemp46;
		float fTemp47 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9526807f * fRec2[0], fRec3[0])));
		fRec49[0] = fTemp0 - fRec49[2] * T_standard_bell_faustpower2_f(fTemp47) + fConst49 * fRec49[1] * fTemp47;
		float fTemp48 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9538353f * fRec2[0], fRec3[0])));
		fRec50[0] = fTemp0 - fRec50[2] * T_standard_bell_faustpower2_f(fTemp48) + fConst50 * fRec50[1] * fTemp48;
		float fTemp49 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9836496f * fRec2[0], fRec3[0])));
		fRec51[0] = fTemp0 - fRec51[2] * T_standard_bell_faustpower2_f(fTemp49) + fConst51 * fRec51[1] * fTemp49;
		float fTemp50 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.99353045f * fRec2[0], fRec3[0])));
		fRec52[0] = fTemp0 - fRec52[2] * T_standard_bell_faustpower2_f(fTemp50) + fConst52 * fRec52[1] * fTemp50;
		float fTemp51 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.99905294f * fRec2[0], fRec3[0])));
		fRec53[0] = fTemp0 - fRec53[2] * T_standard_bell_faustpower2_f(fTemp51) + fConst53 * fRec53[1] * fTemp51;
		outputs[0] = static_cast<FAUSTFLOAT>(0.02f * ((fRec0[0] - fRec0[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(iTemp2, 349))] + (fRec5[0] - fRec5[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(1, iTemp2), 349))] + (fRec6[0] - fRec6[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(2, iTemp2), 349))] + (fRec7[0] - fRec7[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(3, iTemp2), 349))] + (fRec8[0] - fRec8[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(4, iTemp2), 349))] + (fRec9[0] - fRec9[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(5, iTemp2), 349))] + (fRec10[0] - fRec10[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(6, iTemp2), 349))] + (fRec11[0] - fRec11[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(7, iTemp2), 349))] + (fRec12[0] - fRec12[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(8, iTemp2), 349))] + (fRec13[0] - fRec13[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(9, iTemp2), 349))] + (fRec14[0] - fRec14[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(10, iTemp2), 349))] + (fRec15[0] - fRec15[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(11, iTemp2), 349))] + (fRec16[0] - fRec16[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(12, iTemp2), 349))] + (fRec17[0] - fRec17[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(13, iTemp2), 349))] + (fRec18[0] - fRec18[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(14, iTemp2), 349))] + (fRec19[0] - fRec19[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(15, iTemp2), 349))] + (fRec20[0] - fRec20[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(16, iTemp2), 349))] + (fRec21[0] - fRec21[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(17, iTemp2), 349))] + (fRec22[0] - fRec22[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(18, iTemp2), 349))] + (fRec23[0] - fRec23[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(19, iTemp2), 349))] + (fRec24[0] - fRec24[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(20, iTemp2), 349))] + (fRec25[0] - fRec25[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(21, iTemp2), 349))] + (fRec26[0] - fRec26[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(22, iTemp2), 349))] + (fRec27[0] - fRec27[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(23, iTemp2), 349))] + (fRec28[0] - fRec28[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(24, iTemp2), 349))] + (fRec29[0] - fRec29[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(25, iTemp2), 349))] + (fRec30[0] - fRec30[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(26, iTemp2), 349))] + (fRec31[0] - fRec31[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(27, iTemp2), 349))] + (fRec32[0] - fRec32[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(28, iTemp2), 349))] + (fRec33[0] - fRec33[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(29, iTemp2), 349))] + (fRec34[0] - fRec34[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(30, iTemp2), 349))] + (fRec35[0] - fRec35[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(31, iTemp2), 349))] + (fRec36[0] - fRec36[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(32, iTemp2), 349))] + (fRec37[0] - fRec37[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(33, iTemp2), 349))] + (fRec38[0] - fRec38[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(34, iTemp2), 349))] + (fRec39[0] - fRec39[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(35, iTemp2), 349))] + (fRec40[0] - fRec40[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(36, iTemp2), 349))] + (fRec41[0] - fRec41[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(37, iTemp2), 349))] + (fRec42[0] - fRec42[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(38, iTemp2), 349))] + (fRec43[0] - fRec43[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(39, iTemp2), 349))] + (fRec44[0] - fRec44[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(40, iTemp2), 349))] + (fRec45[0] - fRec45[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(41, iTemp2), 349))] + (fRec46[0] - fRec46[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(42, iTemp2), 349))] + (fRec47[0] - fRec47[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(43, iTemp2), 349))] + (fRec48[0] - fRec48[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(44, iTemp2), 349))] + (fRec49[0] - fRec49[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(45, iTemp2), 349))] + (fRec50[0] - fRec50[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(46, iTemp2), 349))] + (fRec51[0] - fRec51[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(47, iTemp2), 349))] + (fRec52[0] - fRec52[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(48, iTemp2), 349))] + (fRec53[0] - fRec53[2]) * ftbl0T_standard_bellSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(49, iTemp2), 349))]));
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
