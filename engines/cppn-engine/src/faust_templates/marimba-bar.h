/* ------------------------------------------------------------
name: "marimba-bar"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_marimba_bar -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_marimba_bar_H__
#define  __T_marimba_bar_H__

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
#define FAUSTCLASS T_marimba_bar
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

const static float fT_marimba_barSIG0Wave0[250] = {1.0f,0.776725f,0.625723f,0.855223f,0.760159f,0.698373f,0.768011f,0.641127f,0.244034f,0.707754f,0.634013f,0.247527f,0.660849f,0.450396f,0.567783f,0.106361f,0.716814f,0.66392f,0.291208f,0.310599f,0.801495f,0.635292f,0.307435f,0.874124f,0.497668f,0.487088f,0.459115f,0.733455f,0.541818f,0.441318f,0.31392f,0.40309f,0.685353f,0.60314f,0.400552f,0.453511f,0.634386f,0.291547f,0.131605f,0.368507f,0.839907f,0.60216f,0.288296f,0.57967f,0.0242493f,0.262746f,0.368588f,0.890284f,0.408963f,0.556072f,0.884427f,0.83211f,0.612015f,0.757176f,0.919477f,1.0f,0.827963f,0.89241f,0.0357408f,0.480789f,0.752872f,0.0546301f,0.235937f,0.362938f,0.444472f,0.101751f,0.703418f,0.453136f,0.316629f,0.490394f,0.982508f,0.551622f,0.602009f,0.666957f,0.77683f,0.905662f,0.0987197f,0.402968f,0.829452f,0.307645f,0.64048f,0.983971f,0.584205f,0.650365f,0.334447f,0.58357f,0.540191f,0.672534f,0.245712f,0.687298f,0.883058f,0.79295f,0.600619f,0.572682f,0.122612f,0.388248f,0.290658f,0.380255f,0.290967f,0.567819f,0.0737721f,0.42099f,0.0786578f,0.393995f,0.268983f,0.260614f,0.494086f,0.238026f,0.0987824f,0.277879f,0.440563f,0.0770212f,0.450591f,0.128137f,0.0368275f,0.128699f,0.329605f,0.374512f,0.36359f,0.272594f,0.379052f,0.305241f,0.0741129f,0.345728f,0.29935f,0.221284f,0.0261391f,0.293202f,0.361885f,0.11433f,0.239005f,0.434156f,0.329583f,0.21946f,0.284175f,0.198555f,0.431976f,0.302985f,1.0f,0.146221f,0.140701f,0.264243f,0.185997f,0.426322f,0.30478f,0.34399f,0.19543f,0.386955f,0.1876f,0.172812f,0.0434115f,0.303761f,0.069454f,0.453943f,0.832451f,0.317817f,0.940601f,1.0f,0.180658f,0.737921f,0.832297f,0.402352f,0.126786f,0.594398f,0.485455f,0.32447f,0.365102f,0.777922f,0.588272f,0.401353f,0.610735f,0.158693f,0.0746072f,0.825099f,0.925459f,0.65377f,0.260792f,0.719384f,0.559908f,0.37259f,0.360035f,0.622939f,0.210271f,0.444595f,0.311286f,0.464309f,0.557231f,0.52408f,0.0701056f,0.320749f,0.19446f,0.727609f,0.522062f,0.394004f,0.235035f,0.395646f,0.494796f,0.517318f,0.109752f,0.692849f,0.00632009f,0.0207583f,0.00306107f,0.0637191f,0.081661f,0.03511f,0.127814f,0.202294f,0.0764145f,0.263127f,0.400199f,0.267278f,0.633385f,1.0f,0.739902f,0.413763f,0.41811f,0.612715f,0.672374f,0.339674f,0.21172f,0.459645f,0.1025f,0.32589f,0.148154f,0.265442f,0.0974305f,0.286438f,0.275213f,0.109111f,0.575089f,0.370283f,0.29411f,0.259826f,0.0648719f,0.583418f,0.282663f,0.182004f,0.117421f,0.417727f,0.16965f,0.24853f,0.122819f,0.185486f,0.0433618f,0.373849f,0.252768f,0.195103f,0.0927835f,0.166543f};
class T_marimba_barSIG0 {
	
  private:
	
	int fT_marimba_barSIG0Wave0_idx;
	int fSampleRate;
	
  public:
	
	int getNumInputsT_marimba_barSIG0() {
		return 0;
	}
	int getNumOutputsT_marimba_barSIG0() {
		return 1;
	}
	
	void instanceInitT_marimba_barSIG0(int sample_rate) {
		fSampleRate = sample_rate;
		fT_marimba_barSIG0Wave0_idx = 0;
	}
	
	void fillT_marimba_barSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = faust_wrap_add(i1, 1)) {
			table[i1] = fT_marimba_barSIG0Wave0[fT_marimba_barSIG0Wave0_idx];
			fT_marimba_barSIG0Wave0_idx = (faust_wrap_add(1, fT_marimba_barSIG0Wave0_idx)) % 250;
		}
	}

};

static T_marimba_barSIG0* newT_marimba_barSIG0() { return (T_marimba_barSIG0*)new T_marimba_barSIG0(); }
static void deleteT_marimba_barSIG0(T_marimba_barSIG0* dsp) { delete dsp; }

static float T_marimba_bar_faustpower2_f(float value) {
	return value * value;
}
static float ftbl0T_marimba_barSIG0[250];

class T_marimba_bar : public FaustDsp {
	
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
	FAUSTFLOAT fHslider3;
	float fSlow3;
	float fRec4[2];
	float fRec0[3];
	float fConst5;
	FAUSTFLOAT fHslider4;
	float fSlow4;
	float fRec5[2];
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
	float fConst54;
	float fRec54[3];
	
 public:
	T_marimba_bar() {
	}
	
	T_marimba_bar(const T_marimba_bar&) = default;
	
	virtual ~T_marimba_bar() = default;
	
	T_marimba_bar& operator=(const T_marimba_bar&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_marimba_bar -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("filename", "marimba-bar.dsp");
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
		m->declare("name", "marimba-bar");
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
		T_marimba_barSIG0* sig0 = newT_marimba_barSIG0();
		sig0->instanceInitT_marimba_barSIG0(sample_rate);
		sig0->fillT_marimba_barSIG0(250, ftbl0T_marimba_barSIG0);
		deleteT_marimba_barSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 1.0f / fConst0;
		fConst2 = 44.1f / fConst0;
		fConst3 = 1.0f - fConst2;
		fConst4 = 6.2831855f / fConst0;
		fConst5 = 0.5f * fConst0 + -1.0f;
		fConst6 = 20.819712f / fConst0;
		fConst7 = 24.094069f / fConst0;
		fConst8 = 50.66214f / fConst0;
		fConst9 = 59.362152f / fConst0;
		fConst10 = 88.6991f / fConst0;
		fConst11 = 115.51008f / fConst0;
		fConst12 = 132.01099f / fConst0;
		fConst13 = 164.47809f / fConst0;
		fConst14 = 182.17719f / fConst0;
		fConst15 = 232.93527f / fConst0;
		fConst16 = 237.9461f / fConst0;
		fConst17 = 251.72577f / fConst0;
		fConst18 = 299.35544f / fConst0;
		fConst19 = 320.56183f / fConst0;
		fConst20 = 329.4274f / fConst0;
		fConst21 = 366.22174f / fConst0;
		fConst22 = 399.28763f / fConst0;
		fConst23 = 410.68658f / fConst0;
		fConst24 = 420.71393f / fConst0;
		fConst25 = 468.28644f / fConst0;
		fConst26 = 494.43643f / fConst0;
		fConst27 = 507.917f / fConst0;
		fConst28 = 565.34845f / fConst0;
		fConst29 = 584.1232f / fConst0;
		fConst30 = 598.1052f / fConst0;
		fConst31 = 612.4893f / fConst0;
		fConst32 = 695.04596f / fConst0;
		fConst33 = 704.15027f / fConst0;
		fConst34 = 715.1899f / fConst0;
		fConst35 = 749.93585f / fConst0;
		fConst36 = 798.24725f / fConst0;
		fConst37 = 816.701f / fConst0;
		fConst38 = 831.0078f / fConst0;
		fConst39 = 838.6607f / fConst0;
		fConst40 = 908.22815f / fConst0;
		fConst41 = 938.94666f / fConst0;
		fConst42 = 955.2515f / fConst0;
		fConst43 = 962.37036f / fConst0;
		fConst44 = 977.6448f / fConst0;
		fConst45 = 993.8931f / fConst0;
		fConst46 = 1056.2349f / fConst0;
		fConst47 = 1079.847f / fConst0;
		fConst48 = 1096.1897f / fConst0;
		fConst49 = 1124.2943f / fConst0;
		fConst50 = 1140.285f / fConst0;
		fConst51 = 1164.89f / fConst0;
		fConst52 = 1196.1237f / fConst0;
		fConst53 = 1207.5654f / fConst0;
		fConst54 = 1228.3942f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.1f);
		fHslider1 = static_cast<FAUSTFLOAT>(1.0f);
		fHslider2 = static_cast<FAUSTFLOAT>(5.0f);
		fHslider3 = static_cast<FAUSTFLOAT>(2.2e+02f);
		fHslider4 = static_cast<FAUSTFLOAT>(2.0f);
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
		for (int l3 = 0; l3 < 2; l3 = faust_wrap_add(l3, 1)) {
			fRec4[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 3; l4 = faust_wrap_add(l4, 1)) {
			fRec0[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = faust_wrap_add(l5, 1)) {
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
		for (int l54 = 0; l54 < 3; l54 = faust_wrap_add(l54, 1)) {
			fRec54[l54] = 0.0f;
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
	
	virtual T_marimba_bar* clone() {
		return new T_marimba_bar(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("marimba-bar");
		ui_interface->addHorizontalSlider("exPos", &fHslider4, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("freq", &fHslider3, FAUSTFLOAT(2.2e+02f), FAUSTFLOAT(8e+01f), FAUSTFLOAT(2e+03f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("t60", &fHslider0, FAUSTFLOAT(0.1f), FAUSTFLOAT(0.01f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
		ui_interface->addHorizontalSlider("t60DecayRatio", &fHslider1, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f), FAUSTFLOAT(5.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("t60DecaySlope", &fHslider2, FAUSTFLOAT(5.0f), FAUSTFLOAT(0.1f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst2 * static_cast<float>(fHslider0);
		fSlow1 = fConst2 * static_cast<float>(fHslider1);
		fSlow2 = fConst2 * static_cast<float>(fHslider2);
		fSlow3 = fConst2 * static_cast<float>(fHslider3);
		fSlow4 = fConst2 * static_cast<float>(fHslider4);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		float fTemp0 = static_cast<float>(inputs[0]);
		fRec1[0] = fSlow0 + fConst3 * fRec1[1];
		fRec2[0] = fSlow1 + fConst3 * fRec2[1];
		fRec3[0] = fSlow2 + fConst3 * fRec3[1];
		float fTemp1 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.0051032123f * fRec2[0], fRec3[0])));
		fRec4[0] = fSlow3 + fConst3 * fRec4[1];
		fRec0[0] = fTemp0 - fRec0[2] * T_marimba_bar_faustpower2_f(fTemp1) + 2.0f * fRec0[1] * fTemp1 * std::cos(fConst4 * fRec4[0]);
		fRec5[0] = fSlow4 + fConst3 * fRec5[1];
		int iTemp2 = faust_wrap_mul(50, static_cast<int>(fRec5[0]));
		float fTemp3 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.0169098f * fRec2[0], fRec3[0])));
		fRec6[0] = fTemp0 - fRec6[2] * T_marimba_bar_faustpower2_f(fTemp3) + 2.0f * fRec6[1] * fTemp3 * std::cos(fConst6 * fRec4[0]);
		float fTemp4 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.019569239f * fRec2[0], fRec3[0])));
		fRec7[0] = fTemp0 - fRec7[2] * T_marimba_bar_faustpower2_f(fTemp4) + 2.0f * fRec7[1] * fTemp4 * std::cos(fConst7 * fRec4[0]);
		float fTemp5 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.041147865f * fRec2[0], fRec3[0])));
		fRec8[0] = fTemp0 - fRec8[2] * T_marimba_bar_faustpower2_f(fTemp5) + 2.0f * fRec8[1] * fTemp5 * std::cos(fConst8 * fRec4[0]);
		float fTemp6 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.04821403f * fRec2[0], fRec3[0])));
		fRec9[0] = fTemp0 - fRec9[2] * T_marimba_bar_faustpower2_f(fTemp6) + 2.0f * fRec9[1] * fTemp6 * std::cos(fConst9 * fRec4[0]);
		float fTemp7 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.07204154f * fRec2[0], fRec3[0])));
		fRec10[0] = fTemp0 - fRec10[2] * T_marimba_bar_faustpower2_f(fTemp7) + 2.0f * fRec10[1] * fTemp7 * std::cos(fConst10 * fRec4[0]);
		float fTemp8 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.09381746f * fRec2[0], fRec3[0])));
		fRec11[0] = fTemp0 - fRec11[2] * T_marimba_bar_faustpower2_f(fTemp8) + 2.0f * fRec11[1] * fTemp8 * std::cos(fConst11 * fRec4[0]);
		float fTemp9 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.10721952f * fRec2[0], fRec3[0])));
		fRec12[0] = fTemp0 - fRec12[2] * T_marimba_bar_faustpower2_f(fTemp9) + 2.0f * fRec12[1] * fTemp9 * std::cos(fConst12 * fRec4[0]);
		float fTemp10 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.13358934f * fRec2[0], fRec3[0])));
		fRec13[0] = fTemp0 - fRec13[2] * T_marimba_bar_faustpower2_f(fTemp10) + 2.0f * fRec13[1] * fTemp10 * std::cos(fConst13 * fRec4[0]);
		float fTemp11 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.14796458f * fRec2[0], fRec3[0])));
		fRec14[0] = fTemp0 - fRec14[2] * T_marimba_bar_faustpower2_f(fTemp11) + 2.0f * fRec14[1] * fTemp11 * std::cos(fConst14 * fRec4[0]);
		float fTemp12 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.18919037f * fRec2[0], fRec3[0])));
		fRec15[0] = fTemp0 - fRec15[2] * T_marimba_bar_faustpower2_f(fTemp12) + 2.0f * fRec15[1] * fTemp12 * std::cos(fConst15 * fRec4[0]);
		float fTemp13 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.1932602f * fRec2[0], fRec3[0])));
		fRec16[0] = fTemp0 - fRec16[2] * T_marimba_bar_faustpower2_f(fTemp13) + 2.0f * fRec16[1] * fTemp13 * std::cos(fConst16 * fRec4[0]);
		float fTemp14 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.20445204f * fRec2[0], fRec3[0])));
		fRec17[0] = fTemp0 - fRec17[2] * T_marimba_bar_faustpower2_f(fTemp14) + 2.0f * fRec17[1] * fTemp14 * std::cos(fConst17 * fRec4[0]);
		float fTemp15 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.24313694f * fRec2[0], fRec3[0])));
		fRec18[0] = fTemp0 - fRec18[2] * T_marimba_bar_faustpower2_f(fTemp15) + 2.0f * fRec18[1] * fTemp15 * std::cos(fConst18 * fRec4[0]);
		float fTemp16 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.2603608f * fRec2[0], fRec3[0])));
		fRec19[0] = fTemp0 - fRec19[2] * T_marimba_bar_faustpower2_f(fTemp16) + 2.0f * fRec19[1] * fTemp16 * std::cos(fConst19 * fRec4[0]);
		float fTemp17 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.26756144f * fRec2[0], fRec3[0])));
		fRec20[0] = fTemp0 - fRec20[2] * T_marimba_bar_faustpower2_f(fTemp17) + 2.0f * fRec20[1] * fTemp17 * std::cos(fConst20 * fRec4[0]);
		float fTemp18 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.29744583f * fRec2[0], fRec3[0])));
		fRec21[0] = fTemp0 - fRec21[2] * T_marimba_bar_faustpower2_f(fTemp18) + 2.0f * fRec21[1] * fTemp18 * std::cos(fConst21 * fRec4[0]);
		float fTemp19 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.32430202f * fRec2[0], fRec3[0])));
		fRec22[0] = fTemp0 - fRec22[2] * T_marimba_bar_faustpower2_f(fTemp19) + 2.0f * fRec22[1] * fTemp19 * std::cos(fConst22 * fRec4[0]);
		float fTemp20 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.33356026f * fRec2[0], fRec3[0])));
		fRec23[0] = fTemp0 - fRec23[2] * T_marimba_bar_faustpower2_f(fTemp20) + 2.0f * fRec23[1] * fTemp20 * std::cos(fConst23 * fRec4[0]);
		float fTemp21 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.3417045f * fRec2[0], fRec3[0])));
		fRec24[0] = fTemp0 - fRec24[2] * T_marimba_bar_faustpower2_f(fTemp21) + 2.0f * fRec24[1] * fTemp21 * std::cos(fConst24 * fRec4[0]);
		float fTemp22 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.38034293f * fRec2[0], fRec3[0])));
		fRec25[0] = fTemp0 - fRec25[2] * T_marimba_bar_faustpower2_f(fTemp22) + 2.0f * fRec25[1] * fTemp22 * std::cos(fConst25 * fRec4[0]);
		float fTemp23 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.401582f * fRec2[0], fRec3[0])));
		fRec26[0] = fTemp0 - fRec26[2] * T_marimba_bar_faustpower2_f(fTemp23) + 2.0f * fRec26[1] * fTemp23 * std::cos(fConst26 * fRec4[0]);
		float fTemp24 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.41253093f * fRec2[0], fRec3[0])));
		fRec27[0] = fTemp0 - fRec27[2] * T_marimba_bar_faustpower2_f(fTemp24) + 2.0f * fRec27[1] * fTemp24 * std::cos(fConst27 * fRec4[0]);
		float fTemp25 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.45917684f * fRec2[0], fRec3[0])));
		fRec28[0] = fTemp0 - fRec28[2] * T_marimba_bar_faustpower2_f(fTemp25) + 2.0f * fRec28[1] * fTemp25 * std::cos(fConst28 * fRec4[0]);
		float fTemp26 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.47442576f * fRec2[0], fRec3[0])));
		fRec29[0] = fTemp0 - fRec29[2] * T_marimba_bar_faustpower2_f(fTemp26) + 2.0f * fRec29[1] * fTemp26 * std::cos(fConst29 * fRec4[0]);
		float fTemp27 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.48578194f * fRec2[0], fRec3[0])));
		fRec30[0] = fTemp0 - fRec30[2] * T_marimba_bar_faustpower2_f(fTemp27) + 2.0f * fRec30[1] * fTemp27 * std::cos(fConst30 * fRec4[0]);
		float fTemp28 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.49746472f * fRec2[0], fRec3[0])));
		fRec31[0] = fTemp0 - fRec31[2] * T_marimba_bar_faustpower2_f(fTemp28) + 2.0f * fRec31[1] * fTemp28 * std::cos(fConst31 * fRec4[0]);
		float fTemp29 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.5645174f * fRec2[0], fRec3[0])));
		fRec32[0] = fTemp0 - fRec32[2] * T_marimba_bar_faustpower2_f(fTemp29) + 2.0f * fRec32[1] * fTemp29 * std::cos(fConst32 * fRec4[0]);
		float fTemp30 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.57191193f * fRec2[0], fRec3[0])));
		fRec33[0] = fTemp0 - fRec33[2] * T_marimba_bar_faustpower2_f(fTemp30) + 2.0f * fRec33[1] * fTemp30 * std::cos(fConst33 * fRec4[0]);
		float fTemp31 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.58087826f * fRec2[0], fRec3[0])));
		fRec34[0] = fTemp0 - fRec34[2] * T_marimba_bar_faustpower2_f(fTemp31) + 2.0f * fRec34[1] * fTemp31 * std::cos(fConst34 * fRec4[0]);
		float fTemp32 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.60909903f * fRec2[0], fRec3[0])));
		fRec35[0] = fTemp0 - fRec35[2] * T_marimba_bar_faustpower2_f(fTemp32) + 2.0f * fRec35[1] * fTemp32 * std::cos(fConst35 * fRec4[0]);
		float fTemp33 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6483376f * fRec2[0], fRec3[0])));
		fRec36[0] = fTemp0 - fRec36[2] * T_marimba_bar_faustpower2_f(fTemp33) + 2.0f * fRec36[1] * fTemp33 * std::cos(fConst36 * fRec4[0]);
		float fTemp34 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6633258f * fRec2[0], fRec3[0])));
		fRec37[0] = fTemp0 - fRec37[2] * T_marimba_bar_faustpower2_f(fTemp34) + 2.0f * fRec37[1] * fTemp34 * std::cos(fConst37 * fRec4[0]);
		float fTemp35 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.6749458f * fRec2[0], fRec3[0])));
		fRec38[0] = fTemp0 - fRec38[2] * T_marimba_bar_faustpower2_f(fTemp35) + 2.0f * fRec38[1] * fTemp35 * std::cos(fConst38 * fRec4[0]);
		float fTemp36 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.68116146f * fRec2[0], fRec3[0])));
		fRec39[0] = fTemp0 - fRec39[2] * T_marimba_bar_faustpower2_f(fTemp36) + 2.0f * fRec39[1] * fTemp36 * std::cos(fConst39 * fRec4[0]);
		float fTemp37 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7376643f * fRec2[0], fRec3[0])));
		fRec40[0] = fTemp0 - fRec40[2] * T_marimba_bar_faustpower2_f(fTemp37) + 2.0f * fRec40[1] * fTemp37 * std::cos(fConst40 * fRec4[0]);
		float fTemp38 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7626139f * fRec2[0], fRec3[0])));
		fRec41[0] = fTemp0 - fRec41[2] * T_marimba_bar_faustpower2_f(fTemp38) + 2.0f * fRec41[1] * fTemp38 * std::cos(fConst41 * fRec4[0]);
		float fTemp39 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7758567f * fRec2[0], fRec3[0])));
		fRec42[0] = fTemp0 - fRec42[2] * T_marimba_bar_faustpower2_f(fTemp39) + 2.0f * fRec42[1] * fTemp39 * std::cos(fConst42 * fRec4[0]);
		float fTemp40 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.7816386f * fRec2[0], fRec3[0])));
		fRec43[0] = fTemp0 - fRec43[2] * T_marimba_bar_faustpower2_f(fTemp40) + 2.0f * fRec43[1] * fTemp40 * std::cos(fConst43 * fRec4[0]);
		float fTemp41 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.79404455f * fRec2[0], fRec3[0])));
		fRec44[0] = fTemp0 - fRec44[2] * T_marimba_bar_faustpower2_f(fTemp41) + 2.0f * fRec44[1] * fTemp41 * std::cos(fConst44 * fRec4[0]);
		float fTemp42 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.80724144f * fRec2[0], fRec3[0])));
		fRec45[0] = fTemp0 - fRec45[2] * T_marimba_bar_faustpower2_f(fTemp42) + 2.0f * fRec45[1] * fTemp42 * std::cos(fConst45 * fRec4[0]);
		float fTemp43 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8578755f * fRec2[0], fRec3[0])));
		fRec46[0] = fTemp0 - fRec46[2] * T_marimba_bar_faustpower2_f(fTemp43) + 2.0f * fRec46[1] * fTemp43 * std::cos(fConst46 * fRec4[0]);
		float fTemp44 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.8770534f * fRec2[0], fRec3[0])));
		fRec47[0] = fTemp0 - fRec47[2] * T_marimba_bar_faustpower2_f(fTemp44) + 2.0f * fRec47[1] * fTemp44 * std::cos(fConst47 * fRec4[0]);
		float fTemp45 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.89032686f * fRec2[0], fRec3[0])));
		fRec48[0] = fTemp0 - fRec48[2] * T_marimba_bar_faustpower2_f(fTemp45) + 2.0f * fRec48[1] * fTemp45 * std::cos(fConst48 * fRec4[0]);
		float fTemp46 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9131535f * fRec2[0], fRec3[0])));
		fRec49[0] = fTemp0 - fRec49[2] * T_marimba_bar_faustpower2_f(fTemp46) + 2.0f * fRec49[1] * fTemp46 * std::cos(fConst49 * fRec4[0]);
		float fTemp47 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9261412f * fRec2[0], fRec3[0])));
		fRec50[0] = fTemp0 - fRec50[2] * T_marimba_bar_faustpower2_f(fTemp47) + 2.0f * fRec50[1] * fTemp47 * std::cos(fConst50 * fRec4[0]);
		float fTemp48 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9461254f * fRec2[0], fRec3[0])));
		fRec51[0] = fTemp0 - fRec51[2] * T_marimba_bar_faustpower2_f(fTemp48) + 2.0f * fRec51[1] * fTemp48 * std::cos(fConst51 * fRec4[0]);
		float fTemp49 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9714935f * fRec2[0], fRec3[0])));
		fRec52[0] = fTemp0 - fRec52[2] * T_marimba_bar_faustpower2_f(fTemp49) + 2.0f * fRec52[1] * fTemp49 * std::cos(fConst52 * fRec4[0]);
		float fTemp50 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.9807864f * fRec2[0], fRec3[0])));
		fRec53[0] = fTemp0 - fRec53[2] * T_marimba_bar_faustpower2_f(fTemp50) + 2.0f * fRec53[1] * fTemp50 * std::cos(fConst53 * fRec4[0]);
		float fTemp51 = std::pow(0.001f, fConst1 / (fRec1[0] * std::pow(1.0f - 0.99770355f * fRec2[0], fRec3[0])));
		fRec54[0] = fTemp0 - fRec54[2] * T_marimba_bar_faustpower2_f(fTemp51) + 2.0f * fRec54[1] * fTemp51 * std::cos(fConst54 * fRec4[0]);
		outputs[0] = static_cast<FAUSTFLOAT>(0.02f * ((fRec0[0] - fRec0[2]) * ((fRec4[0] < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(iTemp2, 249))] : 0.0f) + (fRec6[0] - fRec6[2]) * (((3.31356f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(1, iTemp2), 249))] : 0.0f) + (fRec7[0] - fRec7[2]) * (((3.83469f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(2, iTemp2), 249))] : 0.0f) + (fRec8[0] - fRec8[2]) * (((8.06313f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(3, iTemp2), 249))] : 0.0f) + (fRec9[0] - fRec9[2]) * (((9.44778f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(4, iTemp2), 249))] : 0.0f) + (fRec10[0] - fRec10[2]) * (((14.1169f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(5, iTemp2), 249))] : 0.0f) + (fRec11[0] - fRec11[2]) * (((18.384f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(6, iTemp2), 249))] : 0.0f) + (fRec12[0] - fRec12[2]) * (((21.0102f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(7, iTemp2), 249))] : 0.0f) + (fRec13[0] - fRec13[2]) * (((26.1775f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(8, iTemp2), 249))] : 0.0f) + (fRec14[0] - fRec14[2]) * (((28.9944f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(9, iTemp2), 249))] : 0.0f) + (fRec15[0] - fRec15[2]) * (((37.0728f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(10, iTemp2), 249))] : 0.0f) + (fRec16[0] - fRec16[2]) * (((37.8703f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(11, iTemp2), 249))] : 0.0f) + (fRec17[0] - fRec17[2]) * (((40.0634f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(12, iTemp2), 249))] : 0.0f) + (fRec18[0] - fRec18[2]) * (((47.6439f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(13, iTemp2), 249))] : 0.0f) + (fRec19[0] - fRec19[2]) * (((51.019f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(14, iTemp2), 249))] : 0.0f) + (fRec20[0] - fRec20[2]) * (((52.43f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(15, iTemp2), 249))] : 0.0f) + (fRec21[0] - fRec21[2]) * (((58.286f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(16, iTemp2), 249))] : 0.0f) + (fRec22[0] - fRec22[2]) * (((63.5486f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(17, iTemp2), 249))] : 0.0f) + (fRec23[0] - fRec23[2]) * (((65.3628f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(18, iTemp2), 249))] : 0.0f) + (fRec24[0] - fRec24[2]) * (((66.9587f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(19, iTemp2), 249))] : 0.0f) + (fRec25[0] - fRec25[2]) * (((74.5301f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(20, iTemp2), 249))] : 0.0f) + (fRec26[0] - fRec26[2]) * (((78.692f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(21, iTemp2), 249))] : 0.0f) + (fRec27[0] - fRec27[2]) * (((80.8375f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(22, iTemp2), 249))] : 0.0f) + (fRec28[0] - fRec28[2]) * (((89.978f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(23, iTemp2), 249))] : 0.0f) + (fRec29[0] - fRec29[2]) * (((92.9661f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(24, iTemp2), 249))] : 0.0f) + (fRec30[0] - fRec30[2]) * (((95.1914f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(25, iTemp2), 249))] : 0.0f) + (fRec31[0] - fRec31[2]) * (((97.4807f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(26, iTemp2), 249))] : 0.0f) + (fRec32[0] - fRec32[2]) * (((110.62f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(27, iTemp2), 249))] : 0.0f) + (fRec33[0] - fRec33[2]) * (((112.069f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(28, iTemp2), 249))] : 0.0f) + (fRec34[0] - fRec34[2]) * (((113.826f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(29, iTemp2), 249))] : 0.0f) + (fRec35[0] - fRec35[2]) * (((119.356f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(30, iTemp2), 249))] : 0.0f) + (fRec36[0] - fRec36[2]) * (((127.045f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(31, iTemp2), 249))] : 0.0f) + (fRec37[0] - fRec37[2]) * (((129.982f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(32, iTemp2), 249))] : 0.0f) + (fRec38[0] - fRec38[2]) * (((132.259f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(33, iTemp2), 249))] : 0.0f) + (fRec39[0] - fRec39[2]) * (((133.477f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(34, iTemp2), 249))] : 0.0f) + (fRec40[0] - fRec40[2]) * (((144.549f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(35, iTemp2), 249))] : 0.0f) + (fRec41[0] - fRec41[2]) * (((149.438f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(36, iTemp2), 249))] : 0.0f) + (fRec42[0] - fRec42[2]) * (((152.033f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(37, iTemp2), 249))] : 0.0f) + (fRec43[0] - fRec43[2]) * (((153.166f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(38, iTemp2), 249))] : 0.0f) + (fRec44[0] - fRec44[2]) * (((155.597f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(39, iTemp2), 249))] : 0.0f) + (fRec45[0] - fRec45[2]) * (((158.183f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(40, iTemp2), 249))] : 0.0f) + (fRec46[0] - fRec46[2]) * (((168.105f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(41, iTemp2), 249))] : 0.0f) + (fRec47[0] - fRec47[2]) * (((171.863f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(42, iTemp2), 249))] : 0.0f) + (fRec48[0] - fRec48[2]) * (((174.464f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(43, iTemp2), 249))] : 0.0f) + (fRec49[0] - fRec49[2]) * (((178.937f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(44, iTemp2), 249))] : 0.0f) + (fRec50[0] - fRec50[2]) * (((181.482f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(45, iTemp2), 249))] : 0.0f) + (fRec51[0] - fRec51[2]) * (((185.398f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(46, iTemp2), 249))] : 0.0f) + (fRec52[0] - fRec52[2]) * (((190.369f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(47, iTemp2), 249))] : 0.0f) + (fRec53[0] - fRec53[2]) * (((192.19f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(48, iTemp2), 249))] : 0.0f) + (fRec54[0] - fRec54[2]) * (((195.505f * fRec4[0]) < fConst5) ? ftbl0T_marimba_barSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(49, iTemp2), 249))] : 0.0f)));
		fRec1[1] = fRec1[0];
		fRec2[1] = fRec2[0];
		fRec3[1] = fRec3[0];
		fRec4[1] = fRec4[0];
		fRec0[2] = fRec0[1];
		fRec0[1] = fRec0[0];
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
		fRec54[2] = fRec54[1];
		fRec54[1] = fRec54[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
