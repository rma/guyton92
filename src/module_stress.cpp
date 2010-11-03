/**
 * The stress relaxation module of the Guyton 1992 model.
 */

#include <cmath>    /* for pow(), fabs() */

#include "params.h"
#include "vars.h"
#include "utils.h"
#include "module_stress.h"

/**
 * This function calculates the effect of stress relaxation on the venous
 * volume.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 */
void module_stress(const PARAMS &p, const VARS &v) {
  *v.vv6 = *v.vv6 + ((*v.vve - 0.74) * *p.sr2 - *v.vv6) / *p.srk2 * *v.i;

  *v.vv7 = *v.vv7 + ((*v.vve - 0.74) * *p.sr - *v.vv7)
           * (1 - 1 / pow(2.7183, *v.i / *p.srk));
}
