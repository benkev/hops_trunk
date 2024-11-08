#
# Trivial Makefile
#

dist_check_SCRIPTS = test.sh
check_PROGRAMS = hello_ompi send_recv ping_pong ring ringload

CC = $(MPICC)
CFLAGS = $(AM_CFLAGS) $(OPENMP_CFLAGS)

hello_ompi_SOURCES = hello_ompi.c
send_recv_SOURCES = send_recv.c
ping_pong_SOURCES = ping_pong.c
ring_SOURCES = ring.c
ringload_SOURCES = ringload.c

all-local: $(check_PROGRAMS)

distclean-local:
	rm -rf autom4te.cache 
	rm -f Makefile.in aclocal.m4 autom4te.cache 
	rm -f configure depcomp install-sh missing

#
# eof
#
