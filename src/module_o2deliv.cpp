/**
 * The oxygen delivery and blood flow autoregulation module of the Guyton 1992
 * model.
 */

#include <cmath>    /* for pow(), fabs() */

#include "params.h"
#include "vars.h"
#include "utils.h"
#include "module_o2deliv.h"

/**
 * This function calculates the oxygen delivery and local blood flow control in
 * muscle and non-muscle tissue, and overall blood flow regulation on rapid,
 * intermediate and long-term timescales.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 *
 * \ingroup modules
 */
void module_o2deliv(const PARAMS &p, VARS &v) {
  v.aom = v.auo * p.o2a + 1;

  /* Oxygen consumption. */
  v.mmo = v.aom * p.omm * p.exc * (1 - pow(38.0001 - v.p2o, 3) / 54872);
  v.ovs = v.ovs + ((v.bfm * v.ova - v.rmo)
           / v.hm / 5.25 / v.bfm - v.ovs) / p.z6;

  /* Venous oxygen pressure in the muscle tissue. */
  v.pvo = 57.14 * v.ovs * pow(p.exc, p.excxp2);

  /* Oxygen pressure in the muscle tissue. */
  v.i13 = 0;
  do {
    v.rmo = (v.pvo - v.pmo) * p.pm5 * v.bfm;
    v.do2m = v.rmo - v.mmo;
    if (fabs(v.do2m) < p.z5) {
      v.i13 = 0;
      break;
    }
    /* The volume of oxygen in the muscle tissue. */
    v.qom = v.qom + v.do2m * p.i12;
    if (v.qom < 1e-4) {
      v.qom = 1e-4;
    }
    /* The oxygen pressure in the muscle tissue. */
    v.pmo = p.pk2 * v.qom;

    v.i13 = v.i13 + p.i12;
  } while (v.i13 < v.i);

  /* Calculate the resulting oxygen pressure. */
  v.p2o = v.pmo;
  if (v.p2o > 38) {
    v.p2o = 38;
  }

  /* Local blood flow control in the muscle tissue. */
  /* The oxygen pressure that drives autoregulation. */
  v.pdo = v.pmo - 38;
  v.poe = p.pom * v.pdo + 1;
  do {
    v.amm3 = v.amm1;
    v.amm1 = v.amm1 + (v.poe - v.amm1) / p.a4k * p.i20;
    v.i21 = v.i21 + p.i20;
    if (v.i21 > v.i) {
      v.i21 = 0;
      break;
    }
  } while (fabs(v.amm1 - v.amm3) > p.amm1lm);

  if (v.amm1 < p.amm4) {
    v.amm1 = p.amm4;
  }

  /* Long-term autoregulation of blood flow. */
  v.amm2 = v.amm2 + (v.pdo * p.pom2 + 1 - v.amm2) / p.a4k2 * v.i;

  /* The combined short-term and long-term autoregulation of blood flow. */
  v.amm = v.amm1 * v.amm2;

  /* Oxygen delivery to non-muscle tissue. */
  v.osv = v.osv + ((v.bfn * v.ova - v.dob)
           / v.hm / 5.25 / v.bfn - v.osv) / p.z7;
  /* Venous oxygen pressure in the non-muscle tissue. */
  v.pov = v.osv * 57.14;
  /* Oxygen consumption in non-muscle tissue. */
  v.mo2 = v.aom * p.o2m * (1 - pow(35.0001 - v.p1o, 3.) / 42875);

  /* Oxygen pressure in the non-muscle tissue. */
  v.i11 = 0;
  do {
    v.dob = (v.pov - v.pot) * 12.857 * v.bfn;
    v.do2n = v.dob - v.mo2;
    if (fabs(v.do2n) < p.z4) {
      v.i11 = 0;
      break;
    }

    /* The volume of oxygen in the non-muscle tissue. */
    if (v.qo2 < 6 and v.do2n < 0) {
      v.do2n = 0.1 * v.do2n;
    }
    v.qo2 = v.qo2 + v.do2n * p.i10;
    if (v.qo2 < 0) {
      v.qo2 = 0;
    }
    /* The oxygen pressure in the muscle tissue. */
    v.pot = v.qo2 * 0.48611;

    v.i11 = v.i11 + p.i10;
  } while (v.i11 < v.i);

  /* Calculate the resulting oxygen pressure. */
  v.p1o = v.pot;
  if (v.pot > 35) {
    v.p1o = 35;
  }

  /* Local blood flow control in the non-muscle tissue. */
  /* Rapid autoregulation of blood flow. */
  v.pod = v.pot - p.por;
  v.pob = v.pod * p.pok + 1;
  do {
    v.ar4 = v.ar1;
    v.ar1 = v.ar1 + (v.pob - v.ar1) / p.a1k * p.i16;
    v.i17 = v.i17 + p.i16;
    if (v.i17 > v.i) {
      v.i17 = 0;
      break;
    }
  } while (fabs(v.ar1 - v.ar4) > p.ar1lm);

  if (v.ar1 < 0.5) {
    v.ar1 = 0.5;
  }

  /* Blood flow autoregulation on the intermediate time scale. */
  v.poa = p.pon * v.pod + 1;
  do {
    v.ar5 = v.ar2;
    v.ar2 = v.ar2 + (v.poa - v.ar2) / p.a2k * p.i18;
    v.i19 = v.i19 + p.i18;
    if (v.i19 > v.i) {
      v.i19 = 0;
      break;
    }
  } while (fabs(v.ar2 - v.ar1) > p.ar2lm);

  if (v.ar2 < 0.5) {
    v.ar2 = 0.5;
  }

  /* Long-term blood flow autoregulation. */
  v.poc = p.poz * v.pod + 1;
  v.ar3 = v.ar3 + (v.poc - v.ar3) * v.i / p.a3k;
  if (v.ar3 < 0.3) {
    v.ar3 = 0.3;
  }

  /* Overall blood flow autoregulation. */
  v.arm = (v.ar1 * v.ar2 * v.ar3 - 1) * p.autosn + 1;
}
