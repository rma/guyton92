/**
 * @file
 * Provides the Experiment class, for changing parameters over time.
 */

#include <queue>
#include <vector>
#include <iostream>
#include <istream>
#include <string>
#include <sstream>
#include <cstring>

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
    /* Note: this is white-space sensitive, which is certainly not ideal. */
    if (! line.compare("")) {
      continue;
    }

    /* Turn the line into a stream, to process each word individually. */
    stringstream str = stringstream(line);
    /* Read the parameter name. */
    string pname;
    str >> pname;
    /* Read the parameter value. */
    double pval;
    str >> pval;

    if (! pname.compare("t=")) {
      /* If the parameter name is "t=", this specifies the start of a new set
         of changes, to be applied at the specified time. */

      /* First, add the previous set of parameter values to the queue. */
      changes.push(*cs);
      delete cs;

      /* Then create a new, empty set of parameter values. */
      cs = new PARAM_CHANGES;
      cs->at_time = pval;
    } else {
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
  if (cs.at_time >= time) {
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
