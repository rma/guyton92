#include <iostream>
#include <iomanip>
using namespace std;

#include "params.h"
#include "vars.h"
#include "instr_pa.h"

/**
 * This instrument prints the time and the arterial pressure.
 *
 * @param[in] p The struct of model parameters.
 * @param[in] v The struct of state variables.
 * @param[in] data This parameter is ignored.
 *
 * \ingroup instruments
 */
bool instr_pa(const PARAMS &p, const VARS &v, void *data) {
  cout.setf(ios::left);
  cout << "t: " << setw(7) << v.t << "\tpa: " << setw(7) << v.pa << endl;
  return true;
}
