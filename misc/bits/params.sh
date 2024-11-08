# generate this automatically, later.

#$BBC
# BBC-ID  Physical-BBC-#  IF-ID
#$FREQ
# Band-ID Sky-at0Hz-BBC   NetSB  ChBW    TrkID BBCID          PcalID
#$IF
# IF-ID   Physical-Name   Pol    Tot-LO  NetSB PCal-f-spacing PCal-f-base

VDIF_1032_2_bbc_assign='*
      BBC_assign = &BBC01 : 01 : &IF_XR;
'
#     chan_def = &W : 100062.5 MHz : U : 62.5 MHz : &Ch02 : &BBC01 : &calp;
VDIF_1032_2_freq_assign='*
      chan_def = &W : 100000.0 MHz : U : 62.5 MHz : &Ch01 : &BBC01 : &calp;
      sample_rate = 125.0 Ms/sec;
'
VDIF_1032_2_if_assign='*
      if_def = &IF_XR : XX : R : 100000.0 MHz : U : 0 MHz : 0 Hz;
'

VDIF_1056_2_bbc_assign='*
      BBC_assign = &BBC01 : 01 : &IF_XR;
'
VDIF_544_2_bbc_assign="$VDIF_1056_2_bbc_assign"

#     chan_def = &W : 100064.0 MHz : U : 32.0 MHz : &Ch03 : &BBC01 : &calp;
#     chan_def = &W : 100096.0 MHz : U : 32.0 MHz : &Ch04 : &BBC01 : &calp;
VDIF_544_2_freq_assign='*
      chan_def = &W : 100000.0 MHz : U : 32.0 MHz : &Ch01 : &BBC01 : &calp;
      sample_rate = 64.0 Ms/sec;
'
VDIF_1056_2_freq_assign='*
      chan_def = &W : 100000.0 MHz : U : 32.0 MHz : &Ch01 : &BBC01 : &calp;
      chan_def = &W : 100032.0 MHz : U : 32.0 MHz : &Ch02 : &BBC01 : &calp;
      sample_rate = 64.0 Ms/sec;
'
VDIF_1056_2_if_assign='*
      if_def = &IF_XR : XX : R : 100000.0 MHz : U : 0 MHz : 0 Hz;
'
VDIF_544_2_if_assign="$VDIF_1056_2_if_assign"

# eventually these as well...

# addZoomFreq = freq@100000.00/bw@32.0
# addZoomFreq = freq@100032.00/bw@30.5
# addZoomFreq = freq@100062.50/bw@1.5
#
# addZoomFreq = freq@100064.00/bw@32.0
# addZoomFreq = freq@100096.00/bw@29.0
# addZoomFreq = freq@100125.00/bw@3.0
Vv_zoom=' '
Ww_zoom=' '

y=t
eval W${y}_zoom="''"
eval V${y}_zoom="''"
eval A${y}_zoom="'
  addZoomFreq = freq@100000.00/bw@32.0
'"
eval B${y}_zoom="'
  addZoomFreq = freq@100000.00/bw@32.0
'"

## SUBINT=32000000 FFTRES=0.00003125 \
## ./start_alma.sh xxx \
## /data-sc01/gbc/alma/data/A1.vdif \
## /data-sc01/gbc/alma/data/V3.vdif

## SUBINT=32000000 FFTRES=0.00003125 \
## ./start_alma.sh xxx \
## /data-sc01/gbc/alma/data/A5.vdif \
## /data-sc01/gbc/alma/data/V7.vdif

## SUBINT=32000000 FFTRES=0.00003125 \
## ./start_alma.sh xxx \
## /data-sc01/gbc/alma/data/Ax.vdif \
## /data-sc01/gbc/alma/data/Vx.vdif

## y=v; SUBINT=32000000 FFTRES=0.00003125 \
## ./start_alma.sh xxx \
## /data-sc01/gbc/alma/data/A$y.vdif \
## /data-sc01/gbc/alma/data/V$y.vdif

# SUBINT=32000000 NINT=0.8 FFTRES=0.00390625
## FF_OPTS_X='-bAV'
## manually fix up the channels, then run fourfit?

# ./start_alma.sh xxx \
#  /data-sc01/gbc/alma/data/A$y.vdif \
#  /data-sc01/gbc/alma/data/V$y.vdif

# ./start_alma.sh h4 \
#  /data-sc01/gbc/alma/data/A$y.vdif \
#  /data-sc01/gbc/alma/data/B$y.vdif \
#  /data-sc01/gbc/alma/data/V$y.vdif \
#  /data-sc01/gbc/alma/data/W$y.vdif

# eof
