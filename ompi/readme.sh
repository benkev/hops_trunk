# This directory contains (at the moment) a few tutorials for
# exploring how to use Open MPI.  Eventually it could contain
# non-DiFX OMPI tools.  To avoid polluting the svn space, we'll
# use rsync to make a copy in /tmp/ompi and build there.
TMP=/tmp/ompi

[ -z "$vers" ] && openmpi=openmpi
[ -n "$vers" ] && openmpi=openmpi-$vers
[ -x /opt/$openmpi/bin/mpicc ] || {
    echo "you have apparently set an environment variable 'vers'"
    echo "to something that does not exist--vers=$vers"
    exit 1
}
[ -f Makefile ] && make distclean

[ -d /tmp/ompi ] || mkdir /tmp/ompi
echo
echo synchronizing sources to $TMP
echo
rsync -a --delete ./ $TMP
cd /tmp/ompi

aclocal -I m4
autoconf
autoheader
automake -a

#./configure --prefix=$HOME MPICC=/opt/$openmpi/bin/mpicc
./configure MPICC=/opt/$openmpi/bin/mpicc
#make all install
make all

echo $TMP now contains:
ls

cat <<EOF

Then, assuming you install on each of the named hosts, things
such as these should work (use hosts that have /tmp/ompi built):

export bd=`pwd`
# for h in ... ; do ssh \$h ls -l \$bd/hello_ompi ... ; done
\$bd/test.sh hello sc04 sc05 sk22 sk24 sk29 sk32
\$bd/test.sh ring sc04 sc05 sk22 sk24 sk29 sk32
\$bd/test.sh ringload 1M sc04 sc05 sk22 sk24 sk29 sk32

EOF

exit

#
# eof
#
