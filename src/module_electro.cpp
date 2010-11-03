/**
 * The electrolytes module of the Guyton 1992 model.
 */

#include <cmath>    /* for pow(), fabs() */

#include "params.h"
#include "vars.h"
#include "utils.h"
#include "module_electro.h"

/**
 * This function calculates the extracellular sodium and potassium
 * concentrations, the extracellular fluid volume and total body water.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 */
void module_electro(const PARAMS &p, VARS &v) {
  /* Sodium balance. */
  v.ned = p.nid * v.sth - v.nod + p.trpl * 142;
  v.nae = v.nae + v.ned * v.i;
  v.amk1 = (v.amk - 1) * p.alclk + 1;

  /* Potassium balance. */
  v.ktotd = p.kid - v.kod;
  v.ktot = v.ktot + v.ktotd * v.i;

  /* Water balance. */
  v.vtw = v.vtw + (v.tvd - v.vud) * v.i;

  v.i15 = 0;
  do {
    v.ke = (v.ktot - 3000) / v.amk1 / 9.3333;
    v.ki = v.ktot - v.ke;

    /* The extracellular fluid volume. */
    v.vec = v.vtw - v.vic;

    /* The extracellular sodium concentration. */
    v.cna = v.nae / v.vec;

    v.cki = v.ki / v.vic;
    v.ccd = v.cki - v.cna;
    v.vid = v.ccd * p.vidml;

    /* The intracellular fluid volume. */
    v.vic = v.vic + v.vid * p.i14;

    if (fabs(v.vid) < p.vid1) {
      v.i15 = 0;
      break;
    }

    v.i15 = v.i15 + p.i14;
  } while (v.i15 < v.i);

  /* The extracellular potassium concentration. */
  v.cke = v.ke / v.vec;
}
