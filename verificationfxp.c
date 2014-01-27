/**
 * ============================================================================
 * Name        : verificationfxp.c
 * Author      : Renato Abreu
 * Version     : 1.2
 * Copyright   : Copyright by Renato Abreu
 * Description : BMC verification of digital filters
 * ============================================================================
 */


#include "funcsfxp.h"
#include "magfxp.h"

//#include <time.h>
//#include <stdlib.h>

//#include <assert.h>
//#include <stdio.h>

int nondet_int();
float nondet_float();

/*==========================================================================================================================
 ==================================================== OVERFLOW CHECKING =====================================================
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
		__ESBMC_assume(x[i] >= min_fxp && x[i] <= max_fxp);
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

/*==========================================================================================================================
=================================================== LIMIT CYCLE CHECKING ===================================================
==========================================================================================================================*/
int limitCycle(void)
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

		//// VERIFICANDO SE ESTADOS ANTERIORES DE Y SE REPETEM (METODO 2)
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

	//assert(0);
	return 0;
}

/*==========================================================================================================================
======================================================== SNR CHECKING ======================================================
==========================================================================================================================*/

int errorCheck(void) {

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
//	xsize = 10;
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
		__ESBMC_assume(x[i] >= min_fxp && x[i] <= max_fxp);// && (x[i] <= -10 || x[i] >= 10)
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

		y[i] = iirOutFixedL(yaux, xaux, x[i], a_fxp, b_fxp, Na, Nb);
		yf[i] = iirOutFloatL(yfaux, xfaux, xf[i], a, b, Na, Nb);

//		shiftL(x[i], xaux, Nb);
//		y[i] = iirOutFixed(yaux, xaux, a_fxp, b_fxp, Na, Nb);
//		shiftL(y[i], yaux, Na);
//		shiftLfloat(xf[i], xfaux, Nb);
//		yf[i] = iirOutFloat(yfaux, xfaux, a, b, Na, Nb);
//		shiftLfloat(yf[i], yfaux, Na);

//		shiftLboth(xf[i], xfaux, x[i], xaux, Nb);
//		y[i] = iirOutFixed(yaux, xaux, a_fxp, b_fxp, Na, Nb);
//		yf[i] = iirOutFloat(yfaux, xfaux, a, b, Na, Nb);
//		shiftLboth(yf[i], yfaux, y[i], yaux, Na);

//		shiftLboth(xf[i], xfaux, x[i], xaux, Nb);
//		iirOutBoth(yfaux, xfaux, a, b, &sumf, yaux, xaux, a_fxp, b_fxp, &sum, Na, Nb);
//		shiftLboth(yf[i], yfaux, y[i], yaux, Na);
//		y[i] = sum;
//		yf[i] = sumf;

//		error[i] = iirOutBothL2(yfaux, xfaux, a, b, xf[i], yaux, xaux, a_fxp, b_fxp, x[i], Na, Nb);

//		error[i] = iirOutBothL(yfaux, xfaux, a, b, xf[i], yaux, xaux, a_fxp, b_fxp, x[i], Na, Nb);

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

		assert(error[i] > -0.2f && error[i] < 0.2f);
	}
//	assert(0);
//	assert(error[i] < 1.1f);
//	snrvalue = snrPower(yf, error, xsize);
//	assert(snrvalue == 0.0f);

	return 0;
}

/*==========================================================================================================================
================================================ MAGNITUDE / PHASE CHECKING ================================================
==========================================================================================================================*/

int magnitude() {
	int Npoints = 100;
	double res[Npoints + 1];
	int i, j;
	double aa[Na];
	double bb[Nb];

	for (i = 0; i < Nb; i++) {
		bb[i] = fxp_to_float(fxp_float_to_fxp(b[i]));
	}
	for (i = 0; i < Na; i++) {
		aa[i] = fxp_to_float((fxp_float_to_fxp(a[i])));
	}

	#if FILTERTYPE == DIRECTFORMICASCADE || FILTERTYPE == DIRECTFORMIICASCADE || FILTERTYPE == TRANSPOSEDDIRECTFORMIICASCADE
	assert((Na % 3) == 0 && Na == Nb); //Necessary for this implementation of parallel filters
	double* aptr;
	double* bptr;
	double restemp[Npoints + 1];
	for (j = 0; j < Npoints + 1; ++j) {
		res[j] = 1;
	}
	for (i = 0; i < Na; i += 3) {
		aptr = &aa[i];
		bptr = &bb[i];
		freqz_mag(bptr, 3, aptr, 3, restemp, Npoints);
		for (j = 0; j < Npoints + 1; ++j) {
			res[j] *= restemp[j];
		}
	}
	#elif FILTERTYPE == DIRECTFORMIPARALLEL || FILTERTYPE == DIRECTFORMIIPARALLEL || FILTERTYPE == TRANSPOSEDDIRECTFORMIIPARALLEL
	double* aptr;
	double* bptr;
	double real[Npoints + 1];
	double imag[Npoints + 1];
	double realtemp[Npoints + 1];
	double imagtemp[Npoints + 1];
	for (j = 0; j < Npoints + 1; ++j) {
		real[j] = 0;
		imag[j] = 0;
	}
	for (i = 0; i < Na; i += 3) {
		aptr = &aa[i];
		bptr = &bb[i];
		freqz_complex(bptr, 3, aptr, 3, realtemp, imagtemp, Npoints);
		for (j = 0; j < Npoints + 1; ++j) {
			real[j] += realtemp[j];
			imag[j] += imagtemp[j];
		}
	}
	for (j = 0; j < Npoints + 1; ++j) {
		res[j] = sqrt(real[j] * real[j] + imag[j] * imag[j]);
	}

	#else
	freqz_mag(bb, Nb, aa, Na, res, Npoints);
	#endif

//	verifyMagresp(res, filter, Npoints);
//	verifyLimits(res, filter, Npoints);
	verifyBands(res, filter, Npoints);
	return 0;

}

int phase() {
	int Npoints = 100;
	float res[Npoints + 1];
	float resf[Npoints + 1];
	int i, j;
	float aa[Na];
	float bb[Nb];
	float dif = 0.3f;

	for (i = 0; i < Nb; i++) {
		bb[i] = fxp_to_float(fxp_float_to_fxp(b[i]));
	}
	for (i = 0; i < Na; i++) {
		aa[i] = fxp_to_float((fxp_float_to_fxp(a[i])));
	}

	#if FILTERTYPE == DIRECTFORMICASCADE || FILTERTYPE == DIRECTFORMIICASCADE || FILTERTYPE == TRANSPOSEDDIRECTFORMIICASCADE
	assert((Na % 3) == 0 && Na == Nb); //Necessary for this implementation of parallel filters
	double* aptr;
	double* bptr;
	double* aptrf;
	double* bptrf;
	double restemp[Npoints + 1];
	double restempf[Npoints + 1];
	for (j = 0; j < Npoints + 1; ++j) {
		res[j] = 0;
		resf[j] = 0;
	}
	for (i = 0; i < Na; i += 3) {
		aptr = &aa[i];
		bptr = &bb[i];
		aptrf = &a[i];
		bptrf = &b[i];
		freqz_phase(bptr, 3, aptr, 3, restemp, Npoints);
		freqz_phase(bptrf, 3, aptrf, 3, restempf, Npoints);
		for (j = 0; j < Npoints + 1; ++j) {
			res[j] += restemp[j];
			resf[j] += restempf[j];
		}
	}
	#elif FILTERTYPE == DIRECTFORMIPARALLEL || FILTERTYPE == DIRECTFORMIIPARALLEL || FILTERTYPE == TRANSPOSEDDIRECTFORMIIPARALLEL
	double* aptr;
	double* bptr;
	double real[Npoints + 1];
	double imag[Npoints + 1];
	double realtemp[Npoints + 1];
	double imagtemp[Npoints + 1];
	double* aptrf;
	double* bptrf;
	double realf[Npoints + 1];
	double imagf[Npoints + 1];
	double realtempf[Npoints + 1];
	double imagtempf[Npoints + 1];
	for (j = 0; j < Npoints + 1; ++j) {
		real[j] = 0;
		imag[j] = 0;
		realf[j] = 0;
		imagf[j] = 0;
	}
	for (i = 0; i < Na; i += 3) {
		aptr = &aa[i];
		bptr = &bb[i];
		aptrf = &a[i];
		bptrf = &b[i];
		freqz_complex(bptr, 3, aptr, 3, realtemp, imagtemp, Npoints);
		freqz_complex(bptrf, 3, aptrf, 3, realtempf, imagtempf, Npoints);
		for (j = 0; j < Npoints + 1; ++j) {
			real[j] += realtemp[j];
			imag[j] += imagtemp[j];
			realf[j] += realtempf[j];
			imagf[j] += imagtempf[j];
		}
	}
	for (j = 0; j < Npoints + 1; ++j) {
		res[j] = atan2(imag[j], real[j]);
		resf[j] = atan2(imagf[j], realf[j]);
	}
	#else
	freqz_phase(b, Nb, a, Na, res, Npoints);
	freqz_phase(bb, Nb, aa, Na, resf, Npoints);
	#endif

	verifyPhaseResp(res, resf, dif, Npoints);
	return 0;
}



