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
 * Forward declaration of translate_state().
 */
void translate_state(PARAMS &p, VARS &v);

/**
 * This function calculates the amount of sodium, potassium and water excretion
 * (NOD, KOD, VUD, respectively) and the renal blood flow (RBF).
 *
 * @param[in,out] p  The struct of model parameters.
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
void module_kidney(PARAMS &p, VARS &v) {
  /* Translate Guyton92 parameters and variables into Moore94 equivalents. */
  translate_state(p, v);

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
  scale *= p.rek; /* Account for the functional mass. */

  /* Renal blood flow. */
  v.rbf = 1e-9 * (v.Pas - v.Pg0) / v.Ra; /* L/min */
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

  /*
   * TODO: handle the following variables:
   *   ADHMK effect of adh on sodium and water reabsorption
   *   AMK effect of aldosterone on cell membrane potassium transport
   *   AMNA aldosterone effect on tubular na reasorption
   * TODO: handle the following parameters:
   *   AHMNAR sensitivity control, renal effect of adhmk
   *   ANMKEM sensitivity controller of anmke
   *   ANMNAM sensitivity controller of dtnang
   *   DIURET diuretic effect on tubular reabsorption
   */

  /* Calculate excretions of: Na (mEq/min), K (mEq/min) and volume (L/min). */
  v.nod = EDCT_in_Na * (1 - EDCT_reab_Na) * (1 - CNT_reab_Na)
    * (1 - CCD_reab_Na) * (1 - OMCD_reab_Na) * (1 - IMCD_reab_Na);
  v.kod = EDCT_in_K * (1 - EDCT_reab_K) * (1 - CNT_reab_K)
    * (1 - CCD_reab_K) * (1 - OMCD_reab_K) * (1 - IMCD_reab_K);
  v.vud = EDCT_in_vol * (1 - EDCT_reab_vol) * (1 - CNT_reab_vol)
    * (1 - CCD_reab_vol) * (1 - OMCD_reab_vol) * (1 - IMCD_reab_vol);
}

/**
 * This function translates model parameters and state variables from the
 * original Guyton 1992 model into equivalent (or similar) values for the
 * Moore 1994 model of myogenic autoregulation and glomerular filtration.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 */
void translate_state(PARAMS &p, VARS &v) {
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

  /* Plasma protein concentration, scaled from (g/L) to (g/dL). */
  p.C0 = v.cpp * 0.1;

  /* The autonomic effect on the resistance of the afferent arteriole. */
  v.aumk = (v.aum - 1) * p.arf + 1;
  if (v.aumk < 0.8) {
    v.aumk = 0.8;
  }

  /* The effect of angiotensin on the afferent and efferent resistances. */
  v.anmer = (v.anm - 1) * p.anmem + 1;
  v.anmar = (v.anm - 1) * p.anmam + 1;
  if (v.anmar < p.anmarl) {
    v.anmar = p.anmarl;
  }

  /* Resistance of the afferent arteriole, accounting for autonomic regulation
     and the effect of angiotensin. Here, 0.3 is the base (default) value for
     the Moore94 model; the other values scale it (appropriately?). */
  p.Rb = 0.3 * p.aark * v.pamkrn * v.aumk * v.anmar;
  /* Take into account the effect of ANP on the afferent resistance. The
     default value for the Moore94 parameter (Rb) is 0.8% of the equivalent
     Guyton92 parameter (AAR), and so we scale the ANP effect appropriately. */
  p.Rb = p.Rb - (v.anpx * p.anpxaf + p.anpxaf) * 0.008;

  /* Resistance of the efferent arteriole, accounting for autonomic regulation
     and the effect of angiotensin. Here, 0.3 is the base (default) value for
     the Moore94 model; the other values scale it (appropriately?). */
  p.Re = 0.3 * (p.eark) * ((v.aumk - 1) * p.aumk1 + 1) * v.anmer;

  /*
     Status of the translation:

     Parameters: 'x' translated, 'i' ignored, '?' handled separately.
     x AARK basic afferent arteriolar resistance
     x AARLL
     ? AHMNAR sensitivity control, renal effect of adhmk
     x ANMAM sensitivity effect of angiotensin on afferent arterioles
     x ANMARL
     x ANMEM sensitivity effect of angiotensin on efferent arterioles
     i ANMKEL
     ? ANMKEM sensitivity controller of anmke
     ? ANMNAM sensitivity controller of dtnang
     x ANPXAF sensitivity factor of anp on renal afferent resistance
     x ARF
     x AUMK1 autonomic sensitivity controller on ear
     i CKEEX
     ? DIURET diuretic effect on tubular reabsorption
     i DTNAR sensitivity controller of dtnara
     i DTNARL
     x EARK basic efferent arteriolar resistance
     x EARLL
     i EFAFR
     i GFLC glomerular filtration coefficient
     i GFNDMP gfn damping coefficient
     i GFNLL
     i GPPD damping coefficient glpc
     i MDFL1
     i MDFLKM sensitivity controller of mdflk
     i MDFLW2 iteration limiter, renal autoregulation
     i PXTP proximal tubular hydrostatic pressure
     i RABSC peritubular capillary reabsorption coefficient
     x RAPRSP
     x RCDFDP
     x RCDFPC renal function curve drift coefficient
     x REK total functional renal mass, ratio to normal
     i RFABDM sensitivity controller of rfabd
     i RFABDP rfab damping factor
     i RFABKM proportionality constant, rfabk
     i RNAGTC time constant, renal autoregulation feedback
     i RNAUAD
     i RNAUGN basal renal autoregulation feedback multiplier
     i RTPPR renal oncotic, internal var
     i RTPPRS renal oncotic, internal var
     i RTSPRS renal tissue fluid pressure
     i RVRS
     i URFORM rate of urea formation

     Variables: 'x' translated, 'i' ignored, '?' handled separately.
     ? ADHMK effect of adh on sodium and water reabsorption
     ? AMK effect of aldosterone on cell membrane potassium transport
     ? AMNA aldosterone effect on tubular na reasorption
     x ANM general angiotensin multiplier effect, ratio to normal
     x ANPX anp multiplier, ratio to normal
     x AUM sympathetic vasoconstrictor effect on arteries
     i CKE extracellular potassium concentration
     i CNA extracellular sodium concentration
     i MYOGRS myogenic autoregulation multiplier
     x PA systemic arterial pressure
     x PAMKRN korner pressure effect on renal vessels
     i PPC plasma colloid osmotic pressure
     i VB blood volume
     i VP plasma volume
     i VTW total body water
   */
}
