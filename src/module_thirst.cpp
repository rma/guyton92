/**
 * The thirst drive module of the Guyton 1992 model.
 */

#include <cmath>    /* for pow(), fabs() */

#include "params.h"
#include "vars.h"
#include "utils.h"
#include "module_thirst.h"

/**
 * This function calculates the thirst drive, which affects the salt intake
 * drive and total body water.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 */
void module_thirst(const PARAMS &p, const VARS &v) {
  *v.anmsml = (*v.anm - 1) * *p.anmslt + 1;

  *v.sth = pow(*p.z10 - *v.pot, 2) * *p.z11 * *v.anmsml;
  if (*v.sth < 0.8) {
    *v.sth = 0.8;
  }
  if (*v.sth > 8) {
    *v.sth = 8;
  }

  *v.ahth = ((*v.adhc - 1) * *p.ahthm + 1) * *v.sth * 1e-3;
  if (*v.ahth < 0) {
    *v.ahth = 0;
  }

  /* The thirst drive. */
  *v.anmth = (*v.anm - 1) * *p.anmtm * 1e-3;
  *v.tvz = *v.anmth + *v.ahth;
  if (*v.tvz < 0) {
    *v.tvz = 0;
  }

  *v.tvd = *v.tvd + (*v.tvz + *p.dr - *v.tvd) / *p.tvddl * *v.i;
}
