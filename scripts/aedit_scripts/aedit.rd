!rm sbd.plt
!rm csbd.plt
!rm drate.plt
!rm cdrate.plt
!rm refdpcalx.plt
!rm refdpcals.plt
!rm mbdx.plt
!rm cmbdx.plt
!rm mbds.plt
!rm cmbds.plt
!rm pcalapx1.plt
!rm pcalapx2.plt
!rm pcalapx3.plt
!rm pcalapx4.plt
!rm pcalapx5.plt
!rm pcalapx6.plt
!rm pcalapx7.plt
!rm pcalapx8.plt
!rm pcalaps1.plt
!rm pcalaps2.plt
!rm pcalaps3.plt
!rm pcalaps4.plt
!rm pcalaps5.plt
!rm pcalaps6.plt
!rm shrtscan.out
snrmin 7
qcode 1-9
edit inputs
edit duplicates qcode
param 3 8
close
axis sbd
dev sbd.plt/hpgl
yscale -.25 +.25
grid 2 5
plot
clear plot
!lp -onb -dnetjet -oduplex sbd.plt 
axis csbdelay
dev csbd.plt/hpgl
yscale -.25 +.25
grid 2 5
plot
clear plot
!lp -onb -dnetjet -oduplex csbd.plt  
axis drate
yscale -10.0 +10.0
dev drate.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex drate.plt 
axis crate
yscale -5.0 +5.0
dev cdrate.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex cdrate.plt 
freq X
edit inputs
axis pcal_diff(8)
yscale
dev refdpcalx.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex refdpcalx.plt 
axis mbdelay
yscale -25 +25
dev mbdx.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex mbdx.plt 
axis cmbdelay
yscale -1 +1
freq X
edit inputs
dev cmbdx.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex cmbdx.plt 
unflag freq
freq S
edit inputs
axis pcal_diff(6)
yscale
dev refdpcals.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex refdpcals.plt 
axis mbdelay
yscale -100 +100
dev mbds.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex mbds.plt 
axis cmbdelay
yscale -1 +1
dev cmbds.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex cmbds.plt 

unflag duplicates
unflag freq
edit duplicates procdate
param 1 2 6 7
freq X
edit inputs
yscale
axis pcal_amp(1) pcal_phase(1)
dev pcalapx1.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex pcalapx1.plt 
axis pcal_amp(2) pcal_phase(2)
dev pcalapx2.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex pcalapx2.plt 
axis pcal_amp(3) pcal_phase(3)
dev pcalapx3.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex pcalapx3.plt 
axis pcal_amp(4) pcal_phase(4)
dev pcalapx4.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex pcalapx4.plt 
axis pcal_amp(5) pcal_phase(5)
dev pcalapx5.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex pcalapx5.plt 
axis pcal_amp(6) pcal_phase(6)
dev pcalapx6.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex pcalapx6.plt 
axis pcal_amp(7) pcal_phase(7)
dev pcalapx7.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex pcalapx7.plt 
axis pcal_amp(8) pcal_phase(8)
dev pcalapx8.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex pcalapx8.plt 
unflag freq
freq S
edit inputs
axis pcal_amp(1) pcal_phase(1)
dev pcalaps1.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex pcalaps1.plt 
axis pcal_amp(2) pcal_phase(2)
dev pcalaps2.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex pcalaps2.plt 
axis pcal_amp(3) pcal_phase(3)
dev pcalaps3.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex pcalaps3.plt 
axis pcal_amp(4) pcal_phase(4)
dev pcalaps4.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex pcalaps4.plt 
axis pcal_amp(5) pcal_phase(5)
dev pcalaps5.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex pcalaps5.plt 
axis pcal_amp(6) pcal_phase(6)
dev pcalaps6.plt/hpgl
plot
clear plot
!lp -onb -dnetjet -oduplex pcalaps6.plt 
unflag all
qcode
freq XS
base AK AT AE AV KT KE KV TE TV EV
edit inputs
edit duplicates qcode
snrmin 0
snrmax 30
fraction <=30%
type 2
edit inputs
sort freq
sort base
sort time
outver 1
write shrt1.out
unflag all
freq XS
base DA DK DL DT DE DV AL KL LT LE LV
edit inputs
edit duplicates qcode
snrmin 0
snrmax 30
fraction <=20%
type 2
edit inputs
sort freq
sort base
sort time
outver 1
write shrt2.out
!cat shrt1.out shrt2.out > shrtscan.out
!rm shrt1.out
!rm shrt2.out
exit