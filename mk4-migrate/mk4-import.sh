#!/bin/sh
#
# script to pull original mk4 correlator materials into
# this directory via rsync and perhaps perform some cleanup.
#

[ -z "$HOPS_ROOT" ] && HOPS_ROOT=$HOME/HOPS
[ -z "$source" ] && source=/correlator/dist/mk4/

[ $# -eq 0 -o "x$1" = x"--help" ] && dirs="
doc/help
src/include
src/postproc
src/scripts/share
src/sub
src/correlator/CorAsc2
../../data/ff_testdata
prog/text
../../prog/doc
" || dirs="$@"

[ ${1-"help"} = --help ] && {
    echo Usage: $0 '[source directories]'
    echo ''
    echo Import is from one of these directories
    echo of $source
    for d in $dirs ; do echo '  '$d ; done
    echo into subdirectories of \$HOPS_ROOT/src
    echo HOPS_ROOT defaults to $HOME/HOPS
    exit 0
}

for d in $dirs
do
    case $d in
    doc/help)		    dest=$HOPS_ROOT/src ;;
    src/include)	    dest=$HOPS_ROOT/src ;;
    src/postproc)	    dest=$HOPS_ROOT/src ;;
    src/scripts/share)	    dest=$HOPS_ROOT/src ;;
    src/sub)		    dest=$HOPS_ROOT/src ;;

    src/correlator/CorAsc2)
	dest=$HOPS_ROOT/src/correlator
	[ -d $HOPS_ROOT/src/correlator ] ||
	    mkdir $HOPS_ROOT/src/correlator
	;;
    ../../data/ff_testdata)
	dest=$HOPS_ROOT/src/data
	[ -d $HOPS_ROOT/src/data ] ||
	    mkdir $HOPS_ROOT/src/data
	;;
    *prog/*)
	dest=$HOPS_ROOT/src/correlator/prog
	[ -d $HOPS_ROOT/src/correlator/prog ] ||
	    mkdir $HOPS_ROOT/src/correlator/prog
	;;

    *)  echo no plan for $d
	;;

    esac

    [ -d $dest ] || mkdir -p $dest

    echo \
    rsync -a $source/$d $dest
    rsync -a $source/$d $dest

    find $dest -name Makefile -exec mv {} {}.mk4 \;

    case $d in
    src/sub)
	mv  $HOPS_ROOT/src/sub/dfio/bytflp.h \
	    $HOPS_ROOT/src/sub/dfio/bytflp.h.dist
	sed -e '1i#include "hops_config.h"' \
	    $HOPS_ROOT/src/sub/dfio/bytflp.h.dist > \
	    $HOPS_ROOT/src/sub/dfio/bytflp.h
	mv  $HOPS_ROOT/src/sub/afio/clear_fsumm.c \
	    $HOPS_ROOT/src/sub/afio/clear_fsumm.c.dist
	xxx='    memset(dsumm->baseline, 0, sizeof(dsumm->baseline));'
	sed -e '/dsumm..baseline/s/.*/'"$xxx"'/' \
	    $HOPS_ROOT/src/sub/afio/clear_fsumm.c.dist > \
	    $HOPS_ROOT/src/sub/afio/clear_fsumm.c
	;;
    src/scripts/share)
	mv  $HOPS_ROOT/src/share \
	    $HOPS_ROOT/src/scripts
	mv  $HOPS_ROOT/src/scripts/vhelp \
	    $HOPS_ROOT/src/scripts/vhelp.dist
	sed -e 's/.SRC.awk_scripts.//' \
	    $HOPS_ROOT/src/scripts/vhelp.dist > \
	    $HOPS_ROOT/src/scripts/vhelp
	;;
    ../../data/ff_testdata)
	mv  $HOPS_ROOT/src/data/ff_testdata/testlinux \
	    $HOPS_ROOT/src/data/ff_testdata/testlinux.dist
	sed -e '/fourfit.fourfit/s/^.*-pt/fourfit -pt/' \
	    $HOPS_ROOT/src/data/ff_testdata/testlinux.dist > \
	    $HOPS_ROOT/src/data/ff_testdata/testfourfit.sh
	;;

    *)  echo no cleanup on $d
	;;

    esac
done

#
# eof
#
