/**
 * A modular C++ implementation of the Guyton 1992 model.
 *
 * Robert Moss, 2010.
 *
 * Based on the original monolithic Fortran 77 code and a faithful translation
 * of this code to C++ by Thibault Grosse.
 *
 * The intention of this work is to develop a significantly more detailed model
 * of renal function to replace the renal block of the Guyton 1992 model.
 */

#include <iostream>
#include <cstdlib>
using namespace std;

/* Collect parameters into a single struct and allow parameter values to be
   specified in an external file. */
#include "read_params.h"
/* Collect state variables into a single struct and allow the initial values
   to be specified in an external file. */
#include "read_vars.h"

/* The renal module, separated from the original (monolithic) code. */
#include "module_renal.h"
/* The circulatory dynamics module. */
#include "module_circdyn.h"
/* The autonomic circulation module. */
#include "module_autonom.h"
/* The aldosterone module. */
#include "module_aldost.h"
/* The angiotensin module. */
#include "module_angio.h"
/* The atrial natriuretic peptide (ANP) module. */
#include "module_anp.h"
/* The red blood cell (RBC) and viscosity module. */
#include "module_rbc.h"
/* The oxygen delivery and blood flow autoregulation module. */
#include "module_o2deliv.h"
/* The volume receptors module. */
#include "module_volrec.h"
/* The antidiuretic hormone (ADH) module. */
#include "module_adh.h"
/* The stress relaxation module. */
#include "module_stress.h"
/* The thirst drive module. */
#include "module_thirst.h"
/* The baroreceptor adaptation module. */
#include "module_baro.h"
/* The special circulation variables module. */
#include "module_special.h"
/* The capillary dynamics module. */
#include "module_capdyn.h"
/* The pulmonary fluid dynamics module. */
#include "module_puldyn.h"
/* The electrolytes module. */
#include "module_electro.h"

/* An experiment in rapid autoregulation. */
#include "exp_rapidreg.h"
/* An experiment in transfusion and blood loss. */
#include "exp_transfuse.h"

/* The debugging and instrumentation module. */
#include "debug.h"
/* A filter to reduce the number of notifications. */
#include "filter_times.h"
/* An instrument to print the renal module outputs. */
#include "instr_renal.h"

/**
 * The entry point for the modular Guyton 1992 model.
 *
 * @param[in] argc The number of arguments on the command line.
 * @param[in] argv The arguments provided on the command line.
 */
int main(int argc, char *argv[]) {
  /* This struct holds pointers to all model parameters. */
  PARAMS p;
  /* This struct holds pointers to all state variables. */
  VARS v;

  /* Initialise the PARAMS struct (p). */
  PARAMS_INIT(p);
  /* Initialise the VARS struct (V). */
  VARS_INIT(v);

  /* Allow parameter values to be defined in an external file. */
  switch (argc) {
  case 1:
    /* No arguments, only the command name. */
    break;
  case 2:
    /* One argument, which specifies the parameters file. */
    read_params(p, argv[1]);
    break;
  default:
    cerr << "USAGE: " << argv[0] << " [parameter file]" << endl;
    return EXIT_FAILURE;
  }

  /* The simulation begins at time t = 0. */
  v.t = 0.0;
  /* The time-step size (min). */
  v.i = 0.0030;
  /* The duration of the simulation (min). The default value is four weeks. */
  double tend = 60 * 24 * 7 * 4;

  /* Filter the notifications. */
  bool initial_notification = true;
  add_filter(filter_times, &initial_notification);
  /* Display the outputs of the renal module. */
  add_instrument(instr_renal, NULL);

  /* Notify all registered instruments of the initial model state. */
  notify_instruments(p, v);

  /* The main simulation loop. */
  while (v.t < tend) {
    /* Disable autoregulation if AURG is positive. */
    if (v.aurg <= 0) {
      p.poz = 0;
      p.pon = 0;
      p.pok = 0;
      p.pom = 0;
      p.pom2 = 0;
    }

    /* RPS is described as "double extra-renal resistance". */
    if (v.rps > 0) {
      p.rar = 60;
      v.ram = 180;
    }

    /* Simulate each module of the Guyton 1992 model in turn. */
    module_circdyn(p, v);
    if (! module_autonom(p, v)) {
      /* If the autonomic circulation control module fails a stability check,
         we restart the main simulation loop.
         NOTE: this is the module that increases the simulation time. */
      continue;
    }
    module_aldost(p, v);
    module_angio(p, v);
    module_anp(p, v);
    module_rbc(p, v);
    module_o2deliv(p, v);
    module_volrec(p, v);
    module_adh(p, v);
    module_stress(p, v);
    module_thirst(p, v);
    module_baro(p, v);
    module_special(p, v);
    module_capdyn(p, v);
    module_puldyn(p, v);
    module_renal(p, v);
    module_electro(p, v);

    /* Perform any experiments that have been defined. */
    exp_rapidreg(p, v);
    exp_transfuse(p, v);

    /* Notify all registered instruments of the current model state. */
    notify_instruments(p, v);
  }

  return EXIT_SUCCESS;
}
