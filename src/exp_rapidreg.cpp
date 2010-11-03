/**
 * This experiment causes rapid autoregulation in all vascular beds, except in
 * the kidneys.
 */

#include "params.h"
#include "vars.h"
#include "exp_rapidreg.h"

/**
 * This function causes rapid autoregulation in all vascular beds, except in
 * the kidneys. To run this experiment, set AUTOC to 1.
 *
 * The purpose of this simulation is to cause rapid blood flow autoregulation
 * in all the vascular beds except the kidneys, so that other experiments can
 * run without waiting for autoregulation to develop.
 *
 * The pressure-stretch effect (PAEX) is set to 0, and STA is set to 1 to
 * eliminate autonomic feedback. Z is set to 1 to decrease damping and the
 * normal autoregulation in the model (POZ, PON, POK, POM, POM2) are set to 0.
 *
 * The gain of the new autoregulation is set by AUTOGN (initially to 7).
 * Feedback to the arterial resistances is through RMULT (delayed by RMULT1)
 * to multiply RSM and RSN (the vascular resistance through muscle and
 * non-muscle tissue, respectively).
 *
 * The rate of development of autoregulation is controlled by AUTOK, which
 * normally has a value of 0.1.
 *
 * To test the effect of small increases in volume, a transfusion can be given
 * or fluid can be retained by setting REK to 0.3 (to reduce kidney mass to 30%
 * of the normal mass) and after a short run set NID to 0.5 to increase the
 * salt intake 5-fold.
 *
 * Record increases in VEC, VB, QAO, PA. Note only small increases in all
 * except PA, which is very large. When a large volume of blood is given
 * rapidly, the iteration time limit (I3) must be changed from 20 minutes to a
 * very short interval, possibly as small as 0.005 minutes.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 */
void exp_rapidreg(const PARAMS &p, const VARS &v) {
  if (*v.autoC > 0) {
    /* There is no pressure stretch or autonomic feedback. */
    *p.z = 1;
    *p.sta = 1;
    *p.paex = 0;

    /* No autoregulation. */
    *p.poz = 0;
    *p.pon = 0;
    *p.pok = 0;
    *p.pom = 0;
    *p.pom2 = 0;

    /* Feedback for the arterial resistances. */
    *v.rmult = ((*v.bfn + *v.bfm) / 3.8 - 1) * *p.autogn + 1;
    *v.rmult1 = *v.rmult1 + (*v.rmult - *v.rmult1) * *p.autok;
  }
}
