#!/bin/sh
#
Id="mk4-sync.sh" Xx=""
VERSION="$Id$Xx"
#
# Script to synchronize /correlator/dist and the HOPS branch
#
ME=`basename $0 | cut -d. -f1`
MU=./$ME.sh

#
# list of legal places to run in
#
SVNURL="URL: https://vault.haystack.mit.edu/svn/hops/trunk/mk4-migrate"
hostone="gefera"
hosttwo=""
hosttre=""
legal="$HOME/HOPS/trunk/mk4-migrate"
corr="/correlator"

regions='help incl hops scrp subs corr data text docs'

hops_time=`type -p hops_time` || hops_time=no-such-program

# usage message
USAGE="Usage: $ME.sh Action [regions [specific-files ...]]

where Action is one of these

    import		import mk4 files to svn area
    import-n	same as import, but just say, rather than do

    export		export svn files to mk4 area
    export-n	same as export, but just say, rather than do

    compare		find and describe possible conflicts
    check		terser version of compare
    check-mk4	same thing, but assume mk4 is correct
    check-svn	same thing, but assume svn is correct
    diff		same as compare, but diff them

    status		just show the local svn status
    svndiff		just run a local svn diff
    svnadd		runs an svn add on ? items
    svnrm		runs svn rm on peerless items
    svnrm-n		echoes svn rm on peerless items

and regions is one or more of these (\$corr = $corr):

	    correlator location              hops location
    help    \$corr/dist/mk4/doc/help          ../help
    incl    \$corr/dist/mk4/src/include       ../include
    hops    \$corr/dist/mk4/src/postproc      ../postproc
    scrp    \$corr/dist/mk4/src/scripts/share ../scripts
    subs    \$corr/dist/mk4/src/sub           ../sub
    corr    \$corr/dist/mk4/src/correlator    ../correlator
    data    \$corr/data                       ../data
    text    \$corr/prog/text                  ../correlator/prog/text
    docs    \$corr/prog/doc                   ../correlator/prog/doc

if no regions are specified; these are assumed:

    $regions
	
If specific files are mentioned after the region, then only those files
are processed for the last region mentioned.  (I.e. all files in multiple
regions, or multiple files in only one region.)

This script has no protection to prevent it; but you probably should
not import/export unless you've checked that the local SVN status is
consistent/up-to-date with the repository.

This script is hardwired (at present) to work only with:

    $SVNURL
  Hosts: $hostone $hosttwo $hosttre
   Dirs: $legal

A -v or a -vv may be inserted anywhere on the command line before the last
region to generate limited or extreme commentary during processing.
"

# first go at -v or -vv
verb=false
veryverb=false
[ "$#" -gt 0 ] && [ "$1" = "-v" ] && verb=true && shift
[ "$#" -gt 0 ] && [ "$1" = "-vv" ] && veryverb=true && verb=true && shift

mk4ok=false
svnok=false
case ${1-"--help"} in
*help*)         echo "$USAGE"        ; exit 0   ;;
--version)      echo "$VERSION"      ; exit 0   ;;
import*)                                        ;;
export*)                                        ;;
compare)                                        ;;
check-mk4)      mk4ok=true                      ;;
check-svn)      svnok=true                      ;;
check)                                          ;;
diff)                                           ;;
status)                                         ;;
svndiff)                                        ;;
svnadd)                                         ;;
svnrm)          svnrm="svn rm"                  ;;
svnrm-n)        svnrm="echo svn rm"             ;;
*)              echo not implemented ; exit 4   ;;
esac
action=$1 ; shift
$mk4ok && action=check
$svnok && action=check

# check the host
host=`hostname`
[ "$host" = "$hostone" -o "$host" = "$hosttwo" -o "$host" = "$hosttre" ] || {
	echo You can only run this on $hostone, $hosttwo, or $hosttre
	exit 1
}
# check the current working directory
ok=false
cwd=`pwd`
for d in $legal
do
	[ "$cwd" = "$d" ] && ok=true
done
$ok || {
	echo You need to be in one of these directories:
	echo "$legal"
	exit 1
}
# check the repository
url=`svn info . | grep URL`
[ "$url" = "$SVNURL" ] || {
	echo this script is meant to be run at 1>&2
	echo the top level of the SVN directory: 1>&2
	echo '' $SVNURL 1>&2
	exit 2
}

# another go at -v -vv
[ "$#" -gt 0 ] && [ "$1" = "-v" ] && verb=true && shift
[ "$#" -gt 0 ] && [ "$1" = "-vv" ] && veryverb=true && verb=true && shift

#
# default is to do everything
#
[ "$#" -eq 0 ] && set -- $regions
$verb && echo "## using regions $@"

#
# Build up region list
#
# $svn_dir <-> $mk4_dir for each of $dir_kid (and subdirs)
# svn_* is the svn name for the directory piece
# mk4_* is the mk4 name for the directory piece
# *_kid is a list of files or directories to consider
# *_sed is a list of file matches to ignore
#

region="" last_region="" kiddys=""
while [ $# -gt 0 ]
do
    case $1 in
    -vv) verb=true ; shift ; continue;;
    -v)  verb=true veryverb=true ; shift ; continue;;

    help)
	svn_help=../help
	mk4_help=$corr/dist/mk4/doc/help
	help_kid="."
	help_sed="Makefile fixquotes.o formatter.o"
	last_region=$1
	;;
    incl)
	svn_incl=../include
	mk4_incl=$corr/dist/mk4/src/include
	incl_kid="."
	incl_sed="Makefile"
	last_region=$1
	;;
    hops)
	svn_hops=../postproc
	mk4_hops=$corr/dist/mk4/src/postproc
	hops_kid="."
	hops_sed="Makefile make.log ~"
	last_region=$1
	;;
    scrp)
	svn_scrp=../scripts
	mk4_scrp=$corr/dist/mk4/src/scripts/share
	scrp_kid="."
	scrp_sed="Makefile vhelp"
	last_region=$1
	;;
    subs)
	svn_subs=../sub
	mk4_subs=$corr/dist/mk4/src/sub
	subs_kid="."
	subs_sed="Makefile bytflp"
	last_region=$1
	;;
    corr)
	svn_corr=../correlator
	mk4_corr=$corr/dist/mk4/src/correlator
	corr_kid="CorAsc2"
	corr_sed="Makefile"
	last_region=$1
	;;
    data)
	svn_data=../data
	mk4_data=$corr/data
	data_kid="ff_testdata"
	data_sed="Makefile _testdata.chk_ testfourfit testlinux ~"
	last_region=$1
	;;
    text)
	svn_text=../correlator/prog/text
	mk4_text=$corr/prog/text
	text_kid="."
	text_sed="Makefile MesAlias TaDo.txt TaDo.Txt.hay"
	last_region=$1
	;;
    docs)
	svn_docs=../correlator/prog/doc
	mk4_docs=$corr/prog/doc
	docs_kid="."
	docs_sed="Makefile fixquotes.o formatter.o"
	docs_sed="$docs_sed processing.modes.xls processing.streams.doc"
	docs_sed="$docs_sed backup.of.cvex10.wb backup.of.ivex10.wb"
	docs_sed="$docs_sed mark.5"
	last_region=$1
	;;
    *)
	kiddys="$@"
	break
	;;
    esac
    shift
    region="$region $last_region"
done
echo ''

#
# this overrides the _kid definitions
#
[ -n "$kiddys" ] && {
	region=$last_region
	[ -n "$region" ] || { echo "$USAGE" ; exit 5; }
	eval ${last_region}_kid=\""$kiddys"\"
}

#
# Tackle the regions in turn, building up a list of problematic files.
#
div='#======================================================================='
act='#+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++' 
vid='#-----------------------------------------------------------------------' 
pairs=""
for r in $region
do
    eval svn_dir=\$svn_${r}
    eval mk4_dir=\$mk4_${r}
    eval dir_kid=\$${r}_kid
    eval sed_kid=\$${r}_sed
    seds=''
    for sss in $sed_kid ; do seds="$seds -e /$sss/d" ; done
    $verb && echo "## seds: $seds"
    echo \#\# Mapping $svn_dir '<==>' $mk4_dir with files/subdirs:

    [ $action = "status" ] && {
	echo \# SVN status:;  svn status $svn_dir; continue ;
    }
    [ $action = "svndiff" ] && {
	echo \# SVN svndiff:; svn diff $svn_dir; continue ;
    }
    [ $action = "svnadd" ] && {
	newbies=`svn status $svn_dir | grep '^?' | cut -c2-`
	[ -n "$newbies" ] &&
	echo \# SVN svnadd: && svn add $newbies; continue ;
    }

    for d in $dir_kid
    do
	$verb && echo \#=== svn files ==== $d
	[ -f $svn_dir/$d ] &&
	    svnl=`echo $svn_dir/$d |\
	      sed -e s@${svn_dir}@@ $seds`
	[ -d $svn_dir/$d ] &&
	    svnl=`find $svn_dir/$d -name .svn -prune -o -type f -print |\
	      sed -e s@${svn_dir}@@ $seds`
	$verb && echo \#=== mk4 files ==== $d
	[ -f $mk4_dir/$d ] &&
	    mk4l=`echo $mk4_dir/$d |\
	      sed -e s@${mk4_dir}@@ $seds`
	[ -d $svn_dir/$d ] &&
	    mk4l=`find $mk4_dir/$d -type f -print |\
	      sed -e s@${mk4_dir}@@ $seds`
	cand=`( echo "$svnl" ; echo "$mk4l" ) | sort | uniq`
	for c in $cand
	do
	    $veryverb && {
		    echo $vid
		    echo "# comparing candidate $c"
		    echo \# $svn_dir/$c
		    echo \# $mk4_dir/$c
	    }
	    [ -f "$svn_dir/$c" -a ! -r "$svn_dir/$c" ] && {
	    	echo \# $svn_dir/$c unreadable ; continue; }
	    [ -f "$mk4_dir/$c" -a ! -r "$mk4_dir/$c" ] && {
	    	echo \# $mk4_dir/$c unreadable ; continue; }
	    [ -f "$svn_dir/$c" -a -f "$mk4_dir/$c" ] &&
	        cmp "$svn_dir/$c" "$mk4_dir/$c" >/dev/null ||
	        pairs="$pairs $svn_dir/$c $mk4_dir/$c"
	done
    done
    # echo ''
done

#
# at this point we should have svn-mk4 pairs which differ in some way.
#
svn=""
for mk4 in $pairs
do
    [ -z "$svn" ] && { svn=$mk4 ; continue ; }
    # have a pair
    case $action in
    check|svnrm*)
	;;
    import*)
	;;
    export*)
	;;
    *)
	[ -f $svn ] && ls -l $svn || echo $svn does not exist
	[ -f $mk4 ] && ls -l $mk4 || echo $mk4 does not exist
	;;
    esac

    $verb && { echo \# SVN: $svn ; echo \# MK4: $mk4 ; }

    case $action in
    diff)
	echo "$act"
	diff -u $svn $mk4
	;;
    check)
	case $svn in
	../help/*)        r=help    kyd=../help//                 ;;
	../include/*)     r=incl    kyd=../include//              ;;
	../postproc/*)    r=hops    kyd=../postproc//             ;;
	../scripts/*)     r=scrp    kyd=../scripts//              ;;
	../sub/*)         r=subs    kyd=../sub//                  ;;
	../correlator/prog/text/*)
	                  r=text    kyd=../correlator/prog/text// ;;
	../correlator/prog/doc/*)
	                  r=docs    kyd=../correlator/prog/doc//  ;;
	../correlator/*)  r=corr    kyd=../correlator//           ;;
	../data/*)        r=data    kyd=../data//                 ;;
	*)                r=what    kyd=WTF                       ;;
	esac
	loc=`expr $svn : $kyd'\(.*\)'`
	[ -f $svn -a ! -f $mk4 ] && {
	    { $svnok && {
		echo $MU export $r $loc
		$verb && ls -l $svn 2>&1 | sed 's/^/# /' || true; } } ||
	    { $mk4ok && {
		echo $MU svnrm' ' $r $loc
		$verb && ls -l $svn 2>&1 | sed 's/^/# /' || true; } } ||
	    {
		echo '# THIS:' $MU svnrm' ' $r $loc
		echo '#   OR:' $MU export $r $loc
		ls -l $svn 2>&1 | sed 's/^/# /'
	    }
	}
	[ -f $mk4 -a ! -f $svn ] && {
	    { $mk4ok && {
		echo $MU import $r $loc
		$verb && ls -l $mk4 2>&1 | sed 's/^/# /' || true; } } ||
	    { $svnok && {
		echo $MU rm -f $mk4
		$verb && ls -l $mk4 2>&1 | sed 's/^/# /' || true; } } ||
	    {
		echo '# THIS:' $MU import $r $loc
		echo '#   OR:' '             ' rm -f $mk4
		ls -l $mk4 2>&1 | sed 's/^/# /'
	    }
	}
	# if we have two (different) versions, favor the most recent
	[ -x $hops_time -a -f $svn -a -f $mk4 ] && {
	    svn_k=`$hops_time -q K -f $svn`
	    mk4_k=`$hops_time -q K -f $mk4`
	    [ ${svn_k-"0"} -lt ${mk4_k-"0"} ] &&
		echo $MU import $r $loc &&
		$verb && ls -l $mk4 $svn 2>&1 | sed 's/^/# /'
	    [ ${svn_k-"0"} -gt ${mk4_k-"0"} ] &&
		echo $MU export $r $loc &&
		$verb && ls -l $mk4 $svn 2>&1 | sed 's/^/# /'
	    true
	} || {
		echo "ls -l $svn"
		echo "ls -l $mk4"
	}
	echo "$vid"
	;;
    svnrm*)
	[ -f $svn -a ! -f $mk4 ] && eval $svnrm $svn
	;;
    compare)
	;;
    import-n)
	sdir=`dirname $svn`
	[ -d "$sdir" ] || echo mkdir -p $sdir
	echo cp -p $mk4 $svn
	echo not copied
	;;
    import)
	sdir=`dirname $svn`
	[ -d "$sdir" ] || mkdir -p $sdir
	cp -p $mk4 $svn
	;;
    export-n)
	sdir=`dirname $mk4`
	[ -d "$sdir" ] || echo mkdir -p $sdir
	echo cp $svn $mk4
	echo not copied
	;;
    export)
	sdir=`dirname $mk4`
	[ -d "$sdir" ] || mkdir -p $sdir
	echo cp $svn $mk4
	cp $svn $mk4
	;;
    *)
	echo "$USAGE"
	exit 6
	;;
    esac
    
    case $action in
    check|svnrm*)
	;;
    *)
	echo $vid
	;;
    esac

    # start next pair
    svn="" mk4=""
done

echo ''

exit 0
 
#
# end of file.
#
