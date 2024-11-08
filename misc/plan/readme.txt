# Brian Corey's EGparang
#
# Hi Geoff,
#
# The program is a homebrew called EGparang in my home area on lcc.
# It's a bastardization of other homebrew code and is specific to
# Westford and GGAO, hence the name (E = Westford, G = GGAO, parang
# = parallactic angle, which tells how the linear-pol feeds at the
# two sites are oriented wrt each other).  It's also very user-
# unfriendly, my excuse being that it was cobbled together in a 
# rush to satisfy an immediate need.  But if you should be so unwise
# as to wish to use it, be warned that it's hardwired to work for
# only 20 sources (well, 19 at the moment).  If you want one not in
# the program already, you'll have to edit the code and recompile.
# It's fortran, so source code is EGparang.f.  Have I scared you
# off yet?
#

EGparang idunno 180 30 17.8 9.4

# Brian Corey's dsolv
# Wed, 15 Jun 2016 20:37:45 -0400 (EDT)
# 
# I'm not going to apologize for my code because it was never
# meant to be public and was thrown together for the LMT position
# investigation from some antique and even antiquer code lying
# around from various projects...blah blah....
# 
# dsolv.f is the solver that runs on a file like the 2nd
# attached file.  The calculation of GST is approx, I used the
# J2000 ra/dec source position without precessing to date in the
# input file, there's no allowance for an error in the correlator
# atmosphere model delay (which could be significant at low el,
# but fortunately the ALMA obs don't go all that low), etc.
# So without further work on the input file and on the code,
# I'd declare success if repeatable (over baselines and bands)
# XYZ estimates are obtained at the half meter level, say.
#               AR        AS
# -X(m)       -0.31     -0.23
# -Y(m)       -6.91     -6.33
# -Z(m)       -1.26     -1.00
# offset(ns)  27.6      13.8
# rate(ps/s)  -0.497     0.151
#
# XYZ above refers to the ALMA position.  My program fits for the
# baseline XYZ, and the ref station (ALMA here) goes into the baseline
# position with a negative sign, hence the minus in front of XYZ above.
# So it's looking like true-corr ALMA position is ~+7m in Y and +1m in Z.

With mods by GBC:
./dsolv hiband.XL.AR.dsolvin
...
#
# Model fit parameters:
# param1 =    -0.308756 delta Dx(m)                             
# param2 =    -6.902372 delta Dy(m)                             
# param3 =    -1.257485 delta Dz(m)                             
# param4 =     0.027665 clock offset(us)                       
# param5 =    -0.496543 clock rate(ps/s)                        
#


#
# eof
#
