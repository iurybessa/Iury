/**
 * ============================================================================
 * Name        : verify_timing.c
 * Author      : Renato Abreu
 * Version     : 1.3
 * Copyright   : Copyright by Renato Abreu - PPGEE - UFAM
 * Description : BMC verification of digital filters: Timing verification
 * ============================================================================
 */

#define OVERFLOW_MODE	3 // WRAPAROUND

#include "funcsfxp.h"

int nondet_int();
float nondet_float();

/*==========================================================================================================================
 ==================================================== TIMING VERIFICATION ==================================================
 ==========================================================================================================================*/
int main(void) {

	///////////////////////////// STATES ///////////////////////////////
	float y[xsize];
	float x[xsize];

	int i;
	for (i = 0; i < xsize; ++i) {
		y[i] = 0;
		x[i] = nondet_float();
		__ESBMC_assume(x[i] >= min && x[i] <= max);
	}

	int Nw = Na > Nb ? Na : Nb;

	float yaux[Na];
	float xaux[Nb];
	float waux[Nw];
	for (i = 0; i < Na; ++i) {
		yaux[i] = 0;
	}
	for (i = 0; i < Nb; ++i) {
		xaux[i] = 0;
	}
	for (i = 0; i < Nw; ++i) {
		waux[i] = 0;
	}

	float xk, temp;
	float *aptr, *bptr, *xptr, *yptr, *wptr;
	int j;
	for (i = 0; i < xsize; ++i) {
		///////////////////////////// FILTER DIRECT FORM I ///////////////////////////////
		#if FILTERTYPE == DIRECTFORMI
		shiftL(x[i], xaux, Nb);
		y[i] = iirOutTime(yaux, xaux, a, b, Na, Nb);
		shiftL(y[i], yaux, Na);
		#endif

		///////////////////////////// FILTER DIRECT FORM II ///////////////////////////////
		#if FILTERTYPE == DIRECTFORMII
		shiftR(0, waux, Nw);
		y[i] = iirIIOutTime(waux, x[i], a, b, Na, Nb);
		#endif

		///////////////////////////// FILTER TRANSPOSED DIRECT FORM II ///////////////////////////////
		#if FILTERTYPE == TRANSPOSEDDIRECTFORMII
		y[i] = iirIItOutTime(waux, x[i], a, b, Na, Nb);
		#endif

		///////////////////////////// FILTER DIRECT FORM I CASCADE ///////////////////////////////
		#if FILTERTYPE == DIRECTFORMICASCADE
		assert((Nw % 3) == 0 && Na == Nb); //Necessary for this implementation of cascaded filters
		xk = x[i];
		for (j = 0; j < Na; j += 3) {
			aptr = &a[j];
			bptr = &b[j];
			xptr = &xaux[j];
			yptr = &yaux[j];
			shiftL(xk, xptr, 3);
			y[i] = iirOutTime(yptr, xptr, aptr, bptr, 3, 3);
			shiftL(y[i], yptr, 3);
			xk = y[i];
		}
		#endif

		///////////////////////////// FILTER DIRECT FORM II CASCADE ///////////////////////////////
		#if FILTERTYPE == DIRECTFORMIICASCADE
		assert((Nw % 3) == 0 && Na == Nb); //Necessary for this implementation of cascaded filters
		xk = x[i];
		for (j = 0; j < Nw; j += 3) {
			aptr = &a[j];
			bptr = &b[j];
			wptr = &waux[j];
			shiftR(0, wptr, 3);
			y[i] = iirIIOutTime(wptr, xk, aptr, bptr, 3, 3);
			xk = y[i];
		}
		#endif

		///////////////////////////// FILTER TRANSPOSED DIRECT FORM II CASCADE ///////////////////////////////
		#if FILTERTYPE == TRANSPOSEDDIRECTFORMIICASCADE
		assert((Nw % 3) == 0 && Na == Nb); //Necessary for this implementation of cascaded filters
		xk = x[i];
		for (j = 0; j < Nw; j += 3) {
			aptr = &a[j];
			bptr = &b[j];
			wptr = &waux[j];
			y[i] = iirIItOutTime(wptr, xk, aptr, bptr, 3, 3);
			xk = y[i];
		}
		#endif

		///////////////////////////// FILTER DIRECT FORM I PARALLEL ///////////////////////////////
		#if FILTERTYPE == DIRECTFORMIPARALLEL
		assert((Nw % 3) == 0 && Na == Nb); //Necessary for this implementation of parallel filters
		xk = x[i];
		for (j = 0; j < Na; j += 3) {
			aptr = &a[j];
			bptr = &b[j];
			xptr = &xaux[j];
			yptr = &yaux[j];
			shiftL(xk, xptr, 3);
			temp = iirOutTime(yptr, xptr, aptr, bptr, 3, 3);
			y[i] += temp;
			shiftL(temp, yptr, 3);
		}
		#endif

		///////////////////////////// FILTER DIRECT FORM II PARALLEL ///////////////////////////////
		#if FILTERTYPE == DIRECTFORMIIPARALLEL
		assert((Nw % 3) == 0 && Na == Nb); //Necessary for this implementation of parallel filters
		xk = x[i];
		for (j = 0; j < Nw; j += 3) {
			aptr = &a[j];
			bptr = &b[j];
			wptr = &waux[j];
			shiftR(0, wptr, 3);
			y[i] += iirIIOutTime(wptr, xk, aptr, bptr, 3, 3);
		}
		#endif

		///////////////////////////// FILTER TRANSPOSED DIRECT FORM II PARALLEL ///////////////////////////////
		#if FILTERTYPE == TRANSPOSEDDIRECTFORMIIPARALLEL
		assert((Nw % 3) == 0 && Na == Nb); //Necessary for this implementation of parallel filters
		xk = x[i];
		for (j = 0; j < Nw; j += 3) {
			aptr = &a[j];
			bptr = &b[j];
			wptr = &waux[j];
			y[i] += iirIItOutTime(wptr, xk, aptr, bptr, 3, 3);
		}
		#endif
	}

	return 0;
}
