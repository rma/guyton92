/**
 * The circulatory dynamics module of the Guyton 1992 model.
 */

#include <cmath>    /* for pow(), fabs() */

#include "params.h"
#include "vars.h"
#include "utils.h"
#include "module_circdyn.h"

/**
 * This function calculates various properties of the circulatory system.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 *
 * \ingroup modules
 */
void module_circdyn(const PARAMS &p, VARS &v) {
  /* fun1: pa2, lvm, fun1 */
  double fun1[12] = {0, 1.04, 60, 1.025, 125, 0.97,
                     160, 0.88, 200, 0.59, 240, 0};
  /* fun2: pra, qrn, fun2 */
  double fun2[10] = {-8, 0, -6, 0.75, -2, 2.6, 4, 9.8, 12, 13.5};
  /* fun3: pp2, rvm, fun3 */
  double fun3[12] = {0, 1.06, 32, 0.97, 38.4, 0.93,
                     48, 0.8, 60.8, 0.46, 72, 0};
  /* fun4: pla, qln, fun4 */
  double fun4[10] = {-2, 0.01, 1, 3.6, 5, 9.4, 8, 11.6, 12, 13.5};

  /* Changes in blood volume are divided into various circulatory sections. */
  v.vbd = v.vp + v.vrc - v.vvs - v.vas - v.vla - v.vpa - v.vra;
  /* The approximate fractions are: 40% venous, 15% pulmonary, 26% arterial,
     13% left atrium, 6% right atrium. */
  v.vvs = v.vvs + v.dvs * p.i2 + v.vbd * 0.3986;
  v.vpa = v.vpa + v.dpa * p.i2 + v.vbd * 0.155;
  v.vas = v.vas + v.das * p.i2 + v.vbd * 0.261;
  v.vla = v.vla + v.dla * p.i2 + v.vbd * 0.128;
  v.vra = v.vra + v.dra * p.i2 + v.vbd * 0.0574;

  /* The arterial compartment. */
  /* The excess volume is the total volume, less the unstressed volume. */
  v.vae = v.vas - 0.495;
  v.pa3 = v.pa;
  /* Arterial pressure is the excess volume divided by the compliance.
     NOTE: the compliance was reduced from the original value of 0.00355.
           The attached comment indicated that this was to simulate a
           hypertensive person, whose blood vessels would be less compliant. */
  v.pa = v.vae / 0.00155;
  /* The effect of pressure on arterial distension. */
  v.pam = pow(v.pa / 100, p.paex);

  /* Myogenic autoregulation. */
  v.myogrs = 1;
  if (p.tensgn > 0) {
    v.tens1 = (v.pa + v.pc) / 117 * pow(v.pam, 0.25);
    v.tens2 = (v.tens1 - 1) / (p.tensgn + 1) + 1;
    v.tens = v.tens1 * v.rad;
    v.rad = v.rad + (v.tens2 - v.tens) / p.tenstc * p.i2;
    v.myogrs = pow(1 / v.rad, 4);
  }

  /* The Korner effect
     See: Jennings GL, Nelson L, Esler MD, Leonard P, Korner PI, 1984.
          Effects of changes in physical activity on blood pressure and
          sympathetic tone. J Hypertens Suppl 2, S139-141. */
  v.pamkrn = 1;
  if (v.korner > 0) {
    v.pamk = v.pamk + ((v.pa / 100 - 1) * p.korgn + 1 - v.pamk) / \
              p.kortc * v.i;
    if (v.korren > 0) {
      v.pamkrn = v.pamk;
    }
  }

  /* The effect of pressure on pumping in the left ventricle. */
  v.pa2 = v.pa / v.auh / v.osa;
  funct(&v.pa2, &v.lvm, fun1, 12);
  v.vre = v.vra - 0.1; /* Calculate the excess volume. */
  /* Pressure is the excess volume divided by compliance. */
  v.pra = v.vre / 0.005;
  v.pra1 = (v.pra + 8) * (p.htauml * (v.au - 1) + 1) - 8;
  funct(&v.pra1, &v.qrn, fun2, 10); /* Starling right heart. */

  /* The pulmonary vasculature. */
  v.vpe = v.vpa - 0.30625; /* The excess volume. */
  v.ppa = v.vpe / 0.0048; /* The pressure due to the excess volume. */
  /* The effect of pressure on pulmonary arterial distension. */
  v.pp1 = 0.026 * v.ppa;
  if (v.pp1 < 1e-5) {
    v.pp1 = 1e-5;
  }
  v.cpa = pow(v.pp1, 0.5);
  v.rpa = 1 / v.cpa;

  /* The effect of pressure on pumping in the right ventricle. */
  v.pp2 = v.ppa / v.auh / v.osa;
  funct(&v.pp2, &v.rvm, fun3, 12);

  /* The left atrium. */
  v.vle = v.vla - 0.38; /* The excess volume. */
  v.pla = v.vle / 0.01; /* The pressure due to excess volume. */
  v.pla1 = (v.pla + 4) * (p.htauml * (v.au - 1) + 1) - 4;
  funct(&v.pla1, &v.qln, fun4, 10); /* Starling left heart. */

  /* The effect of atrial pressure on pulmonary venous distension. */
  v.pl1 = v.pla + 18;
  v.rpv = 1 / v.pl1 / 0.0357;
  /* Total pulmonary resistance, derived from pressure difference and flow. */
  v.rpt = v.rpv + v.rpa;
  v.pgl = v.ppa - v.pla;
  v.qpo = v.pgl / v.rpt;

  /* The venous system. */
  /* The unstressed volume is a function of angiotensin, stress relaxation
     and volume receptors. */
  v.vve = v.vvs - v.vvr - (v.anu - 1) * p.any \
           - v.vv7 - v.vv6 - v.atrvfb;
  /* The pressure due to excess volume. */
  v.pvs = 3.7 + (v.vve - 0.74) / p.cv;
  if (v.pvs < 1e-4) {
    v.pvs = 1e-4;
  }
  v.pr1 = v.pra;
  if (v.pra < p.pr1ll) {
    v.pr1 = p.pr1ll; /* The phenomenon of venous collapse. */
  }
  v.pgv = v.pvs - v.pr1;
  /* Venous pressure and resistance. */
  v.rvg = 0.74 / pow((v.pvs / v.vim / 3.7), 0.5);
  v.qvo = v.pgv / v.rvg;
  v.cn3 = v.cn3 + (((v.pc - 17) * p.cn7 + 17) * p.cn2 - v.cn3) * 0.1;
  v.rv1 = p.rvsm / v.cn3;
  v.rvs = v.ave * v.rv1 * v.vim * v.anuvn;
  v.pgs = v.pa - v.pvs;

  /* Systemic resistances. */
  v.r1 = v.anu * v.adhmv * v.aum * v.vim * v.pamk / v.pam / v.atrrfb;
  /* Non-muscular, non-renal resistance. */
  v.rsn = p.rar * v.arm * v.r1 * v.myogrs * v.rmult1 + v.rvs * 1.79;
  v.bfn = v.pgs / v.rsn;
  /* Muscular resistance. */
  v.rsm = v.amm * v.ram * v.r1 * v.myogrs * v.rmult1;
  v.bfm = v.pgs / v.rsm;

  /* Systemic blood flows. */
  v.fisflo = (v.pa - v.pra) * p.fis; /* The (optional) AV fistula. */
  v.sysflo = v.bfm + v.bfn + v.rbf;
   /* Cardiac aortic output. */
  v.qao = v.sysflo + v.fisflo;
  /* Left ventricular output. */
  v.qlo = v.lvm * v.qln * v.auh * p.hsl * v.hmd * v.hpl;
  /* Passive flow due to elevated pressure in the left atrium. */
  v.qlo1 = (v.pla - v.pa) / 3;
  if (v.qlo1 > 0) {
    v.qlo = v.qlo + v.qlo1;
  }
  v.hpef = (1 - p.qrf) * v.auh * v.rvm * p.hsr * v.hmd * v.hpr \
            + p.qrf * v.qlo / v.qln;
  /* Right ventricular output. */
  v.qro = v.qrn * v.hpef;
  v.qpo = v.qlo + (v.qpo - v.qlo) / p.u;
  v.qvo = v.qro + (v.qvo - v.qro) / p.x;

  /* The volume change in each section of the circulation is calculated as:
     the inflow minus the outflow. */
  v.dvs = v.qao - v.qvo; /* Venous volume. */
  v.dpa = v.qro - v.qpo; /* Pulmonary volume. */
  v.das = v.qlo - v.qao; /* Arterial volume. */
  v.dla = v.qpo - v.qlo; /* Left atrium volume. */
  v.dra = v.qvo - v.qro; /* Right atrium volume. */
}
