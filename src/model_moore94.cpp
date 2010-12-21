/**
 * @file
 * A model of tubuloglomerular feedback and myogenic autoregulation.
 * This model comes from <a href="http://dx.doi.org/10.1007/BF02460464">Moore
 * et al, Bull Math Biol 56:3 391-410, 1994</a>.
 */

#include <cmath>     /* for pow(), fabs() */
#include <algorithm> /* for min(), max() */
#include <cfloat>    /* for DBL_MAX */

#include "params.h"
#include "vars.h"
#include "model_moore94.h"

/** The parameters for the Moore94 model. */
struct MOORE94PARAMS {
  /* Parameters for the GFR segment. */
  double H0;   /* The blood hematocrit (no dimensions). */
  double C0;   /* Plasma protein concentration (g/dL). */
  double Re;   /* Efferent resistance (mmHg / (nL/min)). */
  double Rg;   /* Glomerular resistance (mmHg / (nL/min)). */
  double Rb;   /* Basal pre-glomerular resistance (mmHg / (nL/min)). */
  double Pas;  /* Systemic arterial pressure (mmHg). */
  double Paso; /* Reference pressure at which the DMYO is quiescent (mmHg). */
  double Kf;   /* Filtration coefficient ((nL/min) / mmHg). */
  double Pc;   /* Post-efferent capillary pressure (mmHg). */

  /* Parameters for the proximal tubule segment. */
  double Fp;   /* Fractional filtrate reabsorption, convoluted segment. */
  double Fs;   /* Fractional filtrate reabsorption, straight segment. */
  double Rp;   /* Constant tubular reabsorption, convoluted segment. */
  double Rs;   /* Constant tubular reabsorption, straight segment. */
  double Ip;   /* Proximal infusion of tubular fluid. */

  /* Parameters for the ascending limb segment. */
  double Cic;  /* Cortical interstitial NaCl concentration (mM). */
  double Cim;  /* Medullary interstitial NaCl concentration (mM). */
  double Ps;   /* (cm/min). */
  double Vm;   /* (cm/min). */
  double K1;   /* Change in medullary interstitial NaCl (mM/cm). */
  double alx;  /* Length of the medullary and cortical segments (cm). */
  double alr;  /* Radius of the ascending limb (cm). */

  /* Parameters for the TGF segment. */
  double Ct;   /* Minimum NaCl concentration for TGF (mM). */
  double Cs;   /* Saturation NaCl concentration for TGF (mM). */
  double Ktgf; /* Steepness of the TGF response (mmHg / (mM nL/min)). */

  /* Parameters for solving the entire model. */
  double Ga;   /* Scaling coefficient for the AMYO response (0 to 1). */
  double Gd;   /* Scaling coefficient for the DMYO response (0 to 1). */
}
/* Values come from Moore et al, Bull Math Biol 56:3 391-410, 1994. */
moore94_params = {
  /* Parameters for the GFR segment. */
  0.45, 5.5, 0.3, 0.008, 0.3, 140.0, 100.0, 2.5, 4.0,
  /* Parameters for the proximal tubule segment. */
  0.60, 0.10, 0.0, 0.0, 0.0,
  /* Parameters for the ascending limb segment. */
  150.0, 300.0, 1e-5 * 60.0, 7e-5 * 60.0, 50.0 * 10.0, 0.3, 1e-3,
  /* Parameters for the TGF segment. */
  25.0, 65.0, 0.0043,
  /* Parameters for solving the entire model. */
  0.3, 0.3
};

/** The state variables for the Moore94 model. */
struct MOORE94VARS {
  double Ra;    /* Total pre-glomerular resistance (mmHg / (nL/min)). */
  double Pg0;   /* Initial glomerular capillary pressure (mmHg). */
  double GFR;   /* Single-nephron GFR (nL/min). */
  double Qalh;  /* The flow entering the ascending limb (nL/min). */
  double Calh;  /* The NaCl concentration entering the ascending limb (mM). */
  double Ci;    /* The NaCl concentration at the macula densa (mM). */
  double dRtgf; /* The resistance of the TGF segment (mmHg / (nL/min)). */
  double dRma;  /* The resistance of the AMYO segment (mmHg / (nL/min)). */
  double dRmd;  /* The resistance of the DMYO segment (mmHg / (nL/min)). */
  double pa;    /* Systemic arterial pressure (mmHg). */
} moore94_vars;

/**
 * Oncotic pressure (mmHg) as a function of protein concentration (\b g/dL).
 * This equation comes from:
 *   Landis EM, Pappenheimer JR.
 *   Exchange of substances through the capillary walls.
 *   p. 961-1043.
 *   Handbook of physiology: circulation, vol. II.
 *   Washington, DC: American Physiological Society, 1963.
 *
 * @param[in] c The protein concentration (<b>g/dL, presumably</b>).
 */
double oncotic(double c) {
  return 2.1 * c + 0.16 * c * c + 0.009 * c * c * c;
}

/**
 * Solves the GFR segment of the Moore 1994 model. This segment calculates
 * \c SNGFR and \c Pg(0).
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 */
void solve_gfr_model(const PARAMS &p, VARS &v) {
  /* Initial estimates of the outputs. */
  v.Pg0 = v.Pas * 0.4; /* Glomerular pressure at x=0 (mmHg). */
  v.GFR = 20.0; /* Single-nephron filtration rate (nL/min). */

  /* Solve the equations iteratively until they converge. */
  while (true) {
    /* Local variables for this loop. */
    double Ba = (v.Pas - v.Pg0) / v.Ra; /* Afferent blood flow (nL/min). */
    double Be = Ba - v.GFR; /* Efferent blood flow (nL/min). */
    double Q0 = Ba * (1 - p.H0); /* Afferent plasma flow (nL/min). */
    double Pt = 7.5 + 0.13 * v.GFR; /* Bowman's space pressure (mmHg). */

    /* Integration variables. */
    double Q = Q0;
    double Pg = v.Pg0;

    /* Integration step variables. */
    double dx = 1e-3; /* The size of the integration step. */

    /* Integrate dQ/dx and dPg/dx from x=0 to x=1. */
    for (double x = dx; x <= 1.0; x += dx) {
      /* The protein concentration at x. */
      double Cx = p.C0 * Q0 / Q;
      /* The glomerular oncotic pressure at x. */
      double Ponc = oncotic(Cx);

      /* Calculate dQ/dx at x. */
      double dQ = - p.Kf * (Pg - Ponc - Pt);
      /* Calculate dPg/dx at x. */
      double dPg = - p.Rg * Q;

      /* Use Euler's method to calculate Q(x) and Pg(x). */
      Q += dx * dQ;
      Pg += dx * dPg;
    }

    /* Collect the values of Q(x) and Pg(x) at x=1. */
    double Q1 = Q;
    double Pg1 = Pg;

    /* Check if the conservation condition has been met. */
    double diff = v.Pas - Ba * v.Ra - Be * p.Re - p.Pc - (v.Pg0 - Pg1);
    if (fabs(diff) < 1e-3) {
      return;
    }

    /* Improve the guesses for Pg0 and GFR, and try again. */
    v.Pg0 = v.Pg0 - 0.5 * diff;
    v.GFR = 0.5 * (v.GFR + Q0 - Q1);
  }
}

/**
 * Solves the proximal tubule segment of the Moore 1994 model. This segment
 * calculates \c Qalh and \c Calh.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 */
void solve_proximal_model(const PARAMS &p, VARS &v) {
  /* Decrease Fp by 0.9% for each mmHg that Pas is above 100 mmHg.
     See paragraph 3 on page 400 of Moore et al. 1994. */
  double Fp1 = p.Fp;
  if (v.Pas > 100.0) {
    Fp1 = Fp1 * (1.0 - (v.Pas - 100.0) * 0.009);
  }

  double Qtp = v.GFR * (1.0 - Fp1) - p.Rp + p.Ip;
  double Qep = Qtp * (1 - p.Fs) - p.Rs;

  v.Qalh = Qep * p.Cic / p.Cim;
  v.Calh = p.Cim;
}

/**
 * Solves the ascending limb segment of the Moore 1994 model. This segment
 * calculates \c Ci.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 */
void solve_alh_model(const PARAMS &p, VARS &v) {
  double pv_sq = (p.Ps + p.Vm) * (p.Ps + p.Vm);

  double a = p.Ps * p.Cim / (p.Ps + p.Vm);
  double b = p.Ps * p.K1 * v.Qalh / (2 * M_PI * p.alr * pv_sq);
  b *= 1e-6; /* Convert (mM nL)/(cm^3) to (mM nL)/(nL) = mM. */
  double c = 2 * M_PI * p.alr * (p.Ps + p.Vm) / v.Qalh;
  c *= 1e6; /* Convert (c * p.alx) from (cm^3)/(nL) to having no units. */
  double d = p.K1 * p.Ps / (p.Ps + p.Vm);

  /* The NaCl concentration at the cortico-medullary junction. */
  double Ci1 = (v.Calh - a - b) * exp(- c * p.alx) - p.alx * d + a + b;

  /* Change the equation parameters for the cortical segment. */
  a = p.Ps * p.Cic / (p.Ps + p.Vm);
  b = 0;
  d = 0;

  /* The NaCl concentration at the macula densa. */
  v.Ci = (Ci1 - a - b) * exp(- c * p.alx) - p.alx * d + a + b;
}

/**
 * Solves the TGF segment of the Moore 1994 model. This segment calculates
 * \c dRtgf.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 */
void solve_tgf_model(const PARAMS &p, VARS &v) {
  double Ci = v.Ci;

  if (v.Ci < p.Ct) {
    Ci = p.Ct;
  } else if (v.Ci > p.Cs) {
    Ci = p.Cs;
  }

  v.dRtgf = p.Ktgf * (Ci - p.Ct);
}

/**
 * Solves all segments of the Moore 1994 model. In addition to solving the
 * outputs of each model segment, it also calculates the values of \c dRma,
 * \c dRmd and \c Ra.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 */
void solve_moore94_model(const PARAMS &p, VARS &v) {
  double prev_dRtgf = -1.0;

  /* Initially, we assume that the myogenic and TGF resistances are zero. */
  v.dRma = 0;
  v.dRmd = 0;
  v.dRtgf = 0;
  /* Calculate the initial pre-glomerular resistance. */
  v.Ra = p.Rb + v.dRma + v.dRmd + v.dRtgf;

  /* Solve the model for dRtgf = dRmd = dRma = 0. */
  solve_gfr_model(p, v);
  solve_proximal_model(p, v);
  solve_alh_model(p, v);
  solve_tgf_model(p, v);

  double tol = 1e-3; /* The tolerance of the model convergence. */

  /* Keep solving the model until successive estimates of dRtgf converge. */
  while (fabs(v.dRtgf - prev_dRtgf) > tol) {
    prev_dRtgf = v.dRtgf;

    /* Calculate the new predictions for dRma, dRmd and Ra. */
    v.dRma = p.Ga * v.dRtgf * (p.Rb + v.dRmd) / (p.Rg + p.Re);
    v.dRmd = p.Gd * (v.Pas / p.Paso - 1.0) *
             (p.Rb + v.dRma + v.dRtgf + p.Rg + p.Re);
    v.Ra = p.Rb + v.dRma + v.dRmd + v.dRtgf;

    /* Solve the model for the new values of dRtgf, dRma, dRmd and Ra. */
    solve_gfr_model(p, v);
    solve_proximal_model(p, v);
    solve_alh_model(p, v);
    solve_tgf_model(p, v);

    /* Improve the guess for dRtgf, using linear interpolation. */
    v.dRtgf = 0.5 * (v.dRtgf + prev_dRtgf);
  }
}
