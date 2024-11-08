#!/bin/sh
#
# script to unload things from tarball into local source
#

[ -z "$HOPS_ROOT" ] && HOPS_ROOT=$HOME/HOPS
tardir=/correlator/dist/hops/tar
hopsave=${1-"no-such-file"}

[ -d $HOPS_ROOT ] || {
    echo No directory $HOPS_ROOT
    exit 1
}

[ -d $HOPS_ROOT/src ] && cd $HOPS_ROOT/src || {
    echo No directory $HOPS_ROOT/src in which to unload
    exit 2
}

[ -f $hopsave ] || {
    echo No tarball $hopsave to unload, perhaps the most recent of:
    ls -1tr $tardir/* | tail -3
    exit 3
}

tar zxf $hopsave

#
# eof
#
