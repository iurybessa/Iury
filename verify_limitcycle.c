/**
 * ============================================================================
 * Name        : verify_limitcycle.c
 * Author      : Renato Abreu
 * Version     : 1.3
 * Copyright   : Copyright by Renato Abreu - PPGEE - UFAM
 * Description : BMC verification of digital filters: Limit cycle verification
 * ============================================================================
 */

#ifndef OVERFLOW_MODE
#define OVERFLOW_MODE	3 // WRAPAROUND
#endif

#include "funcsfxp.h"

int nondet_int();
float nondet_float();

/*==========================================================================================================================
=================================================== LIMIT CYCLE VERIFICATION ===============================================
==========================================================================================================================*/
int main(void)
{
	int i;
	int Set_xsize_at_least_two_times_Na = 2 * Na;
	assert(xsize >= Set_xsize_at_least_two_times_Na);

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

	for (i = 0; i < xsize; ++i) {
		y[i] = 0;
		x[i] = 0;
	}

	int Nw = Na > Nb ? Na : Nb;

	fxp32_t yaux[Na];
	fxp32_t xaux[Nb];
	fxp32_t waux[Nw];

	fxp32_t y0[Na];
	fxp32_t w0[Nw];

	for (i = 0; i < Na; ++i) {
		#if FILTERTYPE == DIRECTFORMI || FILTERTYPE == DIRECTFORMICASCADE || FILTERTYPE == DIRECTFORMIPARALLEL
		yaux[i] = nondet_int();
		__ESBMC_assume(yaux[i] >= min_fxp && yaux[i] <= max_fxp);
		y0[i] = yaux[i]; //stores initial value for later comparison
		#else
		waux[i] = nondet_int();
		__ESBMC_assume(waux[i] >= min_fxp && waux[i] <= max_fxp);
		w0[i] = waux[i]; //stores initial value for later comparison
		#endif
	}

	for (i = 0; i < Nb; ++i) {
		xaux[i] = 0;
		//xaux[i] = nondet_int(); // Check if its needed. Should not be.
	}

	int j;
	int count = 0;
	int window = order(Na, Nb);
	int notzeros = 0;

	fxp32_t xk;
	fxp32_t *aptr, *bptr, *xptr, *yptr, *wptr;

	///////////////////////////// FILTER ///////////////////////////////
	for(i=0; i<xsize; ++i)
	{
		///////////////////////////// FILTER DIRECT FORM I ///////////////////////////////
		#if FILTERTYPE == DIRECTFORMI
		shiftL(x[i],xaux,Nb);
		y[i] = iirOutFixed(yaux, xaux, a_fxp, b_fxp, Na, Nb);
		shiftL(y[i],yaux,Na);
		#endif

		///////////////////////////// FILTER DIRECT FORM II ///////////////////////////////
		#if FILTERTYPE == DIRECTFORMII
		shiftR(0, waux, Nw);
		y[i] = iirIIOutFixed(waux, x[i], a_fxp, b_fxp, Na, Nb);
		#endif

		///////////////////////////// FILTER TRANSPOSED DIRECT FORM II ///////////////////////////////
		#if FILTERTYPE == TRANSPOSEDDIRECTFORMII
		y[i] = iirIItOutFixed(waux, x[i], a_fxp, b_fxp, Na, Nb);
		#endif

		///////////////////////////// FILTER DIRECT FORM I CASCADE ///////////////////////////////
		#if FILTERTYPE == DIRECTFORMICASCADE
		assert((Nw % 3) == 0 && Na == Nb); //Necessary for this implementation of cascade filters
		xk = x[i];
		for (j = 0; j < Nw; j += 3) {
			aptr = &a_fxp[j];
			bptr = &b_fxp[j];
			xptr = &xaux[j];
			yptr = &yaux[j];
			shiftL(xk, xptr, 3);
			y[i] = iirOutFixed(yptr, xptr, aptr, bptr, 3, 3);
			shiftL(y[i], yptr, 3);
			xk = y[i];
		}
		#endif

		///////////////////////////// FILTER DIRECT FORM II CASCADE ///////////////////////////////
		#if FILTERTYPE == DIRECTFORMIICASCADE
		assert((Nw % 3) == 0 && Na == Nb); //Necessary for this implementation of cascade filters
		xk = x[i];
		for (j = 0; j < Nw; j += 3) {
			aptr = &a_fxp[j];
			bptr = &b_fxp[j];
			wptr = &waux[j];
			shiftR(0, wptr, 3);
			y[i] = iirIIOutFixed(wptr, xk, aptr, bptr, 3, 3);
			xk = y[i];
		}
		#endif

		///////////////////////////// FILTER TRANSPOSED DIRECT FORM II CASCADE ///////////////////////////////
		#if FILTERTYPE == TRANSPOSEDDIRECTFORMIICASCADE
    	assert((Nw % 3) == 0 && Na == Nb); //Necessary for this implementation of cascade filters
		xk = x[i];
		for (j = 0; j < Nw; j += 3) {
			aptr = &a_fxp[j];
			bptr = &b_fxp[j];
			wptr = &waux[j];
			y[i] = iirIItOutFixed(wptr, xk, aptr, bptr, 3, 3);
			xk = y[i];
		}
		#endif

		///////////////////////////// FILTER DIRECT FORM I PARALLEL ///////////////////////////////
		#if FILTERTYPE == DIRECTFORMIPARALLEL
		assert((Nw % 3) == 0 && Na == Nb); //Necessary for this implementation of parallel filters
		xk = x[i];
		for (j = 0; j < Na; j += 3) {
			aptr = &a_fxp[j];
			bptr = &b_fxp[j];
			xptr = &xaux[j];
			yptr = &yaux[j];
			shiftL(xk, xptr, 3);
			temp = iirOutFixed(yptr, xptr, aptr, bptr, 3, 3);
			y[i] += temp;
			shiftL(temp, yptr, 3);
		}
		#endif

		///////////////////////////// FILTER DIRECT FORM II PARALLEL ///////////////////////////////
		#if FILTERTYPE == DIRECTFORMIIPARALLEL
		assert((Nw % 3) == 0 && Na == Nb); //Necessary for this implementation of parallel filters
		xk = x[i];
		for (j = 0; j < Nw; j += 3) {
			aptr = &a_fxp[j];
			bptr = &b_fxp[j];
			wptr = &waux[j];
			shiftR(0, wptr, 3);
			y[i] += iirIIOutFixed(wptr, xk, aptr, bptr, 3, 3);
		}
		#endif

		///////////////////////////// FILTER TRANSPOSED DIRECT FORM II PARALLEL ///////////////////////////////
		#if FILTERTYPE == TRANSPOSEDDIRECTFORMIIPARALLEL
    	assert((Nw % 3) == 0 && Na == Nb); //Necessary for this implementation of parallel filters
		xk = x[i];
		for (j = 0; j < Nw; j += 3) {
			aptr = &a_fxp[j];
			bptr = &b_fxp[j];
			wptr = &waux[j];
			y[i] += iirIItOutFixed(wptr, xk, aptr, bptr, 3, 3);
		}
		#endif

		//// Verify if previous states of y repeats (method 2)
		#if FILTERTYPE == DIRECTFORMI || FILTERTYPE == DIRECTFORMICASCADE || FILTERTYPE == DIRECTFORMIPARALLEL
		for (j = Na - 1; j >= 0; --j) {
			if (yaux[j] == y0[j]) {
				++count;
			}
			if (yaux[j] != 0) {
				++notzeros;
			}
		}
		if (notzeros != 0) {
			assert(count < Na);
		}
		#else
		for (j = Nw - 1; j >= 0; --j) {
			if (waux[j] == w0[j]) {
				++count;
			}
			if (waux[j] != 0) {
				++notzeros;
			}
		}
		if (notzeros != 0) {
			assert(count < Nw);
		}
		#endif

		count = 0;
		notzeros = 0;
	}

	return 0;
}
