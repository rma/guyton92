#include <cstdio>

#include "params.h"
#include "vars.h"
#include "debug.h"

/**
 * The output stream to which the debugging information will be printed.
 */
FILE *out = stderr;

/**
 * The maximum number of times to print the model state in a single execution.
 * Printing the model state at every time-step drastically slows down the
 * simulation, and any differences between the monolithic and modular builds
 * of the model should rapidly become apparent.
 */
#define MAX_PRINTS 50

/**
 * Sets the output stream to which the debugging information is printed.
 * The default setting is to print debugging information to \c stderr.
 *
 * @param[in] stream The output stream to use for printing.
 */
void set_debug_stream(FILE *stream) {
  /* Ensure that the steam is not NULL. */
  if (stream) {
    out = stream;
  }
}

/**
 * This function outputs the entire model state (ie, all parameters and state
 * variables) in order to assist with debugging. The value of each parameter
 * and variable is printed on a separate line, so that discrepancies between
 * model states can be easily identified with line-oriented tools (eg, diff).
 *
 * @param[in] p      The struct of model parameters.
 * @param[in] v      The struct of state variables.
 * @param[in] prefix The (optional) prefix for each line of output. Set this
 *                   to \c NULL when no prefix is desired.
 */
void print_model_state(const PARAMS &p, const VARS &v, char *prefix) {
  /* Avoid printing out the model state at every time-step. */
  static int count = 0;
  if (count++ > MAX_PRINTS) {
    return;
  }

  /* The parameters and variables structs only contain pointers to doubles.
     Therefore, we can typecast them to arrays of pointers to doubles, as
     long as we don't walk off the end of the array. */
  double **params = (double **) &p;
  double ** vars = (double **) &v;

  /* Print the value of each parameter on a separate line. */
  for (int i = 0; i < PARAM_COUNT; i++) {
    if (prefix) {
      fprintf(out, "%s %e\n", prefix, *params[i]);
    } else {
      fprintf(out, "%e\n", *params[i]);
    }
  }

  /* Print the value of each state variable on a separate line. */
  for (int i = 0; i < VAR_COUNT; i++) {
    if (prefix) {
      fprintf(out, "%s %e\n", prefix, *vars[i]);
    } else {
      fprintf(out, "%e\n", *vars[i]);
    }
  }
}
