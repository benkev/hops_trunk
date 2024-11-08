This file is a place to drop various random things relevant to Haystack.

(This file is ancient...)

#
# Some (random) ISSUES:
#

What's the plan, stan?

General stuff
    need to -Wall and remove the explicit extern..() lines
    pruning to includes/sources that are really used, attic the rest

Need DiFX-SVN/HOPS-SVN sync check.

mac os gcc 4.1 issues with fourfit
    1 == 1.0 != 1. go figure
    other mac os issues?
    #if .. #elseif .. #endif  should have Darwin case

vincent's list
     1. only 2 digits on amplitudes
     2. header/column alignment
     3. v5 headers missing
     4. see 2
     5. Y2K compliance
     6. linkdata
     7. vhelp fix
     8. docs out of date
     9. t_cohere of fourfit doesn't take an integer (!?)
    10. coterp busted, needs el, az, u, v, ...
    11. erratic fourfit processing behavior
    12. fourmer + fourfit goes nowhere
    13. search/cofit compatibility issues
    14. multi-page ps -> single page
    15. t_cohere of fourfit is unreliable
    16. closure amplitudes not yet implemented
    17. no ability to phase reference one band/pol to another
    18. phase cal issues
    19. forcing closure
    20. no global fringe fit

    ==> need a v6 with header/column alignment and better precision

#linkdata v hops_data_links.pl
#    hops_data_links.pl could become a replacement for linkdata
#    hops_data_links.pl needs to be added to vhelp
#    need to learn what is or isn't being done by each.
#    then possibly remove linkdata
#    help for hops_data_links.pl

adump
    is adump adequate, or is a perl/python script more useful
    consider adumper.pl?

configuration:
    @PERL@
    @PYTHON@
    @GS@

gets() marked "unsafe" on Apple.
    so don't use it...

alist
    only does type 2?
    check implicit use of DATADIR

fringex
    whazzup with "epoch" (here and elsewhere)

fourfit
  criteria_match.c
    compares only 16 (of 32) bytes in source name
  set_pointers.c/parse_cmdline.c
    if -P not used, the processing will be of the first one encountered
# various unlinked sources:
#   print_at.c
#   output.c
#   print_left.c
#   put_char_at.c
  tmpnam (probably should have a library function replacement)

Random, unfiled

    pformat.txt
    range limits and reconcile
	pformat.txt
	/correlator/prog/text/pformat.txt
	/correlator/dist/mk4/prog/text/pformat.txt

    case-sensitivity of 2-letter station names (difx2mark4/vex)
    can we relax the 1-letter thing (which is apparently a Mk3, limitation)

    remove overly restrictive path-length allocations
    (open_datafile expdir only 10 chars?)

    Root code processing quantum/rollover (2018y057d15h45m02.0000s)

    ansify code from K&R prototypes
    80 cols fascist?

    many more tests

    killed plot windows isn't immediately caught by parent...

    --help
    --version
    so std-options can be activated

    reference hops_config.h everywhere
    #
    # Code migration.  There are two flavors of HOPS:
    #   Unmodified h/w correlator version
    #   Modified s/w correlator version
    #
    # The general plan is to preserve the h/w version unmodified
    # (except for "safe" bug fixes) and to add new functionality
    # in such a way that the h/w version never notices.  There are
    # two ways to test this:
    #   compile environment:   #ifdef HAVE_CONFIG_H will point to a file
    #                          of autoheader-created defines.
    #   run-time environment:  HOPS_SETUP will exist in the environment
    #                          and equal to "true" in normal "new" use.
    # So: if HAVE_CONFIG_H is not defined, we are in the h/w world.
    #     if HOPS_SETUP is not defined, we are in the h/w world.
    #
    # The mk4-migrate contains a few tools for synchonizing with the
    # "new" code with the old /correlator version.
    #

how to properly "Attic" files no longer needed
    initially, make a subdir "attic" and move the offenders there
    later, find and remove such things

how to track files actually used/modified
    probably a $Id$/eof pile is sufficient


#
# Package requirements
#
Debian

    /etc/apt/sources.list
	# for pgplot5
	deb http://ftp.us.debian.org/debian/ squeeze main non-free
	deb-src http://ftp.us.debian.org/debian/ squeeze main non-free

    apt-file update
    apt-get update
    apt-get -y install gcc make autoconf automake
    apt-get -y install rsync subversion subversion-tools
    apt-get -y install flex bison gdb
    apt-get -y install pgplot5 libx11-dev libpng12-dev
    apt-get -y install ghostscript-x ghostscript-doc psutils



#
# Current state at Haystack
#
/swc/hops installed on various machines:

    $ grep -l hops /swc/admin/crontabs/*gbc | cut -d. -f2 | tr -s \\012 ' '
    demi dewi gefera lcc muc00 muc02 muc09 muc10 muc13 sc01
    # Tue Jun 14 15:56:03 EDT 2011

# gefera:
    $ mkdir DIST-$VER ; cd DIST-$VER
    $ ../hops-$VER/configure
    $ make distcheck dist-local

    $ ../trunk/configure PGPLOT_DIR=/usr/local/pgplot-png \
      --enable-difx DIFXROOT=/home/gbc/DiFX/difx-root \
      --enable-devel

# gbc-lt:
    HOPS/MOPS/BEER/
    $HOPS_ROOT/trunk/configure --enable-devel \
	LDFLAGS=-L/usr/X11/lib PGPLOT_DIR=/opt/local/pgplot

# monarth: hmmm.
    #$ ../hops-$VER/configure --enable-pathx PNG12_LIB=/lib/libpng12.so.0
    $ ../hops-$VER/configure
    # updated to 3.5 Tue Jun 14 15:54:54 EDT 2011
# maxwell:
    $ ../hops-$VER/configure
    # updated to 3.5 Tue Jun 14 15:54:54 EDT 2011

#
# DiFX hacked config
#
sed '/^###HEAD/,/^###TAIL/c ### option disabled' configure.ac > configure_ac

[gbc@smm-pc trunk]$ svn status
?      mark5
?      difx
?      misc
?      configure.ac.ok
M      configure.ac
?      postproc/fold
?      postproc/coterp
?      postproc/calamp
?      postproc/pratio
?      postproc/cofit
?      postproc/bispec
?      postproc/search
?      postproc/average
?      correlator/prog


###
### STOP READING HERE....
###



# smm-pc:
    $ ../hops-$VER/configure
    # shifted to /swc nightly
# dewi:
    $ ../hops-$VER/configure
    # shifted to /swc nightly
# lcc:
    # /usr/local/pgplot is a 32-bit version
    $ ../hops-$VER/configure PGPLOT_DIR=/usr/local/pgplot64
    # shifted to /swc nightly
# demi:
    $ ../hops-$VER/configure --enable-xtrap
    # shifted to /swc nightly

# ccc: I have no account...

# and then for any of the above:
    $ make install check



#
# SVN configuration
#
See README.svn.txt.


### mk4-sync now largely not.

#
# This distribution is intended to remain somewhat consistent with
# the existing, standard /correlator installation.  Files found under
# the directories:
#   help include postproc scripts sub correlator data
# must have consistent counterparts in both trees, as synchronized
# through the script mk4-migrate/mk4-sync.sh.  On configured
# machines:

    $ ./mk4-sync.sh check | less

# generates a list of files that may need attention, and

    $ ./mk4-sync.sh --help

# provides some guidance on what to do.
#
# New functionality needs to be put in other directories to avoid
# extra work until the synchronization is no longer needed.
#

### original Makefile.am targets no longer needed:
tardifx: $(TARVERS).tar.gz
	chmod 644 $(TARVERS).tar.gz && \
	scp -p $(TARVERS).tar.gz smm-pc:/usr/local/src
# for tidying-up after dist wh
dist-cleanup:
	$(am__remove_distdir)
	rm -f *-@VERSION@*.tar.gz 
# this goes away (eventually)
dist-local: $(PKGVERS).tar.gz
	chmod 644 $(PKGVERS).tar.gz README.txt
	scp -p $(PKGVERS).tar.gz dewi:/correlator/dist/hops/$(TARBALL)
	scp -p $(PKGVERS).tar.gz gemini:/var/ftp/pub/hops/$(TARBALL)
	scp -p $(PKGVERS).tar.gz smm-pc:/usr/local/src/$(TARBALL)
	scp -p README.txt gemini:/var/ftp/pub/hops/$(READMEV).txt

### Original Migration Notes

4. Migration Notes
------------------

Given the historical tangling of the HOPS package with the Mk4 hardware
correlator software, it was deemed reasonable to install the HOPS
package outside of the (standard) /correlator tree.  This means it
is possible for the two software realms (HOPS and Mk4) to coexist.

The installation point (/Somewhere) can be a subdirectory of /correlator
if you like--the default install location should not overwrite the Mark4
installation locations.  In any case, you will need to adjust your setup
scripts to be sure of which set of tools are found first in your path.

Note however, that some of the environment variables are shared, so
that, it is not advisable to set up any given shell process with both
sets of executables.  E.g. TEXT points to a directory of certain text
files which the tools require, but it is found in different places:

    HOPS:   $HOPS_ROOT/$HOPS_ARCH/share/hops/text
    Mk4:    /correlator/prog/text

In practice this is not a problem as the files in each directory are
(at present, anyway) identical.

The sources for some of the the HOPS tools that are known not to be
working (properly) are present in this distribution.  If you want to
experiment with them, they can be enabled with a configure option:

    ../hops-@VERSION@/configure ... --enable-devel ...


#
# eof
#
