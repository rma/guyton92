#!/bin/sh

IN_FILE="na_intake.base"
EXP_FILE="$(basename $IN_FILE .base).exp"
MODEL="$(cd $(dirname $0) && pwd)/../build/guyton92"

runexperiment() {
    OUT_FILE="$(basename $EXP_FILE .exp)_$1.out"

    cp $IN_FILE $EXP_FILE

    echo "nid $1" >> $EXP_FILE

    I=$((40320 + 60))
    while [ $I -le $((2 * 40320)) ]; do
        echo "t= $I"
        I=$(($I + 60))
    done >> $EXP_FILE

    echo "$1 ..."
    $MODEL -n $EXP_FILE > $OUT_FILE

    rm $EXP_FILE
}

CNA=0.02
while [ $(echo "$CNA <= 0.4" | bc -l) -eq 1 ]; do
    runexperiment $CNA
    CNA=0$(echo $CNA + 0.02 | bc -l)
done

R --vanilla --quiet < na_intake.R
