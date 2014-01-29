/*
 * verificationfxp.h
 *
 *  Created on: May 15, 2013
 *      Author: Renato Abreu
 */

#ifndef VERIFICATIONFXP_H_
#define VERIFICATIONFXP_H_

#define DIRECTFORMI						0
#define DIRECTFORMII					1
#define TRANSPOSEDDIRECTFORMII			2
#define DIRECTFORMICASCADE				3
#define DIRECTFORMIICASCADE				4
#define TRANSPOSEDDIRECTFORMIICASCADE	5
#define DIRECTFORMIPARALLEL				6
#define DIRECTFORMIIPARALLEL			7
#define TRANSPOSEDDIRECTFORMIIPARALLEL	8

#define CLOCK		16000000
#define CYCLE		1/CLOCK
#define SAMPLERATE 	100
#define DEADLINE 	1/SAMPLERATE
#define OVERHEAD	0				//Initialize considering known overhead (time for data conditioning, DAC, ADC, etc...)

#ifndef TESTCASE
#define TESTCASE	1
#endif

typedef struct {
	double gains[20];			// Gains
	double frequencies[20];		// Cut off frequencies
	int profile[20];			// Profile in pairs: 1 for pass, 0 for rejection
} FilterProperties;

/*==========================================================================================================================
======================================================= PARAMETERS =========================================================
==========================================================================================================================*/

//// 1 - Coefficients of a filter with limit cycle, sum|H|=2.0
#if TESTCASE == 1
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	4
#define FXP_IWIDTH		2
float a[] = { 1.0000f, 0.5f };
float b[] = { 1.0000f };
int Na = 2; int Nb = 1;
float max = 1.0f; float min = -1.0f;
int xsize = 6;
FilterProperties filter = {
	{ 0.9, 0.9, 1.5, 1.5},	/* Gains for test bands like firpm */
	{ 0.0, 0.5, 0.8, 1.0},	/* Frequencies */
	{ 0, 0, 1, 1}			/* Profile */
};
#endif

//// 2 - Low Pass from paper, sum|H|=1.2
#if TESTCASE == 2
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	5
#define FXP_IWIDTH		2
float a[] = { 1.0000f, -0.375f, 0.1875f };
float b[] = { 0.21875f, 0.40625f, 0.21875f };
int Na = 3; int Nb = 3;
float max = 1.6f; float min = -1.6f;
int xsize = 6;
FilterProperties filter = {
	{ 0.9, 0.9, 0.3, 0.3},		/* Gains for test bands like firpm */
	{ 0.0, 0.3, 0.6, 1.0},		/* Frequencies */
	{ 1, 1, 0, 0}				/* Profile */
};
#endif

//// 3 - Coefficients of a filter with limit cycle, sum|H|=4.0
#if TESTCASE == 3
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	4
#define FXP_IWIDTH		3
float a[] = { 1.0000f, -0.875f, 0.125f };
float b[] = { 1.0000f };
int Na = 3; int Nb = 1;
float max = 1.0f; float min = -1.0f;
int xsize = 6;
FilterProperties filter = {
	{ 2.0, 2.0, 1.0, 1.0},		/* Gains for test bands like firpm */
	{ 0.0, 0.19, 0.42, 1.0},	/* Frequencies */
	{ 1, 1, 0, 0}				/* Profile */
};
#endif

//// 4 - Coefficients of a filter WITHOUT limit cycle, sum|H|=1.56
#if TESTCASE == 4
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	4
#define FXP_IWIDTH		2
float a[] = { 1.0000f, -0.4375f, 0.125f };
float b[] = { 1.0000f };
int Na = 3; int Nb = 1;
float max = 1.0f; float min = -1.0f;
int xsize = 6;
FilterProperties filter = {
	{ 1.4, 1.4, 0.9, 0.9},		/* Gains for test bands like firpm */
	{ 0.0, 0.24, 0.60, 1.0},	/* Frequencies */
	{ 1, 1, 0, 0}				/* Profile */
};
#endif

////// 5 - Chebyshev I High Pass from Matlab Fs 48000 Fc 14400, sum|H|=1.33
#if TESTCASE == 5
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	6
#define FXP_IWIDTH		2
float a[] = { 1.0000000000f, 0.351350980988748f, 0.329655910298801f };
float b[] = { 0.244576232327513f, -0.489152464655027f, 0.244576232327513f };
int Na = 3; int Nb = 3;
float max = 1.0f; float min = -1.0f;
int xsize = 6;
FilterProperties filter = {
	{ 0.33, 0.33, 0.90, 0.90},		/* Gains for test bands like firpm */
	{ 0.0, 0.4, 0.60, 1.0},			/* Frequencies */
	{ 0, 0, 1, 1}					/* Profile */
};
#endif

////// 6 - Elliptic Band Pass from Matlab Fs 48000 Fc 9600-12000, sum|H|=1.24
#if TESTCASE == 6
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	8
#define FXP_IWIDTH		2
float a[] = { 1.000000000000000f, -0.241575701689109f, 0.525248952448298f };
float b[] = { 0.237375523775851f, 0.0f, -0.237375523775851f };
int Na = 3; int Nb = 3;
float max = 1.0f; float min = -1.0f;
int xsize = 6;
FilterProperties filter = {
	{ 0.4, 0.4, 0.7, 0.7, 0.4, 0.4},		/* Gains for test bands like firpm */
	{ 0.0, 0.26, 0.36, 0.54, 0.66, 1.0},	/* Frequencies */
	{ 0, 0, 1, 1, 0, 0}						/* Profile */
};
#endif

//// 7 - Butterworth Band Stop from Matlab Fs 48000 Fc 7000-10000, sum|H|=1.81 \o/
#if TESTCASE == 7
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	8
#define FXP_IWIDTH		2
float a[] = { 1.00000000f, -0.69921875f, 0.49218750f };
float b[] = { 0.74609375f, -0.69921875f, 0.74609375f };
int Na = 3; int Nb = 3;
float max = 1.0f; float min = -1.0f;
int xsize = 6;
FilterProperties filter = {
	{ 0.8, 0.8, 0.4, 0.4, 0.8, 0.8},		/* Gains for test bands like firpm */
	{ 0.0, 0.22, 0.30, 0.39, 0.5, 1.0},		/* Frequencies */
	{ 1, 1, 0, 0, 1, 1}						/* Profile */
};
#endif

//////////////////////////////////////// ALTO TEMPO DE VERIFICACAO //////////////////////////////////////////

//// 8 - Elliptic Band Pass from Matlab Fs 48000 Fc 10800, sum|H|=0.91
#if TESTCASE == 8
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	7
#define FXP_IWIDTH		1
float a[] = { 1.00000000f, -1.58593750f, 1.99609375f, -1.27343750f, 0.65234375f};
float b[] = { 0.03125000f, 0.00000000f, -0.06250000f, 0.00000000f, 0.03125000f};
int Na = 5; int Nb = 5;
float max = 1.1f; float min = -1.1f;
int xsize = 10;
FilterProperties filter = {
	{ 0.1, 0.1, 0.35, 0.35, 0.1, 0.1},		/* Gains for test bands like firpm */
	{ 0.0, 0.17, 0.23, 0.48, 0.57, 1.0},	/* Frequencies */
	{ 0, 0, 1, 1, 0, 0}						/* Profile */
};
#endif

//// 9 - Butterworth High Pass from Matlab Fs 48000 Fc 10800, sum|H|=1.58
#if TESTCASE == 9
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	6
#define FXP_IWIDTH		2
float a[] = {1.000000000000000f, -0.390641453194462f, 0.534300637154232f, -0.084233712203843f, 0.020651424506044f};
float b[] = {0.126864201691161f, -0.507456806764645f, 0.761185210146968f, -0.507456806764645f, 0.126864201691161f};
int Na = 5; int Nb = 5;
float max = 1.27f; float min = -1.27f;
int xsize = 10;
FilterProperties filter = {
	{ 0.2, 0.2, 0.7, 0.7},			/* Gains for test bands like firpm */
	{ 0.0, 0.33, 0.45, 1.0},		/* Frequencies */
	{ 0, 0, 1, 1}					/* Profile */
};
#endif

//// 10 - Chebyshev I Band Pass from Matlab Fs 48000 Fc 9600-12000, sum|H|=1.51
#if TESTCASE == 10
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	6
#define FXP_IWIDTH		2
float a[] = { 1.000000000000000f, -0.573117207765464f, 1.702032541572362f, -0.481442227311725f, 0.710593476651196f };
float b[] = { 0.020515223631715f, 0.0f, -0.041030447263430f, 0.0f, 0.020515223631715f };
int Na = 5; int Nb = 5;
float max = 1.33f; float min = -1.33f;
int xsize = 10;
FilterProperties filter = {
	{ 0.1, 0.1, 0.88, 0.88, 0.1, 0.1},		/* Gains for test bands like firpm */
	{ 0.0, 0.3, 0.4, 0.5, 0.57, 1.0},	/* Frequencies */
	{ 0, 0, 1, 1, 0, 0}						/* Profile */
};
#endif

//// 11 - Elliptic High Pass from Matlab Fs 48000 Fc 14400, sum|H|=5.39
#if TESTCASE == 11
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	11
#define FXP_IWIDTH		3
float a[] = { 1.000000000000000f, 2.537154002498975f, 4.083133799339507f, 4.163778804114947f, 2.906399348238815f, 1.288372363868242f, 0.299861461786204f };
float b[] = { 0.008423211547291f, -0.030384175350277f, 0.058847504429943f, -0.072145211272913f, 0.058847504429943f, -0.030384175350277f, 0.008423211547291f };
int Na = 7; int Nb = 7;
float max = 1.0f; float min = -1.0f;
int xsize = 14;
FilterProperties filter = {
	{ 0.05, 0.05, 0.8, 0.8},		/* Gains for test bands like firpm */
	{ 0.0, 0.51, 0.583, 1.0},		/* Frequencies */
	{ 0, 0, 1, 1}					/* Profile */
};
#endif

//// 12 - FIR Low Pass from Matlab using kaiser window Fs 48000 Fc 10800, sum|H|=1.9375
#if TESTCASE == 12
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	6
#define FXP_IWIDTH		2
float a[] = { 1.0f };
float b[] = { 0.0137079388484213f, 0.0169394570890696f, -0.0103136271666561f, -0.0235678897859865f, 0.00425573905386702f, 0.0300974131159918f, 0.00525859149464849f, -0.0361330341634925f, -0.0197927122387306f, 0.0412945571144922f,
			0.0434404009140227f, -0.0452473910798497f, -0.0916252050185741f, 0.0477302040786194f, 0.305359910707667f, 0.437191294072981f, 0.305359910707667f, 0.0477302040786194f, -0.0916252050185741f, -0.0452473910798497f, 0.0434404009140227f,
			0.0412945571144922f, -0.0197927122387306f, -0.0361330341634925f, 0.00525859149464849f, 0.0300974131159918f, 0.00425573905386702f, -0.0235678897859865f, -0.0103136271666561f, 0.0169394570890696f, 0.0137079388484213f };
int Na = 1; int Nb = 31;
float max = 1.0f; float min = -1.0f;
int xsize = 31;
FilterProperties filter = {
	{ 0.8, 0.8, 0.1, 0.1},		/* Gains for test bands like firpm */
	{ 0.0, 0.425, 0.48, 1.0},	/* Frequencies */
	{ 1, 1, 0, 0}				/* Profile */
};
#endif

//// 13 - Multiband FIR filter, sum|H|=2.18, f=[0 0.3 0.4 0.6 0.7 0.8 0.9 1]; a = [1 1 0 0 1 1 0 0]; b = firpm(8, f, a);
#if TESTCASE == 13
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	6
#define FXP_IWIDTH		2
float a[] = { 1.0f };
/*float b[] = { 0.00978545312597438f, 0.00651468756258551f, 0.000651551500697511f, 0.0303939038823036f, -0.0174645904700498f, -0.0221494977363414f, -0.0223843969220214f, 0.0553486198561296f, -0.00635759097047748f, 0.0141992808478103f, 0.0399966139901125f, -0.215324565008561f, 0.105571349540255f, 0.126788710716182f, 0.140201610205510f, 0.553804146610691f, 0.140201610205510f, 0.126788710716182f, 0.105571349540255f, -0.215324565008561f, 0.0399966139901125f, 0.0141992808478103f, -0.00635759097047748f, 0.0553486198561296f, -0.0223843969220214f, -0.0221494977363414f, -0.0174645904700498f, 0.0303939038823036f, 0.000651551500697511f, 0.00651468756258551f, 0.00978545312597438f };
float b[] = { 0.0133789607622602f, 0.0375074023139045f, -0.203471892840909f, 0.118294100357927f, 0.158775739486505f, 0.188321471858416f, 0.562634385184289f, 0.188321471858416f, 0.158775739486505f, 0.118294100357927f, -0.203471892840909f, 0.0375074023139045f, 0.0133789607622602f };*/
float b[] = { -0.238140072400548f, 0.0797808589004223f, 0.131364759099746f, 0.157489622900086f, 0.533493382724967f, 0.157489622900086f, 0.131364759099746f, 0.0797808589004223f, -0.238140072400548f };
int Na = 1; int Nb = 9;
float max = 1.0f; float min = -1.0f;
int xsize = 13;
FilterProperties filter = {
	{ 0.75, 0.75, 0.23, 0.23, 0.75, 0.75, 0.21, 0.21 },		/* Gains for test bands like firpm */
	{ 0.0, 0.3, 0.4, 0.6, 0.7, 0.8, 0.9, 1.0 },				/* Frequencies */
	{ 1, 1, 0, 0, 1, 1, 0, 0}								/* Profile */
};
#endif

//// 14 - Multiband FIR filter, sum|H|=1.469, f = [0 0.10 0.15 0.20 0.25 0.65 0.70 0.75 0.80 1]; a = [0 0 1 1 0 0 1 1 0 0]; b = firpm(24, f, a);
#if TESTCASE == 14
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	8
#define FXP_IWIDTH		2
float a[] = { 1.0f };
float b[] = { 0.0121425835102669f, 0.118699082274855f, -0.00688793986334231f, -0.00286008859689086f, 0.0306952282182189f, -0.137159834262563f, -0.0439208808245963f, -0.0575697401407547f, -0.136662286183202f, 0.0585048004955666f, 0.0260338588779149f, 0.00876343397600830f, 0.189570138217693f, 0.00876343397600830f, 0.0260338588779149f, 0.0585048004955666f, -0.136662286183202f, -0.0575697401407547f, -0.0439208808245963f, -0.137159834262563f, 0.0306952282182189f, -0.00286008859689086f, -0.00688793986334231f, 0.118699082274855f, 0.0121425835102669f };
int Na = 1; int Nb = 25;
float max = 1.0f; float min = -1.0f;
int xsize = 25;
FilterProperties filter = {
	{ 0.15, 0.15, 0.9, 0.9, 0.15, 0.15, 0.9, 0.9, 0.15, 0.15 },		/* Gains for test bands like firpm */
	{ 0.0, 0.1, 0.156, 0.196, 0.25, 0.65, 0.705, 0.744, 0.8, 1.0 },				/* Frequencies */
	{ 0, 0, 1, 1, 0, 0, 1, 1, 0, 0}								/* Profile */
};
#endif

//// 15 - Low Pass FIR, wc= 0.33, Front-end Wifi filter, sum|H|=2.921
#if TESTCASE == 15
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	5
#define FXP_IWIDTH		3
float b[] = { -0.00396202000000000f, -0.0110273200000000f, 0.0f, 0.0321608200000000f, 0.0379508900000000f, -0.0307010500000000f, -0.118927800000000f, -0.0689446200000000f, 0.208826800000000f, 0.575020400000000f, 0.746044600000000f, 0.575020400000000f, 0.208826800000000f, -0.0689446200000000f, -0.118927800000000f, -0.0307010500000000f, 0.0379508900000000f, 0.0321608200000000f, 0.0f, -0.0110273200000000f, -0.00396202000000000f };
float a[] = { 1.0f };
int Na = 1; int Nb = 21;
float max = 1.0f; float min = -1.0f;
int xsize = 21;
FilterProperties filter = {
	{ 1.98, 1.98, 0.01, 0.01 },		/* Gains for test bands like firpm */
	{ 0.0, 0.24, 0.51, 1.0 },		/* Frequencies */
	{ 1, 1, 0, 0}					/* Profile */
};
#endif

//// 16 - Highpass Cascaded TDFII, wc = 0.44, sum|H|=12.427
#if TESTCASE == 16
#ifndef FILTERTYPE
#define FILTERTYPE	TRANSPOSEDDIRECTFORMIICASCADE
#endif
#define FXP_PRECISION	5
#define FXP_IWIDTH		5
float b[] = { 1.0f, -2.0f, 1.0f, 1.0f, -2.0f, 1.0f };
float a[] = { 1.0f, -0.22705f, 0.45140f, 1.0f, -0.16359f, 0.04574f };
int Na = 6; int Nb = 6;
float max = 1.0f; float min = -1.0f;
int xsize = 14;
FilterProperties filter = {
	{ 0.45, 0.45, 6.74, 6.74 },		/* Gains for test bands like firpm */
	{ 0.0, 0.25, 0.5, 1.0 },		/* Frequencies */
	{ 0, 0, 1, 1 }					/* Profile */
};
#endif

//// 17 - Band Stop Cascaded DFII Elliptic, sum|H|=2.445
#if TESTCASE == 17
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMIICASCADE
#endif
#define FXP_PRECISION	5
#define FXP_IWIDTH		3
float b[] = { 0.526195030091236f, -0.347857040753543f, 0.526195030091236f, 0.526195030091236f, -0.0119786467226284f, 0.526195030091236f, 1.40904660202421f, -0.494774045802189f, 1.40904660202421f };
float a[] = { 1.0f,	-1.01652516752320f,	0.820696448152777f, 1.0f, 0.482434768772751f, 0.794790792715654f, 1.0f, -0.198042664444506f, 0.127995074757485f };
int Na = 9; int Nb = 9;
float max = 1.0f; float min = -1.0f;
int xsize = 19;
FilterProperties filter = {
	{ 0.9, 0.9, 0.05, 0.05, 0.9, 0.9},		/* Gains for test bands like firpm */
	{ 0.0, 0.302, 0.372, 0.518, 0.597, 1.0},/* Frequencies */
	{ 1, 1, 0, 0, 1, 1}						/* Profile */
};
#endif

//// 18 - Parallel DFII filter lowpass, Proakis pg 593, sum|H|=16.58
#if TESTCASE == 18
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMIIPARALLEL
#endif
#define FXP_PRECISION	5
#define FXP_IWIDTH		5
float b[] = { -0.07375f, -0.0645f,	0.0f, 1.225f, 1.341f, 0.0f };
float a[] = { 1.0f, -0.875f, 0.09375f, 1.0f, -1.0f, 0.5f };
int Na = 6; int Nb = 6;
float max = 1.0f; float min = -1.0f;
int xsize = 13;
FilterProperties filter = {
	{ 4.2, 4.2, 1.4, 1.4},		/* Gains for test bands like firpm */
	{ 0.0, 0.33, 0.52, 1.0},	/* Frequencies */
	{ 1, 1, 0, 0}				/* Profile */
};
#endif

//// 19 - Cascade DFII filter lowpass, Proakis pg 593, sum|H|=7.64
#if TESTCASE == 19
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMIICASCADE
#endif
#define FXP_PRECISION	4
#define FXP_IWIDTH		4
float b[] = { 1.0f, -0.666667f, 0.0f, 1.0f, 1.5f, -1.0f };
float a[] = { 1.0f, -0.875f, 0.09375f, 1.0f, -1.0f, 0.5f };
int Na = 6; int Nb = 6;
float max = 1.0f; float min = -1.0f;
int xsize = 13;
FilterProperties filter = {
	{ 4.4, 4.4, 2.0, 2.0},		/* Gains for test bands like firpm */
	{ 0.0, 0.33, 0.52, 1.0},	/* Frequencies */
	{ 1, 1, 0, 0}				/* Profile */
};
#endif

//// 20 - https://ccrma.stanford.edu/~jos/fp/Parallel_Second_Order_Signal_Flow.html , sum|H|=2.7475
#if TESTCASE == 20
#ifndef FILTERTYPE
#define FILTERTYPE	TRANSPOSEDDIRECTFORMIIPARALLEL
#endif
#define FXP_PRECISION	4
#define FXP_IWIDTH		4
float b[] = { 0.1657f, 0.0f, 0.0f, 0.3788f, -0.2413f, 0.0f, 0.4555f, 0.0922f, 0.0f };
float a[] = { 1.0f, 0.9f, 0.0f, 1.0f, -1.4562f, 0.81f, 1.0f, 0.5562f, 0.81f };
int Na = 9; int Nb = 9;
float max = 1.0f; float min = -1.0f;
int xsize = 19;
FilterProperties filter = {
	{ 1.0, 1.0, 2.0, 2.0, 0.9, 0.9, 2.2, 2.2, 1.0, 1.0, 1.6, 1.6 },				/* Gains for test bands like firpm */
	{ 0.0, 0.1, 0.18, 0.21, 0.28, 0.5, 0.58, 0.62, 0.7, 0.92, 0.97, 1.0 },	/* Frequencies */
	{ 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1 }										/* Profile */
};
#endif


//// 21 Ball and Beam Digital Controller I
#if TESTCASE == 21
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	4
#define FXP_IWIDTH		3
//float a[] = {0.2f,  -0.4f,   0.2f};
//x10⁶
float a[] = {0.3f,  0.1f,   0.8f};
float b[] = {2.0f,  0.0f,   0.6f};
int Na = 3; int Nb = 3;
float max = 1.0f; float min = -1.0f;
int xsize = 10;
#endif

//// 22 Ball and Beam Digital Controller II
#if TESTCASE == 22
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	4
#define FXP_IWIDTH		3
float a[] = {2.0f,  -4.0f,   2.0f};
//x10⁶
//float a[] = {0.3f,  0.1f,   0.8f};
float b[] = {1.0f,  0.0f,   0.25f};
int Na = 3; int Nb = 3;
float max = 1.0f; float min = -1.0f;
int xsize = 10;
#endif

//// 23 Ball and Beam Digital Controller III
#if TESTCASE == 23
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	4
#define FXP_IWIDTH		3
float a[] = {0.2f,  -0.4f,   0.2f};
float b[] = {1.0f,  0.0f,   0.25f};
int Na = 3; int Nb = 3;
float max = 1.0f; float min = -1.0f;
int xsize = 10;
#endif

//// 24 Ball and Beam Digital Controller III - Closed Loop
#if TESTCASE == 24
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	4
#define FXP_IWIDTH		3
float a[] = {0.002f,  0.0166f,   -0.02f, -0.0162f, 0.016f, 0.0016f};
float b[] = {1.0f,  -3.704f,   4.862f, -2.186f, -0.574f, 0.7781f, -0.176f};
int Na = 6; int Nb = 7;
float max = 1.0f; float min = -1.0f;
int xsize = 10;
#endif

//// 25 Ball and Beam Digital Controller IV - Proj Emulaçao
#if TESTCASE == 25
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	4
#define FXP_IWIDTH		3
float a[] = {0.1f,  -2.819f,   2.637f, -0.8187f};
float b[] = {1.0f,  -2.574f,   2.181f, -0.6068f};
int Na = 4; int Nb = 4;
float max = 1.0f; float min = -1.0f;
int xsize = 10;

// ganho 50
#endif

//// 26 Ball and Beam Digital Controller V - Ragazzanni
#if TESTCASE == 26
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	4
#define FXP_IWIDTH		3
float a[] = {0.9439f,  -3.607f,   5.237f, -3.506f, 1.01f, -0.07822f};
float b[] = {10.07f,  91.76f,   -22.63f, -87.17f, 6.615f, 1.364f};
int Na = 6; int Nb = 6;
float max = 1.0f; float min = -1.0f;
int xsize = 10;
/// ganho e09
#endif

//// 27 Ball and Beam Digital Controller V - Ragazzanni
#if TESTCASE == 27
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	6
#define FXP_IWIDTH		2
float a[] = {0.09439f,  -0.3607f,   0.5237f, -0.3506f, 0.101f, -0.007822f};
float b[] = {10.07f,  91.76f,   -22.63f, -87.17f, 6.615f, 1.364f};
int Na = 6; int Nb = 6;
float max = 1.0f; float min = -1.0f;
int xsize = 10;
/// ganho e10
#endif

//// 28 Ball and Beam Digital Controller IV - Proj Emulaçao
#if TESTCASE == 28
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	5
#define FXP_IWIDTH		2
float a[] = {0.01f,  -0.2819f,   0.2637f, -0.08187f};
float b[] = {1.0f,  -2.574f,   2.181f, -0.6068f};
int Na = 4; int Nb = 4;
float max = 1.0f; float min = -1.0f;
int xsize = 10;

// ganho 500
#endif

//// 29 Ball and Beam Digital Controller IV - Proj Emulaçao
#if TESTCASE == 29
#ifndef FILTERTYPE
#define FILTERTYPE	DIRECTFORMI
#endif
#define FXP_PRECISION	5
#define FXP_IWIDTH		2
float a[] = {0.01f,  -0.28f,   0.26f, -0.08f};
float b[] = {1.0f,  -2.57f,   2.18f, -0.60f};
int Na = 4; int Nb = 4;
float max = 1.0f; float min = -1.0f;
int xsize = 10;

//ganho 500
#endif


#endif /* VERIFICATIONFXP_H_ */
