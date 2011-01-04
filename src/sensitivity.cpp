/**
 * @file
 * A program to perform sensitivity analyses of individual modules of the
 * Guyton model. Note that these analyses do not take the remaining modules
 * into account and therefore will generally produce output that differs
 * greatly from that observed when simulating the entire Guyton model.
 */

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

#include <sys/stat.h>
#include <errno.h>

using namespace std;

/* Collect parameters into a single struct and allow parameter values to be
   specified in an external file. */
#include "read_params.h"
/* Collect state variables into a single struct and allow the initial values
   to be specified in an external file. */
#include "read_vars.h"

#include "sensitivity.h"

/* Include all of the modules. */
#include "module_renal.h"
#include "module_circdyn.h"
#include "module_autonom.h"
#include "module_aldost.h"
#include "module_angio.h"
#include "module_anp.h"
#include "module_rbc.h"
#include "module_o2deliv.h"
#include "module_volrec.h"
#include "module_adh.h"
#include "module_stress.h"
#include "module_thirst.h"
#include "module_baro.h"
#include "module_special.h"
#include "module_capdyn.h"
#include "module_puldyn.h"
#include "module_electro.h"
#include "module_kidney.h"

/**
 * The map of module names to module functions.
 */
map<string,modulefn> modules;

/**
 * Determine whether a double is not a number (NaN) and therefore invalid.
 * This is used to check the return value of get_var(), to determine whether
 * a name specifies a model variable or a model parameter.
 */
inline bool isnan(double x) {
    return x != x;
}

/**
 * The entry point for the sensitivity analysis program.
 *
 * @param[in] argc The number of arguments on the command line.
 * @param[in] argv The arguments provided on the command line.
 */
int main(int argc, char *argv[]) {
  /* Validate the command line arguments. In addition to the command there are:
     an input name; minimum value; increment; maximum value; output name(s). */
  if (argc < 7) {
    cerr << "USAGE: " << argv[0];
    cerr << " module_name param_name min_val incr max_val output(s) " << endl;
    return EXIT_FAILURE;
  }

  /* Extract the arguments. */
  char* control_var = argv[2];
  double min_val, inc_val, max_val;
  char** outputs = &argv[6];
  int output_count = argc - 6;

  /* Check that the minimum, increment and maximum values of the control
     parameter are valid. */
  if (1 != sscanf(argv[3], "%lf", &min_val)) {
    cerr << "ERROR: Incorrect numerical value '" << argv[3] << "'" << endl;
    return EXIT_FAILURE;
  }
  if (1 != sscanf(argv[4], "%lf", &inc_val)) {
    cerr << "ERROR: Incorrect numerical value '" << argv[4] << "'" << endl;
    return EXIT_FAILURE;
  }
  if (1 != sscanf(argv[5], "%lf", &max_val)) {
    cerr << "ERROR: Incorrect numerical value '" << argv[5] << "'" << endl;
    return EXIT_FAILURE;
  }

  /* Retrieve the function pointer for the specified module. */
  init_modules();
  string modname(argv[1]);
  modulefn module = modules[modname];

  /* The basename of the output files produced by this program. */
  string basename (modname + "_" + control_var);

  /* Analyse the module dynamics over the range of control values. */
  string datafile (basename + ".ssv");
  cout << datafile << " ... ";
  cout.flush();
  analyse(module, control_var, min_val, inc_val, max_val, outputs,
          output_count, datafile);
  cout << "done" << endl;

  /* Create a gnuplot script to plot the results. */
  string scriptfile (basename + ".gp");
  cout << scriptfile << " ... ";
  cout.flush();
  gnuplot(control_var, output_count, scriptfile, datafile, basename + ".eps");
  cout << "done" << endl;
}

/**
 * Populate the module map with all of the Guyton model modules.
 */
void init_modules() {
  modules.insert(pair<string,modulefn>("renal", module_renal));
  modules.insert(pair<string,modulefn>("circdyn", module_circdyn));
  modules.insert(pair<string,modulefn>("autonom", (modulefn) module_autonom));
  modules.insert(pair<string,modulefn>("aldost", module_aldost));
  modules.insert(pair<string,modulefn>("angio", module_angio));
  modules.insert(pair<string,modulefn>("anp", module_anp));
  modules.insert(pair<string,modulefn>("rbc", module_rbc));
  modules.insert(pair<string,modulefn>("o2deliv", module_o2deliv));
  modules.insert(pair<string,modulefn>("volrec", module_volrec));
  modules.insert(pair<string,modulefn>("adh", module_adh));
  modules.insert(pair<string,modulefn>("stress", module_stress));
  modules.insert(pair<string,modulefn>("thirst", module_thirst));
  modules.insert(pair<string,modulefn>("baro", module_baro));
  modules.insert(pair<string,modulefn>("special", module_special));
  modules.insert(pair<string,modulefn>("capdyn", module_capdyn));
  modules.insert(pair<string,modulefn>("puldyn", module_puldyn));
  modules.insert(pair<string,modulefn>("electro", module_electro));
  modules.insert(pair<string,modulefn>("kidney", module_kidney));
}

/**
 * Perform a sensitivity analysis of a single module of the Guyton model.
 *
 * @param[in] f The function pointer of the module to analyse.
 * @param[in] control_var The name of the control variable/parameter.
 * @param[in] min_val The minimum value of the control.
 * @param[in] inc_val The amount by which the control is incremented.
 * @param[in] max_val The maximum value of the control.
 * @param[in] outputs The names of the output variables.
 * @param[in] output_count The number of output variables.
 * @param[in] outfile The name of the file where the output will be written.
 */
void analyse(modulefn f, char* control_var, double min_val, double inc_val,
             double max_val, char** outputs, int output_count, string outfile) {
  /* This struct holds pointers to all model parameters. */
  PARAMS p;
  /* This struct holds pointers to all state variables. */
  VARS v;
  /* The field separator for the sensitivity analysis output. */
  const char output_sep[] = " ";

  /* Determine whether the control is a variable or parameter of the model. */
  bool is_var = true;
  if (isnan(get_var(v, control_var))) {
    is_var = false;
  }

  ofstream out_data (outfile.c_str());
  out_data.setf (ios::scientific);

  /* Print the column titles. */
  out_data << control_var << output_sep << control_var << output_sep;
  for (int i = 0; i < output_count; i++) {
    out_data << outputs[i] << output_sep;
  }
  out_data << endl;

  /* Initialise the model */
  PARAMS_INIT(p);
  VARS_INIT(v);
  f(p, v);

  /* Record the output for each value of the control. */
  for (double x = min_val; x < (max_val + inc_val); x += inc_val) {
    /* Initialise the model */
    PARAMS_INIT(p);
    VARS_INIT(v);

    /* Set the next value of the control variable. */
    if (is_var) {
      set_var(v, control_var, x);
    } else {
      set_param(p, control_var, x);
    }

    /* Run the module. */
    f(p, v);

    /* Output the results. */
    double norm_x = (x - min_val) / (max_val - min_val);
    out_data << x << output_sep << norm_x << output_sep;
    for (int i = 0; i < output_count; i++) {
      out_data << get_var(v, outputs[i]) << output_sep;
    }
    out_data << endl;
  }
  out_data.close();
}

/**
 * Produce a gnuplot script to plot the results of a sensitivity analysis.
 *
 * @param[in] control_var The name of the control variable/parameter.
 * @param[in] output_count The number of output variables.
 * @param[in] scriptfile The name of the gnuplot script file to be created.
 * @param[in] datafile The name of the file containing the analysis results.
 * @param[in] epsfile The name of the .eps file to contain the resulting plot.
 */
void gnuplot(char* control_var, int output_count, string scriptfile,
             string datafile, string epsfile) {
  char const *out_file = scriptfile.c_str();
  ofstream out_script (out_file);

  /* Set some basic gnuplot options. */
  out_script << "#!/usr/bin/gnuplot -p" << endl;
  out_script << "set terminal postscript monochrome rounded 32" << endl;
  out_script << "set output '" << epsfile << endl;
  out_script << "set xlabel '" << control_var << "'" << endl;
  /* Graph each of the outputs on the same plot. */
  out_script << "plot '" << datafile << "' using 1:3 title column(2)";
  out_script << " with lines lw 4";
  for (int i = 1; i < output_count; i++) {
    out_script << ", '' using 1:" << i+3 << " title column(" << i+3 << ")";
    out_script << " with lines lw 4";
  }
  out_script << endl;

  out_script.close();

  /* Set the user-execute permission for the gnuplot script. */
  mode_t mode = S_IXUSR | S_IWUSR | S_IRUSR;
  struct stat buf;
  /* Get the current permissions. */
  int result = stat(out_file, &buf);
  if (0 == result) {
    /* Add the user-execute permission. */
    mode |= buf.st_mode;
  } else {
    cout << strerror(errno) << endl;
  }
  /* Update the permissions. */
  result = chmod(out_file, mode);
  if (0 != result) {
    cout << strerror(errno) << endl;
  }
}
