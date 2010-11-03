/**
 * The special circulation variables module of the Guyton 1992 model.
 */

#include <cmath>    /* for pow(), fabs() */

#include "params.h"
#include "vars.h"
#include "utils.h"
#include "module_special.h"

/**
 * This function calculates special circulation variables, which are only used
 * for displaying output. They are \b not used as input to any other blocks in
 * the model.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 */
void module_special(const PARAMS &p, VARS &v) {
  /* Mean circulatory pressures. */
  v.pmc = (v.vae + v.vve + v.vre + v.vpe + v.vle) / 0.11;
  v.pms = (v.vae + v.vve + v.vre) / 0.09375;
  v.pmp = (v.vpe + v.vle) / 0.01625;

  /* Heart rate. */
  v.hr = (72 * v.aur + pow(v.pr1, 0.5) * 5) * ((v.hmd - 1) * 0.5 + 1);

  /* Total peripheral resistance. */
  v.rtp = (v.pa - v.pra) / v.qao;

  /* Stroke volume. */
  v.svo = v.qlo / v.hr;
}
