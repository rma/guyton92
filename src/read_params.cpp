#include <iostream>
#include <fstream>
using namespace std;

#include "read_params.h"

/* The maximum acceptable length of a line that contains a parameter name and
   an associated numerical value. */
#define PARAM_LINE_MAX 64

void read_params(const PARAMS &p, const char* infile) {
  cerr << "Reading parameters from \"" << infile << "\" ... ";

  /* Open the input file for reading. */
  ifstream input(infile);
  if (input.fail()) {
    cerr << "\nAn error occurred in read_params() -- ifstream::ifstream.";
    input.close();
    return;
  }

  char line[PARAM_LINE_MAX];
  int param_count = 0;
  while(! input.eof()) {
    /* Read each line in turn. */
    input.getline(line, PARAM_LINE_MAX);

    /* Reaching the EOF can set the failbit flag, so we check for EOF first. */
    if (input.eof()) {
      continue;
    }
    if (input.fail()) {
      cerr << "\nAn error occurred in read_params() -- istream::getline.";
      input.close();
      return;
    }

    /* Parse the line. */
    char param_name[PARAM_LINE_MAX];
    double param_value;
    int count = sscanf(line, "%s %lf", param_name, &param_value);
    if (count < 2) {
      cerr << "\nAn error occurred in read_params() -- sscanf.";
      input.close();
      return;
    }

    /* Set the new parameter value. */
    set_param(p, param_name, param_value);
    param_count++;
  }
  /* The only cleanup task is to close the input file. */
  input.close();

  cerr << param_count << " parameters read.\n";
}
