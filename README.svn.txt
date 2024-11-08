How to install HOPS from an SVN checkout


The tarball contains files which are automaticaly generated and released
with the tarball distribution.  If you are proceding from a fresh checkout
there are a few things you need to do to get started.

0. SVN configuration
--------------------

SVN will work out of the box, but there are a number of files that
get dropped in the source directory that you may wish to tell it to
ignore.  If you do so, then "svn status" will give you a relatively
short list of things that you may have modified, added but not committed,
and so forth.  The line to edit is in ~/.subversion/config and you will
need to uncomment the global-ignores = line and put the following on it.
(Unfortunately it all has to be ONE single line which is hard to show here.)

  global-ignores = *.o *.lo *.la .*.rej *.rej .*~ *~ .#* .DS_Store aclocal.m4
  autom4te.cache configure depcomp install-sh compile missing Makefile.in
  makefile.in config.* *config.h.in *.swp INSTALL COPYING ltmain.sh #*#

1. SVN installation
-------------------

For clarity, I've defined the variable HOPS_ROOT prior to the installation,
but it doesn't actually need to be defined if you start in the right place
and use relative paths correctly.

First check out the sources (I've skipped branches and tags):

    cd $HOPS_ROOT
    HOPS_ROOT=`pwd`
    svn checkout svn+ssh://vault.haystack.mit.edu/svn/hops/trunk

You should see something like this:

    cd trunk
    svn info
    URL: svn+ssh://vault.haystack.mit.edu/svn/hops/trunk
    Repository Root: svn+ssh://vault.haystack.mit.edu/svn/hops
    Repository UUID: 086df2f8-b83c-4c1e-83a2-71b531df77b7

Do the auto-whatever initializations in this order:

    cd trunk
    libtoolize --force
    aclocal -I m4
    autoconf
    autoheader
    automake -a

For convenience ./autogen.sh after the 'cd trunk' does that; it may
also be invoked with ANY argument to wipe all the automade things.

autoscan be run occasionally to update the list of suggested configure rules,
but it will usually suggest macros that did not exist in the minimum autoconf
level (2.59) which unfairly excludes 20-year old systems.

You'll see some comments about linking of several tools into this directory
(i.e. missing install-sh depcomp config.sub config.guess).  It is preferable
to go somewhere else to actually build the tools:

    mkdir $HOPS_ROOT/build-3.xx
    cd $HOPS_ROOT/build-3.xx
    $HOPS_ROOT/trunk/configure --enable-devel # and other options...

See the tarball readme (README.txt.in) for some help with the various
options.  Now you can build it:

    make
    make check
    make install

make install should have placed hops.bash in ~/bin if you have one;
thereafter you can source it (from there or wherever else you've put it):

    . ~/bin/hops.bash

This will define HOPS_ROOT and set up a working environment (path adjustments
and some environment variables).

2. SVN tarball
--------------

To generate a tarball:

    $ make distcheck

makes the tarball and verifies that the tarball can in fact be used.
There are a variety of tarball targets and Makefile variables that can
be set to create special tarballs of partial distributions.  At the
moment the original HOPS package has been partitioned into an essential
part (i.e. fourfit, alist, and other sine qua non pieces) and the rest
(i.e. things that may not be ready for public release).  There are some
new pieces to provide support for Mark5 units, and DiFX in a HOPS context.

3. Local hacks
--------------

There are macros in the m4 directory which are used to insert some "local"
modifications into the configure process.  These are just "hacks" at present.

