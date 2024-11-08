/*
 * $Id$
 *
 * Hook to bypass some phasecal work
 */

#include <stdlib.h>

#define PCAL_HACK_HOOK \
    /* when set, skips loop over station pcal data */ \
    if (getenv("HOPS_FEARFIT_NOPCAL")) return(0);

#include "pcal_interp.c"

/*
 * eof
 */
