#!/bin/sh
#
# script to save things to prevent against accidental overwrite
#

HOPS_ROOT=$HOME/HOPS
source=/correlator/dist/mk4/
hack=/correlator/dist/hops/

save="
./mk4-migrate/mk4-import.sh
./mk4-migrate/mk4-save.sh
./mk4-migrate/mk4-restore.sh
./mk4-migrate/mk4-readme.txt
./configure.ac
./help/Makefile.am
./hops.bash.in
./hops_config.h.in
./Makefile.am
./README.txt.in
./README.haystack.txt
./include/Makefile.am
./scripts/Makefile.am
./correlator/Makefile.am
./correlator/CorAsc2/Makefile.am
./correlator/prog/Makefile.am
./data/Makefile.am
./data/ff_testdata/Makefile.am
./data/ff_testdata/testfourfit.sh
./data/ff_testdata/chk_env.sh
./data/ff_testdata/chk_ff_2843.sh
./data/ff_testdata/chk_ffb_2843.sh
./data/ff_testdata/chk_ff_2836.sh
./data/ff_testdata/chk_alist.sh
./data/ff_testdata/chk_adump.sh
./data/ff_testdata/chk_aedit.sh
./data/ff_testdata/chk_fringex.sh
./data/ff_testdata/chk_cofit.sh
./postproc/adump/Makefile.am
./postproc/aedit/Makefile.am
./postproc/alist/Makefile.am
./postproc/average/Makefile.am
./postproc/bispec/Makefile.am
./postproc/calamp/Makefile.am
./postproc/cofit/Makefile.am
./postproc/coterp/Makefile.am
./postproc/fold/Makefile.am
./postproc/fourfit/Makefile.am
./postproc/fourmer/Makefile.am
./postproc/fplot/Makefile.am
./postproc/fringex/Makefile.am
./postproc/linkdata/Makefile.am
./postproc/Makefile.am
./postproc/pratio/Makefile.am
./postproc/search/Makefile.am
./postproc/sub/Makefile.am
./postproc/sub/x_fplot/Makefile.am
./sub/afio/Makefile.am
./sub/dfio/Makefile.am
./sub/Makefile.am
./sub/util/Makefile.am
./sub/vex/Makefile.am
"

stamp=`date +%Y-%m-%dT%H-%M-%S`

cd $HOPS_ROOT/src
#tar zcf $HOPS_ROOT/tar/hopsave-$stamp.tgz $save
tar zcf hopsave-$stamp.tgz $save

[ -w $hack/tar ] && {
    mv hopsave-$stamp.tgz $hack/tar
    cp -p mk4-* $hack
    echo moved hopsave-$stamp.tgz and mk4-\* to $hack
} || {
    scp -p hopsave-$stamp.tgz dewi:$hack/tar 
    scp -p mk4-* dewi:$hack
    rm hopsave-$stamp.tgz
    echo scp\'d hopsave-$stamp.tgz and mk4-\* to dewi:$hack
}

#
# eof
#
