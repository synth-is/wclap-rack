/* ------------------------------------------------------------
name: "marimba"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_marimba -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_marimba_H__
#define  __T_marimba_H__

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
#define FAUSTCLASS T_marimba
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

const static float fT_marimbaSIG0Wave0[250] = {1.0f,0.776725f,0.625723f,0.855223f,0.760159f,0.698373f,0.768011f,0.641127f,0.244034f,0.707754f,0.634013f,0.247527f,0.660849f,0.450396f,0.567783f,0.106361f,0.716814f,0.66392f,0.291208f,0.310599f,0.801495f,0.635292f,0.307435f,0.874124f,0.497668f,0.487088f,0.459115f,0.733455f,0.541818f,0.441318f,0.31392f,0.40309f,0.685353f,0.60314f,0.400552f,0.453511f,0.634386f,0.291547f,0.131605f,0.368507f,0.839907f,0.60216f,0.288296f,0.57967f,0.0242493f,0.262746f,0.368588f,0.890284f,0.408963f,0.556072f,0.884427f,0.83211f,0.612015f,0.757176f,0.919477f,1.0f,0.827963f,0.89241f,0.0357408f,0.480789f,0.752872f,0.0546301f,0.235937f,0.362938f,0.444472f,0.101751f,0.703418f,0.453136f,0.316629f,0.490394f,0.982508f,0.551622f,0.602009f,0.666957f,0.77683f,0.905662f,0.0987197f,0.402968f,0.829452f,0.307645f,0.64048f,0.983971f,0.584205f,0.650365f,0.334447f,0.58357f,0.540191f,0.672534f,0.245712f,0.687298f,0.883058f,0.79295f,0.600619f,0.572682f,0.122612f,0.388248f,0.290658f,0.380255f,0.290967f,0.567819f,0.0737721f,0.42099f,0.0786578f,0.393995f,0.268983f,0.260614f,0.494086f,0.238026f,0.0987824f,0.277879f,0.440563f,0.0770212f,0.450591f,0.128137f,0.0368275f,0.128699f,0.329605f,0.374512f,0.36359f,0.272594f,0.379052f,0.305241f,0.0741129f,0.345728f,0.29935f,0.221284f,0.0261391f,0.293202f,0.361885f,0.11433f,0.239005f,0.434156f,0.329583f,0.21946f,0.284175f,0.198555f,0.431976f,0.302985f,1.0f,0.146221f,0.140701f,0.264243f,0.185997f,0.426322f,0.30478f,0.34399f,0.19543f,0.386955f,0.1876f,0.172812f,0.0434115f,0.303761f,0.069454f,0.453943f,0.832451f,0.317817f,0.940601f,1.0f,0.180658f,0.737921f,0.832297f,0.402352f,0.126786f,0.594398f,0.485455f,0.32447f,0.365102f,0.777922f,0.588272f,0.401353f,0.610735f,0.158693f,0.0746072f,0.825099f,0.925459f,0.65377f,0.260792f,0.719384f,0.559908f,0.37259f,0.360035f,0.622939f,0.210271f,0.444595f,0.311286f,0.464309f,0.557231f,0.52408f,0.0701056f,0.320749f,0.19446f,0.727609f,0.522062f,0.394004f,0.235035f,0.395646f,0.494796f,0.517318f,0.109752f,0.692849f,0.00632009f,0.0207583f,0.00306107f,0.0637191f,0.081661f,0.03511f,0.127814f,0.202294f,0.0764145f,0.263127f,0.400199f,0.267278f,0.633385f,1.0f,0.739902f,0.413763f,0.41811f,0.612715f,0.672374f,0.339674f,0.21172f,0.459645f,0.1025f,0.32589f,0.148154f,0.265442f,0.0974305f,0.286438f,0.275213f,0.109111f,0.575089f,0.370283f,0.29411f,0.259826f,0.0648719f,0.583418f,0.282663f,0.182004f,0.117421f,0.417727f,0.16965f,0.24853f,0.122819f,0.185486f,0.0433618f,0.373849f,0.252768f,0.195103f,0.0927835f,0.166543f};
class T_marimbaSIG0 {
	
  private:
	
	int fT_marimbaSIG0Wave0_idx;
	int fSampleRate;
	
  public:
	
	int getNumInputsT_marimbaSIG0() {
		return 0;
	}
	int getNumOutputsT_marimbaSIG0() {
		return 1;
	}
	
	void instanceInitT_marimbaSIG0(int sample_rate) {
		fSampleRate = sample_rate;
		fT_marimbaSIG0Wave0_idx = 0;
	}
	
	void fillT_marimbaSIG0(int count, float* table) {
		for (int i1 = 0; i1 < count; i1 = faust_wrap_add(i1, 1)) {
			table[i1] = fT_marimbaSIG0Wave0[fT_marimbaSIG0Wave0_idx];
			fT_marimbaSIG0Wave0_idx = (faust_wrap_add(1, fT_marimbaSIG0Wave0_idx)) % 250;
		}
	}

};

static T_marimbaSIG0* newT_marimbaSIG0() { return (T_marimbaSIG0*)new T_marimbaSIG0(); }
static void deleteT_marimbaSIG0(T_marimbaSIG0* dsp) { delete dsp; }

static float T_marimba_faustpower2_f(float value) {
	return value * value;
}
static float ftbl0T_marimbaSIG0[250];

class T_marimba : public FaustDsp {
	
 private:
	
	int IOTA0;
	float fRec0[2048];
	float fRec1[2];
	int fSampleRate;
	float fConst0;
	float fConst1;
	float fConst2;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst3;
	float fRec4[2];
	float fConst4;
	float fConst5;
	float fConst6;
	float fConst7;
	float fRec5[3];
	float fConst8;
	FAUSTFLOAT fHslider1;
	float fSlow1;
	float fRec6[2];
	float fConst9;
	float fConst10;
	float fConst11;
	float fConst12;
	float fRec7[3];
	float fConst13;
	float fConst14;
	float fConst15;
	float fConst16;
	float fRec8[3];
	float fConst17;
	float fConst18;
	float fConst19;
	float fConst20;
	float fRec9[3];
	float fConst21;
	float fConst22;
	float fConst23;
	float fConst24;
	float fRec10[3];
	float fConst25;
	float fConst26;
	float fConst27;
	float fConst28;
	float fRec11[3];
	float fConst29;
	float fConst30;
	float fConst31;
	float fConst32;
	float fRec12[3];
	float fConst33;
	float fConst34;
	float fConst35;
	float fConst36;
	float fRec13[3];
	float fConst37;
	float fConst38;
	float fConst39;
	float fConst40;
	float fRec14[3];
	float fConst41;
	float fConst42;
	float fConst43;
	float fConst44;
	float fRec15[3];
	float fConst45;
	float fConst46;
	float fConst47;
	float fConst48;
	float fRec16[3];
	float fConst49;
	float fConst50;
	float fConst51;
	float fConst52;
	float fRec17[3];
	float fConst53;
	float fConst54;
	float fConst55;
	float fConst56;
	float fRec18[3];
	float fConst57;
	float fConst58;
	float fConst59;
	float fConst60;
	float fRec19[3];
	float fConst61;
	float fConst62;
	float fConst63;
	float fConst64;
	float fRec20[3];
	float fConst65;
	float fConst66;
	float fConst67;
	float fConst68;
	float fRec21[3];
	float fConst69;
	float fConst70;
	float fConst71;
	float fConst72;
	float fRec22[3];
	float fConst73;
	float fConst74;
	float fConst75;
	float fConst76;
	float fRec23[3];
	float fConst77;
	float fConst78;
	float fConst79;
	float fConst80;
	float fRec24[3];
	float fConst81;
	float fConst82;
	float fConst83;
	float fConst84;
	float fRec25[3];
	float fConst85;
	float fConst86;
	float fConst87;
	float fConst88;
	float fRec26[3];
	float fConst89;
	float fConst90;
	float fConst91;
	float fConst92;
	float fRec27[3];
	float fConst93;
	float fConst94;
	float fConst95;
	float fConst96;
	float fRec28[3];
	float fConst97;
	float fConst98;
	float fConst99;
	float fConst100;
	float fRec29[3];
	float fConst101;
	float fConst102;
	float fConst103;
	float fConst104;
	float fRec30[3];
	float fConst105;
	float fConst106;
	float fConst107;
	float fConst108;
	float fRec31[3];
	float fConst109;
	float fConst110;
	float fConst111;
	float fConst112;
	float fRec32[3];
	float fConst113;
	float fConst114;
	float fConst115;
	float fConst116;
	float fRec33[3];
	float fConst117;
	float fConst118;
	float fConst119;
	float fConst120;
	float fRec34[3];
	float fConst121;
	float fConst122;
	float fConst123;
	float fConst124;
	float fRec35[3];
	float fConst125;
	float fConst126;
	float fConst127;
	float fConst128;
	float fRec36[3];
	float fConst129;
	float fConst130;
	float fConst131;
	float fConst132;
	float fRec37[3];
	float fConst133;
	float fConst134;
	float fConst135;
	float fConst136;
	float fRec38[3];
	float fConst137;
	float fConst138;
	float fConst139;
	float fConst140;
	float fRec39[3];
	float fConst141;
	float fConst142;
	float fConst143;
	float fConst144;
	float fRec40[3];
	float fConst145;
	float fConst146;
	float fConst147;
	float fConst148;
	float fRec41[3];
	float fConst149;
	float fConst150;
	float fConst151;
	float fConst152;
	float fRec42[3];
	float fConst153;
	float fConst154;
	float fConst155;
	float fConst156;
	float fRec43[3];
	float fConst157;
	float fConst158;
	float fConst159;
	float fConst160;
	float fRec44[3];
	float fConst161;
	float fConst162;
	float fConst163;
	float fConst164;
	float fRec45[3];
	float fConst165;
	float fConst166;
	float fConst167;
	float fConst168;
	float fRec46[3];
	float fConst169;
	float fConst170;
	float fConst171;
	float fConst172;
	float fRec47[3];
	float fConst173;
	float fConst174;
	float fConst175;
	float fConst176;
	float fRec48[3];
	float fConst177;
	float fConst178;
	float fConst179;
	float fConst180;
	float fRec49[3];
	float fConst181;
	float fConst182;
	float fConst183;
	float fConst184;
	float fRec50[3];
	float fConst185;
	float fConst186;
	float fConst187;
	float fConst188;
	float fRec51[3];
	float fConst189;
	float fConst190;
	float fConst191;
	float fConst192;
	float fRec52[3];
	float fConst193;
	float fConst194;
	float fConst195;
	float fConst196;
	float fRec53[3];
	float fConst197;
	float fConst198;
	float fConst199;
	float fConst200;
	float fRec54[3];
	float fConst201;
	float fConst202;
	float fConst203;
	float fConst204;
	float fRec55[3];
	float fVec0[2];
	float fVec1[2048];
	float fConst205;
	float fRec2[2];
	float fRec3[2];
	
 public:
	T_marimba() {
	}
	
	T_marimba(const T_marimba&) = default;
	
	virtual ~T_marimba() = default;
	
	T_marimba& operator=(const T_marimba&) = default;
	
	void metadata(Meta* m) { 
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_marimba -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/fdelay4:author", "Julius O. Smith III");
		m->declare("delays.lib/fdelayltv:author", "Julius O. Smith III");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("filename", "marimba.dsp");
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
		m->declare("name", "marimba");
		m->declare("physmodels.lib/name", "Faust Physical Models Library");
		m->declare("physmodels.lib/version", "1.2.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
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
		T_marimbaSIG0* sig0 = newT_marimbaSIG0();
		sig0->instanceInitT_marimbaSIG0(sample_rate);
		sig0->fillT_marimbaSIG0(250, ftbl0T_marimbaSIG0);
		deleteT_marimbaSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 0.0014705883f * fConst0;
		fConst2 = 44.1f / fConst0;
		fConst3 = 1.0f - fConst2;
		fConst4 = std::pow(0.001f, 10.259114f / fConst0);
		fConst5 = T_marimba_faustpower2_f(fConst4);
		fConst6 = 2.0f * fConst4;
		fConst7 = 6.2831855f / fConst0;
		fConst8 = 0.5f * fConst0 + -1.0f;
		fConst9 = std::pow(0.001f, 10.890133f / fConst0);
		fConst10 = T_marimba_faustpower2_f(fConst9);
		fConst11 = 2.0f * fConst9;
		fConst12 = 20.819712f / fConst0;
		fConst13 = std::pow(0.001f, 11.038634f / fConst0);
		fConst14 = T_marimba_faustpower2_f(fConst13);
		fConst15 = 2.0f * fConst13;
		fConst16 = 24.094069f / fConst0;
		fConst17 = std::pow(0.001f, 12.337915f / fConst0);
		fConst18 = T_marimba_faustpower2_f(fConst17);
		fConst19 = 2.0f * fConst17;
		fConst20 = 50.66214f / fConst0;
		fConst21 = std::pow(0.001f, 12.802757f / fConst0);
		fConst22 = T_marimba_faustpower2_f(fConst21);
		fConst23 = 2.0f * fConst21;
		fConst24 = 59.362152f / fConst0;
		fConst25 = std::pow(0.001f, 14.533069f / fConst0);
		fConst26 = T_marimba_faustpower2_f(fConst25);
		fConst27 = 2.0f * fConst25;
		fConst28 = 88.6991f / fConst0;
		fConst29 = std::pow(0.001f, 16.365208f / fConst0);
		fConst30 = T_marimba_faustpower2_f(fConst29);
		fConst31 = 2.0f * fConst29;
		fConst32 = 115.51008f / fConst0;
		fConst33 = std::pow(0.001f, 17.630983f / fConst0);
		fConst34 = T_marimba_faustpower2_f(fConst33);
		fConst35 = 2.0f * fConst33;
		fConst36 = 132.01099f / fConst0;
		fConst37 = std::pow(0.001f, 20.48241f / fConst0);
		fConst38 = T_marimba_faustpower2_f(fConst37);
		fConst39 = 2.0f * fConst37;
		fConst40 = 164.47809f / fConst0;
		fConst41 = std::pow(0.001f, 22.269566f / fConst0);
		fConst42 = T_marimba_faustpower2_f(fConst41);
		fConst43 = 2.0f * fConst41;
		fConst44 = 182.17719f / fConst0;
		fConst45 = std::pow(0.001f, 28.536816f / fConst0);
		fConst46 = T_marimba_faustpower2_f(fConst45);
		fConst47 = 2.0f * fConst45;
		fConst48 = 232.93527f / fConst0;
		fConst49 = std::pow(0.001f, 29.263924f / fConst0);
		fConst50 = T_marimba_faustpower2_f(fConst49);
		fConst51 = 2.0f * fConst49;
		fConst52 = 237.9461f / fConst0;
		fConst53 = std::pow(0.001f, 31.381102f / fConst0);
		fConst54 = T_marimba_faustpower2_f(fConst53);
		fConst55 = 2.0f * fConst53;
		fConst56 = 251.72577f / fConst0;
		fConst57 = std::pow(0.001f, 40.26368f / fConst0);
		fConst58 = T_marimba_faustpower2_f(fConst57);
		fConst59 = 2.0f * fConst57;
		fConst60 = 299.35544f / fConst0;
		fConst61 = std::pow(0.001f, 45.175232f / fConst0);
		fConst62 = T_marimba_faustpower2_f(fConst61);
		fConst63 = 2.0f * fConst61;
		fConst64 = 320.56183f / fConst0;
		fConst65 = std::pow(0.001f, 47.439922f / fConst0);
		fConst66 = T_marimba_faustpower2_f(fConst65);
		fConst67 = 2.0f * fConst65;
		fConst68 = 329.4274f / fConst0;
		fConst69 = std::pow(0.001f, 58.4253f / fConst0);
		fConst70 = T_marimba_faustpower2_f(fConst69);
		fConst71 = 2.0f * fConst69;
		fConst72 = 366.22174f / fConst0;
		fConst73 = std::pow(0.001f, 70.99649f / fConst0);
		fConst74 = T_marimba_faustpower2_f(fConst73);
		fConst75 = 2.0f * fConst73;
		fConst76 = 399.28763f / fConst0;
		fConst77 = std::pow(0.001f, 76.06687f / fConst0);
		fConst78 = T_marimba_faustpower2_f(fConst77);
		fConst79 = 2.0f * fConst77;
		fConst80 = 410.68658f / fConst0;
		fConst81 = std::pow(0.001f, 80.89012f / fConst0);
		fConst82 = T_marimba_faustpower2_f(fConst81);
		fConst83 = 2.0f * fConst81;
		fConst84 = 420.71393f / fConst0;
		fConst85 = std::pow(0.001f, 109.456856f / fConst0);
		fConst86 = T_marimba_faustpower2_f(fConst85);
		fConst87 = 2.0f * fConst85;
		fConst88 = 468.28644f / fConst0;
		fConst89 = std::pow(0.001f, 130.3097f / fConst0);
		fConst90 = T_marimba_faustpower2_f(fConst89);
		fConst91 = 2.0f * fConst89;
		fConst92 = 494.43643f / fConst0;
		fConst93 = std::pow(0.001f, 142.91408f / fConst0);
		fConst94 = T_marimba_faustpower2_f(fConst93);
		fConst95 = 2.0f * fConst93;
		fConst96 = 507.917f / fConst0;
		fConst97 = std::pow(0.001f, 216.13428f / fConst0);
		fConst98 = T_marimba_faustpower2_f(fConst97);
		fConst99 = 2.0f * fConst97;
		fConst100 = 565.34845f / fConst0;
		fConst101 = std::pow(0.001f, 249.36165f / fConst0);
		fConst102 = T_marimba_faustpower2_f(fConst101);
		fConst103 = 2.0f * fConst101;
		fConst104 = 584.1232f / fConst0;
		fConst105 = std::pow(0.001f, 278.13995f / fConst0);
		fConst106 = T_marimba_faustpower2_f(fConst105);
		fConst107 = 2.0f * fConst105;
		fConst108 = 598.1052f / fConst0;
		fConst109 = std::pow(0.001f, 312.0091f / fConst0);
		fConst110 = T_marimba_faustpower2_f(fConst109);
		fConst111 = 2.0f * fConst109;
		fConst112 = 612.4893f / fConst0;
		fConst113 = std::pow(0.001f, 638.4778f / fConst0);
		fConst114 = T_marimba_faustpower2_f(fConst113);
		fConst115 = 2.0f * fConst113;
		fConst116 = 695.04596f / fConst0;
		fConst117 = std::pow(0.001f, 695.5596f / fConst0);
		fConst118 = T_marimba_faustpower2_f(fConst117);
		fConst119 = 2.0f * fConst117;
		fConst120 = 704.15027f / fConst0;
		fConst121 = std::pow(0.001f, 773.2129f / fConst0);
		fConst122 = T_marimba_faustpower2_f(fConst121);
		fConst123 = 2.0f * fConst121;
		fConst124 = 715.1899f / fConst0;
		fConst125 = std::pow(0.001f, 1095.6359f / fConst0);
		fConst126 = T_marimba_faustpower2_f(fConst125);
		fConst127 = 2.0f * fConst125;
		fConst128 = 749.93585f / fConst0;
		fConst129 = std::pow(0.001f, 1859.39f / fConst0);
		fConst130 = T_marimba_faustpower2_f(fConst129);
		fConst131 = 2.0f * fConst129;
		fConst132 = 798.24725f / fConst0;
		fConst133 = std::pow(0.001f, 2311.8018f / fConst0);
		fConst134 = T_marimba_faustpower2_f(fConst133);
		fConst135 = 2.0f * fConst133;
		fConst136 = 816.701f / fConst0;
		fConst137 = std::pow(0.001f, 2755.6304f / fConst0);
		fConst138 = T_marimba_faustpower2_f(fConst137);
		fConst139 = 2.0f * fConst137;
		fConst140 = 831.0078f / fConst0;
		fConst141 = std::pow(0.001f, 3034.9124f / fConst0);
		fConst142 = T_marimba_faustpower2_f(fConst141);
		fConst143 = 2.0f * fConst141;
		fConst144 = 838.6607f / fConst0;
		fConst145 = std::pow(0.001f, 8048.458f / fConst0);
		fConst146 = T_marimba_faustpower2_f(fConst145);
		fConst147 = 2.0f * fConst145;
		fConst148 = 908.22815f / fConst0;
		fConst149 = std::pow(0.001f, 13265.488f / fConst0);
		fConst150 = T_marimba_faustpower2_f(fConst149);
		fConst151 = 2.0f * fConst149;
		fConst152 = 938.94666f / fConst0;
		fConst153 = std::pow(0.001f, 17675.48f / fConst0);
		fConst154 = T_marimba_faustpower2_f(fConst153);
		fConst155 = 2.0f * fConst153;
		fConst156 = 955.2515f / fConst0;
		fConst157 = std::pow(0.001f, 20142.857f / fConst0);
		fConst158 = T_marimba_faustpower2_f(fConst157);
		fConst159 = 2.0f * fConst157;
		fConst160 = 962.37036f / fConst0;
		fConst161 = std::pow(0.001f, 26985.693f / fConst0);
		fConst162 = T_marimba_faustpower2_f(fConst161);
		fConst163 = 2.0f * fConst161;
		fConst164 = 977.6448f / fConst0;
		fConst165 = std::pow(0.001f, 37577.844f / fConst0);
		fConst166 = T_marimba_faustpower2_f(fConst165);
		fConst167 = 2.0f * fConst165;
		fConst168 = 993.8931f / fConst0;
		fConst169 = std::pow(0.001f, 172447.05f / fConst0);
		fConst170 = T_marimba_faustpower2_f(fConst169);
		fConst171 = 2.0f * fConst169;
		fConst172 = 1056.2349f / fConst0;
		fConst173 = std::pow(0.001f, 355973.34f / fConst0);
		fConst174 = T_marimba_faustpower2_f(fConst173);
		fConst175 = 2.0f * fConst173;
		fConst176 = 1079.847f / fConst0;
		fConst177 = std::pow(0.001f, 630229.4f / fConst0);
		fConst178 = T_marimba_faustpower2_f(fConst177);
		fConst179 = 2.0f * fConst177;
		fConst180 = 1096.1897f / fConst0;
		fConst181 = std::pow(0.001f, 2024133.5f / fConst0);
		fConst182 = T_marimba_faustpower2_f(fConst181);
		fConst183 = 2.0f * fConst181;
		fConst184 = 1124.2943f / fConst0;
		fConst185 = std::pow(0.001f, 4549764.5f / fConst0);
		fConst186 = T_marimba_faustpower2_f(fConst185);
		fConst187 = 2.0f * fConst185;
		fConst188 = 1140.285f / fConst0;
		fConst189 = std::pow(0.001f, 2.203328e+07f / fConst0);
		fConst190 = T_marimba_faustpower2_f(fConst189);
		fConst191 = 2.0f * fConst189;
		fConst192 = 1164.89f / fConst0;
		fConst193 = std::pow(0.001f, 5.3122326e+08f / fConst0);
		fConst194 = T_marimba_faustpower2_f(fConst193);
		fConst195 = 2.0f * fConst193;
		fConst196 = 1196.1237f / fConst0;
		fConst197 = std::pow(0.001f, 3.8190633e+09f / fConst0);
		fConst198 = T_marimba_faustpower2_f(fConst197);
		fConst199 = 2.0f * fConst197;
		fConst200 = 1207.5654f / fConst0;
		fConst201 = std::pow(0.001f, 1.5657384e+14f / fConst0);
		fConst202 = T_marimba_faustpower2_f(fConst201);
		fConst203 = 2.0f * fConst201;
		fConst204 = 1228.3942f / fConst0;
		fConst205 = 0.00882353f * fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(2.2e+02f);
		fHslider1 = static_cast<FAUSTFLOAT>(2.0f);
	}
	
	virtual void instanceClear() {
		IOTA0 = 0;
		for (int l0 = 0; l0 < 2048; l0 = faust_wrap_add(l0, 1)) {
			fRec0[l0] = 0.0f;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec1[l1] = 0.0f;
		}
		for (int l2 = 0; l2 < 2; l2 = faust_wrap_add(l2, 1)) {
			fRec4[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 3; l3 = faust_wrap_add(l3, 1)) {
			fRec5[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = faust_wrap_add(l4, 1)) {
			fRec6[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 3; l5 = faust_wrap_add(l5, 1)) {
			fRec7[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 3; l6 = faust_wrap_add(l6, 1)) {
			fRec8[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 3; l7 = faust_wrap_add(l7, 1)) {
			fRec9[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 3; l8 = faust_wrap_add(l8, 1)) {
			fRec10[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 3; l9 = faust_wrap_add(l9, 1)) {
			fRec11[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 3; l10 = faust_wrap_add(l10, 1)) {
			fRec12[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 3; l11 = faust_wrap_add(l11, 1)) {
			fRec13[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 3; l12 = faust_wrap_add(l12, 1)) {
			fRec14[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 3; l13 = faust_wrap_add(l13, 1)) {
			fRec15[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 3; l14 = faust_wrap_add(l14, 1)) {
			fRec16[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 3; l15 = faust_wrap_add(l15, 1)) {
			fRec17[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 3; l16 = faust_wrap_add(l16, 1)) {
			fRec18[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 3; l17 = faust_wrap_add(l17, 1)) {
			fRec19[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 3; l18 = faust_wrap_add(l18, 1)) {
			fRec20[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 3; l19 = faust_wrap_add(l19, 1)) {
			fRec21[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 3; l20 = faust_wrap_add(l20, 1)) {
			fRec22[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 3; l21 = faust_wrap_add(l21, 1)) {
			fRec23[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 3; l22 = faust_wrap_add(l22, 1)) {
			fRec24[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 3; l23 = faust_wrap_add(l23, 1)) {
			fRec25[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 3; l24 = faust_wrap_add(l24, 1)) {
			fRec26[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 3; l25 = faust_wrap_add(l25, 1)) {
			fRec27[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 3; l26 = faust_wrap_add(l26, 1)) {
			fRec28[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 3; l27 = faust_wrap_add(l27, 1)) {
			fRec29[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 3; l28 = faust_wrap_add(l28, 1)) {
			fRec30[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 3; l29 = faust_wrap_add(l29, 1)) {
			fRec31[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 3; l30 = faust_wrap_add(l30, 1)) {
			fRec32[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 3; l31 = faust_wrap_add(l31, 1)) {
			fRec33[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 3; l32 = faust_wrap_add(l32, 1)) {
			fRec34[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 3; l33 = faust_wrap_add(l33, 1)) {
			fRec35[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 3; l34 = faust_wrap_add(l34, 1)) {
			fRec36[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 3; l35 = faust_wrap_add(l35, 1)) {
			fRec37[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 3; l36 = faust_wrap_add(l36, 1)) {
			fRec38[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 3; l37 = faust_wrap_add(l37, 1)) {
			fRec39[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 3; l38 = faust_wrap_add(l38, 1)) {
			fRec40[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 3; l39 = faust_wrap_add(l39, 1)) {
			fRec41[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 3; l40 = faust_wrap_add(l40, 1)) {
			fRec42[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 3; l41 = faust_wrap_add(l41, 1)) {
			fRec43[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 3; l42 = faust_wrap_add(l42, 1)) {
			fRec44[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 3; l43 = faust_wrap_add(l43, 1)) {
			fRec45[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 3; l44 = faust_wrap_add(l44, 1)) {
			fRec46[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 3; l45 = faust_wrap_add(l45, 1)) {
			fRec47[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 3; l46 = faust_wrap_add(l46, 1)) {
			fRec48[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 3; l47 = faust_wrap_add(l47, 1)) {
			fRec49[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 3; l48 = faust_wrap_add(l48, 1)) {
			fRec50[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 3; l49 = faust_wrap_add(l49, 1)) {
			fRec51[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 3; l50 = faust_wrap_add(l50, 1)) {
			fRec52[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 3; l51 = faust_wrap_add(l51, 1)) {
			fRec53[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 3; l52 = faust_wrap_add(l52, 1)) {
			fRec54[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 3; l53 = faust_wrap_add(l53, 1)) {
			fRec55[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = faust_wrap_add(l54, 1)) {
			fVec0[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2048; l55 = faust_wrap_add(l55, 1)) {
			fVec1[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = faust_wrap_add(l56, 1)) {
			fRec2[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 2; l57 = faust_wrap_add(l57, 1)) {
			fRec3[l57] = 0.0f;
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
	
	virtual T_marimba* clone() {
		return new T_marimba(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("marimba");
		ui_interface->addHorizontalSlider("exPos", &fHslider1, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(4.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("freq", &fHslider0, FAUSTFLOAT(2.2e+02f), FAUSTFLOAT(8e+01f), FAUSTFLOAT(2e+03f), FAUSTFLOAT(1.0f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst2 * static_cast<float>(fHslider0);
		fSlow1 = fConst2 * static_cast<float>(fHslider1);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		fRec0[IOTA0 & 2047] = 0.05f * fRec2[1] + 0.95f * fRec0[(faust_wrap_sub(IOTA0, 1)) & 2047];
		fRec1[0] = 0.05f * fRec3[1] + 0.95f * fRec1[1];
		fRec4[0] = fSlow0 + fConst3 * fRec4[1];
		float fTemp0 = fConst1 * (-0.04f + 3.4e+02f / fRec4[0]);
		float fTemp1 = -1.499995f + fTemp0;
		float fTemp2 = std::floor(fTemp1);
		float fTemp3 = -4.0f - fTemp2 + fTemp0;
		float fTemp4 = -3.0f - fTemp2 + fTemp0;
		float fTemp5 = -2.0f - fTemp2 + fTemp0;
		float fTemp6 = static_cast<float>(inputs[0]);
		fRec5[0] = fTemp6 - fConst5 * fRec5[2] + fConst6 * fRec5[1] * std::cos(fConst7 * fRec4[0]);
		fRec6[0] = fSlow1 + fConst3 * fRec6[1];
		int iTemp7 = faust_wrap_mul(50, static_cast<int>(fRec6[0]));
		fRec7[0] = fTemp6 - fConst10 * fRec7[2] + fConst11 * fRec7[1] * std::cos(fConst12 * fRec4[0]);
		fRec8[0] = fTemp6 - fConst14 * fRec8[2] + fConst15 * fRec8[1] * std::cos(fConst16 * fRec4[0]);
		fRec9[0] = fTemp6 - fConst18 * fRec9[2] + fConst19 * fRec9[1] * std::cos(fConst20 * fRec4[0]);
		fRec10[0] = fTemp6 - fConst22 * fRec10[2] + fConst23 * fRec10[1] * std::cos(fConst24 * fRec4[0]);
		fRec11[0] = fTemp6 - fConst26 * fRec11[2] + fConst27 * fRec11[1] * std::cos(fConst28 * fRec4[0]);
		fRec12[0] = fTemp6 - fConst30 * fRec12[2] + fConst31 * fRec12[1] * std::cos(fConst32 * fRec4[0]);
		fRec13[0] = fTemp6 - fConst34 * fRec13[2] + fConst35 * fRec13[1] * std::cos(fConst36 * fRec4[0]);
		fRec14[0] = fTemp6 - fConst38 * fRec14[2] + fConst39 * fRec14[1] * std::cos(fConst40 * fRec4[0]);
		fRec15[0] = fTemp6 - fConst42 * fRec15[2] + fConst43 * fRec15[1] * std::cos(fConst44 * fRec4[0]);
		fRec16[0] = fTemp6 - fConst46 * fRec16[2] + fConst47 * fRec16[1] * std::cos(fConst48 * fRec4[0]);
		fRec17[0] = fTemp6 - fConst50 * fRec17[2] + fConst51 * fRec17[1] * std::cos(fConst52 * fRec4[0]);
		fRec18[0] = fTemp6 - fConst54 * fRec18[2] + fConst55 * fRec18[1] * std::cos(fConst56 * fRec4[0]);
		fRec19[0] = fTemp6 - fConst58 * fRec19[2] + fConst59 * fRec19[1] * std::cos(fConst60 * fRec4[0]);
		fRec20[0] = fTemp6 - fConst62 * fRec20[2] + fConst63 * fRec20[1] * std::cos(fConst64 * fRec4[0]);
		fRec21[0] = fTemp6 - fConst66 * fRec21[2] + fConst67 * fRec21[1] * std::cos(fConst68 * fRec4[0]);
		fRec22[0] = fTemp6 - fConst70 * fRec22[2] + fConst71 * fRec22[1] * std::cos(fConst72 * fRec4[0]);
		fRec23[0] = fTemp6 - fConst74 * fRec23[2] + fConst75 * fRec23[1] * std::cos(fConst76 * fRec4[0]);
		fRec24[0] = fTemp6 - fConst78 * fRec24[2] + fConst79 * fRec24[1] * std::cos(fConst80 * fRec4[0]);
		fRec25[0] = fTemp6 - fConst82 * fRec25[2] + fConst83 * fRec25[1] * std::cos(fConst84 * fRec4[0]);
		fRec26[0] = fTemp6 - fConst86 * fRec26[2] + fConst87 * fRec26[1] * std::cos(fConst88 * fRec4[0]);
		fRec27[0] = fTemp6 - fConst90 * fRec27[2] + fConst91 * fRec27[1] * std::cos(fConst92 * fRec4[0]);
		fRec28[0] = fTemp6 - fConst94 * fRec28[2] + fConst95 * fRec28[1] * std::cos(fConst96 * fRec4[0]);
		fRec29[0] = fTemp6 - fConst98 * fRec29[2] + fConst99 * fRec29[1] * std::cos(fConst100 * fRec4[0]);
		fRec30[0] = fTemp6 - fConst102 * fRec30[2] + fConst103 * fRec30[1] * std::cos(fConst104 * fRec4[0]);
		fRec31[0] = fTemp6 - fConst106 * fRec31[2] + fConst107 * fRec31[1] * std::cos(fConst108 * fRec4[0]);
		fRec32[0] = fTemp6 - fConst110 * fRec32[2] + fConst111 * fRec32[1] * std::cos(fConst112 * fRec4[0]);
		fRec33[0] = fTemp6 - fConst114 * fRec33[2] + fConst115 * fRec33[1] * std::cos(fConst116 * fRec4[0]);
		fRec34[0] = fTemp6 - fConst118 * fRec34[2] + fConst119 * fRec34[1] * std::cos(fConst120 * fRec4[0]);
		fRec35[0] = fTemp6 - fConst122 * fRec35[2] + fConst123 * fRec35[1] * std::cos(fConst124 * fRec4[0]);
		fRec36[0] = fTemp6 - fConst126 * fRec36[2] + fConst127 * fRec36[1] * std::cos(fConst128 * fRec4[0]);
		fRec37[0] = fTemp6 - fConst130 * fRec37[2] + fConst131 * fRec37[1] * std::cos(fConst132 * fRec4[0]);
		fRec38[0] = fTemp6 - fConst134 * fRec38[2] + fConst135 * fRec38[1] * std::cos(fConst136 * fRec4[0]);
		fRec39[0] = fTemp6 - fConst138 * fRec39[2] + fConst139 * fRec39[1] * std::cos(fConst140 * fRec4[0]);
		fRec40[0] = fTemp6 - fConst142 * fRec40[2] + fConst143 * fRec40[1] * std::cos(fConst144 * fRec4[0]);
		fRec41[0] = fTemp6 - fConst146 * fRec41[2] + fConst147 * fRec41[1] * std::cos(fConst148 * fRec4[0]);
		fRec42[0] = fTemp6 - fConst150 * fRec42[2] + fConst151 * fRec42[1] * std::cos(fConst152 * fRec4[0]);
		fRec43[0] = fTemp6 - fConst154 * fRec43[2] + fConst155 * fRec43[1] * std::cos(fConst156 * fRec4[0]);
		fRec44[0] = fTemp6 - fConst158 * fRec44[2] + fConst159 * fRec44[1] * std::cos(fConst160 * fRec4[0]);
		fRec45[0] = fTemp6 - fConst162 * fRec45[2] + fConst163 * fRec45[1] * std::cos(fConst164 * fRec4[0]);
		fRec46[0] = fTemp6 - fConst166 * fRec46[2] + fConst167 * fRec46[1] * std::cos(fConst168 * fRec4[0]);
		fRec47[0] = fTemp6 - fConst170 * fRec47[2] + fConst171 * fRec47[1] * std::cos(fConst172 * fRec4[0]);
		fRec48[0] = fTemp6 - fConst174 * fRec48[2] + fConst175 * fRec48[1] * std::cos(fConst176 * fRec4[0]);
		fRec49[0] = fTemp6 - fConst178 * fRec49[2] + fConst179 * fRec49[1] * std::cos(fConst180 * fRec4[0]);
		fRec50[0] = fTemp6 - fConst182 * fRec50[2] + fConst183 * fRec50[1] * std::cos(fConst184 * fRec4[0]);
		fRec51[0] = fTemp6 - fConst186 * fRec51[2] + fConst187 * fRec51[1] * std::cos(fConst188 * fRec4[0]);
		fRec52[0] = fTemp6 - fConst190 * fRec52[2] + fConst191 * fRec52[1] * std::cos(fConst192 * fRec4[0]);
		fRec53[0] = fTemp6 - fConst194 * fRec53[2] + fConst195 * fRec53[1] * std::cos(fConst196 * fRec4[0]);
		fRec54[0] = fTemp6 - fConst198 * fRec54[2] + fConst199 * fRec54[1] * std::cos(fConst200 * fRec4[0]);
		fRec55[0] = fTemp6 - fConst202 * fRec55[2] + fConst203 * fRec55[1] * std::cos(fConst204 * fRec4[0]);
		fVec0[0] = (fRec5[0] - fRec5[2]) * ((fRec4[0] < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(iTemp7, 249))] : 0.0f) + (fRec7[0] - fRec7[2]) * (((3.31356f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(1, iTemp7), 249))] : 0.0f) + (fRec8[0] - fRec8[2]) * (((3.83469f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(2, iTemp7), 249))] : 0.0f) + (fRec9[0] - fRec9[2]) * (((8.06313f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(3, iTemp7), 249))] : 0.0f) + (fRec10[0] - fRec10[2]) * (((9.44778f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(4, iTemp7), 249))] : 0.0f) + (fRec11[0] - fRec11[2]) * (((14.1169f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(5, iTemp7), 249))] : 0.0f) + (fRec12[0] - fRec12[2]) * (((18.384f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(6, iTemp7), 249))] : 0.0f) + (fRec13[0] - fRec13[2]) * (((21.0102f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(7, iTemp7), 249))] : 0.0f) + (fRec14[0] - fRec14[2]) * (((26.1775f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(8, iTemp7), 249))] : 0.0f) + (fRec15[0] - fRec15[2]) * (((28.9944f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(9, iTemp7), 249))] : 0.0f) + (fRec16[0] - fRec16[2]) * (((37.0728f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(10, iTemp7), 249))] : 0.0f) + (fRec17[0] - fRec17[2]) * (((37.8703f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(11, iTemp7), 249))] : 0.0f) + (fRec18[0] - fRec18[2]) * (((40.0634f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(12, iTemp7), 249))] : 0.0f) + (fRec19[0] - fRec19[2]) * (((47.6439f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(13, iTemp7), 249))] : 0.0f) + (fRec20[0] - fRec20[2]) * (((51.019f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(14, iTemp7), 249))] : 0.0f) + (fRec21[0] - fRec21[2]) * (((52.43f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(15, iTemp7), 249))] : 0.0f) + (fRec22[0] - fRec22[2]) * (((58.286f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(16, iTemp7), 249))] : 0.0f) + (fRec23[0] - fRec23[2]) * (((63.5486f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(17, iTemp7), 249))] : 0.0f) + (fRec24[0] - fRec24[2]) * (((65.3628f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(18, iTemp7), 249))] : 0.0f) + (fRec25[0] - fRec25[2]) * (((66.9587f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(19, iTemp7), 249))] : 0.0f) + (fRec26[0] - fRec26[2]) * (((74.5301f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(20, iTemp7), 249))] : 0.0f) + (fRec27[0] - fRec27[2]) * (((78.692f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(21, iTemp7), 249))] : 0.0f) + (fRec28[0] - fRec28[2]) * (((80.8375f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(22, iTemp7), 249))] : 0.0f) + (fRec29[0] - fRec29[2]) * (((89.978f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(23, iTemp7), 249))] : 0.0f) + (fRec30[0] - fRec30[2]) * (((92.9661f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(24, iTemp7), 249))] : 0.0f) + (fRec31[0] - fRec31[2]) * (((95.1914f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(25, iTemp7), 249))] : 0.0f) + (fRec32[0] - fRec32[2]) * (((97.4807f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(26, iTemp7), 249))] : 0.0f) + (fRec33[0] - fRec33[2]) * (((110.62f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(27, iTemp7), 249))] : 0.0f) + (fRec34[0] - fRec34[2]) * (((112.069f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(28, iTemp7), 249))] : 0.0f) + (fRec35[0] - fRec35[2]) * (((113.826f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(29, iTemp7), 249))] : 0.0f) + (fRec36[0] - fRec36[2]) * (((119.356f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(30, iTemp7), 249))] : 0.0f) + (fRec37[0] - fRec37[2]) * (((127.045f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(31, iTemp7), 249))] : 0.0f) + (fRec38[0] - fRec38[2]) * (((129.982f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(32, iTemp7), 249))] : 0.0f) + (fRec39[0] - fRec39[2]) * (((132.259f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(33, iTemp7), 249))] : 0.0f) + (fRec40[0] - fRec40[2]) * (((133.477f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(34, iTemp7), 249))] : 0.0f) + (fRec41[0] - fRec41[2]) * (((144.549f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(35, iTemp7), 249))] : 0.0f) + (fRec42[0] - fRec42[2]) * (((149.438f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(36, iTemp7), 249))] : 0.0f) + (fRec43[0] - fRec43[2]) * (((152.033f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(37, iTemp7), 249))] : 0.0f) + (fRec44[0] - fRec44[2]) * (((153.166f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(38, iTemp7), 249))] : 0.0f) + (fRec45[0] - fRec45[2]) * (((155.597f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(39, iTemp7), 249))] : 0.0f) + (fRec46[0] - fRec46[2]) * (((158.183f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(40, iTemp7), 249))] : 0.0f) + (fRec47[0] - fRec47[2]) * (((168.105f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(41, iTemp7), 249))] : 0.0f) + (fRec48[0] - fRec48[2]) * (((171.863f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(42, iTemp7), 249))] : 0.0f) + (fRec49[0] - fRec49[2]) * (((174.464f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(43, iTemp7), 249))] : 0.0f) + (fRec50[0] - fRec50[2]) * (((178.937f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(44, iTemp7), 249))] : 0.0f) + (fRec51[0] - fRec51[2]) * (((181.482f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(45, iTemp7), 249))] : 0.0f) + (fRec52[0] - fRec52[2]) * (((185.398f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(46, iTemp7), 249))] : 0.0f) + (fRec53[0] - fRec53[2]) * (((190.369f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(47, iTemp7), 249))] : 0.0f) + (fRec54[0] - fRec54[2]) * (((192.19f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(48, iTemp7), 249))] : 0.0f) + (fRec55[0] - fRec55[2]) * (((195.505f * fRec4[0]) < fConst8) ? ftbl0T_marimbaSIG0[std::max<int>(0, std::min<int>(faust_wrap_add(49, iTemp7), 249))] : 0.0f);
		float fTemp8 = 0.99f * fRec1[0] + 0.02f * fVec0[1];
		fVec1[IOTA0 & 2047] = fTemp8;
		int iTemp9 = static_cast<int>(fTemp1);
		int iTemp10 = static_cast<int>(std::min<float>(fConst205, static_cast<float>(std::max<int>(0, iTemp9))));
		float fTemp11 = -1.0f - fTemp2 + fTemp0;
		float fTemp12 = fTemp0 - fTemp2;
		int iTemp13 = static_cast<int>(std::min<float>(fConst205, static_cast<float>(std::max<int>(0, faust_wrap_add(1, iTemp9)))));
		float fTemp14 = fTemp12 * fTemp11;
		int iTemp15 = static_cast<int>(std::min<float>(fConst205, static_cast<float>(std::max<int>(0, faust_wrap_add(2, iTemp9)))));
		float fTemp16 = fTemp14 * fTemp5;
		int iTemp17 = static_cast<int>(std::min<float>(fConst205, static_cast<float>(std::max<int>(0, faust_wrap_add(3, iTemp9)))));
		float fTemp18 = fTemp16 * fTemp4;
		int iTemp19 = static_cast<int>(std::min<float>(fConst205, static_cast<float>(std::max<int>(0, faust_wrap_add(4, iTemp9)))));
		fRec2[0] = fTemp3 * (fTemp4 * (fTemp5 * (0.041666668f * fVec1[(faust_wrap_sub(IOTA0, iTemp10)) & 2047] * fTemp11 - 0.16666667f * fTemp12 * fVec1[(faust_wrap_sub(IOTA0, iTemp13)) & 2047]) + 0.25f * fTemp14 * fVec1[(faust_wrap_sub(IOTA0, iTemp15)) & 2047]) - 0.16666667f * fTemp16 * fVec1[(faust_wrap_sub(IOTA0, iTemp17)) & 2047]) + 0.041666668f * fTemp18 * fVec1[(faust_wrap_sub(IOTA0, iTemp19)) & 2047];
		fRec3[0] = fTemp3 * (fTemp4 * (fTemp5 * (0.04125f * fTemp11 * fRec0[(faust_wrap_sub(IOTA0, iTemp10)) & 2047] - 0.165f * fTemp12 * fRec0[(faust_wrap_sub(IOTA0, iTemp13)) & 2047]) + 0.2475f * fTemp14 * fRec0[(faust_wrap_sub(IOTA0, iTemp15)) & 2047]) - 0.165f * fTemp16 * fRec0[(faust_wrap_sub(IOTA0, iTemp17)) & 2047]) + 0.04125f * fTemp18 * fRec0[(faust_wrap_sub(IOTA0, iTemp19)) & 2047];
		outputs[0] = static_cast<FAUSTFLOAT>(fRec2[0]);
		IOTA0 = faust_wrap_add(IOTA0, 1);
		fRec1[1] = fRec1[0];
		fRec4[1] = fRec4[0];
		fRec5[2] = fRec5[1];
		fRec5[1] = fRec5[0];
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
		fRec55[2] = fRec55[1];
		fRec55[1] = fRec55[0];
		fVec0[1] = fVec0[0];
		fRec2[1] = fRec2[0];
		fRec3[1] = fRec3[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
