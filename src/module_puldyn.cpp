/**
 * The pulmonary fluid dynamics module of the Guyton 1992 model.
 */

#include <cmath>    /* for pow(), fabs() */

#include "params.h"
#include "vars.h"
#include "utils.h"
#include "module_puldyn.h"

/**
 * This function calculates the effect of pulmonary fluid dynamics on the
 * tissue fluid volume, plasma volume and plasma protein concentration.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 *
 * \ingroup modules
 */
void module_puldyn(const PARAMS &p, VARS &v) {
  v.pcp = (v.ppa - v.pla) * v.rpv / (v.rpv + v.rpa) + v.pla;
  v.ppi = 2 - 0.15 / v.vpf;
  v.cpn = v.ppr / v.vpf;
  v.pos = v.cpn * 0.4;
  v.plf = (v.ppi + 11) * 3e-4;

  /* Lung lymphatic protein flow. */
  v.ppo = v.plf * v.cpn;
  v.ppn = (v.cpp - v.cpn) * 0.000225;
  v.ppz = v.ppn - v.ppo;
  v.ppd = v.ppd + (v.ppz - v.ppd) / p.z18;
  if (v.ppr + v.ppd * v.i - 0.025 < 0) {
    v.ppd = (0.025 - v.ppr) / v.i;
  }

  /* Starling forces at the capillary. */
  v.pfi = (v.pcp - v.ppi + v.pos - v.ppc) * p.cpf;
  v.dfz = v.pfi - v.plf;
  v.dfp = v.dfp + (v.dfz - v.dfp) / p.z19;
  if (v.vpf + v.dfp * v.i - 1e-3 < 0) {
    v.dfp = (1e-3 - v.vpf) / v.i;
  }

  /* Pulmonary fluid and protein. */
  v.vpf = v.vpf + v.dfp * v.i;
  v.ppr = v.ppr + v.ppd * v.i;

  /* Hypertrophy of the heart. */
  v.hpl = v.hpl + (pow((v.pa * v.qao / 500 / p.hsl), p.z13) - v.hpl)
           * v.i / 57600;
  v.hpr = v.hpr + (pow((v.ppa * v.qao / 75 / p.hsr), p.z13) - v.hpr)
           * v.i / 57600;
}
