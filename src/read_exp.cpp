/**
 * @file
 * Provides the Experiment class, for changing parameters over time.
 */

#include <queue>
#include <vector>
#include <iostream>
#include <istream>
#include <ostream>
#include <string>
#include <sstream>
#include <cstring>
#include <cfloat>

#include "read_params.h"
#include "read_exp.h"

using namespace std;

/**
 * @class Experiment
 *
 * In the context of the Guyton model, and experiment is a simulation where
 * parameter values can be changed at predefined times during the simulation.
 *
 * The Experiment class is responsible for processing the definition of such an
 * experiment and updating the model parameters accordingly.
 *
 * @code
 * ifstream input(input_file);
 * Experiment exp(params, input);
 *
 * while (t < exp.stop_at()) {
 *   exp.update(t);
 *   simulate_model(params, variables);
 * }
 * @endcode
 */

/**
 * When a new instance is created, the experiment definition is processed and
 * the model parameters will be automatically updated whenever
 * Experiment::update is called.
 *
 * @param p The struct of model parameters.
 * @param input The input stream from which the experiment definition is read.
 */
Experiment::Experiment(PARAMS &p, std::istream &input) : params(p) {
  err = NULL;

  /* The initial parameter values are applied before the simulation starts. */
  PARAM_CHANGES *cs = new PARAM_CHANGES;
  cs->at_time = 0;

  string line = string();
  while (!input.eof()) {
    /* Read each line in turn, ignoring blank lines. */
    getline(input, line);

    /* Ignore blank lines and comments beginning with '#'. */
    const char *whitespace = " \t\r\n";
    size_t start = line.find_first_not_of(whitespace);
    if (start == string::npos || line[start] == '#') {
      continue;
    }

    /* Turn the line into a stream, to process each word individually. */
    stringstream str(line);
    /* Read the parameter name. */
    string pname;
    str >> pname;

    if (! pname.compare("t=")) {
      /* If the parameter name is "t=", this specifies the start of a new set
         of changes, to be applied at the specified time. */

      /* First, add the previous set of parameter values to the queue. */
      changes.push(*cs);
      delete cs;

      /* Read the scheduled time . */
      double pval;
      str >> pval;

      /* Then create a new, empty set of parameter values. */
      cs = new PARAM_CHANGES;
      cs->at_time = pval;

      times.push_back(pval);
    } else if (! pname.compare("o=")) {
      /* If the parameter name is "o=", the rest of the line contains the
         names of model variables to be included in the output. */
      string varname;
      while (str.good()) {
        str >> varname;
        outputs.push_back(varname);
      }
    } else if (! pname.compare("end-exp")) {
      break;
    } else {
      /* Read the parameter value. */
      double pval;
      str >> pval;

      /* Copy the parameter name. */
      char *str_name = new char [pname.size()+1];
      strcpy(str_name, pname.c_str());

      /* Record the parameter name and the new value. */
      PARAM_CHANGE *ch = new PARAM_CHANGE;
      ch->name = str_name;
      ch->value = pval;

      /* Add this parameter change to the vector of scheduled changes. */
      cs->changes.push_back(*ch);
      delete ch;
    }
  }

  /* Add the current (and final) set of scheduled changes to the queue. */
  changes.push(*cs);
  delete cs;

  orig = new queue<PARAM_CHANGES>(changes);
}

/**
 * The destructor frees all of the resources that were allocated to the
 * Experiment instance. If the experiment was run to completion, all of the
 * resources should have been freed already.
 */
Experiment::~Experiment() {
  if (err) {
    delete err;
  }

  delete orig;

  /* Each parameter change includes the parameter name, which was manually
     allocated in the constructor and therefore needs to be freed. */
  while (! changes.empty()) {
    PARAM_CHANGES cs = changes.front();
    changes.pop();

    vector<PARAM_CHANGE>::iterator it;
    for (it = cs.changes.begin() ; it != cs.changes.end(); it++) {
      delete it->name;
    }
  }
}

/**
 * Returns true if an error occurred when parsing the experiment definition.
 */
bool Experiment::failed() {
  return (err != NULL);
}

/**
 * Returns a message that describes the error in processing the experiment
 * definition. If Experiment::failed returns @c false, this method will return
 * @c NULL.
 */
const string* Experiment::errmsg() {
  return err;
}

/**
 * Updates the model parameters according to the experiment definition.
 *
 * @param time The current simulation time (mins).
 */
void Experiment::update(double time) {
  /* Check if it's time to apply the next set of scheduled changes. */
  PARAM_CHANGES cs = changes.front();
  if (cs.at_time > time) {
    return;
  }

  /* If so, remove this set of changes from the queue. */
  changes.pop();

  /* Set each new parameter value. */
  vector<PARAM_CHANGE>::iterator it;
  for (it = cs.changes.begin() ; it != cs.changes.end(); it++) {
    set_param(params, it->name, it->value);
    delete it->name;
  }
}

/**
 * Returns the time at which the experiment should end (mins).
 */
double Experiment::stop_at() {
  /** The end of the simulation is the final time provided in the experiment
      definition. The only purpose of this time is to define the end of the
      simulation. It would be meaningless to define any parameter changes for
      this time. */
  return changes.back().at_time;
}

/**
 * Returns the times at which the model outputs should be recorded. The array
 * is terminated by the value DBL_MAX,
 */
const double* Experiment::output_times() {
  /* Allocate space for each time and the terminal value. */
  int size = (int) times.size() + 1;
  double* output_times = new double[size];

  /* Record each output time. */
  for (int i = 0; i < size; i++) {
    output_times[i] = times[i];
  }

  /* Terminate the array with DBL_MAX and then return the array. */
  output_times[size - 1] = DBL_MAX;
  return output_times;
}

const std::vector<std::string>& Experiment::output_vars() const {
  return outputs;
}

/**
 * Writes the experiment definition to an output stream.
 *
 * @param out The output stream to which the experiment definition is written.
 */
void Experiment::write_exp(std::ostream &out) {
  /* Print the output names. */
  out << "o=";
  int names = (int) outputs.size();
  for (int n = 0; n < names; n++) {
    out << " " << outputs[n];
  }
  out << endl;

  /* Print the output times and parameter changes. */
  param_changes cs(*orig);
  int size = (int) cs.size();
  for (int i = 0; i < size; i++) {
    PARAM_CHANGES cx = cs.front();
    cs.pop();

    /* Only print "t= 0" if this line was actually included in the input.
       This is only the case if the first output time (times[0]) is zero. */
    if (i > 0 || times[0] == 0) {
      out << "t= " << cx.at_time << endl;
    }

    /* Print the parameter changes (if any). */
    int count = (int) cx.changes.size();
    for (int j = 0; j < count; j++) {
      PARAM_CHANGE c = cx.changes[j];
      out << c.name << " " << c.value << endl;
    }
  }

  /* Finish with the end-exp marker, so that more data can be appended to
     the output without preventing the experiment parser from working. */
  out << "end-exp" << endl;
}
