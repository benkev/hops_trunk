#!/bin/sh
CPPFLAGS='-D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE'
make anoise_maker.o CPPFLAGS="$CPPFLAGS"
make quant_hacker.o CPPFLAGS="$CPPFLAGS"
make anoise_common.o CPPFLAGS="$CPPFLAGS"
make anoise_station.o CPPFLAGS="$CPPFLAGS"
make anoise_extend.o CPPFLAGS="$CPPFLAGS"
make anoise_describe.o CPPFLAGS="$CPPFLAGS"
LOADLIBES='anoise_maker.o quant_hacker.o anoise_common.o anoise_station.o anoise_extend.o anoise_describe.o'
LDLIBS='-lgsl -lfftw3 -lrt -lm'
make anoise LOADLIBES="$LOADLIBES" LDLIBS="$LDLIBS"
./anoise --help

