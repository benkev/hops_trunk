#!/usr/bin/csh
source /correlator/prog/mk4_setup.csh
echo ' '
echo 'This script generates a standard export tape for GSFC'
echo ' '
echo 'Be sure to run it after generating the standard export'
echo 'in order that all the appropriate files are included.'
echo ' '
if ($1 == "") then
  echo 'To run,  enter goddard.tape <experiment number> <experiment name>'
  echo ' '
  exit (1)
endif
if ($2 == "") then
  echo 'To run,  enter goddard.tape <experiment number> <experiment name>'
  echo ' '
  exit (1)
endif
echo 'Experiment number given is > \c'; echo $1
echo 'Experiment name given is > \c'; echo $2
echo ' '
echo 'Do you have a tape mounted in the tape drive? (y/n) '
echo ' '
set answer=$<
if ($answer == "y") then
  cd $CORDATA
  ln -s /correlator/data/$1/cf_$1 cf_$1 
  ln -s /correlator/data/$1/$1.ovex $1.ovex 
  ln -s /correlator/data/$1/$1.lvex $1.lvex 
  ln -s /correlator/data/goddard_export/exp$2/\#c$1 c$1 
  ln -s /correlator/data/goddard_export/exp$2/\#p$1 p$1 
  ln -s $CORDATA/$1/$1.alist.postrepro $1.alist 
  ln -s $CORDATA/$1/$1.alist.type2.final $1.alist.type2.final 
  tar chvf /dev/rmt/0mn $1.alist $1.ovex $1.lvex c$1 p$1 $1.alist.type2.final cf_$1 $1
  echo 'Current working directory is: \c'; pwd
  echo ' '
  echo 'Ready to purge temporary links?'
  echo ' '
  rm -i cf_$1 $1.ovex $1.lvex c$1 p$1 $1.alist $1.alist.type2.final 
endif
if ($answer == "n") then
  echo 'OK - exiting now ...'
  exit (1)
endif
echo 'Bye '
