/************************************************************************/
/*									*/
/* Displays the station-based zenith coherence times on a PGPLOT device	*/
/*									*/
/*	Inputs:		scsum		array of scan info structs	*/
/*									*/
/*	Output:		display						*/
/*									*/
/* Created 27 February 1996 by CJL					*/
/*									*/
/************************************************************************/
#include "coterp.h"

void
plot_sttimes (scsum)
struct scansum *scsum;
    {
    msg ("Terribly sorry, but plotting is not yet implemented.", 2);
    cpgend();
    }
