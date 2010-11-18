/**
 * The capillary dynamics module of the Guyton 1992 model.
 */

#include <cmath>    /* for pow(), fabs() */

#include "params.h"
#include "vars.h"
#include "utils.h"
#include "module_capdyn.h"

/**
 * This function calculates the effect of capillary dynamics, tissue fluid and
 * tissue protein.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 *
 * \ingroup modules
 */
void module_capdyn(const PARAMS &p, VARS &v) {
  /* fun6: vts, vg, fun6 */
  double fun6[12] = {0, 0, 12, 11.4, 15, 14, 18, 16, 21, 17.3, 24, 18};

  /* Capillary membrane dynamics. */
  v.pc = v.rvs * 1.7 * v.bfn + v.pvs;
  /* Starling forces and leakage. */
  v.vtc = (v.pc - v.ppc - v.pgh + v.ptc) * p.cfc + v.vtcpl;

  /* Plasma volume and protein concentration. */
  /* Whole-body fluid shifts in to and out of the plasma. */
  v.vpd = v.vtl - v.vtc - v.dfp + p.trpl;
  /* The plasma volume. */
  v.vp = v.vp + v.vpd * v.i;
  /* The protein concentration in the plasma. */
  v.cpp = v.prp / v.vp;
  /* The drive for protein destruction. */
  v.cppd = v.cpp - p.cpr;
  if (v.cppd < 0) {
    v.cppd = 0;
  }
  /* Protein balance. */
  v.dlp = p.lppr - pow(v.cppd, p.lpde) * p.lpk;

  /* The driving pressure for plasma leakage. */
  v.prcd = v.pc - p.pcr;
  if (v.prcd < 0) {
    v.prcd = 0;
  }
  /* Fluid leakage. */
  v.vtcpl = pow(v.prcd * p.cpk, p.pce);
  /* Protein leakage. */
  v.dpc = v.vtcpl * v.cpp + (v.cpp - v.cpi) * 0.00104;
  /* The net shift of protein in to or out of the plasma. */
  v.dpp = v.dlp + v.dpl - v.dpc - v.ppd + p.trpl * 72;

  /* Total plasma protein. */
  v.prp = v.prp + v.dpp * v.i;
  /* Oncotic pressure of the plasma. */
  v.ppc = 0.28 * v.cpp + 0.0019 * pow(v.cpp, 2);

  /* Systemic tissue fluid volume. */
  v.vts = v.vec - v.vp - v.vpf;
  v.vts2 = v.vts2 + ((v.vts - 12) * p.tsslml - v.vts2) * p.tssltc * v.i;
  v.vts1 = v.vts - v.vts2;

  /* Tissue protein. */
  v.dpi = v.dpc - v.dpl;
  /* Total interstitial protein. */
  v.tsp = v.tsp + v.dpi * v.i;
  v.cpi = v.tsp / v.vts;
  /* Lymph protein flow. */
  v.dpl = v.cpi * v.vtl;
  /* Interstitial oncotic pressure. */
  v.ptcpr = 0.28 * v.cpi + 0.0019 * pow(v.cpi, 2);

  /* Tissue gel and fluid. */
  v.chy = pow(p.hyl / v.vts / 5, p.cmptss);
  /* The hydrostatic pressure of the tissue gel. */
  v.pgh = v.chy * p.pghf + v.ptt;
  v.poshyl = v.chy * 2;
  /* The oncotic pressure of the tissue gel. */
  v.ptc = v.poshyl * v.ptcpr * p.gcopf;

  /* The total tissue pressure. */
  v.ptt = pow((v.vts1 - p.vtsf) / p.vtsf, 2);
  /* Interstitial free-fluid pressure. */
  v.pif = v.pgh - v.poshyl;
  /* Solid tissue pressure. */
  v.pts = v.ptt - v.pif;
  /* The pressure drive for lymph flow. */
  v.pld = v.pif + p.pldf - v.ptt;
  if (v.pld > 7) {
    v.pld = 7;
  }

  /* The lymph flow. */
  v.vtl = v.pld * 0.02;
  if (v.vtl < 0) {
    v.vtl = 0;
  }

  /* The interstitial gel volume. */
  funct(&v.vts, &v.vg, fun6, 12);

  /* The variables VIG and VG are only used for displaying output. */
  v.vif = v.vts - v.vg;
}
