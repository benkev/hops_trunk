#!/bin/sh
#
# simple test harness
#
USAGE="$0 what [...] list of hosts

where what is
    hello       does a hello world on the named hosts
    ping_pong   passes a buffer back and forth between 2 hosts
    ring	passes a token around a ring of hosts
    ringload    takes two additional arguments (a buffer size
		and the number of cycles) and measures performance

The script presumes an environment variable \$bd has been defined
that points to a build directory as well as \$agent pointing to an
agent (/swc/scripts/o.mpi.sh).  Both variables must point to valid
locations on all hosts mentioned.  Finally \$vers may be set to
1.4.5 or 1.10.6 to use those versions.  If \$bd is not set, you
can set HOPS_VERS in the environmnet to look for it in
/swc/hops/build-\$HOPS_VERS/ompi.
"
hops=/swc/hops/build-$HOPS_VERS
[ -z "$bd" ] &&
  [ -d $HOME/ompi -a -x $HOME/ompi/hello_ompi ] && bd=$HOME/ompi
[ -z "$bd" ] &&
  [ -d $hops/ompi -a -x $hops/ompi/hello_ompi ] && bd=$hops/ompi
[ -z "$bd" ] &&
  [ -x ./hello_ompi ] && bd=`pwd`
[ -z "$bd" ] &&
  [ ! x"$1" = "xhelp" -a ! x"$1" = "x--help" ] && {
    echo
    echo an environment variable \$bd must be set to name
    echo the build directory
    echo
    exit 1
}
echo bd is $bd

[ -z "$VERBOSE" ] && export VERBOSE=false
[ "$VERBOSE" = 'true' -o "$VERBOSE" = 'false' ] || export VERBOSE=false
$VERBOSE && echo being verbose
[ -z "$vers" ] && export PATH=/opt/openmpi/bin:$PATH
[ -n "$vers" ] && export PATH=/opt/openmpi-$vers/bin:$PATH

[ -z "$agent$vers" ] && agent=/swc/scripts/o.mpi.sh
[ -z "$agent" -a -n "$vers" ] &&  agent=/swc/scripts/o.mpi-$vers.sh

#lp=/opt/openmpi/lib:/opt/openmpi/lib/openmpi
#lp=/opt/openmpi/lib
#[ -z "$LD_LIBRARY_PATH" ] && 
#    export LD_LIBRARY_PATH=$lp ||
#    export LD_LIBRARY_PATH=${lp}:$LD_LIBRARY_PATH

#mpi=/opt/openmpi/bin/mpirun
mpi=mpirun
$VERBOSE && type -p $mpi

# -d --debug-daemons -x VERBOSE=$VERBOSE

# good reading here: https://www.open-mpi.org/faq/?category=tuning
# --mca pml ob1 forces the ob1 network model, which uses btl

#flags="--noprefix --launch-agent $agent"
flags="--noprefix --launch-agent $agent"
flags="$flags --mca pml ob1"
flags="$flags --mca mpi_yield_when_idle 1"
#flags="$flags --mca rmaps seq"
flags="$flags -x VERBOSE"
#flags="$flags --wait-for-server"
#flags="$flags --do-not-resolve"
#flags="$flags --show-progress"
#flags="$flags --report-bindings"

$VERBOSE && flags="$flags --mca rmaps_base_display_map 1" &&
    flags="$flags --mca orte_base_help_aggregate 0" &&
    flags="$flags --mca OMPI_MCA_btl_base_verbose 30"

[ $# -gt 1 ] || set -- help
what=${1-'help'} ; shift

# handle args
case $what in
ringload) bsize=$1 cycles=$2 ; shift 2 ;;
*) ;;
esac

# everything else is a hostname
hosts=`echo $* | tr ' ' ,`

$VERBOSE &&
case $what in
hello)     echo $mpi $flags -n $# -H $hosts $agent $bd/hello_ompi ;;
ring)      echo $mpi $flags -n $# -H $hosts $agent $bd/$what ;;
ringload)  echo $mpi $flags -n $# -H $hosts $agent $bd/$what $bsize $cycles ;;
ping_pong) echo $mpi $flags -n  2 -H $hosts $agent $bd/$what ;;
*) echo "$USAGE" ;;
esac

case $what in
hello)     exec $mpi $flags -n $# -H $hosts $agent $bd/hello_ompi ;;
ring)      exec $mpi $flags -n $# -H $hosts $agent $bd/$what ;;
ringload)  exec $mpi $flags -n $# -H $hosts $agent $bd/$what $bsize $cycles ;;
ping_pong) exec $mpi $flags -n  2 -H $hosts $agent $bd/$what ;;
*) echo "$USAGE" ;;
esac

#
# eof
#
