#include <iostream>
#include <iomanip>
using namespace std;

#include "params.h"
#include "vars.h"
#include "instr_renal.h"

/**
 * This instrument prints the time, arterial pressure and renal module outputs.
 *
 * @param[in] p The struct of model parameters.
 * @param[in] v The struct of state variables.
 * @param[in] data The field separator for the output. If this is \c NULL,
 *            the space character is used.
 *
 * <b>Renal module outputs:</b>
 * - \b KOD: the rate of potassium excretion (mEq/min).
 * - \b NOD: the rate of sodium excretion (mEq/min).
 * - \b VUD: the rate of water excretion (L/min).
 * - \b RBF: the rate of renal blood flow (L/min).
 * - \b MDFLW: the normalized tubular flow at the macula densa (no units).
 * - \b I5: the time-step variable for the renal autoregulation feedback loop
 *   (min).
 */
bool instr_renal(const PARAMS &p, const VARS &v, void *data) {
  static bool first_time = true;
  char *sep = (char *) " ";

  if (data) {
    /* If data is not null, use the specified separator. */
    sep = (char *) data;
  }

  if (first_time) {
    /* Print the column headers. */
    cout << "t" << sep << "pa" << sep << "kod" << sep << "nod" << sep;
    cout << "vud" << sep << "rbf" << sep << "mdflw" << sep << "i5" << endl;
    first_time = false;
  }

  /* Print the renal module outputs. */
  cout.setf(ios::left);
  cout << v.t << sep << v.pa << sep << v.kod << sep << v.nod << sep;
  cout << v.vud << sep << v.rbf << sep << v.mdflw << sep << v.i5 << endl;
  return true;
}
