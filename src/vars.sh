#!/bin/bash
#
# vars.sh
#
# Automatically generates C++ code for constructing and initialising a struct
# of all model state variables, and for setting the values of individual state
# variables by name.
#
# This script produces the following files:
#   * vars.h   -- Defines the VARS type, set_var() and VARS_INIT.
#   * vars.cpp -- Implements the set_var() function.
#
# NOTE: This script requires the file "vars.lst" to contain all of the model
#       state variable names, each on a separate line, and the file "vars.val"
#       to contain the default state variable values, each on a separate line.
#

#
# The files that are processed/generated by this script.
#
VARS_LIST="vars.lst"
VARS_VALS="vars.val"
VARS_DEFN="vars.h"
VARS_CODE="vars.cpp"

#
# Check whether the list of state variable names exists and is readable.
#
if [ ! -f ${VARS_LIST} ]; then
  echo "ERROR: input file ${VARS_LIST} does not exist."
  exit 2;
fi

if [ ! -r ${VARS_LIST} ]; then
  echo "ERROR: input file ${VARS_LIST} is not readable."
  exit 2;
fi

#
# Check whether the list of state variable values exists and is readable.
#
if [ ! -f ${VARS_VALS} ]; then
  echo "ERROR: input file ${VARS_VALS} does not exist."
  exit 2;
fi

if [ ! -r ${VARS_VALS} ]; then
  echo "ERROR: input file ${VARS_VALS} is not readable."
  exit 2;
fi

#
# Build the header file.
#
cat ${VARS_LIST} |
  awk 'BEGIN { print "struct VARS {"; } ;
       { print "double " $1 ";" } ;
       END { print "};"; }' > ${VARS_DEFN}

echo "void set_var(VARS &v, const char *name, double value);" >> ${VARS_DEFN}

VAR_COUNT=`wc -l ${VARS_LIST} | awk '{ print $1; }'`
echo "#define VAR_COUNT ${VAR_COUNT}" >> ${VARS_DEFN};

#
# Produce the code fragment that will initialise the state variables.
#
echo "#define VARS_INIT(v) do { \\" >> ${VARS_DEFN}

cat ${VARS_VALS} |
  awk '{ print "\tv."$1 " = " $2 "; \\"; }' >> ${VARS_DEFN}

echo "} while (0)" >> ${VARS_DEFN}

#
# Build the code for set_var().
#
echo '#include <stdio.h>' > ${VARS_CODE}
echo '#include <string.h>' >> ${VARS_CODE}
echo '#include "vars.h"' >> ${VARS_CODE}
echo '' >> ${VARS_CODE}

cat ${VARS_LIST} |
  awk 'BEGIN { print "void set_var(VARS &v, const char *name, double value) {"; };
       { print "  if (! strcmp(name, \"" $1 "\")) {";
         print "    v." $1 " = value;";
         print "    return;"
         print "  }"; }
       END { print "  fprintf(stderr, \"ERROR: Unknown variable name \\\"%s\\\"\\n\", name);";
             print "}"; };' >> ${VARS_CODE}
