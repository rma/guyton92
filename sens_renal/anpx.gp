#!/usr/bin/gnuplot -p
set terminal postscript color rounded 32
set output 'anpx.eps'
set xlabel 'anpx'
plot 'anpx.ssv' using 1:3 title column(2) with lines lw 4, '' using 1:4 title column(4) with lines lw 4, '' using 1:5 title column(5) with lines lw 4, '' using 1:6 title column(6) with lines lw 4, '' using 1:7 title column(7) with lines lw 4
