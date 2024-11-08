/*
 * $Id: hook_search.c 1377 2016-07-27 13:15:13Z gbc $
 *
 * Hook which allows variants on the mbd setup made in search.c
 * search.c:#define MBD_GRID_MAX 8192
 */

#define MBD_GRID_MAX 2*8192

#define SEARCH_HOOK \
    do msg("%d<lag<%d x %d<dr<%d x %d<mb<%d", 0,\
            status.win_sb[0], status.win_sb[1],\
            status.win_dr[0], status.win_dr[1],\
            status.win_mb[0], status.win_mb[1]); while(0);

#define search orig_search
#include "search.c"
#undef  search

#include <stdlib.h>

/*
 * call the original routine and expect the preceding hook to try harder.
 */
int search(struct type_pass *pass)
{
    //char *ep = getenv("HOPS_FEARFIT_MBDGRID");
    msg("Modified search in use -- MBD_GRID_MAX = %d", 1, MBD_GRID_MAX);
    orig_search(pass);
}

/*
 * eof
 */
