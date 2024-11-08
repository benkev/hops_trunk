#!/bin/sh
#
# make a plot

cat > zc.gnu <<-EOF
    set term png size 800,800
    set output 'zc.png'
    set size 1.0,1.0
    set origin 0.0,0.0
    set multiplot
    #
    set size 0.475,0.475
    set origin 0.0,0.0
    set xtics 2.5e-07
    set xlabel 'lag (s)'
    set ylabel 'xc amplitude (arb units)'
    set title 'Overlap Channels'
    plot [][0:0.002] \\
	'zc-0.lag' u 2:5 w lp title 'ch0', \\
	'zc-2.lag' u 2:5 w lp title 'ch2', \\
	'zc-4.lag' u 2:5 w lp title 'ch4', \\
	'zc-6.lag' u 2:5 w lp title 'ch6', \\
	'zc-8.lag' u 2:5 w lp title 'ch8', \\
	'zc-A.lag' u 2:5 w lp title 'chA', \\
	'zc-C.lag' u 2:5 w lp title 'chC', \\
	'zc-E.lag' u 2:5 w lp title 'chE'
    #
    set size 0.475,0.475
    set origin 0.0,0.5
    set xtics 2.5e-07
    set xlabel 'lag (s)'
    set ylabel 'xc amplitude (arb units)'
    set title 'Disconnected Channels'
    plot [][0:0.002] \\
	'zc-1.lag' u 2:5 w lp title 'ch1', \\
	'zc-3.lag' u 2:5 w lp title 'ch3', \\
	'zc-5.lag' u 2:5 w lp title 'ch5', \\
	'zc-7.lag' u 2:5 w lp title 'ch7', \\
	'zc-9.lag' u 2:5 w lp title 'ch9', \\
	'zc-B.lag' u 2:5 w lp title 'chB', \\
	'zc-D.lag' u 2:5 w lp title 'chD', \\
	'zc-F.lag' u 2:5 w lp title 'chF'
    #
    set size 0.475,0.475
    set origin 0.5,0.0
    set xtics 0.8e+07
    set xlabel 'freq (Hz)'
    set ylabel 'visibility (arb units)'
    set title 'Overlap Channels'
    plot [][0:1.3e-4] \\
	'zc-0.vis' u 2:5 w lp title 'ch0', \\
	'zc-2.vis' u 2:5 w lp title 'ch2', \\
	'zc-4.vis' u 2:5 w lp title 'ch4', \\
	'zc-6.vis' u 2:5 w lp title 'ch6', \\
	'zc-8.vis' u 2:5 w lp title 'ch8', \\
	'zc-A.vis' u 2:5 w lp title 'chA', \\
	'zc-C.vis' u 2:5 w lp title 'chC', \\
	'zc-E.vis' u 2:5 w lp title 'chE'
    #
    set size 0.475,0.475
    set origin 0.5,0.5
    set xtics 0.8e+07
    set xlabel 'freq (Hz)'
    set ylabel 'visibility (arb units)'
    set title 'Disconnected Channels'
    plot [][0:1.3e-4] \\
	'zc-1.vis' u 2:5 w lp title 'ch1', \\
	'zc-3.vis' u 2:5 w lp title 'ch3', \\
	'zc-5.vis' u 2:5 w lp title 'ch5', \\
	'zc-7.vis' u 2:5 w lp title 'ch7', \\
	'zc-9.vis' u 2:5 w lp title 'ch9', \\
	'zc-B.vis' u 2:5 w lp title 'chB', \\
	'zc-D.vis' u 2:5 w lp title 'chD', \\
	'zc-F.vis' u 2:5 w lp title 'chF'
    unset multiplot
    set output
EOF

gnuplot < zc.gnu
display zc.png

#
# eof
#
