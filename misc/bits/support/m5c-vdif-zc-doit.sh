#!/bin/sh
#
# run through the cases

d1=../data/m5c_kk_20.m5c
o1=28450448
d2=../data/vdif_cc_20.vdif
o2=82363360

ff=64
fc=`expr $ff / 2`
FF=`expr $ff \* 16`
ffts=1000000

for ch in {0..9} {10..15}
do
    lab=`(echo obase=16 ; echo $ch) | bc -q`
    sch=`expr $ch \* $fc`

    cat > zc-$lab.in <<-....EOF
	$d1
	Mark5B-2048-16-2
	$ch
	$o1
	$ff
	0
	$fc
	$d2
	VDIF_8192-2048-1-2
	0
	$o2
	$FF
	$sch
	$fc
	zc-$lab.vis
	zc-$lab.lag
	$ffts
....EOF

    zerocorr zc-$lab.in > zc-$lab.out 2>&1
    ls -l zc-$lab.*
done

#
# eof
#
