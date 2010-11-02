/**
 * The aldosterone module of the Guyton 1992 model.
 */

#include <cmath>    /* for pow(), fabs() */

#include "params.h"
#include "vars.h"
#include "utils.h"
#include "module_aldost.h"

/**
 * This function calculates the effect of aldosterone on the transport of
 * sodium and potassium (AMNA and AMK, respectively).
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 */
void module_aldost(const PARAMS &p, const VARS &v) {
  /* Aldosterone secretion. */
  *v.amrbsc = ((*v.anm - 1) * *p.anmald + 1) * 0.909 * (*v.cke - 3.3);

  *v.amr = (*v.amrbsc - 1) * *p.amkmul + 1;
  if (*v.amr < 0) {
    *v.amr = 0.;
  }

  *v.amr1 = *v.amr + *p.aldinf;
  if (*p.aldkns > 0) {
    *v.amr1 = *p.aldkns;
  }

  *v.amc = *v.amc + (*v.amr1 - *v.amc) * (1 - 1 / pow(2.7183, *v.i / *p.amt));
  *v.am1 = *p.am1ul - (*p.am1ul - 1) / ((*p.am1ll - 1)
           / (*p.am1ll - *p.am1ul) * (*v.amc - 1) * *p.amcsns + 1);
  *v.am = (*v.am1 - 1) * *p.aldmm + 1;

  /* The effect of aldosterone on potassium transport. */
  *v.amk = (*v.am - 1) * *p.amkm + 1;
  if (*v.amk < 0.2) {
    *v.amk = 0.2;
  }

  /* The effect of aldosterone on sodium transport. */
  *v.amna = (*v.am - 1) * *p.amnam + 1;
  if (*v.amna < *p.amnall) {
    *v.amna = *p.amnall;
  }
  if (*v.amna > *p.amnaul) {
    *v.amna = *p.amnaul;
  }
}
