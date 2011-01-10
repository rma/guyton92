#!/bin/bash
#
# get_data.sh
#
# Extracts the simulation results from an output file.
# The preceeding experiment definition is not printed.
#

usage() {
  cat <<EOF

  USAGE       `basename $0` [-e] output

  ARGUMENTS
    output:   The file that contains the results of the simulation.

  OPTIONS
    -e        Extract the experiment definition.

  EXAMPLE
              `basename $0` my_experiment.out

EOF
  exit 1
}

#
# Default parameter values.
#

# Extract the simulation results, not the experiment definition.
SHOWEXP=false

#
# Parse the command line.
#

while getopts "e" OPT; do
  case ${OPT} in
  e) SHOWEXP=true ;;
  ?) usage ;;
  esac
done
shift $(( OPTIND - 1 ))

# Ensure the correct number of arguments were given.
if [ "$#" -ne "1" ]; then
    usage
fi

# Ensure that the output file exists and is readable.
if [ ! -r $1 ]; then
    echo "ERROR: '$1' is not readable." >&2
    exit 1
fi

if ! grep "end-exp" $1 > /dev/null 2>&1; then
    echo "ERROR: '$1' is not a valid input file." >&2
    exit 1;
fi

#
# Extract the specified part of the output file.
#

if [ "${SHOWEXP}" == "true" ]; then
    sed '1,/end-exp/!d' $1
else
    sed '1,/end-exp/d' $1
fi
