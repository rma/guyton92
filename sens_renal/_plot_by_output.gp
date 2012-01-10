#!/usr/bin/gnuplot -p
set terminal postscript color rounded 12 enhanced
set output 'plot_by_output.eps'
set style line 1  lt 1 lw 4 lc rgbcolor "#FF0000"
set style line 2  lt 1 lw 4 lc rgbcolor "#FFAA00"
set style line 3  lt 1 lw 4 lc rgbcolor "#00FF00"
set style line 4  lt 1 lw 4 lc rgbcolor "#00FFFF"
set style line 5  lt 1 lw 4 lc rgbcolor "#0000FF"
set style line 6  lt 1 lw 4 lc rgbcolor "#FF00FF"
set style line 7  lt 2 lw 4 lc rgbcolor "#FF0000"
set style line 8  lt 2 lw 4 lc rgbcolor "#FFAA00"
set style line 9  lt 2 lw 4 lc rgbcolor "#00FF00"
set style line 10 lt 2 lw 4 lc rgbcolor "#00FFFF"
set style line 11 lt 2 lw 4 lc rgbcolor "#0000FF"
set style line 12 lt 2 lw 4 lc rgbcolor "#FF00FF"
set style line 13 lt 4 lw 4 lc rgbcolor "#FF0000"
set style line 14 lt 4 lw 4 lc rgbcolor "#FFAA00"
set style line 15 lt 4 lw 4 lc rgbcolor "#00FF00"
set style line 16 lt 4 lw 4 lc rgbcolor "#00FFFF"
set style line 17 lt 4 lw 4 lc rgbcolor "#0000FF"
set style line 18 lt 4 lw 4 lc rgbcolor "#FF00FF"
set style increment user
set title 'kod' font ',32'
plot \
'adhmk.ssv' using 2:3 title 'adhmk' with lines, \
'amk.ssv' using 2:3 title 'amk' with lines, \
'amna.ssv' using 2:3 title 'amna' with lines, \
'anm.ssv' using 2:3 title 'anm' with lines, \
'anpx.ssv' using 2:3 title 'anpx' with lines, \
'aum.ssv' using 2:3 title 'aum' with lines, \
'cke.ssv' using 2:3 title 'cke' with lines, \
'cna.ssv' using 2:3 title 'cna' with lines, \
'vtw.ssv' using 2:3 title 'vtw' with lines, \
'myogrs.ssv' using 2:3 title 'myogrs' with lines, \
'pa.ssv' using 2:3 title 'pa' with lines, \
'pamkrn.ssv' using 2:3 title 'pamkrn' with lines, \
'ppc.ssv' using 2:3 title 'ppc' with lines, \
'vp.ssv' using 2:3 title 'vp' with lines, \
'vb.ssv' using 2:3 title 'vb' with lines, \
NaN notitle
set title 'nod' font ',32'
plot \
'adhmk.ssv' using 2:4 title 'adhmk' with lines, \
'amk.ssv' using 2:4 title 'amk' with lines, \
'amna.ssv' using 2:4 title 'amna' with lines, \
'anm.ssv' using 2:4 title 'anm' with lines, \
'anpx.ssv' using 2:4 title 'anpx' with lines, \
'aum.ssv' using 2:4 title 'aum' with lines, \
'cke.ssv' using 2:4 title 'cke' with lines, \
'cna.ssv' using 2:4 title 'cna' with lines, \
'vtw.ssv' using 2:4 title 'vtw' with lines, \
'myogrs.ssv' using 2:4 title 'myogrs' with lines, \
'pa.ssv' using 2:4 title 'pa' with lines, \
'pamkrn.ssv' using 2:4 title 'pamkrn' with lines, \
'ppc.ssv' using 2:4 title 'ppc' with lines, \
'vp.ssv' using 2:4 title 'vp' with lines, \
'vb.ssv' using 2:4 title 'vb' with lines, \
NaN notitle
set title 'vud' font ',32'
plot \
'adhmk.ssv' using 2:5 title 'adhmk' with lines, \
'amk.ssv' using 2:5 title 'amk' with lines, \
'amna.ssv' using 2:5 title 'amna' with lines, \
'anm.ssv' using 2:5 title 'anm' with lines, \
'anpx.ssv' using 2:5 title 'anpx' with lines, \
'aum.ssv' using 2:5 title 'aum' with lines, \
'cke.ssv' using 2:5 title 'cke' with lines, \
'cna.ssv' using 2:5 title 'cna' with lines, \
'vtw.ssv' using 2:5 title 'vtw' with lines, \
'myogrs.ssv' using 2:5 title 'myogrs' with lines, \
'pa.ssv' using 2:5 title 'pa' with lines, \
'pamkrn.ssv' using 2:5 title 'pamkrn' with lines, \
'ppc.ssv' using 2:5 title 'ppc' with lines, \
'vp.ssv' using 2:5 title 'vp' with lines, \
'vb.ssv' using 2:5 title 'vb' with lines, \
NaN notitle
set title 'mdflw' font ',32'
plot \
'adhmk.ssv' using 2:6 title 'adhmk' with lines, \
'amk.ssv' using 2:6 title 'amk' with lines, \
'amna.ssv' using 2:6 title 'amna' with lines, \
'anm.ssv' using 2:6 title 'anm' with lines, \
'anpx.ssv' using 2:6 title 'anpx' with lines, \
'aum.ssv' using 2:6 title 'aum' with lines, \
'cke.ssv' using 2:6 title 'cke' with lines, \
'cna.ssv' using 2:6 title 'cna' with lines, \
'vtw.ssv' using 2:6 title 'vtw' with lines, \
'myogrs.ssv' using 2:6 title 'myogrs' with lines, \
'pa.ssv' using 2:6 title 'pa' with lines, \
'pamkrn.ssv' using 2:6 title 'pamkrn' with lines, \
'ppc.ssv' using 2:6 title 'ppc' with lines, \
'vp.ssv' using 2:6 title 'vp' with lines, \
'vb.ssv' using 2:6 title 'vb' with lines, \
NaN notitle
set title 'rbf' font ',32'
plot \
'adhmk.ssv' using 2:7 title 'adhmk' with lines, \
'amk.ssv' using 2:7 title 'amk' with lines, \
'amna.ssv' using 2:7 title 'amna' with lines, \
'anm.ssv' using 2:7 title 'anm' with lines, \
'anpx.ssv' using 2:7 title 'anpx' with lines, \
'aum.ssv' using 2:7 title 'aum' with lines, \
'cke.ssv' using 2:7 title 'cke' with lines, \
'cna.ssv' using 2:7 title 'cna' with lines, \
'vtw.ssv' using 2:7 title 'vtw' with lines, \
'myogrs.ssv' using 2:7 title 'myogrs' with lines, \
'pa.ssv' using 2:7 title 'pa' with lines, \
'pamkrn.ssv' using 2:7 title 'pamkrn' with lines, \
'ppc.ssv' using 2:7 title 'ppc' with lines, \
'vp.ssv' using 2:7 title 'vp' with lines, \
'vb.ssv' using 2:7 title 'vb' with lines, \
NaN notitle
