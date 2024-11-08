/************************************************************************/
/*									*/
/* Given a set of station-based coherence times, and a set of baselines	*/
/* and times, this routine interpolates the station-based coherence	*/
/* times to the epoch of each baseline, and forms the elevation		*/
/* corrected coherence time for that baseline, writing it into the	*/
/* fringesum structure for that baseline.				*/
/*									*/
/*	Inputs:		masterlist	Main baseline data array	*/
/*			nm		size of masterlist		*/
/*			scsum		Array of station-based cotimes	*/
/*									*/
/*	Output:		masterlist	coherence times filled in	*/
/*									*/
/* Created 1 December 1995 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include <math.h>
#include "coterp.h"

int
interp (masterlist, nm, scsum)
fringedata *masterlist;
int nm;
scansum *scsum;
    {
    int i, j, k, st1, st2, index, nscan, scantime, oldtimetag;
    char stn_id;
    double cotime1, cotime2, sumsq;
    fringesum *datum;
    scanstsum *scstsum, *s;
    stsum *st;
    float fit_cotime();
    extern double max_cotime;
					/* First count scans in masterlist */
    oldtimetag = 0;
    nscan = 0;
    for (i=0; i<nm; i++)
	{
	index = masterlist[i].order;
	if (masterlist[index].fdata.time_tag != oldtimetag)
	    {
	    nscan++;
	    oldtimetag = masterlist[index].fdata.time_tag;
	    }
	}
					/* Allocate array of station-based */
					/* coherence times */
    scstsum = (scanstsum *)calloc (nscan, sizeof (scanstsum));
    if (scstsum == NULL)
	{
	msg ("Could not allocate memory for scan station array", 2);
	return (1);
	}
					/* Fill the array as far as possible */
					/* Initialize for 1st pass */
    nscan = 0;
    s = scstsum + nscan;
    s->scantime = masterlist[masterlist[i].order].fdata.time_tag;
					/* Loop over all data, filling in data */
					/* for each scan */
    for (i=0; i<nm; i++)
	{
	index = masterlist[i].order;
	datum = &(masterlist[index].fdata);
					/* New scan, point to next element */
	if (datum->time_tag != s->scantime)
	    {
	    nscan++;
	    s = scstsum + nscan;
	    s->scantime = datum->time_tag;
	    s->nstat = 0;
	    }
					/* Make list of stations and */
					/* elevations in this scan */
	for (j=0; j<2; j++)
	    {
	    stn_id = datum->baseline[j];
	    for (k=0; k<s->nstat; k++)
		if (stn_id == s->station[k].stn_id) break;
	    if (k == s->nstat)
		{
		s->station[k].stn_id = stn_id;
		if (j == 0) s->station[k].elevation = datum->ref_elev;
		else        s->station[k].elevation = datum->rem_elev;
		s->nstat += 1;
		}
	    }
	}
					/* Now loop over scans, and interpolate */
					/* each station within each scan */
    for (i=0; i<=nscan; i++)
	{
	scantime = scstsum[i].scantime;
	for (j=0; j<scstsum[i].nstat; j++)
	    {
	    st = scstsum[i].station + j;
					/* Do a simple linear interpolation */
	    st->srch_cotime = fit_cotime (scantime, st, scsum, SRCH_COTIME);
	    st->noloss_cotime = fit_cotime (scantime, st, scsum, NOLOSS_COTIME);
	    }
	}
					/* Transfer interpolated times to baselines */
    j = 0;
    for (i=0; i<nm; i++)
	{
	index = masterlist[i].order;
	datum = &(masterlist[index].fdata);
					/* Stay pointed at right scan */
	if (datum->time_tag != s->scantime)
	    {
	    for ( ; j<=nscan; j++)
		{
		s = scstsum + j;
		if (datum->time_tag == s->scantime) break;
		}
	    if (j > nscan)
		{
		msg ("Error, lists out of step in interp", 2);
		return (1);
		}
	    }
					/* Get stations and combine cotimes */
	for (k=0; k<s->nstat; k++)
	    if (datum->baseline[0] == s->station[k].stn_id) break;
	st1 = k;
	for (k=0; k<s->nstat; k++)
	    if (datum->baseline[1] == s->station[k].stn_id) break;
	st2 = k;
	if ((st1 == s->nstat) || (st2 == s->nstat))
	    {
	    msg ("Error, unrecognized station in interp", 2);
	    return (1);
	    }
					/* Calculate both coherence times */
					/* Use nearest integer > 0 */
	cotime1 = s->station[st1].srch_cotime;
	cotime2 = s->station[st2].srch_cotime;
	sumsq = 1.0/(cotime1 * cotime1) + 1.0/(cotime2 * cotime2);
	datum->srch_cotime = 1.0 / sqrt(sumsq) + 0.5;
	if (datum->srch_cotime == 0) datum->srch_cotime = 1;
	if (datum->srch_cotime > max_cotime) datum->srch_cotime = max_cotime;

	cotime1 = s->station[st1].noloss_cotime;
	cotime2 = s->station[st2].noloss_cotime;
	sumsq = 1.0/(cotime1 * cotime1) + 1.0/(cotime2 * cotime2);
	datum->noloss_cotime = 1.0 / sqrt(sumsq) + 0.5;
	if (datum->noloss_cotime == 0) datum->noloss_cotime = 1;
	if (datum->noloss_cotime > max_cotime) datum->noloss_cotime = max_cotime;

	}

    return (0);
    }
