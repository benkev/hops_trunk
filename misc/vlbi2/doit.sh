#!/bin/sh
# xcorr script for 2-bit data based on xcorr by DLS

#
# provide the help
#
[ "$#" -ge 2 ] || {
    cat<<....EOF
    Usage: $0 file0 file1 '[options]'

    -proctime frames        (default is 100)
    -rev 0|1                (no idea)
    -2bit 0|1               (default is 0 for 1-bit data)
    -ampfac 1.0             (multiplier on amplitude)
    -verb 0|1|2             (how much crap on your screen)
    -ps plotfile            (name of ps plot)
    -data datafile          (name of file with numbers)

    If vlbi2g does not exist, it will be compiled first.
    To correlate, e.g.:

    $0 sample_if0.m5b sample_if2.m5b -2bit 1 \\
        -proctime 10 -ps plot.ps -verb 0 -data data.txt

    and you can examine the data with, e.g. gnuplot
....EOF
    exit 0
}

#
# create the executable
#
[ -f ./vlbi2 ] && {
    echo move ./vlbi2 aside to proceed.
    exit 4
}
[ -x ./vlbi2g ] || {
    echo building vlbi2g
    [ -f mainv.c -a -f vlbiv.c -a -f four.c ] || {
        echo Missing one of mainv.c vlbiv.c four.c
        exit 1
    }
    echo -n compiling with
    type -p gcc || { echo no gcc ; exit 2; }
    gcc -g -O2 -D_FILE_OFFSET_BITS=64 -o mainv.o -c mainv.c &&
    gcc -g -O2 -D_FILE_OFFSET_BITS=64 -o vlbiv.o -c vlbiv.c &&
    gcc -g -O2 -D_FILE_OFFSET_BITS=64 -o four.o -c four.c &&
    gcc -g -O2 -D_FILE_OFFSET_BITS=64 -o vlbi2 mainv.o vlbiv.o four.o -lm || {
        echo gcc compile failed
        exit 3
    }
    mv vlbi2 vlbi2g
}

# gcc -W -Wall -O3 -D_FILE_OFFSET_BITS=64 vlbi7.c -lm

./vlbi2g $*

#
# eof
#
