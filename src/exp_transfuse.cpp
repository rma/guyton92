/**
 * This experiment allows for transfusion or blood loss, with hematocrits
 * ranging from 0 (pure plasma) to 1 (pure red blood cells).
 */

#include "params.h"
#include "vars.h"
#include "exp_transfuse.h"

/**
 * This function allows for transfusion or blood loss, with hematocrits
 * ranging from 0 (pure plasma) to 1 (pure red blood cells). To run this
 * experiment, set TIMETR to 1.
 *
 * The controlling parameters for this experiment are:
 * - \b TRNSFS the transfusion rate (L/min). Set this to a negative value to
 *      achieve blood loss.
 * - \b TRNSTM the total transfusion time (min).
 * - \b HMTRNS the hematocrit of the blood being transfused (or lost).
 *
 * To give a transfusion of blood with a hematocrit of 40%:
 * - Set TRNSFS to some transfusion rate (eg, a value of 10<sup>-3</sup> is
 *   1 mL/min);
 * - Set TRNSTM to the desired duration of transfusion (min); and
 * - At the time that the transfusion should start, set TIMETR to 1.
 *
 * For transfusing blood with a different hematocrit, set HMTRNS to the desired
 * hematocrit before setting TIMETR to 1.
 *
 * To lose blood or only plasma (ie, zero hematocrit), use the procedure
 * outlined above but with a negative transfusion rate TRNSFS.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 *
 * \ingroup experiments
 */
void exp_transfuse(PARAMS &p, VARS &v) {
  if (p.timetr > 0) {
    double pcnt = 0.01 * v.i;

#ifdef UTTAMSINGH2
    double pcnt_p = p.trnsfs * pcnt;
    v.vp = v.vp + pcnt_p;
    v.nae = v.nae + pcnt_p * 4349.76;
#endif
#ifndef UTTAMSINGH2
    double pcnt_p = p.trnsfs * pcnt * (100 - p.hmtrns);
    double pcnt_c = p.trnsfs * pcnt * p.hmtrns;

    v.vic = v.vic + pcnt_c;
    v.vrc = v.vrc + pcnt_c;
    v.vp = v.vp + pcnt_p;
    v.nae = v.nae + pcnt_p * v.cna;
    v.ke = v.ke + pcnt_p * v.cke;
    v.ki = v.ki + pcnt_c * v.cki;
    v.prp = v.prp + pcnt_p * v.cpp;
#endif

    v.trnstm = v.trnstm + v.i;
    if (v.trnstm > p.timetr) {
      v.trnstm = 0;
      p.timetr = 0;
    }
  }
}
