#include <cstdlib>
#include <iostream>

#include "model_moore94.cpp"

using namespace std;

int main(int argc, char *argv[]) {
  /* This struct holds pointers to all model parameters. */
  PARAMS p;
  /* This struct holds pointers to all state variables. */
  VARS v;

  /* Initialise the PARAMS struct (p). */
  PARAMS_INIT(p);
  /* Initialise the VARS struct (V). */
  VARS_INIT(v);

  cout << "PA\tGFR\tQalh\tNaMD\tdR_TGF\tR_A" << endl;

  for (double pa = 80; pa <= 160; pa += 0.5) {
    v.Pas = pa;
    solve_moore94_model(p, v);
    cout << pa << "\t" << v.GFR << "\t" << v.Qalh << "\t" << v.Ci << "\t" << v.dRtgf << "\t" << v.Ra << endl;
  }

  return 0;
}
