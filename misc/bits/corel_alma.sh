#!/bin/bash
#
# script to try some form of zero baseline test using captured files
#

# things we use from the environment
[ -z "$CALC_SERVER" ] && export CALC_SERVER=smm-pc
[ -z "$DIFX_MESSAGE_GROUP" ] && export DIFX_MESSAGE_GROUP=224.2.2.1
[ -z "$DIFX_MESSAGE_PORT" ] && export DIFX_MESSAGE_PORT=50201

#
# be helpful
#
USAGE="$0 experiment [job-number]

    where a job number is appended to the first argument to make
    a directory for the DiFX test data and products.  If the
    job-number is not supplied it counts forward to an unused
    number starting with 3000.  Canonically:

	$0 tdir [3000]

    If set, environment variables override these defaults:

	CALC_SERVER=$CALC_SERVER
	DIFX_MESSAGE_GROUP=$DIFX_MESSAGE_GROUP
	DIFX_MESSAGE_PORT=$DIFX_MESSAGE_PORT

    The fx manager is on the launch host.  An environment variable
    DIFX_GRINDERS should contain a list of additional processing
    (core) resources, and the number of MPI processes which gets
    launched is 1 + ND + NC.  Each core gets DIFX_NTHREAD threads.

    Other environment variables (LOFREQ; DBFAC, NINT, SUBINT)
    allow tweaking of the input vex2difx has generated.  The parameters
    that differ with each data capture can be in \$DIFX_PARAMS (which
    is sourced it is set and refers to an existing file).

    If \$DIFX_DOIT is unset or 'true', the calculation proceeds after
    2 seconds.  Set it to 'false' to just create the job files.
"
[ ${1-'--help'} = --help ] && { echo "$USAGE" ; exit 0 ; }

#
# specifics for this dataset
#
exper=${1-'tdir'}
num=${2-'3000'}
jobn=$exper
tdir=./$exper
hosts=`hostname`
doit=${DIFX_DOIT-'true'}

[ -z "$DIFX_GRINDERS" ] && {
    echo Some DIFX_GRINDERS are needed
    exit 1
}

# fourfit options before the scan name
ff_opts_x="$FF_OPTS_X"' -c'
# set ... on one of these
# dc_block true|false
# optimize_closure true|false
# fourfit options after the root file
#ff_post_x='set optimize_closure true'
ff_post_x=''

args="$@"
shift $#

# pick up station, mjdStart, mjdStop, scanStart, scanfile
[ -n "$DIFX_PARAMS" -a -f $DIFX_PARAMS ] && . $DIFX_PARAMS &&
    echo sourced $DIFX_PARAMS ||
    echo did not source $DIFX_PARAMS
[ -n "$comment" ] && echo "$comment"
[ -z "$scanStart" ] && { echo scanStart undefined ; exit 2; }

#cepoch='2011y118d00h00m'
cepoch=`expr "$scanStart" : '\(....y...d\).*'`00h00m

#
# make a place to work
#
while [ -d $tdir-$num ] ; do num=`expr $num + 1` ; done
td=$tdir-$num
mkdir $td && cd $td && echo In $td || { echo Cannot make $td ; exit 2; }
wdir=`pwd`
job=${jobn}.$num
sob=${jobn}_$num

# construct scan name and target
dd=`expr $scanStart : '....y\(...\)d..h..m.*'`
hh=`expr $scanStart : '....y...d\(..\)h..m.*'`
mm=`expr $scanStart : '....y...d..h\(..\)m.*'`
[ -n "$scanName" ] || scanName=$dd-$hh$mm
scan=$scanName
hopsrc=`echo $target | tr . _`

etime=${ETIME-'4'}

# sed script for substitutions in *.input if needed
jinseds=''
#[ "$etime" -ne 4 ] && jinseds="
#/^EXECUTE TIME (SEC)/a\
#EXECUTE TIME (SEC): $etime
#
#/^EXECUTE TIME (SEC)/s/^/@/
#/^XMAC STRIDE LENGTH/a\
#XMAC STRIDE LENGTH: 32
#
#/^XMAC STRIDE LENGTH/s/^/@/
#"

#
# st_info is an environment variable with all we need to generate this
# fake vex file for the observation.  The 5 fields per file are (loosely)
#   2-letter antenna name
#   data file host name
#   data file path name
#   file format (e.g. VDIF/8224/2)
#   das name (unused)
#
# generate list of antennas for v2d
antennas=''
formats=''
st_shift=5
set -- $st_info
while [ $# -ge $st_shift ]
do
    st=$1 dm=$2 name=$3 fmt=$4 das=$5 file=''
    [ -n "$antennas" ] && antennas="$antennas,$st"  || antennas="$st"
    shift $st_shift
    fmt=`echo $fmt | tr / _`
    formats="$formats
    $fmt"
done
formats=`echo "$formats" | sort | uniq`

# create input v2d
echo '' ; echo '%%% manufacturing vex input' ; echo ''
cat > $job.v2d <<-EOF
	vex = $exper.vex.obs
	mjdStart = $mjdStart
	mjdStop  = $mjdStop
	antennas = $antennas
	startSeries = $num
	dataBufferFactor = ${DBFAC-16}
	visBufferLength = ${VBLEN-80}
	nDataSegments = ${NDSEG-8}

	SETUP default
	{
	    tInt = ${NINT-0.48}
	    subintNS = ${SUBINT-240000000}
	    FFTSpecRes = ${FFTRES-0.25}
	    specRes = ${SPECRES-0.50}
	}
EOF
rm -f $sob.filesed
nstn=0 set -- $st_info
while [ $# -ge $st_shift ]
do
    [ -z "$nstn" ] && nstn=0
    st=$1 dm=$2 name=$3 fmt=$4 das=$5 file=''
    file=`ssh $dm ls $name 2>/dev/null`
    [ -n "$file" ] && echo found ${dm}:$file || {
	echo "$dm":$name is missing
	exit 1
    }
    eval st_zoom=\$${st}_zoom
    [ -z "$st_zoom" ] && echo ${st}_zoom is needed
    cat >> $job.v2d <<-....EOF

	ANTENNA $st
	{
	  # dm = $dm
	  file = $file
	  format = $fmt
	  phaseCalInt = ${PHASECALINT-0}
	  $st_zoom
....EOF
    cat >> $job.v2d <<-....EOF
	}
....EOF
    echo "s+$file+$dm+" >> $sob.filesed
    hosts="$hosts DATAFILE$nstn"
    nstn=`expr $nstn + 1`
    file=''
    shift $st_shift
done
#
[ -n "$v2dcomment" ] && echo "$v2dcomment" >> $job.v2d
[ -n "$formats" ] && echo "$formats" | sed 's/^/# /' >> $job.v2d

#
# Manufacture a suitable zero-baseline test vex file.
#

rm -f $exper.vex.obs
cat >> $exper.vex.obs <<EOF
VEX_rev = 1.5;
\$GLOBAL;
    ref \$EXPER = AVzoom;
    ref \$EOP = EOP001;
*-----------------------   end \$GLOBAL             ----------------------*
\$EXPER;
  def AVzoom;
    exper_name = $exper;
    exper_num = $num;
    exper_description = AVzoom;
    PI_name = GBC;
    target_correlator = DiFX;
  enddef;
*-----------------------   end \$EXPER              ----------------------*
*----------------------- begin \$MODE               ----------------------*
\$MODE;
  def MINE;
EOF
set -- $st_info
while [ $# -ge $st_shift ]
do
    st=$1 dm=$2 name=$3 fmt=$4 das=$5 file=''
    fmt=`echo $fmt | tr / _`
    cat >> $exper.vex.obs <<....EOF
    * station $st
    ref \$FREQ = $fmt-FREQ:$st;
    ref \$BBC = $fmt-BBC:$st;
    ref \$IF = $fmt-IF:$st;
....EOF
    shift $st_shift
done
cat >> $exper.vex.obs <<EOF
  enddef;
*-----------------------   end \$MODE               ----------------------*
*----------------------- begin \$EOP                ----------------------*
\$EOP;
 def EOP001;
  TAI-UTC = 34 sec;
  A1-TAI = 0.0 sec;
  eop_ref_epoch = $cepoch;
  * min of 5 are required, apparently
  num_eop_points = 5;
  eop_interval = 24 hr;
  ut1-utc  = 0.0 sec  : 0.0 sec  : 0.0 sec  : 0.0 sec  : 0.0 sec  ;
  x_wobble = 0.0 asec : 0.0 asec : 0.0 asec : 0.0 asec : 0.0 asec ;
  y_wobble = 0.0 asec : 0.0 asec : 0.0 asec : 0.0 asec : 0.0 asec ;
 enddef;
*-----------------------   end \$EOP                ----------------------*
*----------------------- begin \$CLOCK              ----------------------*
\$CLOCK;
EOF
set -- $st_info
while [ $# -ge $st_shift ]
do
  st=$1 dm=$2 name=$3 fmt=$4 das=$5 file=''
  eval cearly=\$${1}_early
  [ -z $cearly ] && cearly=0
  cat >> $exper.vex.obs <<..EOF
  def $st; clock_early = $cepoch : $cearly usec: $cepoch : 0; enddef;
..EOF
  shift $st_shift
done
cat >> $exper.vex.obs <<EOF
*-----------------------   end \$CLOCK             ----------------------*
*----------------------- begin \$STATION            ----------------------*
\$STATION;
EOF
set -- $st_info
while [ $# -ge $st_shift ]
do
  st=$1 dm=$2 name=$3 fmt=$4 das=$5 file=''
  cat >> $exper.vex.obs <<..EOF
  def $st;
    ref \$SITE = $st;
    ref \$ANTENNA = FakeAnt;
    ref \$CLOCK = $st;
    ref \$DAS = FakeDAS;
    ref \$TRACKS = FakeTRACKS;
  enddef;
..EOF
  shift $st_shift
done
cat >> $exper.vex.obs <<EOF
*-----------------------   end \$STATION            ----------------------*
*----------------------- begin \$ANTENNA            ----------------------*
\$ANTENNA;
  def FakeAnt;
    antenna_diam =  10.00 m;
    axis_type = az : el;
    axis_offset = 0 m;
    pointing_sector = &n : az : 0 deg : 360 deg : el : 0 deg : 90 deg;
  enddef;
*-----------------------   end \$ANTENNA            ----------------------*
*----------------------- begin \$DAS                ----------------------*
\$DAS;
  def FakeDAS;
*   record_transport_type = VDIF;
  enddef;
*-----------------------   end \$DAS                ----------------------*
*----------------------- begin \$TRACKS             ----------------------*
\$TRACKS;
  def FakeTRACKS;
*   bits_sample = 2;
  enddef;
*-----------------------   end \$TRACKS             ----------------------*
EOF
cat >> $exper.vex.obs <<EOF
*----------------------- begin \$BBC                ----------------------*
\$BBC;
EOF
for fmt in $formats
do
    eval assign=\$${fmt}_bbc_assign
    [ -z "$assign" ] && echo ${fmt}_bbc_assign is needed
    cat >> $exper.vex.obs <<....EOF
    def $fmt-BBC;
      $assign
    enddef;
....EOF
done
cat >> $exper.vex.obs <<EOF
*-----------------------   end \$BBC                ----------------------*
*----------------------- begin \$FREQ               ----------------------*
\$FREQ;
EOF
for fmt in $formats
do
    eval assign=\$${fmt}_freq_assign
    [ -z "$assign" ] && echo ${fmt}_freq_assign is needed
    cat >> $exper.vex.obs <<....EOF
    def $fmt-FREQ;
      $assign
    enddef;
....EOF
done
cat >> $exper.vex.obs <<EOF
*-----------------------   end \$FREQ               ----------------------*
*----------------------- begin \$IF                 ----------------------*
\$IF;
EOF
for fmt in $formats
do
    eval assign=\$${fmt}_if_assign
    [ -z "$assign" ] && echo ${fmt}_if_assign is needed
    cat >> $exper.vex.obs <<....EOF
    def $fmt-IF;
      $assign
    enddef;
....EOF
done
cat >> $exper.vex.obs <<EOF
*-----------------------   end \$IF                 ----------------------*
*-----------------------   begin \$SCHED            ----------------------*
\$SCHED;
* 2010y001d20h58m21.3503s 2010y001d20h58m25.3501s
* i have no idea what the format of station is...
  scan $scanName;
    start = $scanStart;
    mode = MINE;
    * hopefully one of these is visible
    source = $target;
EOF
[ "$etime" -lt 20 ] && et=20 || et=$etime
set -- $st_info
while [ $# -ge $st_shift ]
do
  st=$1 dm=$2 name=$3 fmt=$4 das=$5 file=''
  cat >> $exper.vex.obs <<..EOF
    * real time or 20, whichever is greater, no idea why.
    station = $st : 0 sec : $et sec : 0 ft : 1A : &n : 1;
..EOF
  shift $st_shift
done
cat >> $exper.vex.obs <<EOF
  endscan;
*-----------------------   end \$SCHED              ----------------------*
*----------------------- begin \$SITES              ----------------------*
\$SITE;
* Westford antenna location
EOF
set -- $st_info
while [ $# -ge $st_shift ]
do
  st=$1 dm=$2 name=$3 fmt=$4 das=$5 file=''
  sp='1492206.600 m : -4458130.507 m :  4296015.532 m'
  cat >> $exper.vex.obs <<..EOF
  def $st;
    site_type = fixed;
    site_name = $st;
    site_ID = $st;
    site_position =  $sp;
  enddef;
..EOF
  shift $st_shift
done
cat >> $exper.vex.obs <<EOF
*-----------------------   end \$SITES              ----------------------*
*----------------------- begin \$SOURCE             ----------------------*
\$SOURCE;
* totally bogus source always visible at Westford
  def $target;
    source_type = star;
    source_name = $target;
    IAU_name = 0000+000;
    dec = 81d00'00.0000";
    ra = 03h00m00.00000s;
    ref_coord_frame = J2000;
  enddef;
*-----------------------   end \$SOURCE             ----------------------*
EOF

#
# more or less boilerplate from here on down
#

echo '' ; echo '%%% creating inputs' ; echo ''
echo \
vex2difx $force $job.v2d
vex2difx $force $job.v2d

[ -f $sob.input ] || {
    echo vex2difx did not like your v2d/vex file combination
    echo $wdir/$exper.vex.obs
    echo $wdir/$job.v2d
    exit 1
}
[ -n "$jinseds" ] && {
    mv $sob.input $sob.input.orig
    sed "$jinseds" < $sob.input.orig > $sob.input
}

echo \
calcif2 $sob.input
calcif2 $sob.input

echo ''
echo creating $sob.threads, $sob.machines, et cetera...

# same threads per core in this model
set -- $DIFX_GRINDERS
nc=$#
echo "NUMBER OF CORES:    $nc" > $sob.threads
nt=${DIFX_NTHREAD-8} core=0
while [ $core -lt $nc ]
do
    echo $nt >> $sob.threads
    core=`expr $core + 1`
done

# make sure we have more than enough
set -- $hosts $DIFX_GRINDERS
echo "$@"
nmach=$#
machines="$@"
# need to line up datastreams
# we have one file/file, so ^FILE */0: filename$
rm -f $sob.machines
mach=0
while [ $mach -lt $nmach ]
do
    case $1 in
    DATAFILE*)
	n=`expr $1 : 'DATAFILE\(.*\)'`
	f=`grep '^FILE.'$n/0: $sob.input | cut -d: -f2`
	m=`echo $f | sed -f $sob.filesed`
	>> $sob.machines
	;;
    *)
	m=$1
	;;
    esac
    echo $m >> $sob.machines
    shift
    mach=`expr $mach + 1`
done
echo ''

#
# final check the input input file
#
echo \
checkmpifxcorr $sob.input
checkmpifxcorr $sob.input
inputstatus=$?
[ "$inputstatus" -eq 0 ] || {
    echo checkmpifxcorr was unhappy "(status $inputstatus)"
    exit 2
}

#
# get a copy of the remote configuration script in case hacks are needed
#
cp -p /swc/scripts/sample.mpi.sh $sob.mpi.sh

#
# make the run script
#
sed "/DIFX_JOB_/s/=.*/=$sob/" /swc/scripts/sample.run.sh > $sob.run.sh
chmod +x $sob.run.sh

# manufacture useful one-letter station codes
suffix=stncodes
rm -f $sob.$suffix
set -- $st_info
while [ $# -ge $st_shift ]
do
    st=$1 dm=$2 name=$3 fmt=$4 das=$5 file=''
    s=`echo $st | cut -c1`
    echo $st $s >> $sob.$suffix
    shift $st_shift
done

#
# make a post-processing script
#
cat > $sob.ff.sh <<EOF
#!/bin/bash
# script to postprocess with fourfit
[ -d $sob.difx ] || { echo no output dir; exit 1; }
ff=\`type -p fearfit\`
[ -x "\$ff" ] || ff=\`type -p fourfit\`
[ -z "\$ff" ] && {
    echo no fourfit to run
    exit 1
} || {
    echo Postprocessing to $num/$scan with
    echo \$ff
    rm -f $jobn.v2d
    ln -s $job.v2d $jobn.v2d
    HOPS_STATION_CODE=$sob.$suffix difx2mark4 -e $num $sob
    root=\`echo $num/$scan/$hopsrc.* | sed "s/.*$hopsrc.//"\`
    [ -s "$num/$scan/$hopsrc.\$root" ] || { echo No Root; exit 1; }
    echo '' ; echo Root is \$root ; echo ''
    export HOPS_FEARFIT_XPSPEC=1
  [ -z "x$ff_opts_x$ff_post_x" ] || {
    # beg hacking
    ff_opts_x="$ff_opts_x cf-$num"
    cat > cf-$num <<....EOF
    optimize_closure true
    * other things...
....EOF
    old=$num/$scan/$hopsrc.\$root.orig
    new=$num/$scan/$hopsrc.\$root
    mv \$new \$old
    sed /begin..FREQ/,\\\$d \$old > \$new
    cat >> \$new <<....EOF
*----------------------- begin \\\$FREQ               ----------------------*
\\\$FREQ;
    def VDIF_1032_2-FREQ;
     chan_def = W00U : &W : 100000.0 MHz : U : 32.0 MHz : &Ch01 : &BBC01 : &cp;
     sample_rate = 64.0 Ms/sec;
    enddef;
    def VDIF_544_2-FREQ;
     chan_def = W00U : &W : 100000.0 MHz : U : 32.0 MHz : &Ch01 : &BBC01 : &cp;
     sample_rate = 64.0 Ms/sec;
    enddef;
*-----------------------   end \\\$FREQ               ----------------------*
....EOF
    sed 1,/end..FREQ/d \$old >> \$new
    #
    echo \\
    \$ff \$ff_opts_x $scan $num/$scan/$hopsrc.\$root $ff_post_x
    \$ff \$ff_opts_x $scan $num/$scan/$hopsrc.\$root $ff_post_x
    # end hacking
    [ -f fearfit.spec ] && mv fearfit.spec $num/$scan/ff$num.X
  }
    [ -z "\`type psmerge \`" -o -z "\`type ps2pdf\`" ] && {
	echo no psmerge or ps2pdf
	exit 2
    } || {
	echo Making a summary pdf file
	pushd $num/$scan
	for d in ??.?.*.\$root
        do fplot -d \$d.ps \$d ; ls -l \$d.ps ; done
	psmerge -o$target.ps *.\$root.ps
	ps2pdf $target.ps
	popd
	ls -l $wdir/$num/$scan/$target.pdf | cut -c24-
    }
}
exit 0
# eof
EOF
chmod +x $sob.ff.sh

echo '' ; echo '%%% vi '$wdir/$sob.'{machines,threads,input}' ; echo ''

evince=`which evince`
[ -x "$evince" ] || evince=ls

$doit && {
    echo sleeping 2 seconds, then launching
    sleep 2

    echo pushd $wdir \&\&
    echo ./$sob.run.sh \&\&
    echo ./$sob.ff.sh \&\& popd
    cd $wdir &&
    time ./$sob.run.sh &&
    time ./$sob.ff.sh &&
    echo Done. || echo Failed.

    [ -f $wdir/$num/$scan/$target.pdf -a -n "$evince" ] &&
	$evince $wdir/$num/$scan/$target.pdf &
} || {
    echo pushd $wdir \&\&
    echo ./$sob.run.sh \&\&
    echo ./$sob.ff.sh \&\& popd
    echo $evince $wdir/$num/$scan/$target.pdf &
}
echo ''

#
# eof
#
