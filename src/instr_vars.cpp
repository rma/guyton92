#include <iostream>
#include <vector>
#include <string>
using namespace std;

#include "params.h"
#include "vars.h"
#include "instr_vars.h"

/**
 * This struct type stores the options for this instrument.
 */
struct INSTR_VARS_OPTIONS {
  char *sep; /** The field separator. */
  const std::vector<std::string> *vars; /** The model variables to output. */
};

/**
 * The options for this instrument are:
 *
 * @param[in] sep The field separator for the output. If this is \c NULL,
 *                the space character is used.
 * @param[in] vars The names of the model variables to output for each state
 *                 notification. If this is \c NULL, no output is produced.
 */
void *instr_vars_opts(char* sep, const std::vector<std::string> *vars) {
  INSTR_VARS_OPTIONS *opts = new INSTR_VARS_OPTIONS;
  opts->sep = sep;
  opts->vars = vars;
  return (void *) opts;
}

/**
 * This instrument prints the time and an arbitrary list of model outputs.
 *
 * @param[in] p The struct of model parameters.
 * @param[in] v The struct of state variables.
 * @param[in] data The options for the instrument (see instr_vars_opts()).
 *
 * \ingroup instruments
 */
bool instr_vars(const PARAMS &p, const VARS &v, void *data) {
  static bool first_time = true;
  char *sep = (char *) " ";

  /* If no options were provided, this instrument can't produce any output. */
  if (! data) {
    return false;
  }

  INSTR_VARS_OPTIONS *opts = (INSTR_VARS_OPTIONS *) data;
  if (opts->sep) {
    sep = opts->sep;
  }

  /* Print the column headers. */
  if (first_time) {
    first_time = false;

    cout << "t";
    for (int i = 0; i < (int) opts->vars->size(); i++) {
      cout << sep << opts->vars->at(i);
    }
    cout << endl;
  }

  /* Print the renal module outputs. */
  cout.setf(ios::left);
  cout << v.t;
  for (int i = 0; i < (int) opts->vars->size(); i++) {
    const char* name = opts->vars->at(i).c_str();
    double value = get_var(v, name);
    cout << sep << value;
  }
  cout << endl;

  return true;
}
