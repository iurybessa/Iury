/*
 * magfxp.h
 *
 *  Created on: Jul 29, 2013
 *      Author: rbabreu
 */

#ifndef MAGFXP_H_
#define MAGFXP_H_

#include "fxp.h"
#include "funcsfxp.h"

#define M_PI     3.14159265358979323846

/*==========================================================================================================================
================================================== MAGNITUDE VERIFICATION ==================================================
==========================================================================================================================*/


/*
 * Function to create phase response from transfer function
 */
void freqz_phase(double* num, int lnum, double* den, int lden, double* res, int N) {
	double w;
	int m, i;
	double out_r[N + 1], old_out_r;
	double out_i[N + 1];
	double out_denr[N + 1], out_deni[N + 1];
	for (w = 0, i = 0; w <= M_PI; w += M_PI / N, ++i) {
		out_r[i] = num[0];
		out_i[i] = 0;
		for (m = 1; m < lnum; ++m) {
			old_out_r = out_r[i];
			out_r[i] = cos(w) * out_r[i] - sin(w) * out_i[i] + num[m];
			out_i[i] = sin(w) * old_out_r + cos(w) * out_i[i];
		}

		out_denr[i] = den[0];
		out_deni[i] = 0;
		for (m = 1; m < lden; ++m) {
			old_out_r = out_denr[i];
			out_denr[i] = cos(w) * out_denr[i] - sin(w) * out_deni[i] + den[m];
			out_deni[i] = sin(w) * old_out_r + cos(w) * out_deni[i];
		}

		res[i] = atan2(out_i[i], out_r[i]); //numerator abs
		res[i] = res[i] - atan2(out_deni[i], out_denr[i]); //den abs
	}
}

/*
 * Verifies Phase response
 */
void verifyPhaseResp(double *res, double *fi, double dif, int N) {

	int i;
	for (i = 0; i < N; ++i) {
		assert(fabs(res[i] - fi[i]) <= dif);
	}
}

/*
 * Function to create magnitude response from transfer function
 */
void freqz_mag(double* num, int lnum, double* den, int lden, double* res, int N) {
	double w;
	int m, i;
	double out_r[N + 1], old_out_r;
	double out_i[N + 1];
	double out_denr[N + 1], out_deni[N + 1];
	for (w = 0, i = 0; w <= M_PI; w += M_PI / N, ++i) {
		out_r[i] = num[0];
		out_i[i] = 0;
		for (m = 1; m < lnum; ++m) {
			old_out_r = out_r[i];
			out_r[i] = cos(w) * out_r[i] - sin(w) * out_i[i] + num[m];
			out_i[i] = sin(w) * old_out_r + cos(w) * out_i[i];
		}
		out_denr[i] = den[0];
		out_deni[i] = 0;

		for (m = 1; m < lden; ++m) {
			old_out_r = out_denr[i];
			out_denr[i] = cos(w) * out_denr[i] - sin(w) * out_deni[i] + den[m];
			out_deni[i] = sin(w) * old_out_r + cos(w) * out_deni[i];
		}

		res[i] = sqrt(out_r[i] * out_r[i] + out_i[i] * out_i[i]); //numerator abs
		res[i] = res[i] / sqrt(out_denr[i] * out_denr[i] + out_deni[i] * out_deni[i]); //den abs

//		assert(res[i]>= 0.000015259f && res[i]<= 2147483647.0f);
//		res[i] = 20*fxp_log10(res[i]);
	}
}

/*
 * Function to create complex frequency response from transfer function
 */
void freqz_complex(double* num, int lnum, double* den, int lden, double* real, double* imag, int N) {
	double w;
	int m, i;
	double out_r[N + 1], old_out_r;
	double out_i[N + 1];
	double out_denr[N + 1], out_deni[N + 1];
	double d;
	for (w = 0, i = 0; w <= M_PI; w += M_PI / N, ++i) {
		out_r[i] = num[0];
		out_i[i] = 0;
		for (m = 1; m < lnum; ++m) {
			old_out_r = out_r[i];
			out_r[i] = cos(w) * out_r[i] - sin(w) * out_i[i] + num[m];
			out_i[i] = sin(w) * old_out_r + cos(w) * out_i[i];
		}
		out_denr[i] = den[0];
		out_deni[i] = 0;

		for (m = 1; m < lden; ++m) {
			old_out_r = out_denr[i];
			out_denr[i] = cos(w) * out_denr[i] - sin(w) * out_deni[i] + den[m];
			out_deni[i] = sin(w) * old_out_r + cos(w) * out_deni[i];
		}

		d = out_denr[i] * out_denr[i] + out_deni[i] * out_deni[i];
		real[i] = (out_r[i] * out_denr[i] + out_i[i] * out_deni[i]) / d;
		imag[i] = (out_i[i] * out_denr[i] - out_r[i] * out_deni[i]) / d;
	}
}

/*
 * Verifies Magnitude response of the filter
 */
void verifyLimits(double *res, FilterProperties prop, int N) {

	int i;
	int j;
	double w;
	double w_incr = 1.0 / N;

	for (j = 0, i = 0, w = 0; (w <= 1.0); ++i, w += w_incr) {
		if (w > prop.frequencies[j] && w <= 1.0) {
			++j;
		}
		if (prop.profile[j] == 0 && w <= 1.0) {
			assert(res[i] <= prop.gains[j]);
		} else if (w <= 1.0) {
			assert(res[i] >= prop.gains[j]);
		}
	}
}

/*
 * Verifies Magnitude response of the filter like firpm defined amplitudes
 */
void verifyBands(double *res, FilterProperties prop, int N) {

	int i;
	int j;
	double w;
	double w_incr = 1.0 / N;

	for (j = 1, i = 0, w = 0; (w <= 1.0); ++i, w += w_incr) {
		if (w > prop.frequencies[j] && w <= 1.0) {
			j += 2;
		}
		if (w > prop.frequencies[j - 1] && w <= 1.0) {
			if (prop.profile[j] == 0) {
				assert(res[i] <= prop.gains[j]);
			} else if (prop.profile[j] == 1) {
				assert(res[i] >= prop.gains[j]);
			}
		}
	}
}

/*
 * Verifies Magnitude response of the filter
 */
//void verifyMagresp(double *res, FilterProperties prop, int N) {
//
//	int i;
//	double w;
//	double w_incr = 1.0 / N;
//	if (prop.type == lowpass) {
//		for (i = 0, w = 0; (w <= 1.0); ++i, w += w_incr) {
//			if (w <= prop.wp) {
//				assert(res[i] >= prop.Ap);
//			} else if (w == prop.wc) {
//				assert(res[i] <= prop.Ac);
//			} else if ((w >= prop.wr) && (w <= 1)) {
//				assert(res[i] <= prop.Ar);
//			}
//		}
//	} else if (prop.type == highpass) {
//		for (i = 0, w = 0; (w <= 1.0); ++i, w += w_incr) {
//			if (w <= prop.wr) {
//				assert(res[i] <= prop.Ar);
//			} else if (w == prop.wc) {
//				assert(res[i] <= prop.Ac);
//			} else if ((w > prop.wp) && (w <= 1)) {
//				assert(res[i] >= prop.Ap);
//			}
//		}
//	} else {
//		assert(0);//Filter type not supported
//	}
//}

#endif /* MAGFXP_H_ */
