/**
 * ============================================================================
 * Name        : verify_stability.c
 * Author      : Renato Abreu
 * Version     : 1.3
 * Copyright   : Copyright by Renato Abreu - PPGEE - UFAM
 * Description : BMC verification of digital filters: Poles/Zeros verification
 * ============================================================================
 */

#include "funcsfxp.h"
//#include<assert.h>

/*==========================================================================================================================
 ==================================================== POLES/ZEROS VERIFICATION =============================================
 ==========================================================================================================================*/
int main(void) {

	float aa[Na];
	float bb[Nb];

	int i;

	for (i = 0; i < Nb; i++) {
		bb[i] = fxp_to_float(fxp_float_to_fxp(b[i]));
	}
	for (i = 0; i < Na; i++) {
		aa[i] = fxp_to_float((fxp_float_to_fxp(a[i])));
	}

	assert(check_stability(a, b));
	return 0;
}
