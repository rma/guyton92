/**
 * The red blood cell and viscosity module of the Guyton 1992 model.
 */

#include <cmath>    /* for pow(), fabs() */

#include "params.h"
#include "vars.h"
#include "utils.h"
#include "module_rbc.h"

/**
 * This function calculates the blood viscosity, red blood cell formation and
 * destruction, and pulmonary oxygen uptake.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 */
void module_rbc(const PARAMS &p, VARS &v) {
  v.vb = v.vp + v.vrc;

  /* The hematocrit of the blood. */
  v.hm1 = v.vrc / v.vb;
  v.hm = 100 * v.hm1;

  /* Blood viscosity due to the red blood cells. */
  v.vie = v.hm / (p.hmk - v.hm) / v.hkm;
  v.vib = v.vie + 1.5;
  v.vim = 0.3333 * v.vib;

  /* Red blood cell formation and destruction. */
  /* Oxygen and hematocrit stimulus. */
  v.po2am1 = v.po2amb;
  if (v.po2am1 > 80) {
    v.po2am1 = 80;
  }

  v.hm3 = (v.po2am1 - 40) * v.hm;
  v.hm4 = v.po2amb - 40;
  v.hm5 = v.hm3 + v.hm4;
  if (v.hm5 < 0) {
    v.hm5 = 0;
  }
  v.hm7 = p.hm6 - v.hm5;

  /* Red blood cell kinetics. */
  /* Red blood cell production. */
  v.rc1 = v.hm7 * p.hm8 * p.rek + 5e-6;
  if (v.rc1 < 0) {
    v.rc1 = 0;
  }
  /* Red blood cell destruction. */
  v.rc2 = v.vrc * p.rkc * v.vim;
  v.rcd = v.rc1 - v.rc2 + v.trrbc;
  v.vrc = v.vrc + v.rcd * v.i;

  /* Pulmonary oxygen uptake into the bloodstream. */
  /* Alveolar ventilation and oxygen pressure. */
  v.o2util = v.dob + v.rmo;
  v.alvent = v.o2util * p.vntstm * 0.026667 * v.o2vts2 * v.o2vad2;
  v.po2alv = v.po2amb - v.o2util / v.alvent / 0.761;
  /* Resistance to oxygen diffusion. */
  v.rspdfc = p.pl2 / (p.vptiss + v.vpf);

  v.i9 = 0;
  /* This loop calculates the arterial oxygen pressure. */
  do {
    v.po2ar1 = v.po2art;
    /* Pulmonary oxygren diffusion. */
    v.o2dfs = (v.po2alv - v.po2art) * v.rspdfc;
    v.dova = (v.o2dfs - v.o2util) / v.qro;
    /* Arterial oxygren content. */
    v.ova = v.ova + v.dova * p.i8;
    /* Arterial oxygen saturation. */
    v.osa = v.ova / v.hm / 5.25;

    /* Linear approximation of PO2ART as a function of OSA. */
    if (v.osa > 1) {
      v.po2art = 114 + (v.osa - 1) * 6667;
    } else if (v.osa > 0.936) {
      v.po2art = 74 + (v.osa - 0.936) * 625;
    } else if (v.osa > 0.8) {
      v.po2art = 46 + (v.osa - 0.8) * 205.882;
    } else {
      v.po2art = v.osa * 57.5;
    }

    /* Iterate until stable. */
    v.i9 = v.i9 + p.i8;
    if (v.i9 > v.i) {
      v.i9 = 0;
      break;
    }
  } while (fabs(v.po2art - v.po2ar1) > p.po2adv);

  v.o2vtst = (v.po2art - 67) / 30;
  if (v.o2vtst > 1) {
    v.o2vtst = 1;
  }
  if (v.o2vtst < 0.6) {
    v.o2vtst = 0.6;
  }

  v.o2vts2 = 1 / v.o2vtst;
  v.do2vad = ((v.o2vts2 - 1) * 3 - v.o2vad1) * 5e-4;
  v.o2vad1 = v.o2vad1 + v.do2vad * v.i;
  v.o2vad2 = v.o2vad1 + 1;
}
