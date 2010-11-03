/**
 * The baroreceptor adaptation module of the Guyton 1992 model.
 */

#include <cmath>    /* for pow(), fabs() */

#include "params.h"
#include "vars.h"
#include "utils.h"
#include "module_baro.h"

/**
 * This function calculates the adaptation of the baroreceptors.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 */
void module_baro(const PARAMS &p, VARS &v) {
  v.au2 = v.au6 - 1;
  v.au8 = p.auk * v.au2;
  v.au4 = v.au4 + v.au8 * v.i;

  v.dhm = (v.pot - 10) * p.dhdtr;
  v.hmd = v.hmd + v.dhm * v.i;
  if (v.hmd > 1) {
    v.hmd = 1;
  }
}
