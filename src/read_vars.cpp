#include <iostream>
#include <fstream>
using namespace std;

#include "read_vars.h"

/* The maximum acceptable length of a line that contains a state variable name
   and an associated numerical value. */
#define VAR_LINE_MAX 64

void read_vars(const VARS &v, const char* infile) {
  cerr << "Reading state variables from \"" << infile << "\" ... ";

  /* Open the input file for reading. */
  ifstream input(infile);
  if (input.fail()) {
    cerr << "\nAn error occurred in read_vars() -- ifstream::ifstream.";
    input.close();
    return;
  }

  char line[VAR_LINE_MAX];
  int var_count = 0;
  while(! input.eof()) {
    /* Read each line in turn. */
    input.getline(line, VAR_LINE_MAX);

    /* Reaching the EOF can set the failbit flag, so we check for EOF first. */
    if (input.eof()) {
      continue;
    }
    if (input.fail()) {
      cerr << "\nAn error occurred in read_vars() -- istream::getline.";
      input.close();
      return;
    }

    /* Parse the line. */
    char var_name[VAR_LINE_MAX];
    double var_value;
    int count = sscanf(line, "%s %lf", var_name, &var_value);
    if (count < 2) {
      cerr << "\nAn error occurred in read_vars() -- sscanf.";
      input.close();
      return;
    }

    /* Set the new state variable value. */
    set_var(v, var_name, var_value);
    var_count++;
  }
  /* The only cleanup task is to close the input file. */
  input.close();

  cerr << var_count << " state variables read.\n";
}
