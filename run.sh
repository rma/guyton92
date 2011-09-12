#!/bin/sh
#
# Run one or more experiments and record the results.
#

USAGE="USAGE: `basename ${0}` experiment_file(s)"

if [ "$#" -lt "1" ]; then
    echo ${USAGE}
    exit 2
fi

MODEL="./build/guyton92"

for EXP_FILE in "$@"; do
    EXP_BASE="$(basename ${EXP_FILE} .exp)"
    EXP_NAME=$(echo ${EXP_BASE} | tr [:lower:] [:upper:] | tr -d -c [:alnum:])
    OUT_FILE="$(dirname ${EXP_FILE})/${EXP_BASE}.out"

    if [ ! -r "${EXP_FILE}" ]; then
        echo "ERROR: unable to read experiment '${EXP_FILE}'"
        exit 2
    fi

    echo ${EXP_BASE}

    make clobber
    make model EXPERIMENT="${EXP_NAME}" ECHO=":"
    ./build/guyton92 ${EXP_FILE} | awk 'i{print}/end-exp/{i++}' > ${OUT_FILE}
done

make clobber
