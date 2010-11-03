/**
 * The antidiuretic hormone (ADH) module of the Guyton 1992 model.
 */

#include <cmath>    /* for pow(), fabs() */

#include "params.h"
#include "vars.h"
#include "utils.h"
#include "module_adh.h"

/**
 * This function calculates the effect of ADH on vascular resistance, thirst
 * drive, and sodium reabsorption and water excretion in the kidney.
 * resistance and the venous volume.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 */
void module_adh(const PARAMS &p, VARS &v) {
  /* The influence of osmoreceptors. */
  v.adhna = (v.cna - p.cnr) / (142 - p.cnr);
  if (v.adhna < 0) {
    v.adhna = 0;
  }
  /* Stimulus provided by the arterial pressure. */
  v.adhpa = v.pa1;
  if (v.adhpa > p.adhpul) {
    v.adhpa = p.adhpul;
  }

  /* The production of ADH. */
  v.adhpr = pow(p.adhpul - v.adhpa, 2) * p.adhpam;
  v.adh = v.adhna + v.adhpr + p.adhinf;
  if (v.adh < 0) {
    v.adh = 0;
  }

  v.adhc = v.adhc + (v.adh - v.adhc)
            * (1 - 1 / pow(2.7183, v.i / p.adhtc / p.z16));

  /* The effect of ADH on the kidneys. */
  v.adhz = p.adhkll - p.adhkul;
  v.adhmk = p.adhkul - (p.adhkul - 1)
             / ((p.adhkll - 1) / v.adhz * (v.adhc - 1) + 1);
  if (v.adhmk < p.adhkll) {
    v.adhmk = p.adhkll;
  }

  /* The effect of ADH on vascular resistance. */
  v.adhz = p.adhvll - p.adhvul;
  v.adhmv = p.adhvul - (p.adhvul - 1)
             / ((p.adhvll - 1) / v.adhz * (v.adhc - 1) + 1);
  if (v.adhmv < p.adhvll) {
    v.adhmv = p.adhvll;
  }
}
