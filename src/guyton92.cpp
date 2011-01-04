/**
 * @file
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

#include <cstdlib>
#include <iostream>
#include <fstream>
#include <queue>

using namespace std;

/* Collect parameters into a single struct and allow parameter values to be
   specified in an external file. */
#include "read_params.h"
/* Collect state variables into a single struct and allow the initial values
   to be specified in an external file. */
#include "read_vars.h"
/* Parse experiment definitions and automatically update model parameters. */
#include "read_exp.h"

/* Simulate a single time-step of the model. */
#include "guyton92_step.h"

/* The debugging and instrumentation module. */
#include "debug.h"
/* A filter to reduce the number of notifications. */
#include "filter_times.h"
/* An instrument to print an arbitrary list of module outputs. */
#include "instr_vars.h"

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

  Experiment *e = NULL;
  ifstream input;

  /* Allow parameter values to be defined in an external file. */
  switch (argc) {
  case 1:
    /* No arguments, only the command name. */
    break;
  case 2:
    /* One argument, which specifies the experiment file. */
    input.open(argv[1]);
    e = new Experiment(p, input);
    input.close();
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
  if (e) {
    tend = e->stop_at();
  }

  /* Filter the notifications. */
  const double *output_times = (e) ? e->output_times() : NULL;
  add_filter(filter_times, (void *) output_times);
  /* Display the specified model outputs. */
  const vector<string> *outputs = (e) ? &e->output_vars() : NULL;
  void* opts = instr_vars_opts(NULL, outputs);
  add_instrument(instr_vars, opts);

  /* Notify all registered instruments of the initial model state. */
  notify_instruments(p, v);

  /* The main simulation loop. */
  while (v.t < tend) {
    guyton92_step(p, v, e);
  }

  if (output_times) {
    delete[] output_times;
  }
  delete e;
  return EXIT_SUCCESS;
}
