#!/bin/bash
#
# script to setup a correlation of mk5c or vdif files....
# this version is targeted at the x-fringe test.
#
export HOPS_JULIAN_EPOCH=UTC
export DIFX_GRINDERS='sk04 sk05 sk06'
export DIFX_PARAMS=`pwd`/params.sh

[ $# -gt 1 ] || { echo "Usage: $0 tdir datafiles" ; exit 1 ; }
tdir=${1-'n00'} ; shift
files="$@"

[ -x `which hops_time` ] || { echo source hops.bash ; exit 1; }
[ -x `which mpifxcorr` ] || { echo source setup-difx.bash ; exit 1; }

verb=${VERB-'false'}

export ENUM=7000
export target=Noise
H=`hostname`

export st_info=''
dfiles='# ST : datafiles : details'

ref=''
rem=''

#
# st_info is an environment variable with all we need to generate this
# fake vex file for the observation.  The 5 fields per file are (loosely)
#   2-letter antenna name
#   data file host name
#   data file path name
#   file format (e.g. VDIF/8224/2)
#   das name (unused)
#
# anoise generates (skipping duplicates):
#   firstmjd=56107.5498842593
#   path=/data-sc01/gbc/alma/data/Aa.vdif
#   id=Aa
#   format="VDIF/1032/2"
#   chans=1
#   finalmjd=56107.5499305556
#
for F in $files
do
    set -- `anoise -x $F`
    [ $# -eq 10 ] || { echo anoise came up only with $# ; continue ; }
    an="$*"
    while [ $# -ge 1 ] ; do eval "$1" ; shift ; done
    [ -n "$st_info" ] && st_info="$st_info $id $H $F" || st_info="$id $H $F"
    st_info="$st_info $format ch$chans"
    dfiles="$dfiles
# $id : $F : $an "
    [ -z "$ref" ] && {
	ref=`expr $id : '\(.\).'`
    } || {
	[ -z "$rem" ] && rem=`expr $id : '\(.\).'`
    }
done

export mjdStart=$firstmjd
export mjdStop=$finalmjd
export scanStart=`hops_time -q Vex $mjdStart | sed 's/.....s$/s/'`
export scanStop=`hops_time -q Vex $mjdStop | sed 's/.....s$/s/'`

export scanName=''
export force='--force'

#[ -n "$FFTRES"  ] || export FFTRES=0.00003125
#[ -n "$SPECRES" ] || export SPECRES=0.50
#[ -n "$NINT"    ] || export NINT=0.48
#[ -n "$SUBINT"  ] || export SUBINT=32000000
[ -n "$FFTRES"    ] || export FFTRES=0.00390625
[ -n "$SPECRES"   ] || export SPECRES=0.50
[ -n "$NINT"      ] || export NINT=0.80
[ -n "$SUBINT"    ] || export SUBINT=32000000
[ -n "$FF_OPTS_X" ] || export FF_OPTS_X="-b$ref$rem"

comment="
$dfiles
# Variables:
 mjdStart=$mjdStart
  mjdStop=$mjdStop
scanStart=$scanStart
 scanStop=$scanStop
 scanName=$scanName
   target=$target
 grinders=$DIFX_GRINDERS
   params=$DIFX_PARAMS
   FFTRES=$FFTRES
  SPECRES=$SPECRES
     NINT=$NINT
   SUBINT=$SUBINT
FF_OPTS_X=$FF_OPTS_X
    force=$force
"
echo "$comment"
$verb && echo "# St_info: $st_info"

export v2dcomment="$dfiles
"
$ECHO ./corel_alma.sh $tdir $ENUM

#
# eof
#
