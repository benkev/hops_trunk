#!/bin/sh
#
# verify gsl
#

GSL_RNG_TYPE=mt19937_1999 ./multifit_data | ./multifit_pgrm 19 > result.out
cat > expect.out << EOF
0.1 0.97935 +/- 0.110517
0.2 1.3359 +/- 0.12214
0.3 1.52573 +/- 0.134986
0.4 1.60318 +/- 0.149182
0.5 1.81731 +/- 0.164872
0.6 1.92475 +/- 0.182212
0.7 1.93249 +/- 0.201375
0.8 2.5107 +/- 0.222554
0.9 2.45078 +/- 0.24596
1 2.24949 +/- 0.271828
1.1 3.08955 +/- 0.300417
1.2 3.82315 +/- 0.332012
1.3 4.26766 +/- 0.36693
1.4 3.2597 +/- 0.40552
1.5 4.98914 +/- 0.448169
1.6 4.14527 +/- 0.495303
1.7 5.22382 +/- 0.547395
1.8 6.3838 +/- 0.604965
1.9 6.00277 +/- 0.668589
# best fit: Y = 1.02318 + 0.956201 X + 0.876796 X^2
# covariance matrix:
[ +1.25612e-02, -3.64387e-02, +1.94389e-02  
  -3.64387e-02, +1.42339e-01, -8.48761e-02  
  +1.94389e-02, -8.48761e-02, +5.60243e-02 ]
# chisq = 23.0987
EOF

grep -v resid result.out | cmp - expect.out

# plot '< grep resid result.out' u 1:5:4 with yer, 0

#
# eof
#
