/**
 * ============================================================================
 * Name        : verify_error.c
 * Author      : Renato Abreu
 * Version     : 1.3
 * Copyright   : Copyright by Renato Abreu - PPGEE - UFAM
 * Description : BMC verification of digital filters: Error verification
 * ============================================================================
 */

#ifndef OVERFLOW_MODE
#define OVERFLOW_MODE	2 // SATURATE
#endif

#include "funcsfxp.h"

int nondet_int();
float nondet_float();

/*==========================================================================================================================
======================================================== ERROR VERIFICATION ================================================
==========================================================================================================================*/

int main(void) {

	float errorref = (float)scale_factor_inv[FXP_PRECISION-1];

	fxp32_t a_fxp[Na];
	fxp32_t b_fxp[Nb];
	fxp32_t min_fxp;
	fxp32_t max_fxp;

	// Quantizing coefficients
	fxp_float_to_fxp_array(a, a_fxp, Na);
	fxp_float_to_fxp_array(b, b_fxp, Nb);
	min_fxp = fxp_float_to_fxp(min);
	max_fxp = fxp_float_to_fxp(max);

	///////////////////////////// STATES ///////////////////////////////
	fxp32_t y[xsize];
	fxp32_t x[xsize];
	float yf[xsize];
	float xf[xsize];
	float error[xsize]; // float output minus fxp output
	float snrvalue;

	int i;

	int Nw = Na > Nb ? Na : Nb;

	fxp32_t yaux[Na];
	fxp32_t xaux[Nb];
	fxp32_t waux[Nw];

	float yfaux[Na];
	float xfaux[Nb];
	float wfaux[Nw];

	fxp32_t xk, temp;
	fxp32_t *aptr, *bptr, *xptr, *yptr, *wptr;

	float xkf, tempf;
	float *afptr, *bfptr, *xfptr, *yfptr, *wfptr;

	fxp32_t sum;
	float sumf;

	for (i = 0; i < Na; ++i) {
		yaux[i] = 0;
		yfaux[i] = 0;
	}
	for (i = 0; i < Nb; ++i) {
		xaux[i] = 0;
		xfaux[i] = 0;
	}
	for (i = 0; i < Nw; ++i) {
		waux[i] = 0;
		wfaux[i] = 0;
	}

	for (i = 0; i < xsize; ++i) {
		// Sorting in integer
		y[i] = 0;
		x[i] = nondet_int();
		__ESBMC_assume(x[i] == -1 || x[i] == 1 || x[i] == min_fxp || x[i] == max_fxp || x[i] == 0);
		//__ESBMC_assume(x[i] >= min_fxp && x[i] <= max_fxp);// && (x[i] <= -10 || x[i] >= 10)
		yf[i] = 0.0f;
		xf[i] = fxp_to_float(x[i]);

		//Sorting in float
//		yf[i] = 0.0f;
//		xf[i] = nondet_float();
//		__ESBMC_assume(xf[i] >= min && xf[i] <= max); //&& (xf[i] <= -0.8125f || xf[i] >= 0.8125f)
//		y[i] = 0;
//		x[i] = fxp_float_to_fxp(xf[i]);

		// Getting rand input
//		yf[i] = 0.0f;
//		xf[i] = rand_uni[i];
//		xf[i] *= max; //scale rand input
//		y[i] = 0;
//		x[i] = fxp_float_to_fxp(xf[i]);

//		y[i] = 0;
//		x[i] = fxp_float_to_fxp(rand_uni[i]);
//		yf[i] = 0.0f;
//		xf[i] = fxp_to_float(x[i]);
//		//xf[i] *= max; //scale rand input
	}

	int j;
	for (i = 0; i < xsize; ++i) {
		///////////////////////////// FILTER DIRECT FORM I ///////////////////////////////
		#if FILTERTYPE == DIRECTFORMI
		shiftL(x[i], xaux, Nb);
		y[i] = iirOutFixed(yaux, xaux, a_fxp, b_fxp, Na, Nb);
		shiftL(y[i], yaux, Na);
		shiftLfloat(xf[i], xfaux, Nb);
		yf[i] = iirOutFloat(yfaux, xfaux, a, b, Na, Nb);
		shiftLfloat(yf[i], yfaux, Na);
		#endif

		///////////////////////////// FILTER DIRECT FORM II ///////////////////////////////
		#if FILTERTYPE == DIRECTFORMII
		shiftRboth(0.0f, wfaux, 0, waux, Nw);
		y[i] = iirIIOutFixed(waux, x[i], a_fxp, b_fxp, Na, Nb);
		yf[i] = iirIIOutFloat(wfaux, xf[i], a, b, Na, Nb);
		#endif

		///////////////////////////// FILTER TRANSPOSED DIRECT FORM II ///////////////////////////////
		#if FILTERTYPE == TRANSPOSEDDIRECTFORMII
		y[i] = iirIItOutFixed(waux, x[i], a_fxp, b_fxp, Na, Nb);
		yf[i] = iirIItOutFloat(wfaux, xf[i], a, b, Na, Nb);
		#endif

		error[i] = fxp_to_float(y[i]) - yf[i];

		assert(error[i] > -errorref && error[i] < errorref);
	}

//	snrvalue = snrPower(yf, error, xsize);
//	assert(snrvalue == 0.0f);

	return 0;
}
