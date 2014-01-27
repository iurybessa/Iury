/**
 * ============================================================================
 * Name        : verify_overflow.c
 * Author      : Renato Abreu
 * Version     : 1.3
 * Copyright   : Copyright by Renato Abreu - PPGEE - UFAM
 * Description : BMC verification of digital filters: Overflow verification
 * ============================================================================
 */

#define OVERFLOW_MODE	1 // DETECT_OVERFLOW

#include "funcsfxp.h"

int nondet_int();
float nondet_float();

/*==========================================================================================================================
 ==================================================== OVERFLOW VERIFICATION ================================================
 ==========================================================================================================================*/
int main(void) {

	fxp32_t a_fxp[Na];
	fxp32_t b_fxp[Nb];
	fxp32_t min_fxp;
	fxp32_t max_fxp;

	// Quantizing coefficients
	fxp_float_to_fxp_array(a, a_fxp, Na); //floatToFix(a[i], delta, minFix, maxFix, NOWRAP);
	fxp_float_to_fxp_array(b, b_fxp, Nb); //b[i] = floatToFix(b[i], delta, minFix, maxFix, NOWRAP);
	min_fxp = fxp_float_to_fxp(min);
	max_fxp = fxp_float_to_fxp(max);

	///////////////////////////// STATES ///////////////////////////////

	fxp32_t y[xsize]; //float y[xsize];
	fxp32_t x[xsize]; //float x[xsize];
	//fxp32_t x[6] = {fxp_neg(_fxp_one), _fxp_one, fxp_neg(_fxp_one), _fxp_one, fxp_neg(_fxp_one), _fxp_one};
	//fxp32_t x[6] = {_fxp_one, _fxp_one, _fxp_one, _fxp_one, _fxp_one, _fxp_one};

	int i;
	for (i = 0; i < xsize; ++i) {
		y[i] = 0;
		x[i] = nondet_int();
		__ESBMC_assume(x[i] >= min_fxp && x[i] <= max_fxp);	//outside limits
		__ESBMC_assume(x[i] <= min_fxp + 2 || x[i] >= max_fxp - 2);	//inside limits
	}

	int Nw = Na > Nb ? Na : Nb;

	fxp32_t yaux[Na];
	fxp32_t xaux[Nb];
	fxp32_t waux[Nw];
	for (i = 0; i < Na; ++i) {
		yaux[i] = 0;
	}
	for (i = 0; i < Nb; ++i) {
		xaux[i] = 0;
	}
	for (i = 0; i < Nw; ++i) {
		waux[i] = 0;
	}

	fxp32_t xk, temp;
	fxp32_t *aptr, *bptr, *xptr, *yptr, *wptr;
	int j;
	for (i = 0; i < xsize; ++i) {
		///////////////////////////// FILTER DIRECT FORM I ///////////////////////////////
		#if FILTERTYPE == DIRECTFORMI
		shiftL(x[i], xaux, Nb);
		y[i] = iirOutFixed(yaux, xaux, a_fxp, b_fxp, Na, Nb);
		shiftL(y[i], yaux, Na);
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
		assert((Nw % 3) == 0 && Na == Nb); //Necessary for this implementation of cascaded filters
		xk = x[i];
		for (j = 0; j < Na; j += 3) {
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
		assert((Nw % 3) == 0 && Na == Nb); //Necessary for this implementation of cascaded filters
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
		assert((Nw % 3) == 0 && Na == Nb); //Necessary for this implementation of cascaded filters
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
	}

	return 0;
}
