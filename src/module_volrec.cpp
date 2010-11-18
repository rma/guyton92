/**
 * The volume receptors module of the Guyton 1992 model.
 */

#include <cmath>    /* for pow(), fabs() */

#include "params.h"
#include "vars.h"
#include "utils.h"
#include "module_volrec.h"

/**
 * This function calculates the effect of volume receptors on the arterial
 * resistance and the venous volume.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 *
 * \ingroup modules
 */
void module_volrec(const PARAMS &p, VARS &v) {
  /* Volume receptor output is a function of PRA (right atrial pressure). */
  v.ahz = pow(fabs(v.pra), p.ah10);
  if (v.pra <= 0) {
    v.ahz = -v.ahz;
  }
  v.ahz = v.ahz * p.ah9;

  /* Volume receptor adaptation. */
  v.ahy = v.ahy + (v.ahz - v.ahy) / p.ah11 * v.i;
  v.ah7 = v.ahz - v.ahy;

  /* The volume receptor effect on arterial resistance. */
  v.atrrfb = v.ah7 * v.atrfbm + 1;

  /* The volume receptor effect on the unstressed venous volume. */
  v.atrvfb = v.ah7 * v.atrvm;
}
