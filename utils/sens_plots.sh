#!/bin/bash
#
# sens_plots.sh
#
# Produce plots of module output sensitivity to parameters and input variables.
#

usage() {
  cat <<EOF

  USAGE       `basename $0` module infile outputs

  ARGUMENTS
    module:   The name of a module in the Guyton model, excluding the leading
              "module_" and file extension.

    infile:   A file that specifies the sensitivity analyses to perform. Each
              line of this file should be of the following form:

                control_name minimum increment maximum

              For example, to vary the mean arterial pressure (PA) from 80 to
              120 mmHg in steps of 2 mmHg:

                pa 80 2 120

    outputs:  The names of the output variables to record.

  EXAMPLE
              `basename $0` renal renal_sens.txt nod kod vud mdflw rbf

EOF
  exit 1
}

#
# The sensitivity analyser.
#

# The path to the sensitivity analyser.
ANALYSE=`dirname $0`/"../build/sensitivity"

# Ensure that the sensitivity analyser has been built.
if [ ! -x ${ANALYSE} ]; then
    echo "ERROR: ${ANALYSE} is not executable. Please rebuild it."
    exit 1
fi

#
# Parse the command line.
#

# Ensure the correct number of arguments were given.
if [ "$#" -lt "3" ]; then
    usage
fi

MODULE="$1" # Extract the name of the module.
INFILE="$2" # Extract the name of the input file.
shift 2     # Remove the first two arguments so that $@ refers to the outputs.

# Ensure that the input file exists and is readable.
if [ ! -r ${INFILE} ]; then
    echo "ERROR: '"${INFILE}"' is not readable."
    exit 1
fi

#
# Run the analyses and plot the results.
#

# Run a sensitivity analysis for each line of the input file.
while read LINE; do
    if [ `echo ${LINE} | awk '{ print NF }'` = 4 ]; then
        VAR_NAME=`echo ${LINE} | awk '{ print $1; }'`

        # Run the sensitivity analysis.
        ${ANALYSE} ${MODULE} ${LINE} "$@"

        # Run the generated gnuplot script to plot the results.
        echo -n "${MODULE}_${VAR_NAME}.eps ... "
        GNUPLOT_SCRIPT=./${MODULE}_${VAR_NAME}.gp
        gnuplot < ${GNUPLOT_SCRIPT}
        echo done
    else
        echo "ERROR: skipping invalid line'"${LINE}"'."
    fi
done < ${INFILE}
