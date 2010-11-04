#include <cstdlib>
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



/*
 * Instrumentation code.
 */

/**
 * Each instrument is stored as a function pointer to the instrumentation code
 * and a pointer to data that is specific to that instrument. The list of all
 * registered instruments is stored as a singly-linked list, since there is no
 * need to traverse the list in reverse.
 */
struct list_item {
  bool (*notify)(const PARAMS &p, const VARS &v, void *data);
  void *data;
  list_item *next;
};

/**
 * The list of instruments that have been registered.
 */
list_item *instruments;

/**
 * This function registers an instrument to be notified of the model state at
 * regular intervals.
 *
 * @param[in] instr A pointer to the instrument function.
 * @param[in] data A pointer to the instrument-specific data (if any).
 *
 * @return \c true if the instrument was registered successfully, and \c false
 *         if there is insufficient memory available.
 */
bool add_instrument(instrument instr, void *data) {
  /* Allocate memory for this instrument and check that it succeeded. */
  list_item *entry = (list_item *) malloc( sizeof(list_item) );
  if (! entry) {
    return false;
  }

  /* Populate the instrument with the appropriate details. */
  entry->notify = instr;
  entry->data = data;
  entry->next = instruments;

  /* Add the instrument to the head of the list. */
  instruments = entry;
  return true;
}

/**
 * This function notifies all registered instruments of the current model
 * state.
 *
 * @param[in] p The struct of model parameters.
 * @param[in] v The struct of state variables.
 */
void notify_instruments(const PARAMS &p, const VARS &v) {
  list_item *curr = instruments;
  list_item *prev = NULL;
  list_item *next = NULL;
  bool keep = false;

  /* Notify each instrument in turn. */
  while (curr) {
    /* Notify the instrument and determine if it should be retained.*/
    keep = curr->notify(p, v, curr->data);
    /* Update the pointer to the next instrument. */
    next = curr->next;

    if (keep) {
      /* Update the pointers to the previous and current instruments. */
      prev = curr;
      curr = next;
    } else {
      /* Remove the instrument and update the pointers. */
      free(curr);
      curr = next;
      if (prev) {
        /* A previous instrument exists, so update it's next pointer. */
        prev->next = next;
      } else {
        /* No previous instrument, so the next one becomes the first. */
        instruments = next;
      }
    }
  }
}
