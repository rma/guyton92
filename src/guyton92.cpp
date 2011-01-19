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
#include <sstream>
#include <queue>
#include <vector>
#include <getopt.h>

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
 * Displays the command-line usage for the model, then exits.
 *
 * @param progname The name of the program.
 * @param exitcode The exit code to pass to exit().
 */
void usage(char* progname, int exitcode) {
  cerr << "\n  USAGE: " << progname << " [options] [parameter file]" << endl;
  cerr << "\n  OPTIONS:" << endl;
  cerr << "    -a, --no-filter     " <<
    "Display the output variables after each time-step." << endl;
  cerr << "    -o, --outputs=VARS  " <<
    "Set the output variables (comma-separated list)." << endl;
  cerr << "    -h, --help          " <<
    "Display this help and exit." << endl;
  cerr << "\n  EXAMPLE:" << endl;
  cerr << "    " << progname << " -a -o pa,rbf,vud ./exps/hypertension.exp\n";
  cerr << endl;
  exit(exitcode);
}

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

  /* Options that can be set by command-line parameters. */
  bool use_filter = true; /* Whether or not to filter notifications. */
  bool use_outs = false; /* Whether output variables have been specified. */
  vector<string> outs; /* The specified output variables. */

  static struct option long_options[] = {
    /* Options are distinguished by a single character. */
    {"no-filter", no_argument,       0, 'a'},
    {"outputs",   required_argument, 0, 'o'},
    {"help",      no_argument,       0, 'h'},
    {NULL, NULL, NULL, NULL}
    };
  int option_index = 0;
  int c;

  /* Process every parameter that has been given. */
  while (1) {
    c = getopt_long(argc, argv, "hao:", long_options, &option_index);
    if (c == -1) {
      break; /* No more parameters to process. */
    }

    /* Local variables for processing the list of output variables. */
    istringstream ss;
    string outname;

    switch (c) {
    case 'a':
      /* Don't filter the notifications of the model state. */
      use_filter = false;
      break;
    case 'o':
      /* Use the output variables specified on the command line. */
      use_outs = true;
      ss.str(optarg);
      while (getline(ss, outname, ',')) {
        outs.push_back(outname);
      }
      break;
    case 'h':
      /* Print the usage information. */
      usage(argv[0], EXIT_SUCCESS);
      break;
    case ':':
    case '?':
    default:
      /* Incorrect usage, print the usage information. */
      usage(argv[0], EXIT_FAILURE);
      break;
    }
  }

  /* Shift argc and argv so that the processed parameters are ignored. */
  argc -= optind;
  argv += optind;

  /* Allow an experiment definition to be provided in an external file. */
  Experiment *e = NULL;
  ifstream input;
  switch (argc) {
  case 0:
    /* No experiment definition. */
    break;
  case 1:
    /* One argument, which specifies the experiment file. */
    input.open(argv[0]);
    if (input.fail()) {
      cerr << "ERROR: Unable to open experiment: '" << argv[0] << "'" << endl;
      exit(EXIT_FAILURE);
    } else {
      e = new Experiment(p, input);
    }
    input.close();
    break;
  default:
    /* Incorrect usage, print the usage information. */
    usage(argv[-optind], EXIT_FAILURE);
  }

  /* The simulation begins at time t = 0. */
  v.t = 0.0;
  /* The time-step size (min). */
  v.i = 0.0030;
  /* The duration of the simulation (min). The default value is four weeks. */
  double tend = 60 * 24 * 7 * 4;
  if (e) {
    tend = e->stop_at();
    e->write_exp(cout);
  }

  /* Filter the notifications. */
  const double *output_times = NULL;
  if (use_filter) {
    output_times = (e) ? e->output_times() : NULL;
    add_filter(filter_times, (void *) output_times);
  }
  /* Display the specified model outputs. */
  vector<string> const *outputs =
    (use_outs) ? &outs : (e) ? &e->output_vars() : NULL;
  add_instrument(instr_vars, instr_vars_opts(NULL, outputs));

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
