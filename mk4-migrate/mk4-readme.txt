#
# Auto*d HOPS
#
# Temporary home in /correlator/dist/hops/
#

    mk4-import.sh	imports sources from $source
    mk4-save.sh		captures local mods in a tarball
    mk4-restore.sh	restores local mods from tarball to $HOPS_ROOT/src
    tar/		directory of timestamped tarballs

# by default:
#   source=/correlator/dist/mk4
#   hack=/correlator/dist/hops
#   HOPS_ROOT=$HOME/HOPS
#   HOPS_VERS=0.1

cd $HOPS_ROOT
$hack/mk4-import.sh
$hack/mk4-restore.sh $hack/tar/hopsave-stamp.tgz

cd $HOPS_ROOT/src
aclocal
autoconf
automake -a

mkdir $HOPS_ROOT/bld
cd $HOPS_ROOT/bld
$HOPS_ROOT/src/configure
. ./hops.bash
make
make install
make distcheck
# scp -p hops-$HOPS_VERS.tar.gz dewi:$hack
# rm -rf $HOPS_ROOT/bld

#
# scp into (anywhere, but I've used) ~/HOPS
#
tar zxf ./hops-$HOPS_VERS.tar.gz 
mkdir bld-$HOPS_VERS
cd bld-$HOPS_VERS
../hops-$HOPS_VERS/configure 
. ./hops.bash
# HOPS_SETUP=false . /home/gbc/HOPS/bld-$HOPS_VERS/hops.bash
make
make install

#
# tire-kick testing....
#
cd $HOPS_ROOT/hops-$HOPS_VERS/data
cd ff_testdata/
type fourfit
fourfit -pt -bAE:X -ctestdata/2836/cf2836 testdata/2836/scan001/2145+067.olomfh

#
# eof
#
