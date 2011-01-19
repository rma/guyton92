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
  if (p.glmcubic) {
    v.mdflw = -3374.8165 + 1978.1256 * v.Qalh
      - 386.4192 * v.Qalh * v.Qalh + 25.1648 * v.Qalh * v.Qalh * v.Qalh;
  } else {
    /* This scaling factor converts Qalh to mdflw, so that the mean of Qalh
       (5.087248) corresponds to the mean of mdflw (1.001937). */
    v.mdflw = v.Qalh / 5.077413;
  }

   /* Scale values to the human body (~2x10^6 nephrons) so that the mean RBF
      is equivalent to the mean RBF as calculated by the original module. */
  double scale = 2e6 * 3.531067;

  /* Renal blood flow. */
  v.rbf = 1e-9 * (v.Pas - v.Pg0) / v.Ra; /* L/min */
  v.rbf *= p.rek; /* Account for the functional mass. */
  v.rbf *= scale;

  /* Distal delivery of water, sodium and potassium. */
  double EDCT_in_vol = scale * 1e-9 * v.Qalh; /* L/min */
  double EDCT_in_Na = EDCT_in_vol * v.Ci; /* mmol/min */
  double EDCT_in_K = EDCT_in_vol * v.Ci * v.cke / v.cna; /* mmol/min */

  /* Fractional reabsorption of volume, sodium and potassium, in the early
     distal convoluted tubule (EDCT), the connecting tubule (CNT), and the
     cortical, outer-medullary and inner-medullary collecting ducts (CCD,
     OMCD and IMCD, respectively). */

  /* EDCT fractional reabsorption from DOI: 10.1152/ajprenal.00043.2005
     http://dx.doi.org/10.1152/ajprenal.00043.2005 */
  double EDCT_reab_Na = 0.40;
  double EDCT_reab_K = -0.86;
  double EDCT_reab_vol = 0.10;
  /* CNT fractional reabsorption from DOI: 10.1152/ajprenal.00044.2005
     http://dx.doi.org/10.1152/ajprenal.00044.2005 */
  double CNT_reab_Na = 0.36;
  double CNT_reab_K = -5.60;
  double CNT_reab_vol = 0.42;
  /* CCD fractional reabsorption from AJP Renal 280:F1072-F1092, 2001. */
  double CCD_reab_Na = 0.19;
  double CCD_reab_K = 0.06;
  double CCD_reab_vol = 0.63;
  /* OMCD fractional reabsorption from AJP Renal 279:F24-F45, 2000. */
  double OMCD_reab_Na = -0.25;
  double OMCD_reab_K = 0.42;
  double OMCD_reab_vol = 0.54;
  /* IMCD fractional reabsorption from AJP Renal 274:F841-F855, 1998. */
  double IMCD_reab_Na = 0.71;
  double IMCD_reab_K = 0.65;
  double IMCD_reab_vol = 0.77;

  /* Calculate excretions of: Na (mEq/min), K (mEq/min) and volume (L/min). */
  v.nod = EDCT_in_Na * (1 - EDCT_reab_Na) * (1 - CNT_reab_Na)
    * (1 - CCD_reab_Na) * (1 - OMCD_reab_Na) * (1 - IMCD_reab_Na);
  v.kod = EDCT_in_K * (1 - EDCT_reab_K) * (1 - CNT_reab_K)
    * (1 - CCD_reab_K) * (1 - OMCD_reab_K) * (1 - IMCD_reab_K);
  v.vud = EDCT_in_vol * (1 - EDCT_reab_vol) * (1 - CNT_reab_vol)
    * (1 - CCD_reab_vol) * (1 - OMCD_reab_vol) * (1 - IMCD_reab_vol);

  /* TODO: calculate the new plasma concentrations of Na and K? */
}
