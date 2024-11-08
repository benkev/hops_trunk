/************************************************************************/
/*									*/
/* The job of this routine is to fill in the scsum array elements, one	*/
/* by one, converting baseline-based coherence times to station-based	*/
/* coherence times along the way.					*/
/*									*/
/*	Inputs:		fitlist		Afile data with baseline cotime	*/
/*			nf		size of fitlist			*/
/*			scsum		Preallocated but empty scan	*/
/*					summary array			*/
/*									*/
/*	Output:		scsum		All filled in			*/
/*			return value	0=OK, else bad			*/
/*									*/
/* Created November 30 1995 by CJL					*/
/*									*/
/************************************************************************/
#include "coterp.h"

int
stationize (fitlist, nf, scsum)
fringedata *fitlist;
int nf;
scansum *scsum;
    {
    int index, i, j, k, scan, ret;
    int year,day,hour,min,sec;
    char stn_id;
    fringesum *datum;
    scansum *s;

					/* Initialize for 1st pass */
    scan = 0;
    s = scsum + scan;
    s->scantime = fitlist[fitlist[0].order].fdata.time_tag;
    s->nb = 0;
					/* Loop over all data, filling */
					/* in data for each scan */
    for (i=0; i<nf; i++)
	{
	index = fitlist[i].order;
	datum = &(fitlist[index].fdata);
					/* New scan, point to next scsum */
					/* array element */
	if (datum->time_tag != s->scantime)
	    {
	    scan++;
	    s = scsum + scan;
	    s->scantime = datum->time_tag;
	    s->nb = 0;
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
					/* Make list of baselines and */
					/* coherence times in this scan */
	strcpy (s->basetimes[s->nb].baseline, datum->baseline);
	s->basetimes[s->nb].srch_cotime = datum->srch_cotime;
	s->basetimes[s->nb].noloss_cotime = datum->noloss_cotime;
	s->nb += 1;
	}
					/* Terminate scsum array */
    scsum[scan+1].scantime = -1;
					/* Fit each scan for station-based */
					/* coherence times */
    ret = 1;
    for (i=0; i<=scan; i++) 
	{
	s = scsum + i;
	int_to_time (s->scantime, &year, &day, &hour, &min, &sec);
	if ((ret = fit_times (s)) != 0) 
		{
		printf("Coterp barfing on scan %2d %03d-%02d%02d%02d\n",year,
			day,hour,min,sec);
		break;
		}
	for (j=0; j<s->nstat; j++)
	    {
	    printf ("%2d %03d-%02d%02d%02d %c %f %f\n", year,
                        day,hour,min,sec, s->station[j].stn_id,
			s->station[j].srch_cotime,
			s->station[j].noloss_cotime);
	    }
	}
    return (ret);
    }
