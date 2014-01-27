/**
 * ============================================================================
 * Name        : verify_freqz.c
 * Author      : Renato Abreu
 * Version     : 1.3
 * Copyright   : Copyright by Renato Abreu - PPGEE - UFAM
 * Description : BMC verification of digital filters: Frequency response
 * ============================================================================
 */

#define OVERFLOW_MODE	2 // SATURATE

#include "funcsfxp.h"
#include "magfxp.h"

int nondet_int();
float nondet_float();

/*==========================================================================================================================
================================================ MAGNITUDE / PHASE VERIFICATION ============================================
==========================================================================================================================*/

int main(void) {
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
	double res[Npoints + 1];
	double resf[Npoints + 1];
	int i, j;
	double aa[Na];
	double bb[Nb];
	double ad[Na];
	double bd[Nb];
	double dif = 0.3;

	for (i = 0; i < Nb; i++) {
		bb[i] = fxp_to_float(fxp_float_to_fxp(b[i]));
		bd[i] = b[i];
	}
	for (i = 0; i < Na; i++) {
		aa[i] = fxp_to_float((fxp_float_to_fxp(a[i])));
		ad[i] = a[i];
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
