/**
 * The autonomic circulation control module of the Guyton 1992 model.
 */

#include <cmath>    /* for pow(), fabs() */

#include "params.h"
#include "vars.h"
#include "utils.h"
#include "module_autonom.h"

/**
 * This function calculates autonomic control of the circulation.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 *
 * @return \c false if the main simulation loop should start again,
 *         otherwise \c true.
 *
 * \ingroup modules
 */
bool module_autonom(const PARAMS &p, VARS &v) {
  if (p.sta - p.aumin > 0) {
    v.au = p.sta;
  } else {
    /* The pressure that is driving autonomic receptors. */
    v.pa1 = v.pa - p.exe;
    if (v.crrflx > 1e-7) {
      v.pa1 = v.crrflx;
    }

    /* chemoreceptors: effect of PA */
    v.auc = 0;
    if (v.pa1 < 80) {
      v.auc = 0.005 * (80. - v.pa1);
    }
    if (v.pa1 < 40) {
      v.auc = 0.2;
    }
    v.auc = v.auc * p.auc1;

    /* chemoreceptors: effect of art PO2 */
    v.auc2 = 0;
    if (v.po2art <= 40) {
      v.auc2 = p.o2chmo * 40;
    } else if (v.po2art <= 80) {
      v.auc2 = (80 - v.po2art) * p.o2chmo;
    }

    /* Arterial baroreceptors */
    v.aub = 0;
    if (v.pa1 < 160) {
      v.aub = 0.016667 * (160 - v.pa1);
    }
    if (v.pa1 < 80) {
      v.aub = 1.3336;
    }
    v.a1b = (v.aub - 1) * p.aux + 1;

    /* CNS ischemic reflex */
    v.aun = 0;
    if (v.pa1 < 40) {
      v.aun = 0.04 * (40 - v.pa1);
    }
    v.aun = v.aun * p.aun1;
    v.au6a = v.a1b - v.au4;
    v.au6b = v.au6;
    v.au6 = (v.au6a - v.au6) / p.barotc * v.i + v.au6;
    if (v.i > 0.166666) {
      v.au6 = v.au6a;
    }
    v.au6c = v.au6 + (v.au6 - v.au6b) * p.mdmp;
    v.aulp = (15 / (v.pla + v.pra + v.ppa) - 1) * p.aulpm + 1;
    v.auttl = (v.aulp * (v.auc + v.auc2 + v.au6c + v.aun) \
                * pow(p.exc, p.excxp) - 1) * p.excml + 1;
    if (v.auttl < 0) {
      v.auttl = 0;
    }

    v.dau = v.auttl - v.au1;
    v.au1 = v.au1 + v.dau * p.i2 / p.audmp;
    v.au = p.aumax - (p.aumax - 1) / pow(2.7183, p.auslp * (v.au1 - 1));
    if (v.au < p.aumin) {
      v.au = p.aumin;
    }
  }

  /* Autonomic drive on the various target organs/tissues. */
  v.auo = v.au - 1;
  v.auh = v.auo * p.auv + 1; /* Heart strength. */
  v.aur = v.auo * p.aus + 1; /* Heart rate. */
  v.vvr = p.vv9 - (v.au * p.aul) + p.aul; /* Basic venous volume. */
  v.aum = pow(v.auo * p.aum1 + 1, p.aum2); /* Arterial resistance. */
  v.ave = v.auo * v.auy + 1; /* Venous resistance. */

  /* Stability: repeat the short loop (dT = I2) if the change is too large. */
  v.i5 = v.i5 + p.i2;
  if (v.i5 <= v.i) {
    if (fabs(v.pa - v.pa3) > p.pa4) {
      return false;
    } else if (fabs(v.qao - v.qlo) > p.qaolm) {
      return false;
    } else if (fabs(v.qao - v.qpo) > p.qaolm) {
      return false;
    } else if (fabs(v.qao - v.qro) > p.qaolm * 2) {
      return false;
    }
  }

  /* Time: compute the long loop time increment (dT = I). */
  v.i5 = 0;
  v.i = v.i * 1.2; /* Increase the time increment (I) by 20%. */
  if (p.i3 < v.i) {
    v.i = p.i3; /* Ensure that we're within the upper limit (I3). */
  }
  v.i1 = fabs(p.vp1 / v.vpd / v.i); /* VP1/VPD is the stability test. */
  if (v.i1 < v.i) {
    v.i = v.i1;
  }

  /* Increase the simulation time by one time-step. */
  v.t = v.t + v.i;
  return true;
}
