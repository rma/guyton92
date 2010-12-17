#!/bin/bash
#
# find_vars.sh
#
# Searches for references to state variables in model modules.
#

usage() {
  cat <<EOF

  USAGE       `basename $0` [-e -p] command module_or_variable_name

  COMMANDS
    on-lhs:   Find all variables on the left-hand side of the equations in
              the given module.

    on-rhs:   Find all variables on the right-hand side of the equations in
              the given module.

    from-lhs: For each variable on the left-hand side of an equation in the
              given module, search for references to this variable in the
              other modules.

    from-rhs: For each variable on the right-hand side of an equation in the
              given module, search for definitions of this variable in the
              other modules.

    find-lhs: For the given state variable, find the modules where the
              variable is on the left-hand side of an equation.

    find-rhs: For the given state variable, find the modules where the
              variable is on the right-hand side of an equation.

  OPTIONS
    -e        Display the equations containing the matching variables.
              By default, only the variable and module names are displayed.

    -p        Search for model parameters, not state variables. Note that
              this is not a sensible option for "from-lhs" and "from-rhs",
              because parameters never appear on the left-hand side.
EOF
  exit 1
}

#
# Default parameter values.
#

# Only display variable and module names, and not the equations.
SHOWEQNS=false
# The on-lhs and on-rhs commands have a separate flag, as these commands are
# also used by from-lhs and from-rhs, which require on-lhs and on-rhs to
# only return the matching variable names, and not the equations.
ON_SHOWEQNS=false
# Whether to search for variables or parameters
FINDPARAMS=false

# The relative path to the source code for the model.
SRCDIR="../src"

#
# Parse the command line.
#

while getopts "ep" OPT; do
  case ${OPT} in
  e) SHOWEQNS=true ;;
  p) FINDPARAMS=true ;;
  ?) usage ;;
  esac
done
shift $(( OPTIND - 1 ))

# Ensure the correct number of arguments were given.
if [ "$#" != "2" ]; then
  usage
fi

# Extract the chosen command.
COMMAND="$1"

# Ensure the command is valid and extract the module name.
case ${COMMAND} in
  on-lhs|on-rhs|from-lhs|from-rhs)
    # Ensure the module name ends with ".cpp".
    MODULE="${SRCDIR}/`basename $2 .cpp`.cpp"
    # Ensure that the module exists and is readable.
    if [ ! -r ${MODULE} ]; then
      echo
      echo "  ERROR       invalid module \"${MODULE}\""
      usage
    fi ;;

  find-lhs|find-rhs)
    # This is a state variable name, don't enforce a ".cpp" ending.
    MODULE="$2" ;;

  *) echo
     echo "  ERROR       invalid command \"${COMMAND}\""
     usage ;;
esac

#
# Useful constants.
#

if [ "${FINDPARAMS}" = "true" ]; then
  V_OR_P='p'
else
  V_OR_P='v'
fi

RE_VAR="${V_OR_P}\.([a-zA-Z0-9_])+"
RE_LHS='^[[:space:]]+'${RE_VAR}
RE_RHS='=.*'${RE_VAR}

if [ "${SHOWEQNS}" = "true" ]; then
  # Show all matching lines.
  GREPFLAGS=""
else
  # Print the name of each file that contains a match.
  GREPFLAGS="-l"
fi

ALL_MODULES="${SRCDIR}/module_*.cpp"
OTHER_MODULES=""
for MOD in ${ALL_MODULES}; do
  if [ "${MOD}" != "${MODULE}" ]; then
    OTHER_MODULES="${OTHER_MODULES} ${MOD}"
  fi
done

#
# Functions for the individual commands.
#

on-lhs() {
  # Find all variables on the left-hand side of an equation.
  if [ "${ON_SHOWEQNS}" = "true" ]; then
    egrep ${RE_LHS} ${MODULE}
  else
    egrep -o ${RE_LHS} ${MODULE} | awk -F '.' '{ print $2; }' | sort | uniq
  fi
}

on-rhs() {
  # Find all variables on the right-hand side of an equation.
  if [ "${ON_SHOWEQNS}" = "true" ]; then
    egrep ${RE_RHS} ${MODULE}
  else
    egrep -o ${RE_RHS} ${MODULE} | egrep -o ${RE_VAR} |
      awk -F '.' '{ print $2; }' | sort | uniq
  fi
}

from-rhs() {
  # Find when each variable is on the right-hand side of an equation.
  while read VAR; do
    RE="=.*${V_OR_P}\.${VAR}[[:punct:][:space:]]"
    if egrep -q ${RE} ${OTHER_MODULES}; then
      echo ${VAR}
      egrep ${GREPFLAGS} ${RE} ${OTHER_MODULES}
      echo
    fi
  done
}

from-lhs() {
  # Find when each variable is on the left-hand side of an equation.
  while read VAR; do
    RE="^[[:space:]]*${V_OR_P}\.${VAR}[[:punct:][:space:]]"
    if egrep -q ${RE} ${OTHER_MODULES}; then
      echo ${VAR}
      egrep ${GREPFLAGS} ${RE} ${OTHER_MODULES}
      echo
    fi
  done
}

find-lhs() {
  # Find all instances of the variable on the left-hand side of an equation.
  RE="^[[:space:]]*${V_OR_P}\.${MODULE}[[:punct:][:space:]]"
  egrep ${GREPFLAGS} ${RE} ${ALL_MODULES}
}

find-rhs() {
  # Find all instances of the variable on the right-hand side of an equation.
  RE="=.*${V_OR_P}\.${MODULE}[[:punct:][:space:]]"
  egrep ${GREPFLAGS} ${RE} ${ALL_MODULES}
}

#
# Perform the specified command.
#

case ${COMMAND} in
  on-lhs) ON_SHOWEQNS="${SHOWEQNS}" on-lhs ;;

  on-rhs) ON_SHOWEQNS="${SHOWEQNS}" on-rhs ;;

  from-lhs) on-lhs | from-rhs ;;

  from-rhs) on-rhs | from-lhs ;;

  find-lhs) find-lhs ;;

  find-rhs) find-rhs ;;
esac
