#!/bin/bash
#
# Script to manufacture a cf
#
ME=`basename $0`
VERSION='$Id: fearfit-make-cf.sh 631 2012-02-07 22:06:05Z gbc $'
#
prefix=${prefix-'cf'}
samplers=${samplers-'abcdefghijklmno qrstuvwxyzABCDE'}
nsamplers=2
cfdir=${cfdir-'cfs'}
expn=${expn-'9999'}
target=${target-'target'}
stamp=${stamp-'*'}
ref=S
rem=C
ref_freq=229345.0
swap=0
verb=false
#
USAGE="$ME [options] scans

where the options are suitably quoted keyword=value pairs
and the defaults are in parentheses:

    verb        set this to true if you want more feedback
    prefix      prepended to the scan name ($prefix)
    samplers    a pair of freq-code strings ($samplers)
    cfdir       a directory ($cfdir) for the new control files
    expn        scans are expected in \$DATADIR/$expn/
    target      the root file is $target.$stamp in scan directory
    stamp       is the time stamp of the root file
    ref         id of the reference station ($ref)
    rem         id of the remote station ($rem)
    swap        if you want the manual phase on rem rather than ref
    words       a string of additional control file directives
    ref_freq    ref_freq ($ref_freq) for the joined fit

and the scans one or more scans of fourmered data to be processed.
This script generates a fourfit control file with manual pc phases
and delay offset so as to (try to) flatline the multiband delay.

So for example, assuming the correlation $ref$rem exists, and you
want to assign phases/delays to $rem based on all but the first 500
seconds of the scan(s), this command

    $ME expn=9095 target=1921-293 swap=1 words=\"'stop -500'\" \\
        095-1310 095-1355 ...

generates 2 (or more) control files as well as a postscript plot for
each.  The derived phases and delays are assigned to the reference
station.  Control files are considered 'precious', so this script will
not overwrite them (i.e. use a different prefix, cfdir, or nuke 'em).
The additional directives are appended to fearfit runs, but are not
included in the control file.
"
#
[ $# -eq 0 -o x$1 = 'x--help' ] && { echo "$USAGE" ; exit 0 ; }
[ x$1 = 'x--version' ] && { echo "$VERSION" ; exit 0 ; }
#
while [ $# -ge 1 ] ; do case "$1" in
verb=*)     eval "$1" ;;
prefix=*)   eval "$1" ;;
samplers=*) eval "$1" ;;
cfdir=*)    eval "$1" ;;
expn=*)     eval "$1" ;;
target=*)   eval "$1" ;;
stamp=*)    eval "$1" ;;
ref=*)      eval "$1" ;;
rem=*)      eval "$1" ;;
swap=*)     eval "$1" ;;
words=*)    eval "$1" ;;
ref_freq=*) eval "$1" ;;
*)          break ;;
esac ; shift ; done
scans="$@"

[ -d "$cfdir" ] || mkdir $cfdir
[ -d "$cfdir" ] || { echo could not make $cfdir >&2 ; exit 1; }

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

# HOPS_FEARFIT_PCPHASE_VERB
# HOPS_FEARFIT_PCPHASE_NOMAST
[ "$swap" -eq 0 ]                     || export HOPS_FEARFIT_PCPHASE_SWAP=1
[ -n "$HOPS_FEARFIT_PCPHASE_MEDFR"  ] || export HOPS_FEARFIT_PCPHASE_MEDFR=0.5
# HOPS_FEARFIT_PCPHASE_POLICY
# HOPS_FEARFIT_PCPHASE_NCHAN
# HOPS_FEARFIT_PCPHASE_DELTA
# HOPS_FEARFIT_PCPHASE_DERATE

set -- $samplers
allfreqs="`frq $1` `frq $2`"

for scan in $scans
do
    ctl=$cfdir/$prefix-$scan.cf
    [ -f $ctl ] && { echo skipping $prefix-$scan ; continue ; }
    # echo looking for $DATADIR/$expn/$scan/$target.$stamp
    set -- $DATADIR/$expn/$scan/$target.$stamp no-such-file
    root=$1
    [ -f "$root" ] || { echo no root for scan $scan found ; exit 1 ; }
    cat > $ctl <<-....EOF
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
	max_parity 0.01
	use_samples true
	ref_freq $ref_freq
	optimize_closure true
	dc_block false

	pc_mode manual
	samplers $nsamplers $samplers
	freqs $allfreqs

....EOF

    export HOPS_FEARFIT_PCPHASE_NOMAST=1

    psoutput=$cfdir/$prefix-$scan.ps
    cmd="fearfit -t -d diskfile:/dev/null -b$ref$rem -c$ctl $root"
    fin="fearfit -t -d diskfile:$psoutput -b$ref$rem -c$ctl $root"
    pat='^([^*]|. snr)'
    fpt='^(. snr)'

    chans=''
    for s in $samplers
    do
        freqs=`frq $s`
        echo $scan $ref$rem w/ $freqs
        $verb && echo \
            $cmd set freqs $freqs $words && wc -l $ctl
        cf=`$cmd set freqs $freqs $words | egrep "$pat"`
        echo "$cf" >> $ctl
        chans="$chans $freqs"
    done

    echo $scan $ref$rem w/ $chans
    $verb && echo \
        $cmd set freqs $chans $words && wc -l $ctl
    cf=`$cmd set freqs $chans $words | egrep "$pat"`
    echo "$cf" >> $ctl

    export HOPS_FEARFIT_PCPHASE_NOMAST=0

    echo $scan $ref$rem w/ $chans
    $verb && echo \
        $cmd set freqs $chans $words && wc -l $ctl
    cf=`$cmd set freqs $chans $words | egrep "$pat"`
    echo "$cf" >> $ctl

    echo $scan $ref$rem w/ $chans .
    $verb && echo \
        $cmd set freqs $chans $words && wc -l $ctl
    cf=`$fin set freqs $chans $words | egrep "$fpt"`
    echo "$cf" >> $ctl
    echo "$cf"

done
#
# eof
#
