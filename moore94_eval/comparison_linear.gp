#!/usr/bin/gnuplot
set terminal postscript solid rounded 16

set output 'compare_nod.eps'
set title 'Na+ excretion'
set xlabel 'NOD (new)'
set ylabel 'NOD (Guyton)'
plot '< tail -n +100 ./comparison.linear' using 1:2 notitle

set output 'compare_kod.eps'
set title 'K+ excretion'
set xlabel 'KOD (new)'
set ylabel 'KOD (Guyton)'
plot '< tail -n +100 ./comparison.linear' using 3:4 notitle

set output 'compare_vud.eps'
set title 'Urine volume'
set xlabel 'VUD (new)'
set ylabel 'VUD (Guyton)'
plot '< tail -n +100 ./comparison.linear' using 5:6 notitle

set output 'compare_rbf.eps'
set title 'Renal blood flow'
set xlabel 'RBF (new)'
set ylabel 'RBF (Guyton)'
plot '< tail -n +100 ./comparison.linear' using 7:8 notitle

set output 'compare_mdflw.eps'
set title 'Macula densa flow'
set xlabel 'MDFLW (new)'
set ylabel 'MDFLW (Guyton)'
plot '< tail -n +100 ./comparison.linear' using 9:10 notitle

set output 'compare_qalh.eps'
set title 'Qalh vs MDFLW'
set xlabel 'Qalh (new)'
set ylabel 'MDFLW (Guyton)'
glm(x) = x / 5.077413
plot '< tail -n +100 ./comparison.linear' using 11:10 notitle \
  w points lc rgbcolor "#AFAFAF", \
  glm(x) w lines lw 4 title 'LINEAR'
