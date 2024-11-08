/************************************************************************/
/*									*/
/* Resets a root structure to its initial state, freeing up any		*/
/* previously allocated memory.						*/
/*									*/
/*	Inputs:		root		Target structure		*/
/*									*/
/*	Output:		root		in pristine state		*/
/*									*/
/* Created September 15 1995 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include "mk4_data.h"
#include "mk4_sizes.h"
#include "mk4_dfio.h"

void
clear_mk4root (struct mk4_root *root)
    {
    int i, j;
					/* Free all allocated memory */
    for (i=0; i<root->nalloc; i++)
	free (root->allocated[i]);
    root->nalloc = 0;
					/* Set all pointers to null */
    root->id = NULL;
    root->t010 = NULL;
    root->t011 = NULL;

    for (i=0; i<MAXSTATIONS; i++)
	{
	root->stations[i].t020 = NULL;
	root->stations[i].t021 = NULL;
	for (j=0; j<MAXFREQ; j++)
	    root->stations[i].t022[j] = NULL;
	}
    }
