/**
 * @file
 * The replacement for the original renal module of the Guyton 1992 model.
 * This model is based on <a href="http://dx.doi.org/10.1007/BF02460464">Moore
 * et al, Bull Math Biol 56:3 391-410, 1994</a>.
 */

#include "params.h"
#include "vars.h"
#include "module_kidney.h"
#include "model_moore94.h"

/**
 * This function calculates the amount of sodium, potassium and water excretion
 * (NOD, KOD, VUD, respectively) and the renal blood flow (RBF).
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 *
 * <b>Kidney module outputs:</b>
 * - \b KOD, \b NOD and \b VUD are used by the \link module_electro.cpp
 *   electrolytes\endlink module.
 * - \b MDFLW is used by the \link module_angio.cpp angiotensin\endlink module.
 * - \b RBF is used by the \link module_circdyn.cpp circulatory dynamics
 *   \endlink module.
 *
 * <b>Parameters that affect the kidney module:</b>
 * - \b raprsp: overriding value of renal arterial pressure (mmHg)
 * - \b rcdfdp: renal function curve damping factor (none)
 * - \b rcdfpc: renal function curve drift coefficient (none)
 * - \b rek: total functional renal mass, ratio to normal (none)
 *
 * \ingroup modules
 */
void module_kidney(const PARAMS &p, VARS &v) {
  /* Calculate the arterial pressure (v.Pas) for the Moore 1994 model. */
  if (p.raprsp > 0) {
    /* The renal arterial pressure (PAR) is controlled by the parameter
       RAPRSP (servocontrol). */
    v.Pas = p.raprsp;
  } else {
    /* The renal arterial pressure (PAR) depends on the mean arterial pressure
       (PA), but can be lowered through the use of a Goldblatt clamp (GBL). */
    v.Pas = v.pa - v.gbl;
  }
  /* Renal function drift. */
  if (v.rfcdft > 0) {
    v.Pas += ((100 + (v.Pas - 100) * p.rcdfpc) - v.par) / p.rcdfdp * v.i;
  }

  /* TODO: Translate all relevant parameters into model-specific parameters. */

  /* Solve the Moore 1994 model of glomerular filtration. */
  solve_moore94_model(p, v);

  /* A linear regression was used to estimate MDFLW from Qalh. */
  double mdflw = -3374.8165 + 1978.1256 * v.Qalh \
    - 386.4192 * v.Qalh * v.Qalh + 25.1648 * v.Qalh * v.Qalh * v.Qalh;

  /* Renal blood flow. */
  double rbf = 1e-9 * (v.Pas - v.Pg0) / v.Ra; /* L/min */
  rbf *= p.rek; /* Account for the functional mass. */

  /* Distal delivery of water, sodium and potassium. */
  double distal_H2O = 1e-9 * v.Qalh; /* L/min */
  double distal_Na = v.Ci; /* mM */
  double distal_Na_rate = distal_H2O * distal_Na; /* mM/min */
  double distal_K = distal_Na * v.cke / v.cna; /* mM */
  double distal_K_rate = distal_H2O * distal_K; /* mM/min */

  /* TODO: Solve models of the distal tubule and collecting duct. */

  /* TODO: Return KOD and NOD (mEq/min), VUD and RBF (L/min) and MDFLW. */
}
