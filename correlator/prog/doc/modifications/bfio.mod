1994/09/02, 16:47; cjl; bfio; all; Added byte-reversal routines to accommodate DEC alpha machines
1994/09/14, 12:48; cjl; bfio; read_corel; modified error printouts to be more informative, less annoying
1995/01/04, 13:45; cjl; bfio; swap.c; fix bug in float_reverse
1995/01/18, 10:52; cjl; bfio; read_root; Fix error in type-1000 record initialization
1995/04/25, 13:01; cjl; bfio; read_root; Trapped HP floating point format files
1995/08/17, 16:21; cjl; bfio; read_fringe; Enhance error reporting
1997/11/21, 10:22; cjl; bfio; data.h; Increased MAX_T2000 to 100, eliminates bug for spectral line
1998/05/19, 07:23; cjl; bfio; read_root; Used memset for t1000 init, copybuf/memcpy cause undiagnosed segfault under linux
