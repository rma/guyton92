/**
 * The renal module of the Guyton 1992 model.
 */

#include <cmath>    /* for pow(), fabs() */

#include "params.h"
#include "vars.h"
#include "module_renal.h"

/**
 * This function calculates the amount of sodium, potassium and water excretion
 * (NOD, KOD, VUD, respectively) and the renal blood flow (RBF).
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 *
 * Inputs to this block are:
 * - t Current simulation time (minutes).
 * - i Time-step size (minutes).
 * - PA (Circulation)
 * - AUM (Autonomic)
 * - ANM (Angiotensin)
 * - AMNA (Aldosterone)
 * - ANPX (Atrial natriuretic peptide)
 * - HM1 (Red cells)
 * - PPC (Capillary dynamics)
 * - ADHMK (Vasopressin)
 * - CNA (Electrolytes)
 * - CKE (Electrolytes)
 * - VTW (Electrolytes)
 * - VP (Capillary dynamics)
 * - VB (Red cells)
 * - AMK (Aldosterone) Aldosterone effect on potassium transport.
 * - PAMKRN (Circulatory dynamics)
 * - MYOGRS (Circulatory dynamics)
 *
 * Outputs from this black are:
 * - NOD (sodium excretion) used by Electrolytes::NED (sodium balance).
 * - KOD (potassium excretion) used by Electrolytes::KTOTD (potassium balance).
 * - VUD (water excretion) used by Electrolytes::VTW (water balance).
 * - RBF (renal blood flow) used by Circulatory dynamics::SYSFLO.
 * - MDFLW used by Angiotensin::MDFLW3. Needs explicit initialisation.
 *
 */
void module_renal(const PARAMS &p, const VARS &v) {
  /**
   * Local variables that don't need explicit initialisation:
   * - PAR Renal arterial pressure (mmHg).
   * - AUMK Autonomic effect on afferent resistance.
   * - ANMAR Angiotensin effect on afferent resistance.
   * - ANMER Angiotensin effect on efferent resistance.
   * - AAR
   * - EAR
   * - MDFLW1
   * - DTNANG
   * - VUD1
   * - RR Renal resistance.
   * - RFN
   * - ANMKE
   * - NODN
   * - GFR (output? not used anywhere...)
   * - EFAFPR
   * - PLURC
   * - APD
   * - GLP Glomerular pressure (mmHg).
   * - PFL Starling forces (mmHg, presumably).
   * - PTFL
   * - RCPRS Renal peritubular capillary pressure (mmHg).
   * - RTSPPC Renal tissue oncotic pressure (mmHg).
   * - RABSPR Starling forces across the tubule (mmHg).
   * - RFAB1
   * - RFABD
   * - RFABK
   * - DTNAI Sodium delivery to the distal tubule.
   * - DTNARA Sodium reabsorption in the distal tubule.
   * - DTKI Potassium delivery to the distal tubule.
   * - DTKSC Potassium secretion in the distal tubule.
   * - OSMOPN
   * - OSMOP1
   * - MDFLK
   * - I5 (loop variable, shared with Autonomic control, but set to zero)
   * - AAR1
   *
   * Local variables that need explicit initialisation:
   * - RNAUG1
   * - RNAUG2
   * - RNAUG3
   * - KODN
   * - VUDN
   * - GFN
   * - GLPC
   * - DTURI
   * - UROD
   * - PLUR
   * - RFAB
   * - DTKA
   *
   * Local variables that should really be parameters, because they are only
   * used once in the entire model and their values are never changed:
   * - GBL Goldblatt clamp on the renal artery.
   * - RFCDFT
   * - RNAULL
   * - RNAUUL
   * - GLPCA
   * - RFABX
   * - ANMNAM
   * - MDFLKM
   * - VUD2
   */

  /* The original code includes a single local variable. */
  double aar1;

  /* The loop variable I5 is shared with Autonomic control, but is always set
     to zero before reaching the renal module. */
  //*v.i5 = 0.0;

  if (*p.raprsp > 0) {
    /* The renal arterial pressure (PAR) is controlled by the parameter
       RAPRSP (servocontrol). */
    *v.par = *p.raprsp;
  } else {
  /* The renal arterial pressure (PAR) depends on the mean arterial pressure
     (PA), but can be lowered through the use of a Goldblatt clamp (GBL). */
    *v.par = *v.pa - *v.gbl;
  }

  /* Renal function drift. */
  if (*v.rfcdft > 0) {
    *v.par = *v.par + ((100 + (*v.pa - 100) * *p.rcdfpc) - *v.par) / *p.rcdfdp * *v.i;
  }

  /* The autonomic effect on the resistance of the afferent arteriole. */
  *v.aumk = (*v.aum - 1) * *p.arf + 1;
  if (*v.aumk < 0.8) {
    *v.aumk = 0.8;
  }

  /* The effect of Angiotensin on the afferent and efferent resistances. */
  *v.anmer = (*v.anm - 1) * *p.anmem + 1;
  *v.anmar = (*v.anm - 1) * *p.anmam + 1;
  if (*v.anmar < *p.anmarl) {
    *v.anmar = *p.anmarl;
  }

  /* Renal autoregulation feedback in the afferent and efferent arterioles. */
  do {
    *v.rnaug1 = *v.rnaug1 + (((*v.mdflw - 1) * *p.rnaugn + 1) - *v.rnaug1) / *p.rnagtc;
    if (*v.rnaug1 < *v.rnaull) {
      *v.rnaug1 = *v.rnaull;
    }
    if (*v.rnaug1 > *v.rnauul) {
      *v.rnaug1 = *v.rnauul;
    }
    *v.rnaug2 = *v.rnaug1 - *v.rnaug3;
    *v.rnaug3 = *v.rnaug3 + (*v.rnaug2 - 1) * *p.rnauad * *p.i4;
    /* AAR1 is the sole local variable in this module. */
    aar1 = *p.aark * *v.pamkrn * *v.aumk * *v.rnaug2 * *v.anmar * 40 * *v.myogrs;

    /* Resistance of the afferent arteriole. */
    *v.aar = aar1 - *v.anpx * *p.anpxaf + *p.anpxaf;
    if (*v.aar < *p.aarll) {
      *v.aar = *p.aarll;
    }

    /* Resistance of the efferent arteriole. */
    *v.ear = 43.333 * *p.eark * *v.anmer * ((*v.rnaug2 - 1) * *p.efafr + 1) * *v.myogrs;
    *v.ear = *v.ear * ((*v.aumk - 1) * *p.aumk1 + 1);
    if (*v.ear < *p.earll) {
      *v.ear = *p.earll;
    }

    /* Total renal resistance. */
    *v.rr = *v.aar + *v.ear;
    /* Normal renal bloodflow. */
    *v.rfn = *v.par / *v.rr;

    /* The effect of filtration fraction on GLPC. */
    if (*v.glpca > 0) {
      *v.efafpr = *v.rfn * *v.vp / *v.vb / (*v.rfn * *v.vp / *v.vb - *v.gfn);
      if (*v.efafpr < 1) {
        *v.efafpr = 1;
      }
      *v.glpc = *v.glpc + (pow(*v.efafpr, 1.35) * *v.ppc * 0.98 - *v.glpc) / *p.gppd;
    } else {
      *v.glpc = *v.ppc + 4; /* Glomerular oncotic pressure. */
    }
    /* Renal bloodflow. */
    *v.rbf = *p.rek * *v.rfn;

    /* Glomerular filtration rate. */
    *v.apd = *v.aar * *v.rfn;
    *v.glp = *v.par - *v.apd;/* Glomerular pressure. */
    *v.pfl = *v.glp - *v.glpc - *p.pxtp;/* Starling forces. */
    *v.gfn = *v.gfn + (*v.pfl * *p.gflc - *v.gfn) / *p.gfndmp;
    if (*v.gfn < *p.gfnll) {
      *v.gfn = *p.gfnll;
    }
    *v.gfr = *v.gfn * *p.rek;

    /* Filtrate flow in the proximal tubule and macula densa. */
    *v.ptfl = *v.gfn * 8;
    *v.mdflw1 = *v.mdflw;
    *v.mdflw = (*v.ptfl - 1) * *p.mdfl1 + 1;
    if (*v.mdflw < 0) {
      *v.mdflw = 0;
    }
    *v.i5 = *v.i5 + *p.i4;
    if (*v.i5 > *v.i) {
      /* This exits the loop if the next iteration exceeds the size of the
         simulation time-step. */
      *v.i5 = 0;
      break;
    }

  } while (fabs(*v.mdflw - *v.mdflw1) > *p.mdflw2);

  /* Peritubular capillary pressure. */
  *v.rcprs = ((*v.rfn - 1.2) * *v.rfabx + 1.2) * *p.rvrs;
  /* Renal tissue oncotic pressure. */
  *v.rtsppc = *v.glpc * *p.rtppr - *p.rtpprs;
  if (*v.rtsppc < 1) {
    *v.rtsppc = 1;
  }
  /* Starling forces across the tubule. */
  *v.rabspr = *v.glpc + *p.rtsprs - *v.rcprs - *v.rtsppc;
  *v.rfab1 = *v.rabspr * *p.rabsc;
  *v.rfab = *v.rfab + (*v.rfab1 - *v.rfab) / *p.rfabdp;
  *v.rfabd = (*v.rfab - 1) * *p.rfabdm + 1;
  if (*v.rfabd < 1e-4) {
    *v.rfabd = 1e-4;
  }
  /* The effect on potassium. */
  *v.rfabk = (*v.rfabd - 1) * *p.rfabkm;

  /* Sodium and potassium handling. */
  /* Na distal delivery. */
  *v.dtnai = *v.mdflw * *v.cna * .0061619;
  *v.dtnara = *v.amna * *v.rfabd * *p.dtnar / *p.diuret * ((*v.adhmk - 1) * *p.ahmnar + 1);
  /* Non-angiotensin distal tubule/collecting duct sodium reabsorption. */
  if (*v.dtnara < *p.dtnarl) {
    *v.dtnara = *p.dtnarl;
  }

  /* Potassium handling. */
  *v.dtki = *v.dtnai * *v.cke / *v.cna;

  *v.anmke = (*v.anm - 1) * *p.anmkem + 1;
  if (*v.anmke < *p.anmkel) {
    *v.anmke = *p.anmkel;
  }
  *v.mdflk = (*v.mdflw - 1) * *p.mdflkm + 1;
  if (*v.mdflk < 0.1) {
    *v.mdflk = 0.1;
  }

  /* Potassium secretion. */
  *v.dtksc = pow(*v.cke / 4.4, *p.ckeex) * *v.amk * 0.08 * *v.mdflk / *v.anmke;
  /* The effect of angiontensin on distal tubule sodium reabsorption. */
  *v.dtnang = ((*v.anm - 1) * *p.anmnam + 1) * 0.1;
  if (*v.dtnang < 0) {
    *v.dtnang = 0;
  }

  /* The loop for solving urinary excretion. */
  do {
    *v.nodn = *v.dtnai - *v.dtnara - *v.dtnang;
    if (*v.nodn < 1e-8) {
      *v.nodn = 1e-8;
    }
    /* Potassium excretion. */
    *v.dtka = *v.dtka + (*v.kodn / *v.vudn * 0.0004518 - *v.dtka) * *p.i6;
    *v.kodn = *v.dtki + *v.dtksc - *v.dtka - *v.rfabk;
    if (*v.kodn < 0) {
      *v.kodn = 0;
    }
    /* Osmolar and water excretion. */
    *v.osmopn = *v.dturi + 2 * (*v.nodn + *v.kodn);
    *v.osmop1 = *v.osmopn - 0.6;
    if (*v.osmopn > 0.6) {
      *v.osmopn = 0.6;
    }
    if (*v.osmop1 < 0) {
      *v.osmop1 = 0;
    }
    *v.vud1 = *v.vudn;
    *v.vudn = *v.osmopn / 600 / *v.adhmk + *v.osmop1 / 360;
  } while (fabs(*v.vudn - *v.vud1) > *v.vud2);


  /* Water and electrolyte excretion. */
  *v.vud = *v.vudn * *p.rek;
  *v.nod = *v.nodn * *p.rek;
  *v.kod = *v.kodn * *p.rek;

  /* Urea dynamics. */
  *v.plur = *v.plur + (*p.urform - *v.urod) * *v.i;
  *v.plurc = *v.plur / *v.vtw;
  *v.dturi = pow(*v.gfn, 2) * *v.plurc * 3.84;
  *v.urod = *v.dturi * *p.rek;
}
