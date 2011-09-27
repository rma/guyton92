#include <queue>
#include <string>
#include <iostream>
#include <fstream>
#include <cfloat>
#include <cstdio>

using namespace std;

/* Collect parameters into a single struct. */
#include "params.h"
/* Collect state variables into a single struct. */
#include "vars.h"
/* Parse experiment definitions and automatically update model parameters. */
#include "read_exp.h"

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
/* The replacement for the original renal module. */
#include "module_kidney.h"

/* An experiment in rapid autoregulation. */
#include "exp_rapidreg.h"
/* An experiment in transfusion and blood loss. */
#include "exp_transfuse.h"

/* The debugging and instrumentation module. */
#include "debug.h"

/**
 * Simulates a single time-step of the model.
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 * @param[in] e      The chosen experiment (if any) to run.
 */
extern "C" void guyton92_step(PARAMS &p, VARS &v, Experiment *e) {
  if (e) {
    e->update(v.t);
  }

  /* Disable autoregulation if AURG is negative. */
  if (v.aurg <= 0) {
    p.poz = 0;
    p.pon = 0;
    p.pok = 0;
    p.pom = 0;
    p.pom2 = 0;
  }

  /* RPS is described as "doubling the extra-renal resistance". */
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
    return;
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
  if (p.newkidney) {
    module_kidney(p, v); /* Run the replacement renal module. */
  } else {
    module_renal(p, v); /* Run the original renal module. */
  }
  module_electro(p, v);

  /* Perform any experiments that have been defined. */
  exp_rapidreg(p, v);
  exp_transfuse(p, v);

  /* Notify all registered instruments of the current model state. */
  notify_instruments(p, v);
}

/**
 * Creates a new set of parameters.
 */
extern "C" PARAMS * new_params() {
  PARAMS *p = new PARAMS;
  PARAMS_INIT ((*p));
  return p;
}

/**
 * Creates a new set of variables.
 */
extern "C" VARS * new_vars() {
  VARS *v = new VARS;
  VARS_INIT ((*v));
  return v;
}

/**
 * Deletes an existing set of parameters.
 */
extern "C" void del_params(PARAMS * p) {
  delete p;
}

/**
 * Deletes an existing set of variables.
 */
extern "C" void del_vars(VARS * v) {
  delete v;
}

/**
 * Opens an existing file for (textual) input.
 */
extern "C" istream * exp_file_stream(const char* filename) {
  ifstream* input = new ifstream();
  input->open(filename);
  if (input->fail()) {
    return NULL;
  } else {
    return input;
  }
}

/**
 * Creates a new experiment, whose definition is parsed from the input stream.
 */
extern "C" Experiment * exp_new(PARAMS &p, istream &input) {
  return new Experiment(p, input);
}

/**
 * Deletes an existing experiment.
 */
extern "C" void exp_delete(Experiment *e) {
  delete e;
}

/**
 * Check whether the experiment encountered a parsing error.
 */
extern "C" bool exp_failed(Experiment *e) {
  if (e) {
    return e->failed();
  } else {
    return true;
  }
}

/**
 * Returns a message that describes the error in the experiment definition.
 */
extern "C" const char * exp_errmsg(Experiment *e) {
  if (e) {
    const string *errstr = e->errmsg();
    if (errstr) {
      return errstr->c_str();
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}

/**
 * Returns the time at which the experiment should be stopped (mins).
 */
extern "C" double exp_stop_at(Experiment *e) {
  if (e) {
    return e->stop_at();
  } else {
    return -1.0;
  }
}

/**
 * Returns the number of variables whose values should be logged.
 */
extern "C" int exp_output_count(Experiment *e) {
  if (e) {
    return e->output_vars().size();
  } else {
    return -1;
  }
}

/**
 * Returns the name of a variable whose value should be logged.
 */
extern "C" const char * exp_output_var(Experiment *e, unsigned int i) {
  if (e) {
    if (i < e->output_vars().size()) {
      return e->output_vars()[i].c_str();
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}

/**
 * Returns the times at which the model outputs should be recorded. The array
 * is terminated by the sentinel value \c exp_sentinel_time().
 */
extern "C" const double * exp_output_times(Experiment *e) {
  if (e) {
    return e->output_times();
  } else {
    return NULL;
  }
}

/**
 * The sentinel value for the output times returned by \c exp_output_times().
 */
extern "C" double exp_sentinel_time() {
  return DBL_MAX;
}
