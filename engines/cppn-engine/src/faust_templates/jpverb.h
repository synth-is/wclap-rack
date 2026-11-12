/* ------------------------------------------------------------
name: "jpverb"
Code generated with Faust 2.88.0 (https://faust.grame.fr)
Compilation options: -lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_jpverb -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0
------------------------------------------------------------ */

#ifndef  __T_jpverb_H__
#define  __T_jpverb_H__

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
#define FAUSTCLASS T_jpverb
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

const static int iT_jpverbSIG0Wave0[2048] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199,211,223,227,229,233,239,241,251,257,263,269,271,277,281,283,293,307,311,313,317,331,337,347,349,353,359,367,373,379,383,389,397,401,409,419,421,431,433,439,443,449,457,461,463,467,479,487,491,499,503,509,521,523,541,547,557,563,569,571,577,587,593,599,601,607,613,617,619,631,641,643,647,653,659,661,673,677,683,691,701,709,719,727,733,739,743,751,757,761,769,773,787,797,809,811,821,823,827,829,839,853,857,859,863,877,881,883,887,907,911,919,929,937,941,947,953,967,971,977,983,991,997,1009,1013,1019,1021,1031,1033,1039,1049,1051,1061,1063,1069,1087,1091,1093,1097,1103,1109,1117,1123,1129,1151,1153,1163,1171,1181,1187,1193,1201,1213,1217,1223,1229,1231,1237,1249,1259,1277,1279,1283,1289,1291,1297,1301,1303,1307,1319,1321,1327,1361,1367,1373,1381,1399,1409,1423,1427,1429,1433,1439,1447,1451,1453,1459,1471,1481,1483,1487,1489,1493,1499,1511,1523,1531,1543,1549,1553,1559,1567,1571,1579,1583,1597,1601,1607,1609,1613,1619,1621,1627,1637,1657,1663,1667,1669,1693,1697,1699,1709,1721,1723,1733,1741,1747,1753,1759,1777,1783,1787,1789,1801,1811,1823,1831,1847,1861,1867,1871,1873,1877,1879,1889,1901,1907,1913,1931,1933,1949,1951,1973,1979,1987,1993,1997,1999,2003,2011,2017,2027,2029,2039,2053,2063,2069,2081,2083,2087,2089,2099,2111,2113,2129,2131,2137,2141,2143,2153,2161,2179,2203,2207,2213,2221,2237,2239,2243,2251,2267,2269,2273,2281,2287,2293,2297,2309,2311,2333,2339,2341,2347,2351,2357,2371,2377,2381,2383,2389,2393,2399,2411,2417,2423,2437,2441,2447,2459,2467,2473,2477,2503,2521,2531,2539,2543,2549,2551,2557,2579,2591,2593,2609,2617,2621,2633,2647,2657,2659,2663,2671,2677,2683,2687,2689,2693,2699,2707,2711,2713,2719,2729,2731,2741,2749,2753,2767,2777,2789,2791,2797,2801,2803,2819,2833,2837,2843,2851,2857,2861,2879,2887,2897,2903,2909,2917,2927,2939,2953,2957,2963,2969,2971,2999,3001,3011,3019,3023,3037,3041,3049,3061,3067,3079,3083,3089,3109,3119,3121,3137,3163,3167,3169,3181,3187,3191,3203,3209,3217,3221,3229,3251,3253,3257,3259,3271,3299,3301,3307,3313,3319,3323,3329,3331,3343,3347,3359,3361,3371,3373,3389,3391,3407,3413,3433,3449,3457,3461,3463,3467,3469,3491,3499,3511,3517,3527,3529,3533,3539,3541,3547,3557,3559,3571,3581,3583,3593,3607,3613,3617,3623,3631,3637,3643,3659,3671,3673,3677,3691,3697,3701,3709,3719,3727,3733,3739,3761,3767,3769,3779,3793,3797,3803,3821,3823,3833,3847,3851,3853,3863,3877,3881,3889,3907,3911,3917,3919,3923,3929,3931,3943,3947,3967,3989,4001,4003,4007,4013,4019,4021,4027,4049,4051,4057,4073,4079,4091,4093,4099,4111,4127,4129,4133,4139,4153,4157,4159,4177,4201,4211,4217,4219,4229,4231,4241,4243,4253,4259,4261,4271,4273,4283,4289,4297,4327,4337,4339,4349,4357,4363,4373,4391,4397,4409,4421,4423,4441,4447,4451,4457,4463,4481,4483,4493,4507,4513,4517,4519,4523,4547,4549,4561,4567,4583,4591,4597,4603,4621,4637,4639,4643,4649,4651,4657,4663,4673,4679,4691,4703,4721,4723,4729,4733,4751,4759,4783,4787,4789,4793,4799,4801,4813,4817,4831,4861,4871,4877,4889,4903,4909,4919,4931,4933,4937,4943,4951,4957,4967,4969,4973,4987,4993,4999,5003,5009,5011,5021,5023,5039,5051,5059,5077,5081,5087,5099,5101,5107,5113,5119,5147,5153,5167,5171,5179,5189,5197,5209,5227,5231,5233,5237,5261,5273,5279,5281,5297,5303,5309,5323,5333,5347,5351,5381,5387,5393,5399,5407,5413,5417,5419,5431,5437,5441,5443,5449,5471,5477,5479,5483,5501,5503,5507,5519,5521,5527,5531,5557,5563,5569,5573,5581,5591,5623,5639,5641,5647,5651,5653,5657,5659,5669,5683,5689,5693,5701,5711,5717,5737,5741,5743,5749,5779,5783,5791,5801,5807,5813,5821,5827,5839,5843,5849,5851,5857,5861,5867,5869,5879,5881,5897,5903,5923,5927,5939,5953,5981,5987,6007,6011,6029,6037,6043,6047,6053,6067,6073,6079,6089,6091,6101,6113,6121,6131,6133,6143,6151,6163,6173,6197,6199,6203,6211,6217,6221,6229,6247,6257,6263,6269,6271,6277,6287,6299,6301,6311,6317,6323,6329,6337,6343,6353,6359,6361,6367,6373,6379,6389,6397,6421,6427,6449,6451,6469,6473,6481,6491,6521,6529,6547,6551,6553,6563,6569,6571,6577,6581,6599,6607,6619,6637,6653,6659,6661,6673,6679,6689,6691,6701,6703,6709,6719,6733,6737,6761,6763,6779,6781,6791,6793,6803,6823,6827,6829,6833,6841,6857,6863,6869,6871,6883,6899,6907,6911,6917,6947,6949,6959,6961,6967,6971,6977,6983,6991,6997,7001,7013,7019,7027,7039,7043,7057,7069,7079,7103,7109,7121,7127,7129,7151,7159,7177,7187,7193,7207,7211,7213,7219,7229,7237,7243,7247,7253,7283,7297,7307,7309,7321,7331,7333,7349,7351,7369,7393,7411,7417,7433,7451,7457,7459,7477,7481,7487,7489,7499,7507,7517,7523,7529,7537,7541,7547,7549,7559,7561,7573,7577,7583,7589,7591,7603,7607,7621,7639,7643,7649,7669,7673,7681,7687,7691,7699,7703,7717,7723,7727,7741,7753,7757,7759,7789,7793,7817,7823,7829,7841,7853,7867,7873,7877,7879,7883,7901,7907,7919,7927,7933,7937,7949,7951,7963,7993,8009,8011,8017,8039,8053,8059,8069,8081,8087,8089,8093,8101,8111,8117,8123,8147,8161,8167,8171,8179,8191,8209,8219,8221,8231,8233,8237,8243,8263,8269,8273,8287,8291,8293,8297,8311,8317,8329,8353,8363,8369,8377,8387,8389,8419,8423,8429,8431,8443,8447,8461,8467,8501,8513,8521,8527,8537,8539,8543,8563,8573,8581,8597,8599,8609,8623,8627,8629,8641,8647,8663,8669,8677,8681,8689,8693,8699,8707,8713,8719,8731,8737,8741,8747,8753,8761,8779,8783,8803,8807,8819,8821,8831,8837,8839,8849,8861,8863,8867,8887,8893,8923,8929,8933,8941,8951,8963,8969,8971,8999,9001,9007,9011,9013,9029,9041,9043,9049,9059,9067,9091,9103,9109,9127,9133,9137,9151,9157,9161,9173,9181,9187,9199,9203,9209,9221,9227,9239,9241,9257,9277,9281,9283,9293,9311,9319,9323,9337,9341,9343,9349,9371,9377,9391,9397,9403,9413,9419,9421,9431,9433,9437,9439,9461,9463,9467,9473,9479,9491,9497,9511,9521,9533,9539,9547,9551,9587,9601,9613,9619,9623,9629,9631,9643,9649,9661,9677,9679,9689,9697,9719,9721,9733,9739,9743,9749,9767,9769,9781,9787,9791,9803,9811,9817,9829,9833,9839,9851,9857,9859,9871,9883,9887,9901,9907,9923,9929,9931,9941,9949,9967,9973,10007,10009,10037,10039,10061,10067,10069,10079,10091,10093,10099,10103,10111,10133,10139,10141,10151,10159,10163,10169,10177,10181,10193,10211,10223,10243,10247,10253,10259,10267,10271,10273,10289,10301,10303,10313,10321,10331,10333,10337,10343,10357,10369,10391,10399,10427,10429,10433,10453,10457,10459,10463,10477,10487,10499,10501,10513,10529,10531,10559,10567,10589,10597,10601,10607,10613,10627,10631,10639,10651,10657,10663,10667,10687,10691,10709,10711,10723,10729,10733,10739,10753,10771,10781,10789,10799,10831,10837,10847,10853,10859,10861,10867,10883,10889,10891,10903,10909,10937,10939,10949,10957,10973,10979,10987,10993,11003,11027,11047,11057,11059,11069,11071,11083,11087,11093,11113,11117,11119,11131,11149,11159,11161,11171,11173,11177,11197,11213,11239,11243,11251,11257,11261,11273,11279,11287,11299,11311,11317,11321,11329,11351,11353,11369,11383,11393,11399,11411,11423,11437,11443,11447,11467,11471,11483,11489,11491,11497,11503,11519,11527,11549,11551,11579,11587,11593,11597,11617,11621,11633,11657,11677,11681,11689,11699,11701,11717,11719,11731,11743,11777,11779,11783,11789,11801,11807,11813,11821,11827,11831,11833,11839,11863,11867,11887,11897,11903,11909,11923,11927,11933,11939,11941,11953,11959,11969,11971,11981,11987,12007,12011,12037,12041,12043,12049,12071,12073,12097,12101,12107,12109,12113,12119,12143,12149,12157,12161,12163,12197,12203,12211,12227,12239,12241,12251,12253,12263,12269,12277,12281,12289,12301,12323,12329,12343,12347,12373,12377,12379,12391,12401,12409,12413,12421,12433,12437,12451,12457,12473,12479,12487,12491,12497,12503,12511,12517,12527,12539,12541,12547,12553,12569,12577,12583,12589,12601,12611,12613,12619,12637,12641,12647,12653,12659,12671,12689,12697,12703,12713,12721,12739,12743,12757,12763,12781,12791,12799,12809,12821,12823,12829,12841,12853,12889,12893,12899,12907,12911,12917,12919,12923,12941,12953,12959,12967,12973,12979,12983,13001,13003,13007,13009,13033,13037,13043,13049,13063,13093,13099,13103,13109,13121,13127,13147,13151,13159,13163,13171,13177,13183,13187,13217,13219,13229,13241,13249,13259,13267,13291,13297,13309,13313,13327,13331,13337,13339,13367,13381,13397,13399,13411,13417,13421,13441,13451,13457,13463,13469,13477,13487,13499,13513,13523,13537,13553,13567,13577,13591,13597,13613,13619,13627,13633,13649,13669,13679,13681,13687,13691,13693,13697,13709,13711,13721,13723,13729,13751,13757,13759,13763,13781,13789,13799,13807,13829,13831,13841,13859,13873,13877,13879,13883,13901,13903,13907,13913,13921,13931,13933,13963,13967,13997,13999,14009,14011,14029,14033,14051,14057,14071,14081,14083,14087,14107,14143,14149,14153,14159,14173,14177,14197,14207,14221,14243,14249,14251,14281,14293,14303,14321,14323,14327,14341,14347,14369,14387,14389,14401,14407,14411,14419,14423,14431,14437,14447,14449,14461,14479,14489,14503,14519,14533,14537,14543,14549,14551,14557,14561,14563,14591,14593,14621,14627,14629,14633,14639,14653,14657,14669,14683,14699,14713,14717,14723,14731,14737,14741,14747,14753,14759,14767,14771,14779,14783,14797,14813,14821,14827,14831,14843,14851,14867,14869,14879,14887,14891,14897,14923,14929,14939,14947,14951,14957,14969,14983,15013,15017,15031,15053,15061,15073,15077,15083,15091,15101,15107,15121,15131,15137,15139,15149,15161,15173,15187,15193,15199,15217,15227,15233,15241,15259,15263,15269,15271,15277,15287,15289,15299,15307,15313,15319,15329,15331,15349,15359,15361,15373,15377,15383,15391,15401,15413,15427,15439,15443,15451,15461,15467,15473,15493,15497,15511,15527,15541,15551,15559,15569,15581,15583,15601,15607,15619,15629,15641,15643,15647,15649,15661,15667,15671,15679,15683,15727,15731,15733,15737,15739,15749,15761,15767,15773,15787,15791,15797,15803,15809,15817,15823,15859,15877,15881,15887,15889,15901,15907,15913,15919,15923,15937,15959,15971,15973,15991,16001,16007,16033,16057,16061,16063,16067,16069,16073,16087,16091,16097,16103,16111,16127,16139,16141,16183,16187,16189,16193,16217,16223,16229,16231,16249,16253,16267,16273,16301,16319,16333,16339,16349,16361,16363,16369,16381,16411,16417,16421,16427,16433,16447,16451,16453,16477,16481,16487,16493,16519,16529,16547,16553,16561,16567,16573,16603,16607,16619,16631,16633,16649,16651,16657,16661,16673,16691,16693,16699,16703,16729,16741,16747,16759,16763,16787,16811,16823,16829,16831,16843,16871,16879,16883,16889,16901,16903,16921,16927,16931,16937,16943,16963,16979,16981,16987,16993,17011,17021,17027,17029,17033,17041,17047,17053,17077,17093,17099,17107,17117,17123,17137,17159,17167,17183,17189,17191,17203,17207,17209,17231,17239,17257,17291,17293,17299,17317,17321,17327,17333,17341,17351,17359,17377,17383,17387,17389,17393,17401,17417,17419,17431,17443,17449,17467,17471,17477,17483,17489,17491,17497,17509,17519,17539,17551,17569,17573,17579,17581,17597,17599,17609,17623,17627,17657,17659,17669,17681,17683,17707,17713,17729,17737,17747,17749,17761,17783,17789,17791,17807,17827,17837,17839,17851,17863};
class T_jpverbSIG0 {
	
  private:
	
	int iT_jpverbSIG0Wave0_idx;
	int fSampleRate;
	
  public:
	
	int getNumInputsT_jpverbSIG0() {
		return 0;
	}
	int getNumOutputsT_jpverbSIG0() {
		return 1;
	}
	
	void instanceInitT_jpverbSIG0(int sample_rate) {
		fSampleRate = sample_rate;
		iT_jpverbSIG0Wave0_idx = 0;
	}
	
	void fillT_jpverbSIG0(int count, int* table) {
		for (int i1 = 0; i1 < count; i1 = faust_wrap_add(i1, 1)) {
			table[i1] = iT_jpverbSIG0Wave0[iT_jpverbSIG0Wave0_idx];
			iT_jpverbSIG0Wave0_idx = (faust_wrap_add(1, iT_jpverbSIG0Wave0_idx)) % 2048;
		}
	}

};

static T_jpverbSIG0* newT_jpverbSIG0() { return (T_jpverbSIG0*)new T_jpverbSIG0(); }
static void deleteT_jpverbSIG0(T_jpverbSIG0* dsp) { delete dsp; }

static int itbl0T_jpverbSIG0[2048];
static float T_jpverb_faustpower2_f(float value) {
	return value * value;
}

class T_jpverb : public FaustDsp {
	
 private:
	
	int fSampleRate;
	float fConst0;
	float fConst1;
	FAUSTFLOAT fHslider0;
	float fSlow0;
	float fConst2;
	int iVec0[2];
	float fRec0[2];
	int IOTA0;
	float fVec1[16384];
	FAUSTFLOAT fHslider1;
	float fSlow1;
	float fRec64[2];
	float fRec63[2];
	float fVec2[2];
	float fRec1[2];
	float fVec3[16384];
	float fRec65[2];
	float fVec4[2];
	float fRec2[2];
	float fVec5[16384];
	float fRec66[2];
	float fVec6[2];
	float fRec3[2];
	float fVec7[16384];
	float fRec67[2];
	float fVec8[2];
	float fRec4[2];
	float fVec9[16384];
	float fRec68[2];
	float fVec10[2];
	float fRec5[2];
	float fVec11[16384];
	float fRec69[2];
	float fVec12[2];
	float fRec6[2];
	float fVec13[16384];
	float fRec70[2];
	float fVec14[2];
	float fRec7[2];
	float fVec15[16384];
	float fRec71[2];
	float fVec16[2];
	float fRec8[2];
	float fVec17[16384];
	float fRec72[2];
	float fVec18[2];
	float fRec9[2];
	FAUSTFLOAT fHslider2;
	float fSlow2;
	float fRec73[2];
	float fConst3;
	FAUSTFLOAT fHslider3;
	float fSlow3;
	float fRec76[2];
	float fRec74[2];
	float fRec75[2];
	float fVec19[1024];
	float fVec20[16384];
	float fRec77[2];
	float fVec21[2];
	float fRec10[2];
	float fVec22[16384];
	float fRec78[2];
	float fVec23[2];
	float fRec11[2];
	float fVec24[1024];
	float fVec25[16384];
	float fRec79[2];
	float fVec26[2];
	float fRec12[2];
	float fVec27[16384];
	float fRec80[2];
	float fVec28[2];
	float fRec13[2];
	float fVec29[16384];
	float fRec81[2];
	float fVec30[2];
	float fRec14[2];
	float fVec31[16384];
	float fRec82[2];
	float fVec32[2];
	float fRec15[2];
	float fVec33[16384];
	float fRec83[2];
	float fVec34[2];
	float fRec16[2];
	float fVec35[16384];
	float fRec84[2];
	float fVec36[2];
	float fRec17[2];
	float fVec37[16384];
	float fRec85[2];
	float fVec38[2];
	float fRec18[2];
	float fVec39[16384];
	float fRec86[2];
	float fVec40[2];
	float fRec19[2];
	float fVec41[16384];
	float fRec87[2];
	float fVec42[2];
	float fRec20[2];
	float fVec43[16384];
	float fRec88[2];
	float fVec44[2];
	float fRec21[2];
	float fVec45[16384];
	float fRec89[2];
	float fVec46[2];
	float fRec22[2];
	float fVec47[16384];
	float fVec48[16384];
	float fRec90[2];
	float fVec49[2];
	float fRec23[2];
	float fConst4;
	FAUSTFLOAT fHslider4;
	float fSlow4;
	float fRec91[2];
	float fRec24[2];
	float fVec50[16384];
	float fVec51[16384];
	float fRec92[2];
	float fVec52[2];
	float fRec25[2];
	float fRec26[2];
	float fRec27[2];
	float fRec28[2];
	float fRec29[3];
	float fRec30[3];
	float fRec31[3];
	float fRec32[3];
	float fRec33[3];
	float fRec34[3];
	float fRec35[3];
	float fRec36[3];
	FAUSTFLOAT fHslider5;
	float fSlow5;
	float fRec93[2];
	float fVec53[2];
	float fRec37[2];
	float fRec38[2];
	float fRec39[3];
	float fVec54[2];
	float fRec40[2];
	float fRec41[2];
	float fRec42[3];
	float fRec43[3];
	float fRec44[3];
	float fRec45[3];
	float fRec46[3];
	float fRec47[3];
	float fRec48[3];
	float fRec49[3];
	FAUSTFLOAT fHslider6;
	float fSlow6;
	float fRec94[2];
	FAUSTFLOAT fHslider7;
	float fSlow7;
	float fRec95[2];
	FAUSTFLOAT fHslider8;
	float fSlow8;
	float fRec96[2];
	FAUSTFLOAT fHslider9;
	float fSlow9;
	float fRec97[2];
	FAUSTFLOAT fHslider10;
	float fSlow10;
	float fRec98[2];
	float fVec55[1024];
	float fRec50[2];
	float fRec51[3];
	float fVec56[1024];
	float fRec52[2];
	FAUSTFLOAT fHslider11;
	float fSlow11;
	float fRec99[2];
	float fVec57[16384];
	float fVec58[2];
	float fRec53[2];
	float fVec59[16384];
	float fRec100[2];
	float fVec60[2];
	float fRec54[2];
	float fVec61[16384];
	float fVec62[2];
	float fRec55[2];
	float fVec63[16384];
	float fVec64[2];
	float fRec56[2];
	float fVec65[16384];
	float fRec101[2];
	float fVec66[2];
	float fRec57[2];
	float fVec67[16384];
	float fVec68[2];
	float fRec58[2];
	float fVec69[16384];
	float fVec70[2];
	float fRec59[2];
	float fVec71[16384];
	float fRec102[2];
	float fVec72[2];
	float fRec60[2];
	float fRec61[2];
	float fRec62[2];
	
 public:
	T_jpverb() {
	}
	
	T_jpverb(const T_jpverb&) = default;
	
	virtual ~T_jpverb() = default;
	
	T_jpverb& operator=(const T_jpverb&) = default;
	
	void metadata(Meta* m) { 
		m->declare("analyzers.lib/name", "Faust Analyzer Library");
		m->declare("analyzers.lib/version", "1.4.0");
		m->declare("basics.lib/name", "Faust Basic Element Library");
		m->declare("basics.lib/version", "1.23.0");
		m->declare("compile_options", "-lang cpp -fpga-mem-th 4 -os -ec -ct 1 -cn T_jpverb -scn FaustDsp -es 1 -mcd 16 -mdd 1024 -mdy 33 -single -ftz 0");
		m->declare("delays.lib/fdelay1a:author", "Julius O. Smith III");
		m->declare("delays.lib/fdelay4:author", "Julius O. Smith III");
		m->declare("delays.lib/fdelayltv:author", "Julius O. Smith III");
		m->declare("delays.lib/name", "Faust Delay Library");
		m->declare("delays.lib/version", "1.2.0");
		m->declare("filename", "jpverb.dsp");
		m->declare("filters.lib/filterbank:author", "Julius O. Smith III");
		m->declare("filters.lib/filterbank:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/filterbank:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/fir:author", "Julius O. Smith III");
		m->declare("filters.lib/fir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/fir:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/highpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highpass:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/highpass_plus_lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/highpass_plus_lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/highpass_plus_lowpass:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/iir:author", "Julius O. Smith III");
		m->declare("filters.lib/iir:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/iir:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass0_highpass1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass0_highpass1:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
		m->declare("filters.lib/lowpass:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/lowpass:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/name", "Faust Filters Library");
		m->declare("filters.lib/nlf2:author", "Julius O. Smith III");
		m->declare("filters.lib/nlf2:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/nlf2:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/tf1:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1:license", "LicenseRef-STK-4.3");
		m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
		m->declare("filters.lib/tf1s:copyright", "Copyright (C) 2003-2019 by Julius O. Smith III <jos@ccrma.stanford.edu>");
		m->declare("filters.lib/tf1s:license", "LicenseRef-STK-4.3");
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
		m->declare("name", "jpverb");
		m->declare("oscillators.lib/name", "Faust Oscillator Library");
		m->declare("oscillators.lib/version", "1.8.0");
		m->declare("platform.lib/name", "Generic Platform Library");
		m->declare("platform.lib/version", "1.3.0");
		m->declare("reverbs.lib/jpverb:author", "Julian Parker, bug fixes and minor interface changes by Till Bovermann");
		m->declare("reverbs.lib/jpverb:license", "MIT");
		m->declare("reverbs.lib/name", "Faust Reverb Library");
		m->declare("reverbs.lib/version", "1.5.1");
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
		T_jpverbSIG0* sig0 = newT_jpverbSIG0();
		sig0->instanceInitT_jpverbSIG0(sample_rate);
		sig0->fillT_jpverbSIG0(2048, itbl0T_jpverbSIG0);
		deleteT_jpverbSIG0(sig0);
	}
	
	virtual void instanceConstants(int sample_rate) {
		fSampleRate = sample_rate;
		fConst0 = std::min<float>(1.92e+05f, std::max<float>(1.0f, static_cast<float>(fSampleRate)));
		fConst1 = 44.1f / fConst0;
		fConst2 = 1.0f - fConst1;
		fConst3 = 6.2831855f / fConst0;
		fConst4 = 3.1415927f / fConst0;
	}
	
	virtual void instanceResetUserInterface() {
		fHslider0 = static_cast<FAUSTFLOAT>(0.4f);
		fHslider1 = static_cast<FAUSTFLOAT>(1.5f);
		fHslider2 = static_cast<FAUSTFLOAT>(0.2f);
		fHslider3 = static_cast<FAUSTFLOAT>(2.0f);
		fHslider4 = static_cast<FAUSTFLOAT>(3e+03f);
		fHslider5 = static_cast<FAUSTFLOAT>(5e+02f);
		fHslider6 = static_cast<FAUSTFLOAT>(0.5f);
		fHslider7 = static_cast<FAUSTFLOAT>(0.7f);
		fHslider8 = static_cast<FAUSTFLOAT>(1.0f);
		fHslider9 = static_cast<FAUSTFLOAT>(1.0f);
		fHslider10 = static_cast<FAUSTFLOAT>(2.0f);
		fHslider11 = static_cast<FAUSTFLOAT>(0.7f);
	}
	
	virtual void instanceClear() {
		for (int l0 = 0; l0 < 2; l0 = faust_wrap_add(l0, 1)) {
			iVec0[l0] = 0;
		}
		for (int l1 = 0; l1 < 2; l1 = faust_wrap_add(l1, 1)) {
			fRec0[l1] = 0.0f;
		}
		IOTA0 = 0;
		for (int l2 = 0; l2 < 16384; l2 = faust_wrap_add(l2, 1)) {
			fVec1[l2] = 0.0f;
		}
		for (int l3 = 0; l3 < 2; l3 = faust_wrap_add(l3, 1)) {
			fRec64[l3] = 0.0f;
		}
		for (int l4 = 0; l4 < 2; l4 = faust_wrap_add(l4, 1)) {
			fRec63[l4] = 0.0f;
		}
		for (int l5 = 0; l5 < 2; l5 = faust_wrap_add(l5, 1)) {
			fVec2[l5] = 0.0f;
		}
		for (int l6 = 0; l6 < 2; l6 = faust_wrap_add(l6, 1)) {
			fRec1[l6] = 0.0f;
		}
		for (int l7 = 0; l7 < 16384; l7 = faust_wrap_add(l7, 1)) {
			fVec3[l7] = 0.0f;
		}
		for (int l8 = 0; l8 < 2; l8 = faust_wrap_add(l8, 1)) {
			fRec65[l8] = 0.0f;
		}
		for (int l9 = 0; l9 < 2; l9 = faust_wrap_add(l9, 1)) {
			fVec4[l9] = 0.0f;
		}
		for (int l10 = 0; l10 < 2; l10 = faust_wrap_add(l10, 1)) {
			fRec2[l10] = 0.0f;
		}
		for (int l11 = 0; l11 < 16384; l11 = faust_wrap_add(l11, 1)) {
			fVec5[l11] = 0.0f;
		}
		for (int l12 = 0; l12 < 2; l12 = faust_wrap_add(l12, 1)) {
			fRec66[l12] = 0.0f;
		}
		for (int l13 = 0; l13 < 2; l13 = faust_wrap_add(l13, 1)) {
			fVec6[l13] = 0.0f;
		}
		for (int l14 = 0; l14 < 2; l14 = faust_wrap_add(l14, 1)) {
			fRec3[l14] = 0.0f;
		}
		for (int l15 = 0; l15 < 16384; l15 = faust_wrap_add(l15, 1)) {
			fVec7[l15] = 0.0f;
		}
		for (int l16 = 0; l16 < 2; l16 = faust_wrap_add(l16, 1)) {
			fRec67[l16] = 0.0f;
		}
		for (int l17 = 0; l17 < 2; l17 = faust_wrap_add(l17, 1)) {
			fVec8[l17] = 0.0f;
		}
		for (int l18 = 0; l18 < 2; l18 = faust_wrap_add(l18, 1)) {
			fRec4[l18] = 0.0f;
		}
		for (int l19 = 0; l19 < 16384; l19 = faust_wrap_add(l19, 1)) {
			fVec9[l19] = 0.0f;
		}
		for (int l20 = 0; l20 < 2; l20 = faust_wrap_add(l20, 1)) {
			fRec68[l20] = 0.0f;
		}
		for (int l21 = 0; l21 < 2; l21 = faust_wrap_add(l21, 1)) {
			fVec10[l21] = 0.0f;
		}
		for (int l22 = 0; l22 < 2; l22 = faust_wrap_add(l22, 1)) {
			fRec5[l22] = 0.0f;
		}
		for (int l23 = 0; l23 < 16384; l23 = faust_wrap_add(l23, 1)) {
			fVec11[l23] = 0.0f;
		}
		for (int l24 = 0; l24 < 2; l24 = faust_wrap_add(l24, 1)) {
			fRec69[l24] = 0.0f;
		}
		for (int l25 = 0; l25 < 2; l25 = faust_wrap_add(l25, 1)) {
			fVec12[l25] = 0.0f;
		}
		for (int l26 = 0; l26 < 2; l26 = faust_wrap_add(l26, 1)) {
			fRec6[l26] = 0.0f;
		}
		for (int l27 = 0; l27 < 16384; l27 = faust_wrap_add(l27, 1)) {
			fVec13[l27] = 0.0f;
		}
		for (int l28 = 0; l28 < 2; l28 = faust_wrap_add(l28, 1)) {
			fRec70[l28] = 0.0f;
		}
		for (int l29 = 0; l29 < 2; l29 = faust_wrap_add(l29, 1)) {
			fVec14[l29] = 0.0f;
		}
		for (int l30 = 0; l30 < 2; l30 = faust_wrap_add(l30, 1)) {
			fRec7[l30] = 0.0f;
		}
		for (int l31 = 0; l31 < 16384; l31 = faust_wrap_add(l31, 1)) {
			fVec15[l31] = 0.0f;
		}
		for (int l32 = 0; l32 < 2; l32 = faust_wrap_add(l32, 1)) {
			fRec71[l32] = 0.0f;
		}
		for (int l33 = 0; l33 < 2; l33 = faust_wrap_add(l33, 1)) {
			fVec16[l33] = 0.0f;
		}
		for (int l34 = 0; l34 < 2; l34 = faust_wrap_add(l34, 1)) {
			fRec8[l34] = 0.0f;
		}
		for (int l35 = 0; l35 < 16384; l35 = faust_wrap_add(l35, 1)) {
			fVec17[l35] = 0.0f;
		}
		for (int l36 = 0; l36 < 2; l36 = faust_wrap_add(l36, 1)) {
			fRec72[l36] = 0.0f;
		}
		for (int l37 = 0; l37 < 2; l37 = faust_wrap_add(l37, 1)) {
			fVec18[l37] = 0.0f;
		}
		for (int l38 = 0; l38 < 2; l38 = faust_wrap_add(l38, 1)) {
			fRec9[l38] = 0.0f;
		}
		for (int l39 = 0; l39 < 2; l39 = faust_wrap_add(l39, 1)) {
			fRec73[l39] = 0.0f;
		}
		for (int l40 = 0; l40 < 2; l40 = faust_wrap_add(l40, 1)) {
			fRec76[l40] = 0.0f;
		}
		for (int l41 = 0; l41 < 2; l41 = faust_wrap_add(l41, 1)) {
			fRec74[l41] = 0.0f;
		}
		for (int l42 = 0; l42 < 2; l42 = faust_wrap_add(l42, 1)) {
			fRec75[l42] = 0.0f;
		}
		for (int l43 = 0; l43 < 1024; l43 = faust_wrap_add(l43, 1)) {
			fVec19[l43] = 0.0f;
		}
		for (int l44 = 0; l44 < 16384; l44 = faust_wrap_add(l44, 1)) {
			fVec20[l44] = 0.0f;
		}
		for (int l45 = 0; l45 < 2; l45 = faust_wrap_add(l45, 1)) {
			fRec77[l45] = 0.0f;
		}
		for (int l46 = 0; l46 < 2; l46 = faust_wrap_add(l46, 1)) {
			fVec21[l46] = 0.0f;
		}
		for (int l47 = 0; l47 < 2; l47 = faust_wrap_add(l47, 1)) {
			fRec10[l47] = 0.0f;
		}
		for (int l48 = 0; l48 < 16384; l48 = faust_wrap_add(l48, 1)) {
			fVec22[l48] = 0.0f;
		}
		for (int l49 = 0; l49 < 2; l49 = faust_wrap_add(l49, 1)) {
			fRec78[l49] = 0.0f;
		}
		for (int l50 = 0; l50 < 2; l50 = faust_wrap_add(l50, 1)) {
			fVec23[l50] = 0.0f;
		}
		for (int l51 = 0; l51 < 2; l51 = faust_wrap_add(l51, 1)) {
			fRec11[l51] = 0.0f;
		}
		for (int l52 = 0; l52 < 1024; l52 = faust_wrap_add(l52, 1)) {
			fVec24[l52] = 0.0f;
		}
		for (int l53 = 0; l53 < 16384; l53 = faust_wrap_add(l53, 1)) {
			fVec25[l53] = 0.0f;
		}
		for (int l54 = 0; l54 < 2; l54 = faust_wrap_add(l54, 1)) {
			fRec79[l54] = 0.0f;
		}
		for (int l55 = 0; l55 < 2; l55 = faust_wrap_add(l55, 1)) {
			fVec26[l55] = 0.0f;
		}
		for (int l56 = 0; l56 < 2; l56 = faust_wrap_add(l56, 1)) {
			fRec12[l56] = 0.0f;
		}
		for (int l57 = 0; l57 < 16384; l57 = faust_wrap_add(l57, 1)) {
			fVec27[l57] = 0.0f;
		}
		for (int l58 = 0; l58 < 2; l58 = faust_wrap_add(l58, 1)) {
			fRec80[l58] = 0.0f;
		}
		for (int l59 = 0; l59 < 2; l59 = faust_wrap_add(l59, 1)) {
			fVec28[l59] = 0.0f;
		}
		for (int l60 = 0; l60 < 2; l60 = faust_wrap_add(l60, 1)) {
			fRec13[l60] = 0.0f;
		}
		for (int l61 = 0; l61 < 16384; l61 = faust_wrap_add(l61, 1)) {
			fVec29[l61] = 0.0f;
		}
		for (int l62 = 0; l62 < 2; l62 = faust_wrap_add(l62, 1)) {
			fRec81[l62] = 0.0f;
		}
		for (int l63 = 0; l63 < 2; l63 = faust_wrap_add(l63, 1)) {
			fVec30[l63] = 0.0f;
		}
		for (int l64 = 0; l64 < 2; l64 = faust_wrap_add(l64, 1)) {
			fRec14[l64] = 0.0f;
		}
		for (int l65 = 0; l65 < 16384; l65 = faust_wrap_add(l65, 1)) {
			fVec31[l65] = 0.0f;
		}
		for (int l66 = 0; l66 < 2; l66 = faust_wrap_add(l66, 1)) {
			fRec82[l66] = 0.0f;
		}
		for (int l67 = 0; l67 < 2; l67 = faust_wrap_add(l67, 1)) {
			fVec32[l67] = 0.0f;
		}
		for (int l68 = 0; l68 < 2; l68 = faust_wrap_add(l68, 1)) {
			fRec15[l68] = 0.0f;
		}
		for (int l69 = 0; l69 < 16384; l69 = faust_wrap_add(l69, 1)) {
			fVec33[l69] = 0.0f;
		}
		for (int l70 = 0; l70 < 2; l70 = faust_wrap_add(l70, 1)) {
			fRec83[l70] = 0.0f;
		}
		for (int l71 = 0; l71 < 2; l71 = faust_wrap_add(l71, 1)) {
			fVec34[l71] = 0.0f;
		}
		for (int l72 = 0; l72 < 2; l72 = faust_wrap_add(l72, 1)) {
			fRec16[l72] = 0.0f;
		}
		for (int l73 = 0; l73 < 16384; l73 = faust_wrap_add(l73, 1)) {
			fVec35[l73] = 0.0f;
		}
		for (int l74 = 0; l74 < 2; l74 = faust_wrap_add(l74, 1)) {
			fRec84[l74] = 0.0f;
		}
		for (int l75 = 0; l75 < 2; l75 = faust_wrap_add(l75, 1)) {
			fVec36[l75] = 0.0f;
		}
		for (int l76 = 0; l76 < 2; l76 = faust_wrap_add(l76, 1)) {
			fRec17[l76] = 0.0f;
		}
		for (int l77 = 0; l77 < 16384; l77 = faust_wrap_add(l77, 1)) {
			fVec37[l77] = 0.0f;
		}
		for (int l78 = 0; l78 < 2; l78 = faust_wrap_add(l78, 1)) {
			fRec85[l78] = 0.0f;
		}
		for (int l79 = 0; l79 < 2; l79 = faust_wrap_add(l79, 1)) {
			fVec38[l79] = 0.0f;
		}
		for (int l80 = 0; l80 < 2; l80 = faust_wrap_add(l80, 1)) {
			fRec18[l80] = 0.0f;
		}
		for (int l81 = 0; l81 < 16384; l81 = faust_wrap_add(l81, 1)) {
			fVec39[l81] = 0.0f;
		}
		for (int l82 = 0; l82 < 2; l82 = faust_wrap_add(l82, 1)) {
			fRec86[l82] = 0.0f;
		}
		for (int l83 = 0; l83 < 2; l83 = faust_wrap_add(l83, 1)) {
			fVec40[l83] = 0.0f;
		}
		for (int l84 = 0; l84 < 2; l84 = faust_wrap_add(l84, 1)) {
			fRec19[l84] = 0.0f;
		}
		for (int l85 = 0; l85 < 16384; l85 = faust_wrap_add(l85, 1)) {
			fVec41[l85] = 0.0f;
		}
		for (int l86 = 0; l86 < 2; l86 = faust_wrap_add(l86, 1)) {
			fRec87[l86] = 0.0f;
		}
		for (int l87 = 0; l87 < 2; l87 = faust_wrap_add(l87, 1)) {
			fVec42[l87] = 0.0f;
		}
		for (int l88 = 0; l88 < 2; l88 = faust_wrap_add(l88, 1)) {
			fRec20[l88] = 0.0f;
		}
		for (int l89 = 0; l89 < 16384; l89 = faust_wrap_add(l89, 1)) {
			fVec43[l89] = 0.0f;
		}
		for (int l90 = 0; l90 < 2; l90 = faust_wrap_add(l90, 1)) {
			fRec88[l90] = 0.0f;
		}
		for (int l91 = 0; l91 < 2; l91 = faust_wrap_add(l91, 1)) {
			fVec44[l91] = 0.0f;
		}
		for (int l92 = 0; l92 < 2; l92 = faust_wrap_add(l92, 1)) {
			fRec21[l92] = 0.0f;
		}
		for (int l93 = 0; l93 < 16384; l93 = faust_wrap_add(l93, 1)) {
			fVec45[l93] = 0.0f;
		}
		for (int l94 = 0; l94 < 2; l94 = faust_wrap_add(l94, 1)) {
			fRec89[l94] = 0.0f;
		}
		for (int l95 = 0; l95 < 2; l95 = faust_wrap_add(l95, 1)) {
			fVec46[l95] = 0.0f;
		}
		for (int l96 = 0; l96 < 2; l96 = faust_wrap_add(l96, 1)) {
			fRec22[l96] = 0.0f;
		}
		for (int l97 = 0; l97 < 16384; l97 = faust_wrap_add(l97, 1)) {
			fVec47[l97] = 0.0f;
		}
		for (int l98 = 0; l98 < 16384; l98 = faust_wrap_add(l98, 1)) {
			fVec48[l98] = 0.0f;
		}
		for (int l99 = 0; l99 < 2; l99 = faust_wrap_add(l99, 1)) {
			fRec90[l99] = 0.0f;
		}
		for (int l100 = 0; l100 < 2; l100 = faust_wrap_add(l100, 1)) {
			fVec49[l100] = 0.0f;
		}
		for (int l101 = 0; l101 < 2; l101 = faust_wrap_add(l101, 1)) {
			fRec23[l101] = 0.0f;
		}
		for (int l102 = 0; l102 < 2; l102 = faust_wrap_add(l102, 1)) {
			fRec91[l102] = 0.0f;
		}
		for (int l103 = 0; l103 < 2; l103 = faust_wrap_add(l103, 1)) {
			fRec24[l103] = 0.0f;
		}
		for (int l104 = 0; l104 < 16384; l104 = faust_wrap_add(l104, 1)) {
			fVec50[l104] = 0.0f;
		}
		for (int l105 = 0; l105 < 16384; l105 = faust_wrap_add(l105, 1)) {
			fVec51[l105] = 0.0f;
		}
		for (int l106 = 0; l106 < 2; l106 = faust_wrap_add(l106, 1)) {
			fRec92[l106] = 0.0f;
		}
		for (int l107 = 0; l107 < 2; l107 = faust_wrap_add(l107, 1)) {
			fVec52[l107] = 0.0f;
		}
		for (int l108 = 0; l108 < 2; l108 = faust_wrap_add(l108, 1)) {
			fRec25[l108] = 0.0f;
		}
		for (int l109 = 0; l109 < 2; l109 = faust_wrap_add(l109, 1)) {
			fRec26[l109] = 0.0f;
		}
		for (int l110 = 0; l110 < 2; l110 = faust_wrap_add(l110, 1)) {
			fRec27[l110] = 0.0f;
		}
		for (int l111 = 0; l111 < 2; l111 = faust_wrap_add(l111, 1)) {
			fRec28[l111] = 0.0f;
		}
		for (int l112 = 0; l112 < 3; l112 = faust_wrap_add(l112, 1)) {
			fRec29[l112] = 0.0f;
		}
		for (int l113 = 0; l113 < 3; l113 = faust_wrap_add(l113, 1)) {
			fRec30[l113] = 0.0f;
		}
		for (int l114 = 0; l114 < 3; l114 = faust_wrap_add(l114, 1)) {
			fRec31[l114] = 0.0f;
		}
		for (int l115 = 0; l115 < 3; l115 = faust_wrap_add(l115, 1)) {
			fRec32[l115] = 0.0f;
		}
		for (int l116 = 0; l116 < 3; l116 = faust_wrap_add(l116, 1)) {
			fRec33[l116] = 0.0f;
		}
		for (int l117 = 0; l117 < 3; l117 = faust_wrap_add(l117, 1)) {
			fRec34[l117] = 0.0f;
		}
		for (int l118 = 0; l118 < 3; l118 = faust_wrap_add(l118, 1)) {
			fRec35[l118] = 0.0f;
		}
		for (int l119 = 0; l119 < 3; l119 = faust_wrap_add(l119, 1)) {
			fRec36[l119] = 0.0f;
		}
		for (int l120 = 0; l120 < 2; l120 = faust_wrap_add(l120, 1)) {
			fRec93[l120] = 0.0f;
		}
		for (int l121 = 0; l121 < 2; l121 = faust_wrap_add(l121, 1)) {
			fVec53[l121] = 0.0f;
		}
		for (int l122 = 0; l122 < 2; l122 = faust_wrap_add(l122, 1)) {
			fRec37[l122] = 0.0f;
		}
		for (int l123 = 0; l123 < 2; l123 = faust_wrap_add(l123, 1)) {
			fRec38[l123] = 0.0f;
		}
		for (int l124 = 0; l124 < 3; l124 = faust_wrap_add(l124, 1)) {
			fRec39[l124] = 0.0f;
		}
		for (int l125 = 0; l125 < 2; l125 = faust_wrap_add(l125, 1)) {
			fVec54[l125] = 0.0f;
		}
		for (int l126 = 0; l126 < 2; l126 = faust_wrap_add(l126, 1)) {
			fRec40[l126] = 0.0f;
		}
		for (int l127 = 0; l127 < 2; l127 = faust_wrap_add(l127, 1)) {
			fRec41[l127] = 0.0f;
		}
		for (int l128 = 0; l128 < 3; l128 = faust_wrap_add(l128, 1)) {
			fRec42[l128] = 0.0f;
		}
		for (int l129 = 0; l129 < 3; l129 = faust_wrap_add(l129, 1)) {
			fRec43[l129] = 0.0f;
		}
		for (int l130 = 0; l130 < 3; l130 = faust_wrap_add(l130, 1)) {
			fRec44[l130] = 0.0f;
		}
		for (int l131 = 0; l131 < 3; l131 = faust_wrap_add(l131, 1)) {
			fRec45[l131] = 0.0f;
		}
		for (int l132 = 0; l132 < 3; l132 = faust_wrap_add(l132, 1)) {
			fRec46[l132] = 0.0f;
		}
		for (int l133 = 0; l133 < 3; l133 = faust_wrap_add(l133, 1)) {
			fRec47[l133] = 0.0f;
		}
		for (int l134 = 0; l134 < 3; l134 = faust_wrap_add(l134, 1)) {
			fRec48[l134] = 0.0f;
		}
		for (int l135 = 0; l135 < 3; l135 = faust_wrap_add(l135, 1)) {
			fRec49[l135] = 0.0f;
		}
		for (int l136 = 0; l136 < 2; l136 = faust_wrap_add(l136, 1)) {
			fRec94[l136] = 0.0f;
		}
		for (int l137 = 0; l137 < 2; l137 = faust_wrap_add(l137, 1)) {
			fRec95[l137] = 0.0f;
		}
		for (int l138 = 0; l138 < 2; l138 = faust_wrap_add(l138, 1)) {
			fRec96[l138] = 0.0f;
		}
		for (int l139 = 0; l139 < 2; l139 = faust_wrap_add(l139, 1)) {
			fRec97[l139] = 0.0f;
		}
		for (int l140 = 0; l140 < 2; l140 = faust_wrap_add(l140, 1)) {
			fRec98[l140] = 0.0f;
		}
		for (int l141 = 0; l141 < 1024; l141 = faust_wrap_add(l141, 1)) {
			fVec55[l141] = 0.0f;
		}
		for (int l142 = 0; l142 < 2; l142 = faust_wrap_add(l142, 1)) {
			fRec50[l142] = 0.0f;
		}
		for (int l143 = 0; l143 < 3; l143 = faust_wrap_add(l143, 1)) {
			fRec51[l143] = 0.0f;
		}
		for (int l144 = 0; l144 < 1024; l144 = faust_wrap_add(l144, 1)) {
			fVec56[l144] = 0.0f;
		}
		for (int l145 = 0; l145 < 2; l145 = faust_wrap_add(l145, 1)) {
			fRec52[l145] = 0.0f;
		}
		for (int l146 = 0; l146 < 2; l146 = faust_wrap_add(l146, 1)) {
			fRec99[l146] = 0.0f;
		}
		for (int l147 = 0; l147 < 16384; l147 = faust_wrap_add(l147, 1)) {
			fVec57[l147] = 0.0f;
		}
		for (int l148 = 0; l148 < 2; l148 = faust_wrap_add(l148, 1)) {
			fVec58[l148] = 0.0f;
		}
		for (int l149 = 0; l149 < 2; l149 = faust_wrap_add(l149, 1)) {
			fRec53[l149] = 0.0f;
		}
		for (int l150 = 0; l150 < 16384; l150 = faust_wrap_add(l150, 1)) {
			fVec59[l150] = 0.0f;
		}
		for (int l151 = 0; l151 < 2; l151 = faust_wrap_add(l151, 1)) {
			fRec100[l151] = 0.0f;
		}
		for (int l152 = 0; l152 < 2; l152 = faust_wrap_add(l152, 1)) {
			fVec60[l152] = 0.0f;
		}
		for (int l153 = 0; l153 < 2; l153 = faust_wrap_add(l153, 1)) {
			fRec54[l153] = 0.0f;
		}
		for (int l154 = 0; l154 < 16384; l154 = faust_wrap_add(l154, 1)) {
			fVec61[l154] = 0.0f;
		}
		for (int l155 = 0; l155 < 2; l155 = faust_wrap_add(l155, 1)) {
			fVec62[l155] = 0.0f;
		}
		for (int l156 = 0; l156 < 2; l156 = faust_wrap_add(l156, 1)) {
			fRec55[l156] = 0.0f;
		}
		for (int l157 = 0; l157 < 16384; l157 = faust_wrap_add(l157, 1)) {
			fVec63[l157] = 0.0f;
		}
		for (int l158 = 0; l158 < 2; l158 = faust_wrap_add(l158, 1)) {
			fVec64[l158] = 0.0f;
		}
		for (int l159 = 0; l159 < 2; l159 = faust_wrap_add(l159, 1)) {
			fRec56[l159] = 0.0f;
		}
		for (int l160 = 0; l160 < 16384; l160 = faust_wrap_add(l160, 1)) {
			fVec65[l160] = 0.0f;
		}
		for (int l161 = 0; l161 < 2; l161 = faust_wrap_add(l161, 1)) {
			fRec101[l161] = 0.0f;
		}
		for (int l162 = 0; l162 < 2; l162 = faust_wrap_add(l162, 1)) {
			fVec66[l162] = 0.0f;
		}
		for (int l163 = 0; l163 < 2; l163 = faust_wrap_add(l163, 1)) {
			fRec57[l163] = 0.0f;
		}
		for (int l164 = 0; l164 < 16384; l164 = faust_wrap_add(l164, 1)) {
			fVec67[l164] = 0.0f;
		}
		for (int l165 = 0; l165 < 2; l165 = faust_wrap_add(l165, 1)) {
			fVec68[l165] = 0.0f;
		}
		for (int l166 = 0; l166 < 2; l166 = faust_wrap_add(l166, 1)) {
			fRec58[l166] = 0.0f;
		}
		for (int l167 = 0; l167 < 16384; l167 = faust_wrap_add(l167, 1)) {
			fVec69[l167] = 0.0f;
		}
		for (int l168 = 0; l168 < 2; l168 = faust_wrap_add(l168, 1)) {
			fVec70[l168] = 0.0f;
		}
		for (int l169 = 0; l169 < 2; l169 = faust_wrap_add(l169, 1)) {
			fRec59[l169] = 0.0f;
		}
		for (int l170 = 0; l170 < 16384; l170 = faust_wrap_add(l170, 1)) {
			fVec71[l170] = 0.0f;
		}
		for (int l171 = 0; l171 < 2; l171 = faust_wrap_add(l171, 1)) {
			fRec102[l171] = 0.0f;
		}
		for (int l172 = 0; l172 < 2; l172 = faust_wrap_add(l172, 1)) {
			fVec72[l172] = 0.0f;
		}
		for (int l173 = 0; l173 < 2; l173 = faust_wrap_add(l173, 1)) {
			fRec60[l173] = 0.0f;
		}
		for (int l174 = 0; l174 < 2; l174 = faust_wrap_add(l174, 1)) {
			fRec61[l174] = 0.0f;
		}
		for (int l175 = 0; l175 < 2; l175 = faust_wrap_add(l175, 1)) {
			fRec62[l175] = 0.0f;
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
	
	virtual T_jpverb* clone() {
		return new T_jpverb(*this);
	}
	
	virtual int getSampleRate() {
		return fSampleRate;
	}
	
	virtual void buildUserInterface(UI* ui_interface) {
		ui_interface->openVerticalBox("jpverb");
		ui_interface->addHorizontalSlider("damp", &fHslider6, FAUSTFLOAT(0.5f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("earlyDiff", &fHslider11, FAUSTFLOAT(0.7f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("high", &fHslider7, FAUSTFLOAT(0.7f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("highCutoff", &fHslider4, FAUSTFLOAT(3e+03f), FAUSTFLOAT(1.5e+03f), FAUSTFLOAT(1e+04f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("low", &fHslider8, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("lowCutoff", &fHslider5, FAUSTFLOAT(5e+02f), FAUSTFLOAT(1e+02f), FAUSTFLOAT(4e+03f), FAUSTFLOAT(1.0f));
		ui_interface->addHorizontalSlider("mid", &fHslider9, FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("mix", &fHslider0, FAUSTFLOAT(0.4f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("modDepth", &fHslider2, FAUSTFLOAT(0.2f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("modFreq", &fHslider3, FAUSTFLOAT(2.0f), FAUSTFLOAT(0.0f), FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("size", &fHslider1, FAUSTFLOAT(1.5f), FAUSTFLOAT(0.5f), FAUSTFLOAT(5.0f), FAUSTFLOAT(0.01f));
		ui_interface->addHorizontalSlider("t60", &fHslider10, FAUSTFLOAT(2.0f), FAUSTFLOAT(1.0f), FAUSTFLOAT(6e+01f), FAUSTFLOAT(0.01f));
		ui_interface->closeBox();
	}
	
	void control() {
		fSlow0 = fConst1 * static_cast<float>(fHslider0);
		fSlow1 = fConst1 * static_cast<float>(fHslider1);
		fSlow2 = fConst1 * static_cast<float>(fHslider2);
		fSlow3 = fConst1 * static_cast<float>(fHslider3);
		fSlow4 = fConst1 * static_cast<float>(fHslider4);
		fSlow5 = fConst1 * static_cast<float>(fHslider5);
		fSlow6 = fConst1 * static_cast<float>(fHslider6);
		fSlow7 = fConst1 * static_cast<float>(fHslider7);
		fSlow8 = fConst1 * static_cast<float>(fHslider8);
		fSlow9 = fConst1 * static_cast<float>(fHslider9);
		fSlow10 = fConst1 * static_cast<float>(fHslider10);
		fSlow11 = fConst1 * static_cast<float>(fHslider11);
	}
	
	virtual void frame(FAUSTFLOAT* RESTRICT inputs, FAUSTFLOAT* RESTRICT outputs) {
		float fTemp0 = static_cast<float>(inputs[0]);
		iVec0[0] = 1;
		fRec0[0] = fSlow0 + fConst2 * fRec0[1];
		float fTemp1 = 0.7602446f * fRec62[1] - 0.6496369f * fRec1[1];
		float fTemp2 = 0.6496369f * fRec2[1];
		float fTemp3 = 0.7602446f * fRec61[1];
		fVec1[IOTA0 & 16383] = 0.70710677f * fTemp1 + 0.70710677f * (fTemp2 - fTemp3);
		int iTemp4 = faust_wrap_sub(1, iVec0[1]);
		fRec64[0] = fSlow1 + fConst2 * fRec64[1];
		int iTemp5 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(1e+01f * fRec64[0]), 2047))];
		fRec63[0] = 0.9999f * (fRec63[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp5))) + 0.0001f * static_cast<float>(iTemp5);
		float fTemp6 = -1.49999f + fRec63[0];
		float fTemp7 = fVec1[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp6))))) & 16383];
		fVec2[0] = fTemp7;
		float fTemp8 = std::floor(fTemp6);
		fRec1[0] = fVec2[1] - (2.0f - fRec63[0] + fTemp8) * (fRec1[1] - fTemp7) / (fRec63[0] - fTemp8);
		fVec3[IOTA0 & 16383] = 0.70710677f * fTemp1 + 0.70710677f * (fTemp3 - fTemp2);
		int iTemp9 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(1.1e+02f * fRec64[0]), 2047))];
		fRec65[0] = 0.9999f * (fRec65[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp9))) + 0.0001f * static_cast<float>(iTemp9);
		float fTemp10 = -1.49999f + fRec65[0];
		float fTemp11 = fVec3[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp10))))) & 16383];
		fVec4[0] = fTemp11;
		float fTemp12 = std::floor(fTemp10);
		fRec2[0] = fVec4[1] - (2.0f - fRec65[0] + fTemp12) * (fRec2[1] - fTemp11) / (fRec65[0] - fTemp12);
		float fTemp13 = 0.7602446f * fRec1[1] + 0.6496369f * fRec62[1];
		float fTemp14 = 0.7602446f * fTemp13 - 0.6496369f * fRec4[1];
		float fTemp15 = 0.7602446f * fRec2[1] + 0.6496369f * fRec61[1];
		float fTemp16 = 0.7602446f * fTemp15;
		float fTemp17 = 0.6496369f * fRec3[1];
		fVec5[IOTA0 & 16383] = 0.70710677f * fTemp14 + 0.70710677f * (fTemp16 - fTemp17);
		int iTemp18 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(1.4e+02f * fRec64[0]), 2047))];
		fRec66[0] = 0.9999f * (fRec66[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp18))) + 0.0001f * static_cast<float>(iTemp18);
		float fTemp19 = -1.49999f + fRec66[0];
		float fTemp20 = fVec5[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp19))))) & 16383];
		fVec6[0] = fTemp20;
		float fTemp21 = std::floor(fTemp19);
		fRec3[0] = fVec6[1] - (2.0f - fRec66[0] + fTemp21) * (fRec3[1] - fTemp20) / (fRec66[0] - fTemp21);
		fVec7[IOTA0 & 16383] = 0.70710677f * fTemp14 + 0.70710677f * (fTemp17 - fTemp16);
		int iTemp22 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(4e+01f * fRec64[0]), 2047))];
		fRec67[0] = 0.9999f * (fRec67[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp22))) + 0.0001f * static_cast<float>(iTemp22);
		float fTemp23 = -1.49999f + fRec67[0];
		float fTemp24 = fVec7[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp23))))) & 16383];
		fVec8[0] = fTemp24;
		float fTemp25 = std::floor(fTemp23);
		fRec4[0] = fVec8[1] - (2.0f - fRec67[0] + fTemp25) * (fRec4[1] - fTemp24) / (fRec67[0] - fTemp25);
		float fTemp26 = 0.7602446f * fRec4[1] + 0.6496369f * fTemp13;
		float fTemp27 = 0.7602446f * fTemp26 - 0.6496369f * fRec6[1];
		float fTemp28 = 0.7602446f * fRec3[1] + 0.6496369f * fTemp15;
		float fTemp29 = 0.7602446f * fTemp28;
		float fTemp30 = 0.6496369f * fRec5[1];
		fVec9[IOTA0 & 16383] = 0.70710677f * fTemp27 + 0.70710677f * (fTemp29 - fTemp30);
		int iTemp31 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(1.7e+02f * fRec64[0]), 2047))];
		fRec68[0] = 0.9999f * (fRec68[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp31))) + 0.0001f * static_cast<float>(iTemp31);
		float fTemp32 = -1.49999f + fRec68[0];
		float fTemp33 = fVec9[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp32))))) & 16383];
		fVec10[0] = fTemp33;
		float fTemp34 = std::floor(fTemp32);
		fRec5[0] = fVec10[1] - (2.0f - fRec68[0] + fTemp34) * (fRec5[1] - fTemp33) / (fRec68[0] - fTemp34);
		fVec11[IOTA0 & 16383] = 0.70710677f * fTemp27 + 0.70710677f * (fTemp30 - fTemp29);
		int iTemp35 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(7e+01f * fRec64[0]), 2047))];
		fRec69[0] = 0.9999f * (fRec69[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp35))) + 0.0001f * static_cast<float>(iTemp35);
		float fTemp36 = -1.49999f + fRec69[0];
		float fTemp37 = fVec11[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp36))))) & 16383];
		fVec12[0] = fTemp37;
		float fTemp38 = std::floor(fTemp36);
		fRec6[0] = fVec12[1] - (2.0f - fRec69[0] + fTemp38) * (fRec6[1] - fTemp37) / (fRec69[0] - fTemp38);
		float fTemp39 = 0.7602446f * fRec6[1] + 0.6496369f * fTemp26;
		float fTemp40 = 0.7602446f * fTemp39 - 0.6496369f * fRec7[1];
		float fTemp41 = 0.6496369f * fRec8[1];
		float fTemp42 = 0.7602446f * fRec5[1] + 0.6496369f * fTemp28;
		float fTemp43 = 0.7602446f * fTemp42;
		fVec13[IOTA0 & 16383] = 0.70710677f * fTemp40 + 0.70710677f * (fTemp41 - fTemp43);
		int iTemp44 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(1e+02f * fRec64[0]), 2047))];
		fRec70[0] = 0.9999f * (fRec70[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp44))) + 0.0001f * static_cast<float>(iTemp44);
		float fTemp45 = -1.49999f + fRec70[0];
		float fTemp46 = fVec13[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp45))))) & 16383];
		fVec14[0] = fTemp46;
		float fTemp47 = std::floor(fTemp45);
		fRec7[0] = fVec14[1] - (2.0f - fRec70[0] + fTemp47) * (fRec7[1] - fTemp46) / (fRec70[0] - fTemp47);
		fVec15[IOTA0 & 16383] = 0.70710677f * fTemp40 + 0.70710677f * (fTemp43 - fTemp41);
		int iTemp48 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(2e+02f * fRec64[0]), 2047))];
		fRec71[0] = 0.9999f * (fRec71[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp48))) + 0.0001f * static_cast<float>(iTemp48);
		float fTemp49 = -1.49999f + fRec71[0];
		float fTemp50 = fVec15[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp49))))) & 16383];
		fVec16[0] = fTemp50;
		float fTemp51 = std::floor(fTemp49);
		fRec8[0] = fVec16[1] - (2.0f - fRec71[0] + fTemp51) * (fRec8[1] - fTemp50) / (fRec71[0] - fTemp51);
		float fTemp52 = 0.7602446f * fRec7[1] + 0.6496369f * fTemp39;
		float fTemp53 = 0.7602446f * fTemp52 - 0.6496369f * fRec9[1];
		float fTemp54 = 0.6496369f * fRec11[1];
		float fTemp55 = 0.7602446f * fRec8[1] + 0.6496369f * fTemp42;
		float fTemp56 = 0.7602446f * fTemp55;
		fVec17[IOTA0 & 16383] = 0.70710677f * fTemp53 + 0.70710677f * (fTemp54 - fTemp56);
		int iTemp57 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(1.3e+02f * fRec64[0]), 2047))];
		fRec72[0] = 0.9999f * (fRec72[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp57))) + 0.0001f * static_cast<float>(iTemp57);
		float fTemp58 = -1.49999f + fRec72[0];
		float fTemp59 = fVec17[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp58))))) & 16383];
		fVec18[0] = fTemp59;
		float fTemp60 = std::floor(fTemp58);
		fRec9[0] = fVec18[1] - (2.0f - fRec72[0] + fTemp60) * (fRec9[1] - fTemp59) / (fRec72[0] - fTemp60);
		fRec73[0] = fSlow2 + fConst2 * fRec73[1];
		fRec76[0] = fSlow3 + fConst2 * fRec76[1];
		float fTemp61 = fConst3 * fRec76[0];
		float fTemp62 = std::sin(fTemp61);
		float fTemp63 = std::cos(fTemp61);
		fRec74[0] = fRec75[1] * fTemp62 + fRec74[1] * fTemp63;
		fRec75[0] = static_cast<float>(iTemp4) - fTemp62 * fRec74[1] + fRec75[1] * fTemp63;
		float fTemp64 = 5e+01f * fRec73[0] * (1.0f + fRec75[0]);
		float fTemp65 = 3.500005f + fTemp64;
		float fTemp66 = std::floor(fTemp65);
		float fTemp67 = 1.0f - fTemp66 + fTemp64;
		float fTemp68 = 2.0f - fTemp66 + fTemp64;
		float fTemp69 = 3.0f - fTemp66 + fTemp64;
		float fTemp70 = 0.7602446f * fRec9[1] + 0.6496369f * fTemp52;
		fVec19[IOTA0 & 1023] = fTemp70;
		int iTemp71 = static_cast<int>(fTemp65);
		int iTemp72 = std::min<int>(512, std::max<int>(0, iTemp71));
		float fTemp73 = 4.0f - fTemp66 + fTemp64;
		float fTemp74 = 5.0f - fTemp66 + fTemp64;
		int iTemp75 = std::min<int>(512, std::max<int>(0, faust_wrap_add(1, iTemp71)));
		float fTemp76 = fTemp74 * fTemp73;
		int iTemp77 = std::min<int>(512, std::max<int>(0, faust_wrap_add(2, iTemp71)));
		float fTemp78 = fTemp76 * fTemp69;
		int iTemp79 = std::min<int>(512, std::max<int>(0, faust_wrap_add(3, iTemp71)));
		float fTemp80 = fTemp78 * fTemp68;
		int iTemp81 = std::min<int>(512, std::max<int>(0, faust_wrap_add(4, iTemp71)));
		fVec20[IOTA0 & 16383] = fTemp67 * (fTemp68 * (fTemp69 * (0.041666668f * fVec19[(faust_wrap_sub(IOTA0, iTemp72)) & 1023] * fTemp73 - 0.16666667f * fTemp74 * fVec19[(faust_wrap_sub(IOTA0, iTemp75)) & 1023]) + 0.25f * fTemp76 * fVec19[(faust_wrap_sub(IOTA0, iTemp77)) & 1023]) - 0.16666667f * fTemp78 * fVec19[(faust_wrap_sub(IOTA0, iTemp79)) & 1023]) + 0.041666668f * fTemp80 * fVec19[(faust_wrap_sub(IOTA0, iTemp81)) & 1023];
		int iTemp82 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(54.0f * fRec64[0]), 2047))];
		fRec77[0] = 0.995f * (fRec77[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp82))) + 0.005f * static_cast<float>(iTemp82);
		float fTemp83 = -1.49999f + fRec77[0];
		float fTemp84 = fVec20[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp83))))) & 16383];
		fVec21[0] = fTemp84;
		float fTemp85 = std::floor(fTemp83);
		fRec10[0] = fVec21[1] - (2.0f - fRec77[0] + fTemp85) * (fRec10[1] - fTemp84) / (fRec77[0] - fTemp85);
		fVec22[IOTA0 & 16383] = 0.70710677f * fTemp53 + 0.70710677f * (fTemp56 - fTemp54);
		int iTemp86 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(2.3e+02f * fRec64[0]), 2047))];
		fRec78[0] = 0.9999f * (fRec78[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp86))) + 0.0001f * static_cast<float>(iTemp86);
		float fTemp87 = -1.49999f + fRec78[0];
		float fTemp88 = fVec22[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp87))))) & 16383];
		fVec23[0] = fTemp88;
		float fTemp89 = std::floor(fTemp87);
		fRec11[0] = fVec23[1] - (2.0f - fRec78[0] + fTemp89) * (fRec11[1] - fTemp88) / (fRec78[0] - fTemp89);
		float fTemp90 = 5e+01f * fRec73[0] * (1.0f - fRec75[0]);
		float fTemp91 = 3.500005f + fTemp90;
		float fTemp92 = std::floor(fTemp91);
		float fTemp93 = 2.0f - fTemp92 + fTemp90;
		float fTemp94 = 3.0f - fTemp92 + fTemp90;
		float fTemp95 = 0.7602446f * fRec11[1] + 0.6496369f * fTemp55;
		fVec24[IOTA0 & 1023] = fTemp95;
		int iTemp96 = static_cast<int>(fTemp91);
		float fTemp97 = 4.0f - fTemp92 + fTemp90;
		float fTemp98 = 5.0f - fTemp92 + fTemp90;
		float fTemp99 = fTemp98 * fTemp97;
		float fTemp100 = fTemp99 * fTemp94;
		fVec25[IOTA0 & 16383] = (1.0f - fTemp92 + fTemp90) * (fTemp93 * (fTemp94 * (0.041666668f * fVec24[(faust_wrap_sub(IOTA0, std::min<int>(512, std::max<int>(0, iTemp96)))) & 1023] * fTemp97 - 0.16666667f * fTemp98 * fVec24[(faust_wrap_sub(IOTA0, std::min<int>(512, std::max<int>(0, faust_wrap_add(1, iTemp96))))) & 1023]) + 0.25f * fTemp99 * fVec24[(faust_wrap_sub(IOTA0, std::min<int>(512, std::max<int>(0, faust_wrap_add(2, iTemp96))))) & 1023]) - 0.16666667f * fTemp100 * fVec24[(faust_wrap_sub(IOTA0, std::min<int>(512, std::max<int>(0, faust_wrap_add(3, iTemp96))))) & 1023]) + 0.041666668f * fTemp100 * fTemp93 * fVec24[(faust_wrap_sub(IOTA0, std::min<int>(512, std::max<int>(0, faust_wrap_add(4, iTemp96))))) & 1023];
		int iTemp101 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(204.0f * fRec64[0]), 2047))];
		fRec79[0] = 0.995f * (fRec79[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp101))) + 0.005f * static_cast<float>(iTemp101);
		float fTemp102 = -1.49999f + fRec79[0];
		float fTemp103 = fVec25[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp102))))) & 16383];
		fVec26[0] = fTemp103;
		float fTemp104 = std::floor(fTemp102);
		fRec12[0] = fVec26[1] - (2.0f - fRec79[0] + fTemp104) * (fRec12[1] - fTemp103) / (fRec79[0] - fTemp104);
		float fTemp105 = 0.7602446f * fRec10[0] - 0.6496369f * fRec13[1];
		float fTemp106 = 0.6496369f * fRec14[1];
		float fTemp107 = 0.7602446f * fRec12[0];
		fVec27[IOTA0 & 16383] = 0.70710677f * fTemp105 + 0.70710677f * (fTemp106 - fTemp107);
		int iTemp108 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(125.0f * fRec64[0]), 2047))];
		fRec80[0] = 0.9999f * (fRec80[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp108))) + 0.0001f * static_cast<float>(iTemp108);
		float fTemp109 = -1.49999f + fRec80[0];
		float fTemp110 = fVec27[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp109))))) & 16383];
		fVec28[0] = fTemp110;
		float fTemp111 = std::floor(fTemp109);
		fRec13[0] = fVec28[1] - (2.0f - fRec80[0] + fTemp111) * (fRec13[1] - fTemp110) / (fRec80[0] - fTemp111);
		fVec29[IOTA0 & 16383] = 0.70710677f * fTemp105 + 0.70710677f * (fTemp107 - fTemp106);
		int iTemp112 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(25.0f * fRec64[0]), 2047))];
		fRec81[0] = 0.9999f * (fRec81[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp112))) + 0.0001f * static_cast<float>(iTemp112);
		float fTemp113 = -1.49999f + fRec81[0];
		float fTemp114 = fVec29[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp113))))) & 16383];
		fVec30[0] = fTemp114;
		float fTemp115 = std::floor(fTemp113);
		fRec14[0] = fVec30[1] - (2.0f - fRec81[0] + fTemp115) * (fRec14[1] - fTemp114) / (fRec81[0] - fTemp115);
		float fTemp116 = 0.7602446f * fRec13[1] + 0.6496369f * fRec10[0];
		float fTemp117 = 0.7602446f * fTemp116 - 0.6496369f * fRec15[1];
		float fTemp118 = 0.6496369f * fRec16[1];
		float fTemp119 = 0.7602446f * fRec14[1] + 0.6496369f * fRec12[0];
		float fTemp120 = 0.7602446f * fTemp119;
		fVec31[IOTA0 & 16383] = 0.70710677f * fTemp117 + 0.70710677f * (fTemp118 - fTemp120);
		int iTemp121 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(155.0f * fRec64[0]), 2047))];
		fRec82[0] = 0.9999f * (fRec82[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp121))) + 0.0001f * static_cast<float>(iTemp121);
		float fTemp122 = -1.49999f + fRec82[0];
		float fTemp123 = fVec31[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp122))))) & 16383];
		fVec32[0] = fTemp123;
		float fTemp124 = std::floor(fTemp122);
		fRec15[0] = fVec32[1] - (2.0f - fRec82[0] + fTemp124) * (fRec15[1] - fTemp123) / (fRec82[0] - fTemp124);
		fVec33[IOTA0 & 16383] = 0.70710677f * fTemp117 + 0.70710677f * (fTemp120 - fTemp118);
		int iTemp125 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(55.0f * fRec64[0]), 2047))];
		fRec83[0] = 0.9999f * (fRec83[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp125))) + 0.0001f * static_cast<float>(iTemp125);
		float fTemp126 = -1.49999f + fRec83[0];
		int iTemp127 = std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp126)));
		float fTemp128 = fVec33[(faust_wrap_sub(IOTA0, iTemp127)) & 16383];
		fVec34[0] = fTemp128;
		float fTemp129 = std::floor(fTemp126);
		float fTemp130 = 2.0f - fRec83[0] + fTemp129;
		float fTemp131 = fRec83[0] - fTemp129;
		fRec16[0] = fVec34[1] - fTemp130 * (fRec16[1] - fTemp128) / fTemp131;
		float fTemp132 = 0.7602446f * fRec15[1] + 0.6496369f * fTemp116;
		float fTemp133 = 0.7602446f * fTemp132 - 0.6496369f * fRec18[1];
		float fTemp134 = 0.7602446f * fRec16[1] + 0.6496369f * fTemp119;
		float fTemp135 = 0.7602446f * fTemp134;
		float fTemp136 = 0.6496369f * fRec17[1];
		fVec35[IOTA0 & 16383] = 0.70710677f * fTemp133 + 0.70710677f * (fTemp135 - fTemp136);
		int iTemp137 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(85.0f * fRec64[0]), 2047))];
		fRec84[0] = 0.9999f * (fRec84[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp137))) + 0.0001f * static_cast<float>(iTemp137);
		float fTemp138 = -1.49999f + fRec84[0];
		int iTemp139 = std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp138)));
		float fTemp140 = fVec35[(faust_wrap_sub(IOTA0, iTemp139)) & 16383];
		fVec36[0] = fTemp140;
		float fTemp141 = std::floor(fTemp138);
		float fTemp142 = 2.0f - fRec84[0] + fTemp141;
		float fTemp143 = fRec84[0] - fTemp141;
		fRec17[0] = fVec36[1] - fTemp142 * (fRec17[1] - fTemp140) / fTemp143;
		fVec37[IOTA0 & 16383] = 0.70710677f * fTemp133 + 0.70710677f * (fTemp136 - fTemp135);
		int iTemp144 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(185.0f * fRec64[0]), 2047))];
		fRec85[0] = 0.9999f * (fRec85[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp144))) + 0.0001f * static_cast<float>(iTemp144);
		float fTemp145 = -1.49999f + fRec85[0];
		float fTemp146 = fVec37[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp145))))) & 16383];
		fVec38[0] = fTemp146;
		float fTemp147 = std::floor(fTemp145);
		fRec18[0] = fVec38[1] - (2.0f - fRec85[0] + fTemp147) * (fRec18[1] - fTemp146) / (fRec85[0] - fTemp147);
		float fTemp148 = 0.7602446f * fRec18[1] + 0.6496369f * fTemp132;
		float fTemp149 = 0.7602446f * fTemp148 - 0.6496369f * fRec19[1];
		float fTemp150 = 0.6496369f * fRec20[1];
		float fTemp151 = 0.7602446f * fRec17[1] + 0.6496369f * fTemp134;
		float fTemp152 = 0.7602446f * fTemp151;
		fVec39[IOTA0 & 16383] = 0.70710677f * fTemp149 + 0.70710677f * (fTemp150 - fTemp152);
		int iTemp153 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(215.0f * fRec64[0]), 2047))];
		fRec86[0] = 0.9999f * (fRec86[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp153))) + 0.0001f * static_cast<float>(iTemp153);
		float fTemp154 = -1.49999f + fRec86[0];
		int iTemp155 = std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp154)));
		float fTemp156 = fVec39[(faust_wrap_sub(IOTA0, iTemp155)) & 16383];
		fVec40[0] = fTemp156;
		float fTemp157 = std::floor(fTemp154);
		float fTemp158 = 2.0f - fRec86[0] + fTemp157;
		float fTemp159 = fRec86[0] - fTemp157;
		fRec19[0] = fVec40[1] - fTemp158 * (fRec19[1] - fTemp156) / fTemp159;
		fVec41[IOTA0 & 16383] = 0.70710677f * fTemp149 + 0.70710677f * (fTemp152 - fTemp150);
		int iTemp160 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(115.0f * fRec64[0]), 2047))];
		fRec87[0] = 0.9999f * (fRec87[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp160))) + 0.0001f * static_cast<float>(iTemp160);
		float fTemp161 = -1.49999f + fRec87[0];
		int iTemp162 = std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp161)));
		float fTemp163 = fVec41[(faust_wrap_sub(IOTA0, iTemp162)) & 16383];
		fVec42[0] = fTemp163;
		float fTemp164 = std::floor(fTemp161);
		float fTemp165 = 2.0f - fRec87[0] + fTemp164;
		float fTemp166 = fRec87[0] - fTemp164;
		fRec20[0] = fVec42[1] - fTemp165 * (fRec20[1] - fTemp163) / fTemp166;
		float fTemp167 = 0.7602446f * fRec19[1] + 0.6496369f * fTemp148;
		float fTemp168 = 0.7602446f * fTemp167 - 0.6496369f * fRec22[1];
		float fTemp169 = 0.7602446f * fRec20[1] + 0.6496369f * fTemp151;
		float fTemp170 = 0.7602446f * fTemp169;
		float fTemp171 = 0.6496369f * fRec21[1];
		fVec43[IOTA0 & 16383] = 0.70710677f * fTemp168 + 0.70710677f * (fTemp170 - fTemp171);
		int iTemp172 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(145.0f * fRec64[0]), 2047))];
		fRec88[0] = 0.9999f * (fRec88[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp172))) + 0.0001f * static_cast<float>(iTemp172);
		float fTemp173 = -1.49999f + fRec88[0];
		int iTemp174 = std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp173)));
		float fTemp175 = fVec43[(faust_wrap_sub(IOTA0, iTemp174)) & 16383];
		fVec44[0] = fTemp175;
		float fTemp176 = std::floor(fTemp173);
		float fTemp177 = 2.0f - fRec88[0] + fTemp176;
		float fTemp178 = fRec88[0] - fTemp176;
		fRec21[0] = fVec44[1] - fTemp177 * (fRec21[1] - fTemp175) / fTemp178;
		fVec45[IOTA0 & 16383] = 0.70710677f * fTemp168 + 0.70710677f * (fTemp171 - fTemp170);
		int iTemp179 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(245.0f * fRec64[0]), 2047))];
		fRec89[0] = 0.9999f * (fRec89[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp179))) + 0.0001f * static_cast<float>(iTemp179);
		float fTemp180 = -1.49999f + fRec89[0];
		float fTemp181 = fVec45[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp180))))) & 16383];
		fVec46[0] = fTemp181;
		float fTemp182 = std::floor(fTemp180);
		fRec22[0] = fVec46[1] - (2.0f - fRec89[0] + fTemp182) * (fRec22[1] - fTemp181) / (fRec89[0] - fTemp182);
		float fTemp183 = 5e+01f * fRec73[0] * (1.0f - fRec74[0]);
		float fTemp184 = 3.500005f + fTemp183;
		float fTemp185 = std::floor(fTemp184);
		float fTemp186 = 2.0f - fTemp185 + fTemp183;
		float fTemp187 = 3.0f - fTemp185 + fTemp183;
		float fTemp188 = 0.7602446f * fRec21[1] + 0.6496369f * fTemp169;
		fVec47[IOTA0 & 16383] = fTemp188;
		int iTemp189 = static_cast<int>(fTemp184);
		float fTemp190 = 4.0f - fTemp185 + fTemp183;
		float fTemp191 = 5.0f - fTemp185 + fTemp183;
		float fTemp192 = fTemp191 * fTemp190;
		float fTemp193 = fTemp192 * fTemp187;
		fVec48[IOTA0 & 16383] = (1.0f - fTemp185 + fTemp183) * (fTemp186 * (fTemp187 * (0.041666668f * fVec47[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, iTemp189)))) & 16383] * fTemp190 - 0.16666667f * fTemp191 * fVec47[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, faust_wrap_add(1, iTemp189))))) & 16383]) + 0.25f * fTemp192 * fVec47[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, faust_wrap_add(2, iTemp189))))) & 16383]) - 0.16666667f * fTemp193 * fVec47[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, faust_wrap_add(3, iTemp189))))) & 16383]) + 0.041666668f * fTemp193 * fTemp186 * fVec47[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, faust_wrap_add(4, iTemp189))))) & 16383];
		int iTemp194 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(34.0f * fRec64[0]), 2047))];
		fRec90[0] = 0.995f * (fRec90[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp194))) + 0.005f * static_cast<float>(iTemp194);
		float fTemp195 = -1.49999f + fRec90[0];
		float fTemp196 = fVec48[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp195))))) & 16383];
		fVec49[0] = fTemp196;
		float fTemp197 = std::floor(fTemp195);
		fRec23[0] = fVec49[1] - (2.0f - fRec90[0] + fTemp197) * (fRec23[1] - fTemp196) / (fRec90[0] - fTemp197);
		fRec91[0] = fSlow4 + fConst2 * fRec91[1];
		float fTemp198 = std::tan(fConst4 * fRec91[0]);
		float fTemp199 = 1.0f / fTemp198;
		float fTemp200 = 1.0f - fTemp199;
		float fTemp201 = 1.0f + fTemp199;
		fRec24[0] = -((fRec24[1] * fTemp200 - (fRec23[1] + fRec23[0])) / fTemp201);
		float fTemp202 = 5e+01f * fRec73[0] * (1.0f + fRec74[0]);
		float fTemp203 = 3.500005f + fTemp202;
		float fTemp204 = std::floor(fTemp203);
		float fTemp205 = 1.0f - fTemp204 + fTemp202;
		float fTemp206 = 2.0f - fTemp204 + fTemp202;
		float fTemp207 = 3.0f - fTemp204 + fTemp202;
		float fTemp208 = 0.7602446f * fRec22[1] + 0.6496369f * fTemp167;
		fVec50[IOTA0 & 16383] = fTemp208;
		int iTemp209 = static_cast<int>(fTemp203);
		int iTemp210 = std::max<int>(0, iTemp209);
		float fTemp211 = 4.0f - fTemp204 + fTemp202;
		float fTemp212 = 5.0f - fTemp204 + fTemp202;
		int iTemp213 = std::max<int>(0, faust_wrap_add(1, iTemp209));
		float fTemp214 = fTemp212 * fTemp211;
		int iTemp215 = std::max<int>(0, faust_wrap_add(2, iTemp209));
		float fTemp216 = fTemp214 * fTemp207;
		int iTemp217 = std::max<int>(0, faust_wrap_add(3, iTemp209));
		float fTemp218 = fTemp216 * fTemp206;
		int iTemp219 = std::max<int>(0, faust_wrap_add(4, iTemp209));
		fVec51[IOTA0 & 16383] = fTemp205 * (fTemp206 * (fTemp207 * (0.041666668f * fVec50[(faust_wrap_sub(IOTA0, std::min<int>(8192, iTemp210))) & 16383] * fTemp211 - 0.16666667f * fTemp212 * fVec50[(faust_wrap_sub(IOTA0, std::min<int>(8192, iTemp213))) & 16383]) + 0.25f * fTemp214 * fVec50[(faust_wrap_sub(IOTA0, std::min<int>(8192, iTemp215))) & 16383]) - 0.16666667f * fTemp216 * fVec50[(faust_wrap_sub(IOTA0, std::min<int>(8192, iTemp217))) & 16383]) + 0.041666668f * fTemp218 * fVec50[(faust_wrap_sub(IOTA0, std::min<int>(8192, iTemp219))) & 16383];
		int iTemp220 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(134.0f * fRec64[0]), 2047))];
		fRec92[0] = 0.995f * (fRec92[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp220))) + 0.005f * static_cast<float>(iTemp220);
		float fTemp221 = -1.49999f + fRec92[0];
		float fTemp222 = fVec51[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp221))))) & 16383];
		fVec52[0] = fTemp222;
		float fTemp223 = std::floor(fTemp221);
		fRec25[0] = fVec52[1] - (2.0f - fRec92[0] + fTemp223) * (fRec25[1] - fTemp222) / (fRec92[0] - fTemp223);
		fRec26[0] = -((fTemp200 * fRec26[1] - (fRec25[1] + fRec25[0])) / fTemp201);
		fRec27[0] = -((fTemp200 * fRec27[1] - (fRec25[0] - fRec25[1]) / fTemp198) / fTemp201);
		fRec28[0] = -((fTemp200 * fRec28[1] - (fRec23[0] - fRec23[1]) / fTemp198) / fTemp201);
		float fTemp224 = 1.0f + (-1.618034f + fTemp199) / fTemp198;
		float fTemp225 = T_jpverb_faustpower2_f(fTemp198);
		float fTemp226 = 1.0f - 1.0f / fTemp225;
		float fTemp227 = 1.0f + (1.618034f + fTemp199) / fTemp198;
		fRec29[0] = fRec28[0] - (fRec29[2] * fTemp224 + 2.0f * fRec29[1] * fTemp226) / fTemp227;
		fRec30[0] = fRec26[0] - (fTemp224 * fRec30[2] + 2.0f * fTemp226 * fRec30[1]) / fTemp227;
		fRec31[0] = fRec27[0] - (fTemp224 * fRec31[2] + 2.0f * fTemp226 * fRec31[1]) / fTemp227;
		float fTemp228 = fTemp225 * fTemp227;
		float fTemp229 = 1.0f + (-0.618034f + fTemp199) / fTemp198;
		float fTemp230 = 1.0f + (0.618034f + fTemp199) / fTemp198;
		fRec32[0] = (fRec31[2] + (fRec31[0] - 2.0f * fRec31[1])) / fTemp228 - (fRec32[2] * fTemp229 + 2.0f * fTemp226 * fRec32[1]) / fTemp230;
		fRec33[0] = (fRec30[2] + fRec30[0] + 2.0f * fRec30[1]) / fTemp227 - (fTemp229 * fRec33[2] + 2.0f * fTemp226 * fRec33[1]) / fTemp230;
		fRec34[0] = fRec24[0] - (fTemp224 * fRec34[2] + 2.0f * fTemp226 * fRec34[1]) / fTemp227;
		fRec35[0] = (fRec34[2] + fRec34[0] + 2.0f * fRec34[1]) / fTemp227 - (fTemp229 * fRec35[2] + 2.0f * fTemp226 * fRec35[1]) / fTemp230;
		fRec36[0] = (fRec29[2] + (fRec29[0] - 2.0f * fRec29[1])) / fTemp228 - (fTemp229 * fRec36[2] + 2.0f * fTemp226 * fRec36[1]) / fTemp230;
		fRec93[0] = fSlow5 + fConst2 * fRec93[1];
		float fTemp231 = std::tan(fConst4 * fRec93[0]);
		float fTemp232 = 1.0f / fTemp231;
		float fTemp233 = 1.0f - fTemp232;
		float fTemp234 = (fRec33[2] + fRec33[0] + 2.0f * fRec33[1]) / fTemp230;
		fVec53[0] = fTemp234;
		float fTemp235 = 1.0f + fTemp232;
		fRec37[0] = -((fRec37[1] * fTemp233 - (fTemp234 + fVec53[1])) / fTemp235);
		fRec38[0] = -((fTemp233 * fRec38[1] - (fTemp234 - fVec53[1]) / fTemp231) / fTemp235);
		float fTemp236 = fTemp225 * fTemp230;
		float fTemp237 = 1.0f + (-1.618034f + fTemp232) / fTemp231;
		float fTemp238 = T_jpverb_faustpower2_f(fTemp231);
		float fTemp239 = 1.0f - 1.0f / fTemp238;
		float fTemp240 = 2.0f * fRec39[1] * fTemp239;
		float fTemp241 = 1.0f + (1.618034f + fTemp232) / fTemp231;
		fRec39[0] = (fRec32[2] + (fRec32[0] - 2.0f * fRec32[1])) / fTemp236 - (fRec39[2] * fTemp237 + fTemp240) / fTemp241;
		float fTemp242 = (fRec35[2] + fRec35[0] + 2.0f * fRec35[1]) / fTemp230;
		fVec54[0] = fTemp242;
		fRec40[0] = -((fTemp233 * fRec40[1] - (fTemp242 - fVec54[1]) / fTemp231) / fTemp235);
		fRec41[0] = -((fTemp233 * fRec41[1] - (fTemp242 + fVec54[1])) / fTemp235);
		float fTemp243 = 1.0f + (-1.618034f + fTemp232) / fTemp231;
		float fTemp244 = 1.0f + (1.618034f + fTemp232) / fTemp231;
		fRec42[0] = fRec41[0] - (fRec42[2] * fTemp243 + 2.0f * fTemp239 * fRec42[1]) / fTemp244;
		fRec43[0] = fRec40[0] - (fTemp243 * fRec43[2] + 2.0f * fTemp239 * fRec43[1]) / fTemp244;
		float fTemp245 = 1.0f + (-0.618034f + fTemp232) / fTemp231;
		float fTemp246 = 1.0f + (0.618034f + fTemp232) / fTemp231;
		fRec44[0] = (fRec42[2] + fRec42[0] + 2.0f * fRec42[1]) / fTemp244 - (fRec44[2] * fTemp245 + 2.0f * fTemp239 * fRec44[1]) / fTemp246;
		float fTemp247 = 2.0f * fTemp239 * fRec45[1];
		fRec45[0] = (fRec36[2] + (fRec36[0] - 2.0f * fRec36[1])) / fTemp236 - (fTemp237 * fRec45[2] + fTemp247) / fTemp241;
		fRec46[0] = fRec38[0] - (fTemp243 * fRec46[2] + 2.0f * fTemp239 * fRec46[1]) / fTemp244;
		fRec47[0] = fRec37[0] - (fTemp243 * fRec47[2] + 2.0f * fTemp239 * fRec47[1]) / fTemp244;
		float fTemp248 = fTemp238 * fTemp244;
		fRec48[0] = (fRec46[2] + (fRec46[0] - 2.0f * fRec46[1])) / fTemp248 - (fTemp245 * fRec48[2] + 2.0f * fTemp239 * fRec48[1]) / fTemp246;
		fRec49[0] = (fRec43[2] + (fRec43[0] - 2.0f * fRec43[1])) / fTemp248 - (fTemp245 * fRec49[2] + 2.0f * fTemp239 * fRec49[1]) / fTemp246;
		fRec94[0] = fSlow6 + fConst2 * fRec94[1];
		float fTemp249 = 1.0f - fRec94[0];
		float fTemp250 = 0.5f * fTemp0;
		fRec95[0] = fSlow7 + fConst2 * fRec95[1];
		fRec96[0] = fSlow8 + fConst2 * fRec96[1];
		fRec97[0] = fSlow9 + fConst2 * fRec97[1];
		fRec98[0] = fSlow10 + fConst2 * fRec98[1];
		float fTemp251 = std::pow(1e+01f, -(0.51f * ((-0.25f + 1.25f * fRec64[0]) / fRec98[0])));
		float fTemp252 = fTemp250 + ((fRec45[2] + (fTemp247 + fTemp237 * fRec45[0]) / fTemp241) * fRec95[0] + ((fRec44[2] + fRec44[0] + 2.0f * fRec44[1]) * fRec96[0] + (fRec49[2] + (fRec49[0] - 2.0f * fRec49[1])) * fRec97[0] / fTemp238) / fTemp246) * fTemp251;
		fVec55[IOTA0 & 1023] = fTemp252;
		fRec50[0] = fTemp249 * (fTemp67 * (fTemp68 * (fTemp69 * (0.041666668f * fTemp73 * fVec55[(faust_wrap_sub(IOTA0, iTemp72)) & 1023] - 0.16666667f * fTemp74 * fVec55[(faust_wrap_sub(IOTA0, iTemp75)) & 1023]) + 0.25f * fTemp76 * fVec55[(faust_wrap_sub(IOTA0, iTemp77)) & 1023]) - 0.16666667f * fTemp78 * fVec55[(faust_wrap_sub(IOTA0, iTemp79)) & 1023]) + 0.041666668f * fTemp80 * fVec55[(faust_wrap_sub(IOTA0, iTemp81)) & 1023]) + fRec94[0] * fRec50[1];
		fRec51[0] = (fRec47[2] + fRec47[0] + 2.0f * fRec47[1]) / fTemp244 - (fTemp245 * fRec51[2] + 2.0f * fTemp239 * fRec51[1]) / fTemp246;
		float fTemp253 = fTemp250 + fTemp251 * (fRec95[0] * (fRec39[2] + (fTemp240 + fTemp237 * fRec39[0]) / fTemp241) + (fRec96[0] * (fRec51[2] + fRec51[0] + 2.0f * fRec51[1]) + fRec97[0] * (fRec48[2] + (fRec48[0] - 2.0f * fRec48[1])) / fTemp238) / fTemp246);
		fVec56[IOTA0 & 1023] = fTemp253;
		fRec52[0] = fTemp249 * (fTemp205 * (fTemp206 * (fTemp207 * (0.041666668f * fTemp211 * fVec56[(faust_wrap_sub(IOTA0, std::min<int>(512, iTemp210))) & 1023] - 0.16666667f * fTemp212 * fVec56[(faust_wrap_sub(IOTA0, std::min<int>(512, iTemp213))) & 1023]) + 0.25f * fTemp214 * fVec56[(faust_wrap_sub(IOTA0, std::min<int>(512, iTemp215))) & 1023]) - 0.16666667f * fTemp216 * fVec56[(faust_wrap_sub(IOTA0, std::min<int>(512, iTemp217))) & 1023]) + 0.041666668f * fTemp218 * fVec56[(faust_wrap_sub(IOTA0, std::min<int>(512, iTemp219))) & 1023]) + fRec94[0] * fRec52[1];
		fRec99[0] = fSlow11 + fConst2 * fRec99[1];
		float fTemp254 = std::cos(fRec99[0]);
		float fTemp255 = std::sin(fRec99[0]);
		float fTemp256 = fRec52[0] * fTemp254 - fRec53[1] * fTemp255;
		float fTemp257 = fTemp255 * fRec54[1];
		float fTemp258 = fTemp254 * fRec50[0];
		fVec57[IOTA0 & 16383] = 0.70710677f * fTemp256 + 0.70710677f * (fTemp257 - fTemp258);
		float fTemp259 = fVec57[(faust_wrap_sub(IOTA0, iTemp127)) & 16383];
		fVec58[0] = fTemp259;
		fRec53[0] = fVec58[1] - fTemp130 * (fRec53[1] - fTemp259) / fTemp131;
		fVec59[IOTA0 & 16383] = 0.70710677f * fTemp256 + 0.70710677f * (fTemp258 - fTemp257);
		int iTemp260 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(2.4e+02f * fRec64[0]), 2047))];
		fRec100[0] = 0.9999f * (fRec100[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp260))) + 0.0001f * static_cast<float>(iTemp260);
		float fTemp261 = -1.49999f + fRec100[0];
		float fTemp262 = fVec59[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp261))))) & 16383];
		fVec60[0] = fTemp262;
		float fTemp263 = std::floor(fTemp261);
		fRec54[0] = fVec60[1] - (2.0f - fRec100[0] + fTemp263) * (fRec54[1] - fTemp262) / (fRec100[0] - fTemp263);
		float fTemp264 = fRec53[1] * fTemp254 + fTemp255 * fRec52[0];
		float fTemp265 = fTemp254 * fTemp264 - fTemp255 * fRec55[1];
		float fTemp266 = fTemp255 * fRec56[1];
		float fTemp267 = fTemp254 * fRec54[1] + fTemp255 * fRec50[0];
		float fTemp268 = fTemp254 * fTemp267;
		fVec61[IOTA0 & 16383] = 0.70710677f * fTemp265 + 0.70710677f * (fTemp266 - fTemp268);
		float fTemp269 = fVec61[(faust_wrap_sub(IOTA0, iTemp155)) & 16383];
		fVec62[0] = fTemp269;
		fRec55[0] = fVec62[1] - fTemp158 * (fRec55[1] - fTemp269) / fTemp159;
		fVec63[IOTA0 & 16383] = 0.70710677f * fTemp265 + 0.70710677f * (fTemp268 - fTemp266);
		float fTemp270 = fVec63[(faust_wrap_sub(IOTA0, iTemp139)) & 16383];
		fVec64[0] = fTemp270;
		fRec56[0] = fVec64[1] - fTemp142 * (fRec56[1] - fTemp270) / fTemp143;
		float fTemp271 = fTemp254 * fRec55[1] + fTemp255 * fTemp264;
		float fTemp272 = fTemp254 * fTemp271 - fTemp255 * fRec58[1];
		float fTemp273 = fTemp254 * fRec56[1] + fTemp255 * fTemp267;
		float fTemp274 = fTemp254 * fTemp273;
		float fTemp275 = fTemp255 * fRec57[1];
		fVec65[IOTA0 & 16383] = 0.70710677f * fTemp272 + 0.70710677f * (fTemp274 - fTemp275);
		int iTemp276 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(1.9e+02f * fRec64[0]), 2047))];
		fRec101[0] = 0.9999f * (fRec101[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp276))) + 0.0001f * static_cast<float>(iTemp276);
		float fTemp277 = -1.49999f + fRec101[0];
		float fTemp278 = fVec65[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp277))))) & 16383];
		fVec66[0] = fTemp278;
		float fTemp279 = std::floor(fTemp277);
		fRec57[0] = fVec66[1] - (2.0f - fRec101[0] + fTemp279) * (fRec57[1] - fTemp278) / (fRec101[0] - fTemp279);
		fVec67[IOTA0 & 16383] = 0.70710677f * fTemp272 + 0.70710677f * (fTemp275 - fTemp274);
		float fTemp280 = fVec67[(faust_wrap_sub(IOTA0, iTemp162)) & 16383];
		fVec68[0] = fTemp280;
		fRec58[0] = fVec68[1] - fTemp165 * (fRec58[1] - fTemp280) / fTemp166;
		float fTemp281 = fTemp254 * fRec58[1] + fTemp255 * fTemp271;
		float fTemp282 = fTemp254 * fTemp281 - fTemp255 * fRec60[1];
		float fTemp283 = fTemp254 * fRec57[1] + fTemp255 * fTemp273;
		float fTemp284 = fTemp254 * fTemp283;
		float fTemp285 = fTemp255 * fRec59[1];
		fVec69[IOTA0 & 16383] = 0.70710677f * fTemp282 + 0.70710677f * (fTemp284 - fTemp285);
		float fTemp286 = fVec69[(faust_wrap_sub(IOTA0, iTemp174)) & 16383];
		fVec70[0] = fTemp286;
		fRec59[0] = fVec70[1] - fTemp177 * (fRec59[1] - fTemp286) / fTemp178;
		fVec71[IOTA0 & 16383] = 0.70710677f * fTemp282 + 0.70710677f * (fTemp285 - fTemp284);
		int iTemp287 = itbl0T_jpverbSIG0[std::max<int>(0, std::min<int>(static_cast<int>(175.0f * fRec64[0]), 2047))];
		fRec102[0] = 0.9999f * (fRec102[1] + static_cast<float>(faust_wrap_mul(iTemp4, iTemp287))) + 0.0001f * static_cast<float>(iTemp287);
		float fTemp288 = -1.49999f + fRec102[0];
		float fTemp289 = fVec71[(faust_wrap_sub(IOTA0, std::min<int>(8192, std::max<int>(0, static_cast<int>(fTemp288))))) & 16383];
		fVec72[0] = fTemp289;
		float fTemp290 = std::floor(fTemp288);
		fRec60[0] = fVec72[1] - (2.0f - fRec102[0] + fTemp290) * (fRec60[1] - fTemp289) / (fRec102[0] - fTemp290);
		fRec61[0] = fTemp254 * fRec59[1] + fTemp255 * fTemp283;
		fRec62[0] = fTemp254 * fRec60[1] + fTemp255 * fTemp281;
		outputs[0] = static_cast<FAUSTFLOAT>(fTemp0 * (1.0f - fRec0[0]) + 0.5f * fRec0[0] * (fRec62[0] + fRec61[0]));
		iVec0[1] = iVec0[0];
		fRec0[1] = fRec0[0];
		IOTA0 = faust_wrap_add(IOTA0, 1);
		fRec64[1] = fRec64[0];
		fRec63[1] = fRec63[0];
		fVec2[1] = fVec2[0];
		fRec1[1] = fRec1[0];
		fRec65[1] = fRec65[0];
		fVec4[1] = fVec4[0];
		fRec2[1] = fRec2[0];
		fRec66[1] = fRec66[0];
		fVec6[1] = fVec6[0];
		fRec3[1] = fRec3[0];
		fRec67[1] = fRec67[0];
		fVec8[1] = fVec8[0];
		fRec4[1] = fRec4[0];
		fRec68[1] = fRec68[0];
		fVec10[1] = fVec10[0];
		fRec5[1] = fRec5[0];
		fRec69[1] = fRec69[0];
		fVec12[1] = fVec12[0];
		fRec6[1] = fRec6[0];
		fRec70[1] = fRec70[0];
		fVec14[1] = fVec14[0];
		fRec7[1] = fRec7[0];
		fRec71[1] = fRec71[0];
		fVec16[1] = fVec16[0];
		fRec8[1] = fRec8[0];
		fRec72[1] = fRec72[0];
		fVec18[1] = fVec18[0];
		fRec9[1] = fRec9[0];
		fRec73[1] = fRec73[0];
		fRec76[1] = fRec76[0];
		fRec74[1] = fRec74[0];
		fRec75[1] = fRec75[0];
		fRec77[1] = fRec77[0];
		fVec21[1] = fVec21[0];
		fRec10[1] = fRec10[0];
		fRec78[1] = fRec78[0];
		fVec23[1] = fVec23[0];
		fRec11[1] = fRec11[0];
		fRec79[1] = fRec79[0];
		fVec26[1] = fVec26[0];
		fRec12[1] = fRec12[0];
		fRec80[1] = fRec80[0];
		fVec28[1] = fVec28[0];
		fRec13[1] = fRec13[0];
		fRec81[1] = fRec81[0];
		fVec30[1] = fVec30[0];
		fRec14[1] = fRec14[0];
		fRec82[1] = fRec82[0];
		fVec32[1] = fVec32[0];
		fRec15[1] = fRec15[0];
		fRec83[1] = fRec83[0];
		fVec34[1] = fVec34[0];
		fRec16[1] = fRec16[0];
		fRec84[1] = fRec84[0];
		fVec36[1] = fVec36[0];
		fRec17[1] = fRec17[0];
		fRec85[1] = fRec85[0];
		fVec38[1] = fVec38[0];
		fRec18[1] = fRec18[0];
		fRec86[1] = fRec86[0];
		fVec40[1] = fVec40[0];
		fRec19[1] = fRec19[0];
		fRec87[1] = fRec87[0];
		fVec42[1] = fVec42[0];
		fRec20[1] = fRec20[0];
		fRec88[1] = fRec88[0];
		fVec44[1] = fVec44[0];
		fRec21[1] = fRec21[0];
		fRec89[1] = fRec89[0];
		fVec46[1] = fVec46[0];
		fRec22[1] = fRec22[0];
		fRec90[1] = fRec90[0];
		fVec49[1] = fVec49[0];
		fRec23[1] = fRec23[0];
		fRec91[1] = fRec91[0];
		fRec24[1] = fRec24[0];
		fRec92[1] = fRec92[0];
		fVec52[1] = fVec52[0];
		fRec25[1] = fRec25[0];
		fRec26[1] = fRec26[0];
		fRec27[1] = fRec27[0];
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
		fRec93[1] = fRec93[0];
		fVec53[1] = fVec53[0];
		fRec37[1] = fRec37[0];
		fRec38[1] = fRec38[0];
		fRec39[2] = fRec39[1];
		fRec39[1] = fRec39[0];
		fVec54[1] = fVec54[0];
		fRec40[1] = fRec40[0];
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
		fRec94[1] = fRec94[0];
		fRec95[1] = fRec95[0];
		fRec96[1] = fRec96[0];
		fRec97[1] = fRec97[0];
		fRec98[1] = fRec98[0];
		fRec50[1] = fRec50[0];
		fRec51[2] = fRec51[1];
		fRec51[1] = fRec51[0];
		fRec52[1] = fRec52[0];
		fRec99[1] = fRec99[0];
		fVec58[1] = fVec58[0];
		fRec53[1] = fRec53[0];
		fRec100[1] = fRec100[0];
		fVec60[1] = fVec60[0];
		fRec54[1] = fRec54[0];
		fVec62[1] = fVec62[0];
		fRec55[1] = fRec55[0];
		fVec64[1] = fVec64[0];
		fRec56[1] = fRec56[0];
		fRec101[1] = fRec101[0];
		fVec66[1] = fVec66[0];
		fRec57[1] = fRec57[0];
		fVec68[1] = fVec68[0];
		fRec58[1] = fRec58[0];
		fVec70[1] = fVec70[0];
		fRec59[1] = fRec59[0];
		fRec102[1] = fRec102[0];
		fVec72[1] = fVec72[0];
		fRec60[1] = fRec60[0];
		fRec61[1] = fRec61[0];
		fRec62[1] = fRec62[0];
	}
	
	virtual void compute(int count, FAUSTFLOAT** RESTRICT inputs, FAUSTFLOAT** RESTRICT outputs) {}

};

#endif
