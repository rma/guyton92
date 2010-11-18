/**
 * The angiotensin module of the Guyton 1992 model.
 */

#include <cmath>    /* for pow(), fabs() */

#include "params.h"
#include "vars.h"
#include "utils.h"
#include "module_angio.h"

/**
 * This function calculates the effect of angiotensin on the venous volume
 * (VVE), various arterial and venous resistances (R1, RVS, ANMAR, ANMER),
 * aldosterone secretion (AMRBSC), salt intake (ANMSML) and renal sodium
 * reabsorption (DTNANG).
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 *
 * \ingroup modules
 */
void module_angio(const PARAMS &p, VARS &v) {
  v.mdflw3 = v.mdflw3 + (v.mdflw - v.mdflw3) * p.mdflwx;
  if (v.mdflw3 > 1) {
    v.angscr = 1 / (1 + (v.mdflw3 - 1) * 72);
  }
  if (v.mdflw3 <= 1) {
    v.angscr = 10 - 9 / (1 + (1 - v.mdflw3) * 8);
  }

  v.anx = (v.angscr - 1) * p.anxm;
  v.anx1 = v.anx1 + (v.anx - v.anx1) / p.anv * v.i;

  v.anpr = (v.angscr + v.anx1) * p.rek;
  if (v.anpr < 1e-5) {
    v.anpr = 1e-5;
  }

  v.anpr1 = v.anpr + p.anginf;
  if (p.angkns > 0) {
    v.anpr1 = p.angkns;
  }

  v.anc = v.anc + (v.anpr1 - v.anc)
           * (1 - 1 / pow(2.7183, v.i / p.ant / p.z12));
  v.anm = p.anmul - (p.anmul - 1) / ((p.anmll - 1)
           / (p.anmll - p.anmul) * (v.anc - 1) * p.ancsns + 1);

  v.anu = (v.anm - 1) * p.anum + 1;
  if (v.anu < p.anull) {
    v.anu = p.anull;
  }

  v.anuvn = (v.anu - 1) * p.anuvm + 1;
}
