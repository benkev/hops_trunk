AC_PREREQ(2.59)
AC_INIT([chops], [0.00], [chops@chops.org])

LT_INIT([shared])
#we need LTDL in order to be able to install 
#the libs to some place other than /usr/local/lib/
LT_WITH_LTDL

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign -Wno-extra-portability subdir-objects])

AC_PROG_CC

################################################################################
AC_CONFIG_HEADERS([hops_config.h])



#
# For configure testing, set ac_hops=false to speed things up
#
if [test -z "$ac_hops"]; then ac_hops=true; else ac_hops=false; fi

# Checks for header files.
if [$ac_hops] ; then
AC_FUNC_ALLOCA
AC_HEADER_DIRENT
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h fcntl.h float.h inttypes.h limits.h])
AC_CHECK_HEADERS([locale.h netdb.h netinet/in.h stddef.h stdint.h])
AC_CHECK_HEADERS([stdlib.h string.h strings.h sys/socket.h unistd.h])
fi

# Checks for typedefs, structures, and compiler characteristics.
AC_C_BIGENDIAN(
    AC_DEFINE(WORDS_BIGENDIAN, 1, [this is a BigEndian Host]),
    AC_DEFINE(WORDS_BIGENDIAN, 0, [this is a LittleEndian Host])
)

# additional items for hops_config
#quick and dirty to get this working
AC_DEFINE_UNQUOTED(HOPS_SVN_REV, [$VERSION], [Haystack SVN rev number])
AC_DEFINE_UNQUOTED(HOPS_REVISION, [$VERSION], [HOPS package rev number])
AH_BOTTOM(
[/* Worry about the byte order in data files */
#if WORDS_BIGENDIAN
/* Data are fine as they are */
#else /* WORDS_BIGENDIAN */
# define BYTFLP
#endif /* WORDS_BIGENDIAN */])
if [$ac_hops] ; then
AC_C_CONST
AC_TYPE_SIZE_T
AC_STRUCT_TM
AC_C_VOLATILE
fi

# Checks for library functions.
if [$ac_hops] ; then
AC_FUNC_CLOSEDIR_VOID
AC_FUNC_LSTAT
AC_FUNC_LSTAT_FOLLOWS_SLASHED_SYMLINK
AC_FUNC_MALLOC
AC_FUNC_MEMCMP
AC_FUNC_REALLOC
AC_FUNC_STAT
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([floor getcwd isascii memset mkdir modf pow rint sqrt])
AC_CHECK_FUNCS([strcasecmp strchr strdup strerror strncasecmp strrchr])
AC_CHECK_FUNCS([strspn strstr])
fi


###############################################################################

# Checks for programs.
AC_PATH_PROG([AUTORECONF], [autoreconf], [/bin/false])
AC_PATH_PROG([ACLOCAL], [aclocal], [/bin/false])
AC_PROG_CC
AC_PROG_F77
AC_F77_LIBRARY_LDFLAGS
# this must follow AC_PROG_CC and AC_PROG_F77 for some reason
if [test x$enable_mark5 = xyes] ; then
	AC_PROG_CXX
else
	# define the conditionals AC_PROG_CXX normally does
	am__fastdepCXX_TRUE='#'
	am__fastdepCXX_FALSE='#'
fi
# HOPS prints and displays with ghostscript (gs)
# which might be at /usr/bin/gs, but which could be elsewhere
AC_PATH_PROG(GS_EXEC, [gs], [/bin/false])
AC_SUBST(GS_EXEC)
# copypage changed behavior between gs versions 9.05 and 9.10
# 8.7x (sci linux) copypage ok
# 9.05 (fedora 16) copypage ok
# 9.10 (MacPorts) copypage not ok
# 9.14 (fedora 19) copypage not ok
AC_ARG_VAR(GS_COPYPAGE_OK, [Set to 1 when gs version is prior to 9.10])
if [test "x$GS_EXEC" = "x/bin/false" ] ; then
	#AC_GBC_NOTICE([%%% have_gs is false (required)])
	#AC_GBC_NOTICE([PATH does not include ghostscript (gs)])
	GS_COPYPAGE_OK=1
	#AC_GBC_NOTICE([no gs so no gs version])
else
	#AC_GBC_NOTICE([%%% have_gs is true (required)])
	#AC_GBC_NOTICE([GS_EXEC is $GS_EXEC])
	gsvers=`$GS_EXEC --version`
	if [test -z "$GS_COPYPAGE_OK" ] ; then
		`echo $gsvers | awk '{if ($1 < 9.10) exit 1; else exit 0;}'`
		GS_COPYPAGE_OK=$?
#   modern autoconf macro:
#   AS_VERSION_COMPARE([$gsvers], [9.10],
#	   [GS_COPYPAGE_OK=1], [GS_COPYPAGE_OK=0], [GS_COPYPAGE_OK=0])
	fi
	#AC_GBC_NOTICE([$GS_EXEC version is $gsvers, cf 9.10])
fi
#AC_GBC_NOTICE([GS_COPYPAGE_OK is $GS_COPYPAGE_OK])
AC_SUBST(GS_COPYPAGE_OK)

################################################################################

# PERL/Python might be in nonstandard locations
AC_PATH_PROGS(PERL, perl, /bin/false)
#AC_PATH_PROGS(PYTHON, python, /bin/false)
AM_PATH_PYTHON([2.4],,[:])
if [test "$PYTHON" != : -a "$pythondir" != ""] ; then
    have_python=true
else
    have_python=false
fi
AM_CONDITIONAL([HAVE_PYTHON], [$have_python])

if [ $have_python = "true" ] ; then
    AC_CONFIG_FILES( [source/python_src/mk4_module/setup.py])
    AC_CONFIG_FILES( [source/python_src/hopstest_module/setup.py])
    AC_CONFIG_FILES( [source/python_src/hopstest_module/hopstestsuite.py])
    AC_CONFIG_FILES( [source/python_src/ffcontrol_module/setup.py])
    AC_CONFIG_FILES( [source/python_src/vpal_module/setup.py])

    AC_DEFINE_UNQUOTED([PYTHON_SITE_PREFIX], ["$pythondir"], [Python install dir])
fi

#AC_DEFINE_UNQUOTED([AC_INSTALL_PREFIX], ["$prefix"], [install prefix])

AC_DEFINE([AC_INSTALL_PREFIX], [I HATE AUTOCONF], [dumb install prefix])

################################################################################

#
# FFTW3 is now required
#
PKG_CHECK_MODULES(FFTW3, fftw3, [hasfftw=true], [hasfftw=false])
if [$hasfftw] ; then
	AC_DEFINE([HAVE_FFTW], 1, [Define to 1 if FFTW is available])
else
	AC_DEFINE([HAVE_FFTW], 0, [Define to 1 if FFTW is available])
fi
AC_SUBST(FFTW3_CFLAGS)
AC_SUBST(FFTW3_LIBS)
AM_CONDITIONAL(HAVE_FFTW, $hasfftw)

################################################################################


AC_ARG_VAR(X_INSANE, [Anything you need to append to the X11 link chain])

# Check for X11 -- this isn't robust against all cases, but
# there are enough environment variables to cover many bases.
have_x11=false
# --enable-xtrap
if [test x${enable_pathx-no}${enable_xtrap-no} = xnoyes ] ; then
	#AC_GBC_NOTICE([using ac_path_xtra method])
	AC_PATH_XTRA
	AC_SUBST(X_CFLAGS)
	AC_SUBST(X_PRE_LIBS)
	AC_SUBST(X_LIBS)
	AC_SUBST(X_IDIOT,'-lX11')
	AC_SUBST(X_EXTRA_LIBS)
	have_x11=true
fi
# --enable-pathx
if [test x${enable_pathx-no}${enable_xtrap-no} = xyesno ] ; then
	#AC_GBC_NOTICE([using ac_path_x method])
	AC_PATH_X
	AC_SUBST(X_CFLAGS,'-I$x_includes')
	AC_SUBST(X_PRE_LIBS)
	AC_SUBST(X_LIBS,'-L$x_libraries')
	AC_SUBST(X_IDIOT,'-lX11')
	AC_SUBST(X_EXTRA_LIBS)
	have_x11=true
fi
# --enable-pathx and --enable-xtrap or neither
if [test x$have_x11 = xfalse ] ; then
	#AC_GBC_NOTICE([using default X11 method])
	AC_CHECK_LIB(X11, XOpenDisplay, have_x11=true, have_x11=false)
	AC_SUBST(X_CFLAGS)
	AC_SUBST(X_PRE_LIBS)
	AC_SUBST(X_LIBS,'-lX11')
	AC_SUBST(X_IDIOT)
	AC_SUBST(X_EXTRA_LIBS)
fi
#AC_GBC_NOTICE([%%% have_x11 is $have_x11 (required)])
AM_CONDITIONAL(HAVE_X11, [test x$have_x11 = xtrue])
AC_SUBST(X11_LIB,
	'$(X_PRE_LIBS) $(X_LIBS) $(X_IDIOT) $(X_EXTRA_LIBS) $(X_INSANE)')

#
# Check for PGPLOT is complicated, and at present, not optional.
# pgxwin_server is needed for X11 installations and AC_PATH_PROG()
# is a convenient way to search among multiple directories.
# It is hard to get the search order right...in all cases.
#
# Further complicated by the DiFX use of the PGPLOTDIR variable.
# If that is defined and PGPLOT_DIR isn't, we'll take that as a hint.
# otherwise we'll fall back on /usr/local/pgplot as Tim intended.
#
if [test -z "$PGPLOT_DIR" -a -n "$PGPLOTDIR" ] ; then
	pgplotdefault="$PGPLOTDIR"
else
	pgplotdefault='/usr/local/pgplot'
fi
#
AC_ARG_VAR(PGPLOT_DIR, [PGPLOT installation directory])
if [test x${enable_pgplot-yes} = xyes ] ; then
	#AC_GBC_NOTICE([searching for pgxwin_server])
	AC_PATH_PROG(PGXWIN_SERVER, pgxwin_server, false,
	[${PGPLOT_DIR-"$pgplotdefault"} /usr/lib/pgplot5 dnl
	 /usr/lib/pgplot64 /usr/local/pgplot-png dnl
	 /usr/bin /opt/local/bin /sw/lib/pgplot])
	pgpdir=`AS_DIRNAME(["${PGXWIN_SERVER-/no-such-dir}"])`
	if [test "$pgpdir" = '/usr/bin'] ; then
	# for Debian packages
	pgpdir='/usr/lib'
	pgplib=$pgpdir/libpgplot.a
	cpglib=$pgpdir/libcpgplot.a
	grfont=${PGPLOT_DIR-'/usr/lib/pgplot5'}/grfont.dat
	#AC_GBC_NOTICE([pgpdir was /usr/bin, revised to $pgpdir])
	elif [test "$pgpdir" = '/opt/local/bin'] ; then
	# for MacPorts
	pgpdir='/opt/local/lib'
	pgplib=$pgpdir/libpgplot.a
	cpglib=$pgpdir/libcpgplot.a
	grfont=/opt/local/share/pgplot/grfont.dat
	#AC_GBC_NOTICE([pgpdir was /opt/local, revised to $pgpdir])
	else
	# well, no pgxwin_server, but PGPLOT may yet be functional
	if [test "$pgpdir" = '/no-such-dir'] ; then
		pgpdir=${PGPLOT_DIR="$pgplotdefault"}
	fi
	pgplib=$pgpdir/libpgplot.a
	cpglib=$pgpdir/libcpgplot.a
	grfont=$pgpdir/grfont.dat
	#AC_GBC_NOTICE([pgpdir is $pgpdir])
	fi
	if [test -d $pgpdir -a -f $pgplib -a -f $cpglib -a $grfont ] ; then
	#AC_GBC_NOTICE([PGPLOT_DIR is $pgpdir])
	have_pgplot=true
	PGPLOT_DIR=$pgpdir
	# PGP_LIB=['$(FLIBS) -L$(PGPLOT_DIR) -lcpgplot -lpgplot']
	PGP_LIB=['-L$(PGPLOT_DIR) -lcpgplot -lpgplot $(FLIBS)']
	else
	#AC_GBC_NOTICE([PGPLOT was too hard to find.])
	#AC_GBC_NOTICE([Try setting PGPLOT_DIR to point to a directory])
	#AC_GBC_NOTICE([that contains symbolic links to the installed])
	#AC_GBC_NOTICE([files libpgplot.a, libcpgplot.a and grfont.dat,])
	#AC_GBC_NOTICE([and also make sure pgxwin_server is on your PATH.])
	have_pgplot=false
	PGPLOT_DIR=''
	PGP_LIB=''
	fi
	# does this PGPLOT compilation require PNG(12)?
	need_png=optional
	if [test -f $pgplib] ; then
	pndriv=`ar t $pgplib pndriv.o`
	if [test x$pndriv = xpndriv.o] ; then
		need_png=required
	fi
	fi
fi
#AC_GBC_NOTICE([%%% have_pgplot is $have_pgplot ($need_png)])
AM_CONDITIONAL(HAVE_PGPLOT, [test x$have_pgplot = xtrue])
AC_SUBST(PGP_LIB)
AC_SUBST(PGPLOT_DIR)

# Check for PNG (primitive) which MAY be required by pgplot
# One could provide the linkage stubs if PNG is required, but missing.
# If built within the DiFX environment, it might be available.
AC_ARG_VAR(PNG12_LIB, [Path to libpng12 shared object])
AC_CHECK_LIB(png12, png_access_version_number, have_png=true, have_png=false)
if [$have_png] ; then
	#AC_GBC_NOTICE([Found png12 normally])
	PNG_LIB='-lpng12'
elif [test $need_png = required]; then
	#AC_GBC_NOTICE([Looking harder for png12 need_png is $need_png])
	if [test -z "$PNG12_LIB" ]; then
	PNG12_LIB=${DIFX_ARCH_ROOT}/lib/libpng12.so.0
	elif [test -z "$PNG12_LIB" ]; then
	PNG12_LIB=/usr/lib/libpng12.so.0
	fi
	if [test -f "$PNG12_LIB" ] ; then
	have_png=true
	PNG_LIB=$PNG12_LIB
	else
	have_png=false
	PNG_LIB=''
	fi
#else
	#AC_GBC_NOTICE([Skipping png12 since need_png is $need_png])
fi
#AC_GBC_NOTICE([%%% have_png is $have_png ($need_png)])
AM_CONDITIONAL(HAVE_PNG, [test x$have_png = xtrue -o x$need_png = xoptional])
AC_SUBST(PNG_LIB)

################################################################################


AC_CONFIG_FILES([
 Makefile
 source/Makefile
 source/c_src/Makefile
 source/c_src/mk4util/Makefile
 source/c_src/dfio/Makefile
 source/c_src/afio/Makefile
 source/c_src/vex/Makefile
 source/c_src/fourfit_libs/Makefile
 source/c_src/fourfit_libs/ffcontrol/Makefile
 source/c_src/fourfit_libs/ffcore/Makefile
 source/c_src/fourfit_libs/ffio/Makefile
 source/c_src/fourfit_libs/ffmath/Makefile
 source/c_src/fourfit_libs/ffsearch/Makefile
 source/c_src/fourfit_libs/ffplot/Makefile
 source/c_src/applications/Makefile
 source/c_src/applications/fourfit/Makefile
 source/c_src/applications/alist/Makefile
 source/c_src/applications/aedit/Makefile
 source/python_src/Makefile
 source/python_src/mk4_module/Makefile
 source/python_src/hopstest_module/Makefile
 source/python_src/ffcontrol_module/Makefile
 source/python_src/vpal_module/Makefile
 data/Makefile
])

AC_CONFIG_FILES(chopsenv.sh,
    [chmod +x hops.bash;
     echo "";
     echo "to set up your environment, now and in the future:";
     echo "  "source ./chopsenv.sh;
     echo "";
    ])


AC_OUTPUT
