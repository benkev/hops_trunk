/************************************************************************/
/*									*/
/* Linearly interpolates the elevation-independent station-based	*/
/* coherence time for one station, using information present in the	*/
/* scsum array								*/
/*									*/
/*	Inputs:		scantime	Time tag of the datum		*/
/*			st		struct of station to be fitted	*/
/*			scsum		calibrator station-based	*/
/*					coherence times			*/
/*			type		search or noloss cotimes	*/
/*									*/
/*	Output:		return value	interpolated coherence time	*/
/*									*/
/* Created 1 December 1995 by CJL					*/
/*									*/
/************************************************************************/
#include "coterp.h"

float
fit_cotime (scantime, st, scsum, type)
int scantime;
stsum *st;
scansum *scsum;
int type;
    {
    int before, after, onlyscan, stna, stnb, i, j;
    float cotime, cotimea, cotimeb, tdiff, tdiffa, tdiffb;
    float elfactor();
    char station;

    if ((type != SRCH_COTIME) && (type != NOLOSS_COTIME)) 
	{
	msg ("Bad type in fit_cotime\n", 2);
	return (-1.0);
	}

    before = after = onlyscan = -1;
    station = st->stn_id;
					/* Locate scantime in scsum array */
    i = 0;
    while (scsum[i].scantime > 0)
	{
	if (scsum[i].scantime == scantime) 
	    {
	    onlyscan = i;
/*	    break; */
	    }
	if (scsum[i].scantime < scantime) before = i;
	if ((scsum[i].scantime > scantime) && (after == -1)) after = i;
	i++;
	}
					/* Set onlyscan if we are setting */
					/* coherence times to value for a single */
					/* scan */
    if (onlyscan < 0)
	if ((before < 0) || (after < 0))
	    {
	    if (before == -1) onlyscan = after;
	    else if (after == -1) onlyscan = before;
	    }
					/* Simple assignment according to type */
    if (onlyscan >= 0)
	{
	for (j=0; j<scsum[onlyscan].nstat; j++)
	    if (scsum[onlyscan].station[j].stn_id == station) break;
	if (j < scsum[onlyscan].nstat) 
/*	if (j == scsum[onlyscan].nstat) 
	    {
	    msg ("Station '%c' not found in fit_cotime()", 2, station);
	    return (-2.0);
	    } */
	    {
	    if (type == SRCH_COTIME) 
		cotime = scsum[onlyscan].station[j].srch_cotime;
	    else
		cotime = scsum[onlyscan].station[j].noloss_cotime;
					/* Correct for elevation in cal scan */
	    cotime /= elfactor (scsum[onlyscan].station[j].elevation);
					/* Correct back for elevation in */
					/* target scan */
	    cotime *= elfactor (st->elevation);
	    return (cotime);
	    }
	}
					/* Need to interpolate */
					/* First locate station indices */
    for (stnb=0; stnb<scsum[before].nstat; stnb++)
	if (scsum[before].station[stnb].stn_id == station) break;
    if (stnb == scsum[before].nstat)
	{
	msg ("WARNING!  Assuming default 10 secs for station '%c'", 2, station);
	return (10.0);
	}

    for (stna=0; stna<scsum[after].nstat; stna++)
	if (scsum[after].station[stna].stn_id == station) break;
    if (stna == scsum[after].nstat)
	{
	msg ("WARNING!  Assuming default 10 secs for station '%c'", 2, station);
	return (10.0);
	}
					/* Now get corresponding cotimes */
    if (type == SRCH_COTIME)
	{
	cotimeb = scsum[before].station[stnb].srch_cotime;
	cotimea = scsum[after].station[stna].srch_cotime;
	}
    else 
	{
	cotimeb = scsum[before].station[stnb].noloss_cotime;
	cotimea = scsum[after].station[stna].noloss_cotime;
	}
					/* Correct for cal scan elevations */
    cotimeb /= elfactor (scsum[before].station[stnb].elevation);
    cotimea /= elfactor (scsum[after].station[stna].elevation);
					/* Time differences */
    tdiff = (float)(scsum[after].scantime - scsum[before].scantime);
    tdiffb = (float)(scantime - scsum[before].scantime);
    tdiffa = (float)(scsum[after].scantime - scantime);
					/* Interpolate to el-independent number */
    cotime = (cotimea*tdiffb + cotimeb*tdiffa) / tdiff;
					/* Correct for scan elevation */
    cotime *= elfactor (st->elevation);
    return (cotime);
    }
