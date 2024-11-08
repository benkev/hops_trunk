#!/bin/sh
#
# $Id: hook_import.sh 1992 2017-09-13 20:16:10Z barrettj $
#
# Script to manage the list of borrowed or hacked fearfit sources
#
[ ${testverb-'0'} -gt 0 ] && verb=true || verb=false

# after review, change numbers here
# FEARFIT#1
hackees='make_plotdata.c norm_fx.c output.c search.c freq_spacing.c vrot.c'
pdlc=430
nmlc=415
oplc=145
shlc=289
fslc=94
ttlc=1431
vrlc=58
expect="$pdlc make_plotdata.c $nmlc norm_fx.c $oplc output.c"
expect="$expect $shlc search.c $fslc freq_spacing.c $vrlc vrot.c"
expect="$expect $ttlc total"
[ ${testverb-'0'} -gt 0 -a $# -ge 1 ] && (
    cd $1 && pwd && wc -l $hackees
    echo "$expect"
)

# with no arguments is a quick check that sources have probably not changed
[ $# -eq 0 ] && {
    [ -n "$ffdir" ] || { echo no ffdir ; exit 1 ; }
    set -- `cd $ffdir ; wc -l $hackees`
    $verb && echo have "$@" && echo test "$expect"
    while [ $# -ge 2 ]
    do # FEARFIT#2
        case $2 in
        make_plotdata.c)    lines=$pdlc  ;;
        norm_fx.c)          lines=$nmlc  ;;
        output.c)           lines=$oplc  ;;
        search.c)           lines=$shlc  ;;
        freq_spacing.c)     lines=$fslc  ;;
        vrot.c)             lines=$vrlc  ;;
        total)              lines=$ttlc  ;;
        *)                  { echo wtf:$2; exit 2 ; } ;;
        esac
        [ $1 -eq $lines ] || { echo $2 has changed to $1 -ne $lines; exit 3; }
        shift 2
    done
    exit 0
}

ffdir=$1
shift

# 2nd argument, if present, is the name of source file to doctor
[ $# -ge 1 ] && mksrc=true || mksrc=false
for s
do
    case $s in
    make_plotdata.c) seds='/^....return(0);/i\
#ifdef PLOTDATA_HOOK\
#warning "plotdata hacking enabled -- see hook_make_plotdata.c"\
    PLOTDATA_HOOK;\
#endif /* PLOTDATA_HOOK */
'
        ;;
    # note: this has been broken since FFT1 disappeared
    # needs to be reworked....
    norm.c) seds='/^........S.i..=.s_mult..S.i.,.factor.;/a\
#ifdef NORM_HACK_HOOK\
#warning "spectral hacking enabled -- see hook_norm.c"\
    NORM_HACK_HOOK;\
#endif /* NORM_HACK_HOOK */
        '
        ;;
    output.c) seds='/....if..make_postplot/i\
#ifdef OUTPUT_HOOK\
#warning "output hacking enabled -- see hook_output.c"\
    OUTPUT_HOOK;\
#endif /* OUTPUT_HOOK */
        '
        ;;
    search.c) seds='/.define MBD_GRID_MAX 8192/d
/fftplan =/a\
#ifdef SEARCH_HOOK\
#warning "search hacking enabled -- see hook_search.c"\
    SEARCH_HOOK;\
#endif /* SEARCH_HOOK  */
        '
        ;;
    freq_spacing.c)
        seds='/0.001/s/0.001/fs_tol/
/2047/s/2047/fs_mgpb4m1/
/256/s/256/fs_divmax/
/i.<.8/s/i.<.8/i < fs_power2/
/Freq.*dims!!/c\
    msg("Frequency spacing is a challenge, trying harder....", 2);\
#ifdef FREQ_SPACING_HOOK\
#warning "freq_spacing hacking enabled -- see hook_freq_spacing.c"\
    FREQ_SPACING_HOOK;\
#endif /* FREQ_SPACING_HOOK */
        '
        ;;
    vrot.c)
        seds='/complex.vrot/s/vrot/orig_vrot/'
        ;;
    *) # FEARFIT#3
        ;;
    esac
    sed "$seds" < $ffdir/$s > $s
done
$mksrc && exit 0

mkinc=fearfit.mk
ffmak=$ffdir/Makefile.am

#
# construct the list of fourfit sources
#
set -- `sed -e '/^fourfit_sources/,/^fourfit_SOURCES/!d' \
           -e '/^fourfit_sources/d' \
           -e '/^fourfit_SOURCES/d' \
            $ffmak`

#
# header
#
rm -f fearfit.mk
cat > $mkinc <<-EOF
	#
	# fourfit sources borrowed from $1
	#
	fourfit_sources = \\
EOF

tab='	'
bs=''
while [ $# -gt 0 ]
do
    b=''
    case $1 in
    # hooked versions
    # FEARFIT#4
    make_plotdata.c) b=$1; echo "$tab"'$(srcdir)/hook_'$1' \' >> $mkinc ;;
    norm.c)          b=$1; echo "$tab"'$(srcdir)/hook_'$1' \' >> $mkinc ;;
    output.c)        b=$1; echo "$tab"'$(srcdir)/hook_'$1' \' >> $mkinc ;;
    search.c)        b=$1; echo "$tab"'$(srcdir)/hook_'$1' \' >> $mkinc ;;
    freq_spacing.c)  b=$1; echo "$tab"'$(srcdir)/hook_'$1' \' >> $mkinc ;;
    vrot.c)          b=$1; echo "$tab"'$(srcdir)/hook_'$1' \' >> $mkinc ;;
#   read_sdata.c)    b=$1; echo "$tab"'$(srcdir)/hook_'$1' \' >> $mkinc ;;

    # goes to the end of the list, see below
    fourfit.c)          ;;

    # unmodified sources
    *.c)                echo "$tab"'$(fourfitdir)/'$1' \' >> $mkinc ;;

    # other junk (\)
    *)                  ;;
    esac
    shift
    [ -n "$b" ] && bs="$bs $b"
done

echo "$tab"'$(fourfitdir)/fourfit.c' >> $mkinc
echo '' >> $mkinc
echo "BUILT_SOURCES = $bs" >> $mkinc

# built source rules
for b in $bs
do
    echo "$b: "'$(fourfitdir)/'$b >> $mkinc
    echo "$tab"'$(srcdir)/hook_import.sh $(srcdir)/../fourfit' $b >> $mkinc
    echo '' >> $mkinc
done

cat >> $mkinc <<-EOF
	#
	# eof
	#
EOF

#
# eof
#
