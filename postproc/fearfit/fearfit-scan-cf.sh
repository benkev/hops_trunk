#!/bin/bash
#
# Script to survey manufactured cfs
#
ME=`basename $0`
VERSION='$Id: fearfit-scan-cf.sh 627 2012-02-04 00:54:24Z gbc $'
#
prefix=${prefix-'cf'}
samplers=${samplers-'abcdefghijklmno qrstuvwxyzABCDE'}
nsamplers=2
cfdir=${cfdir-'cfs'}
#expn=${expn-'9999'}
#target=${target-'target'}
#stamp=${stamp-'*'}
ref=S
rem=C
swap=0
odir=${odir-'Output'}
cfscan=${cfscan-'cfscan.txt'}

USAGE="$ME [options] roots

where the options are suitably quoted keyword=value pairs
and the defaults are in parentheses:

    prefix      prepended to the scan name ($prefix)
    samplers    a pair of freq-code strings ($samplers)
    cfdir       a directory ($cfdir) for the new control files
    ref         id of the reference station ($ref)
    rem         id of the remote station ($rem)
    swap        if you want the manual phase on rem rather than ref
    odir        name of output dir ($odir) for ps plots and
    cfscan      name of summary report (\$odir/$cfscan)

and the roots are typically globbed by the shell.  E.g.

    $ME samplers=\"'ijklmno qrstuvw'\" \$DATADIR/9095/*/1921*

for every root found, fit results are added to the scan file,
and adjustments are attempted to improve the fit.
"
#
[ $# -eq 0 -o x$1 = 'x--help' ] && { echo "$USAGE" ; exit 0 ; }
[ x$1 = 'x--version' ] && { echo "$VERSION" ; exit 0 ; }
#
while [ $# -ge 1 ] ; do case "$1" in
prefix=*)   eval "$1" ;;
samplers=*) eval "$1" ;;
cfdir=*)    eval "$1" ;;
ref=*)      eval "$1" ;;
rem=*)      eval "$1" ;;
swap=*)     eval "$1" ;;
odir=*)     eval "$1" ;;
cfscan=*)   eval "$1" ;;
*)          break ;;
esac ; shift ; done
roots="$@"

cfscan=$odir/$cfscan

# convert a sampler string into a space separated list of channels
function frq {
    fr=$1 freqs=''
    while [ -n "$fr" ]
    do
            f=`expr $fr : '\(.\).*'`
            fr=`expr $fr : '.\(.*\)'`
            freqs="$freqs $f"
    done
    echo $freqs
}

set -- $samplers
sets="set freqs `frq $1` `frq $2`"

[ -d $odir ] || mkdir $odir
rm -f $cfscan

[ -n "$HOPS_FEARFIT_PCPHASE_VERB"   ] || export HOPS_FEARFIT_PCPHASE_VERB=0
# HOPS_FEARFIT_PCPHASE_NOMAST
[ "$swap" -eq 0 ]                     || export HOPS_FEARFIT_PCPHASE_SWAP=1
# HOPS_FEARFIT_PCPHASE_MEDFR
[ -n "$HOPS_FEARFIT_PCPHASE_POLICY" ] || export HOPS_FEARFIT_PCPHASE_POLICY=2
[ -n "$HOPS_FEARFIT_PCPHASE_NCHAN"  ] || export HOPS_FEARFIT_PCPHASE_NCHAN=4
[ -n "$HOPS_FEARFIT_PCPHASE_DELTA"  ] || export HOPS_FEARFIT_PCPHASE_DELTA=0.0
# HOPS_FEARFIT_PCPHASE_DERATE

cat > $cfscan <<EOF
*
* `date`
*
* HOPS_FEARFIT_PCPHASE_VERB=$HOPS_FEARFIT_PCPHASE_VERB
* HOPS_FEARFIT_PCPHASE_NOMAST=$HOPS_FEARFIT_PCPHASE_NOMAST
* HOPS_FEARFIT_PCPHASE_SWAP=$HOPS_FEARFIT_PCPHASE_SWAP
* HOPS_FEARFIT_PCPHASE_MEDFR=$HOPS_FEARFIT_PCPHASE_MEDFR
* HOPS_FEARFIT_PCPHASE_POLICY=$HOPS_FEARFIT_PCPHASE_POLICY
* HOPS_FEARFIT_PCPHASE_NCHAN=$HOPS_FEARFIT_PCPHASE_NCHAN
* HOPS_FEARFIT_PCPHASE_DERATE=$HOPS_FEARFIT_PCPHASE_DERATE
* HOPS_FEARFIT_PCPHASE_DELTA=$HOPS_FEARFIT_PCPHASE_DELTA
*
EOF

[ "$swap" -eq 1 ] && stn=$rem || stn=$ref

for rd in $roots
do
    r=`basename $rd`
    for dc in $cfdir/$prefix-*.cf
    do
        c=`basename $dc`
        ofile=$odir/$r-${c/.cf/}

        echo '* snr asis '$r'-'$c 1>> $cfscan
        dfo=diskfile:$ofile-0.ps
        echo '*' \
        fearfit -t -d $dfo -b$ref$rem -c $dc $rd $sets >> $cfscan
        fearfit -t -d $dfo -b$ref$rem -c $dc $rd $sets >> $cfscan
        echo '*' exit status $? >> $cfscan

        echo '* snr dgap '$r'-'$c 1>> $cfscan
        dfo=diskfile:$ofile-1.ps
        spha=`sed -e '/applying sampler phase delta/,+2!d' \
                  -e 's/..applying.sampler.phase.*//' $cfscan |\
              tail -3`
        sadj="if station $stn $spha"
        echo '* applied this:' >> $cfscan
        echo "* $sadj" >> $cfscan
        echo '*' \
        fearfit -t -d $dfo -b$ref$rem -c $dc $rd $sets $sadj >> $cfscan
        fearfit -t -d $dfo -b$ref$rem -c $dc $rd $sets $sadj >> $cfscan
        echo '*' exit status "$?" >> $cfscan
        echo '*' >> $cfscan

        ls -l $ofile-?.ps
        ls -l $ofile-?.ps | sed 's/^/* /' >> $cfscan
    done
done

echo \
grep snr $cfscan \| paste -d\' \' - - 
grep snr $cfscan  | paste -d' '   - - 

#
# eof
#
