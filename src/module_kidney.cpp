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

  /* NOTE: the fractional volume reabsorption values differ slightly from
     the published values for the individual collecting duct segments,
     particularly for the cortical collecting duct (CCD). Here, the published
     value is 0.63, but according to Figure 3 (F1241) and text (F1244) of
     Weinstein, AJP Renal 283(6):F1237-F1251, 2002.

     NOTE: the Na reabsorption is decreased from 0.71 to 0.67, as per
     the text (F1245) of Weinstein, AJP Renal 283(6):F1237-F1251, 2002.

     http://dx.doi.org/10.1152/ajprenal.00162.2002 */

  /* EDCT fractional reabsorption from DOI: 10.1152/ajprenal.00043.2005
     http://dx.doi.org/10.1152/ajprenal.00043.2005
     Text (F709--F710) and Table 5 (F711). */
  double EDCT_reab_Na = 0.40;
  double EDCT_reab_K = -0.86;
  double EDCT_reab_vol = 0.10;
  /* CNT fractional reabsorption from DOI: 10.1152/ajprenal.00044.2005
     http://dx.doi.org/10.1152/ajprenal.00044.2005
     Text (F729) and Table 4 (F730). */
  double CNT_reab_Na = 0.36;
  double CNT_reab_K = -5.60;
  double CNT_reab_vol = 0.42;
  /* CCD fractional reabsorption from AJP Renal 280:F1072-F1092, 2001.
     Table 5 (F1085). */
  double CCD_reab_Na = 0.19;
  double CCD_reab_K = 0.06;
  double CCD_reab_vol = 0.29; /* The cited value is 0.63. */
  /* OMCD fractional reabsorption from AJP Renal 279:F24-F45, 2000.
     Table 5 (F35). */
  double OMCD_reab_Na = -0.25;
  double OMCD_reab_K = 0.42;
  double OMCD_reab_vol = 0.52; /* The cited value is 0.54. */
  /* IMCD fractional reabsorption from AJP Renal 274:F841-F855, 1998.
     Text (F846), calculated volume reabsorption from Na reabsorption
     and end-tubule Na concentration. */
  double IMCD_reab_Na = 0.67;
  double IMCD_reab_K = 0.65;
  double IMCD_reab_vol = 0.66; /* The cited value is 0.77. */

  /* The effect of Angiotensin II on volume, Na and K fluxes in the EDCT and
     CNT, as per Wang and Giebisch, AJP Renal 271(1):F143-F149, 1996.
     Tables 1 (F144) and 3 (F147). Effects were calculated by:
     (angiotensin II flux) / (control flux). */
  double EDCT_ang_Na = 1.77;
  double EDCT_ang_K = 1.0;
  double EDCT_ang_vol = 1.62;
  double CNT_ang_Na = 1.98;
  double CNT_ang_K = 0.46;
  double CNT_ang_vol = 2.26;
  /* The effect of ADH on volume, Na and K fluxes in the DT, as per
     Field, Stanton and Giebisch, Kidney Int 25(3):502-511, 1984.
     http://dx.doi.org/10.1038/ki.1984.46
     Table 5 (507). Effects were calculated by:
     (ADH flux) / (control flux). */
  // double DT_adh_Na = 1.23;
  // double DT_adh_K = 1.86;
  // double DT_adh_vol = 0.11;

  /* TODO: use the formula frac^(1/change) ???
     eg, f(x,y) = x**(1/y) for x in [0,1], y in (0, N). */

  double Na_Frac = (1 - (1 - EDCT_reab_Na) / EDCT_ang_Na);
  double K_Frac = (1 - (1 - EDCT_reab_K) / EDCT_ang_K);
  double vol_Frac = (1 - (1 - EDCT_reab_vol) / EDCT_ang_vol);
  Na_Frac = (1 - (1 - Na_Frac) / CNT_ang_Na);
  K_Frac = (1 - (1 - K_Frac) / CNT_ang_K);
  vol_Frac = (1 - (1 - vol_Frac) / CNT_ang_vol);
  // Na_Frac =

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

  /*
   * From CCD paper: aldosterone has negligible effect on volume reabsorption,
   * but increases Na reabsorption by 68% (0.19 to 0.32) and K reabsorption is
   * reduced to secretion (0.06 to -0.15).
   *
   * Aldosterone:
   * AMK has min: 0.2 max: 21.4277 mean: 2.730067
   *         98.5% < 10
   * Treat it logarithmically? ie, log10(AMK/0.2) / 2 --> [0,1]
   * AMNA has min: 0.04 max: 15 mean: 5.383318
   * Treat it logarithmically?
   *
   * Angiotensin:
   * ANM has min: 0.779673 max: 1.47464 mean: 1.118037
   * ANMNAM has min: 0.333 max: 3 mean: 1.544788
   * ANMKEM has min: 0.667 max: 5 mean: 2.629994
   *
   * ADH:
   * ADHMK has min: 0.2 max: 4.17458 mean: 1.016653 sd: 0.2272518
   *           [0.2,2]
   * AHMNAR has min: 0.1 max: 1.2 mean: 0.5929415 sd: 0.2925694
   *
   */

  // Na:
  // v.amna * ((v.adhmk - 1) * p.ahmnar + 1)
  // + ((v.anm - 1) * p.anmnam + 1) * 0.1

  // K:
  // v.amk * 0.08 / ((v.anm - 1) * p.anmkem + 1)

  // H2O:
  // v.osmopn = v.dturi + 2 * (v.nodn + v.kodn)
  // v.osmopn is the urinary osmolar excretion (mEq/min, 'normal'/max is 0.6)
  // v.osmopn / 600 --> [0, 0.001]
  // v.osmop1 = v.osmopn - 0.6 = v.dturi + 2 * (v.nodn + v.kodn)
  // dturi max: 2.81551 nodn max: 3.72234 kodn max: 3.44045
  // v.osmop1 max: 17.44792
  // v.osmop1 / 360 max: 0.04846644
  // v.adhmk has min: 0.2 max: 4.17458 mean: 1.016653 sd: 0.2272518
  //         99.6% in [0.2, 2]
  // v.vudn = v.osmopn / 600 / v.adhmk + v.osmop1 / 360
  // v.vudn in [0, 0.005] + v.osmop1/ 360  (L/min)
  // ...

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
