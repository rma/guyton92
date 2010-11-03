/**
 * The atrial natriuretic peptide (ANP) module of the Guyton 1992 model.
 */

#include <cmath>    /* for pow(), fabs() */

#include "params.h"
#include "vars.h"
#include "utils.h"
#include "module_anp.h"

/**
 * This function calculates the effect of ANP on the resistance of the afferent
 * arteriole.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 */
void module_anp(const PARAMS &p, VARS &v) {
  /* ANP in the left atrium. */
  v.anpl = v.pla - 1;
  if (v.anpl < 0) {
    v.anpl = 0;
  }

  /* ANP is more sensitive to the right atrium (PRA) than the left atrium. */
  v.anpr2 = (v.pra + 1) * 2;
  if (v.anpr2 < 0) {
    v.anpr2 = 0;
  }

  v.anp = (v.anpl + v.anpr2) / 3;
  v.anp1 = v.anp + v.anpinf;
  if (v.anpkns > 0) {
    v.anp1 = v.anpkns;
  }

  v.anpc = v.anpc + (v.anp1 - v.anpc)
            * (1 - 1 / pow(2.7183, v.i / p.anptc / p.z14));

  /* The effect of ANP on the resistance of the afferent arteriole. */
  v.anpx = p.anpxul - p.anpxul / (0.5555556 * (1 + v.anpc));
  if (v.anpx < -1) {
    v.anpx = -1;
  }
}
