/************************************************************************/
/*                                                                      */
/* This routine takes information derived from an A-file line, and      */
/* searches the pre-filled "dal" structure array for applicable cal     */
/* information.  If found, the calibration factor by which the fringe   */
/* amplitude should be multiplied to yield correlated flux density      */
/* in Janskys is returned.  A cal measurement is judged as suitable or  */
/* not using two criteria.  First, it must be close to the VLBI scan in */
/* time (in the range begin-MAXT or end+MAXT seconds), and second, it   */
/* must be taken while pointing at the same source as the VLBI scan (to */
/* ensure that we are looking at the same elevation).                   */
/*                                                                      */
/*      Inputs:         itim            VLBI scan time tag, HOPS format */
/*                      stn1, stn2      Station codes for baseline      */
/*                      el1, el2        Elevations in degrees           */
/*                      dur             Scan length in seconds          */
/*                      source          Name of source in VLBI scan     */
/*                                                                      */
/*      Output:         return value    Calibration factor, -1 if error */
/*                                                                      */
/* Created October 1996 by JAB                                          */
/* Converted to CJL's style, and moderately enhanced by CJL,            */
/*  October 24 1996                                                     */
/*                                                                      */
/************************************************************************/
#include <stdio.h>
#include "calamp.h"
#include "mk4_util.h"

float 
calfact (int itim,
         char stn1,
         char stn2,
         float el1,
         float el2,
         int dur,
         char *source)
    { 
    int i, st, stno, ns[2], ncal[2]; 
    int mtim, ttol; 
    double totcal[2]; 
    double s2, c2, e0, e, si, co, G, t1, t2, cal;
    int y, d, h, m, s, yy, dd, hh, mm, ss; 
    extern char * sta[];
    extern int sta_size, need_same_source, caltime;
    extern char stl[];
    extern struct Dal dal;
                                        /* Used in various places */
    int_to_time (itim, &y, &d, &h, &m, &s);

                                        /* Find stn1 and stn2 */ 
    for (i = 0; i < sta_size; i++) 
        if (stl[i] == stn1)      ns[0] = i; 
        else if (stl[i] == stn2) ns[1] = i; 
                                        /* Not found */ 
    if (ns[0] == 0 || ns[1] == 0)
        { 
        msg ("ERROR: \007 Can't find station code %c or %c", 2, stn1, stn2); 
        return(-1.0); 
        } 
                                        /* Check elevation values */ 
    if (el1 < 0.0 || el1 > 90.0 || el2 < 0.0 || el2 > 90.0) 
        { 
        msg ("ERROR: \007 Elevation %f or %f out of range", 2, el1, el2); 
        return(-1.0); 
        } 
                                        /* Check dur */
    if (dur < 2 || dur > 1500) 
        { 
        msg("ERROR: \007 Duration %d is out of range", 2, dur); 
        return(-1.0); 
        } 
                                        /* Mid time of scan */ 
    mtim = itim+dur/2; 
                                        /* Time tolerance */
    ttol = caltime + dur/2;
                                        /* For each station */ 

    ncal[0] = ncal[1] = 0; 
    totcal[0] = totcal[1] = 0.0;
    for (st = 0; st < 2; st++) 
        { 
        stno = ns[st];
                                        /* Loop through array to find cal values */ 
        for (i = 1; i < MAXRUNS; i++) 
            {
                                        /* Is this cal close enough in time? */
            if (abs (dal.ctime[stno][i] - mtim) > ttol) continue;
                                        /* OK, is it on the right source? */
            if (strcasecmp(source, dal.sources[stno][i]) != 0) 
                {
                if (need_same_source) continue;
                else
                    {
                    msg("Source warning at itim = %d %03d %02d:%02d:%02d", 
                                                        1, y, d, h, m, s); 
                    int_to_time(dal.ctime[stno][i], &yy, &dd, &hh, &mm, &ss); 
                    msg("  caltime(%s) = %d %03d %02d:%02d:%02d", 
                                -1, sta[stno], yy, dd, hh, mm, ss); 
                    msg("  source(VLBI) = %s, source(%s) = %s", 
                                -1, source, sta[stno], dal.sources[stno][i]); 
                    } 
                }
                                        /* Yes, we have a live one. */
                                        /* Calculate cal value - 6-token */
            if (dal.ctigs[stno][i] > 0.0) cal = dal.ctigs[stno][i];
                                        /* 3-token */
            else
                {
                /* gain = exp(-(4*pi/l)^2*(s2*(sinE-sinE0)^2+c2*(cosE-cosE0)^2)) */
                s2 = dal.parms[stno][i][0];
                c2 = dal.parms[stno][i][1];
                e0 = dal.parms[stno][i][2]*M_PI/180.0;
                G = dal.parms[stno][i][3];
                                        /* Better have read in some parms */
                                        /* for this station! */
                if (s2 == 0.0 && c2 == 0.0 && e0 == 0.0 && G == 0.0)
                    {
                    msg ("Error ... using 3-token formula with no PARAM info", 2);
                    msg ("Skipping station '%s', scan %d %03d %02d:%02d:%02d", 2,
                        sta[stno], y, d, h, m, s);
                    return (-1);
                    }
                if (st == 0) e = el1*M_PI/180.0;
                else e = el2*M_PI/180.0;
                si = sin(e)-sin(e0);
                co = cos(e)-cos(e0);
                cal = -dal.ctigs[stno][i] / exp(GK*(s2*si*si+c2*co*co)) / G;  
                }
                                        /* Add it in. */
            totcal[st] += cal;
            ncal[st] += 1;
            }           /* End of for i */
        }
                                        /* Do we have both stations? */
    if (ncal[0] == 0 || ncal[1] == 0)
        { 
        msg("ERROR \007 at itim = %d %03d %02d:%02d:%02d", 
                                                1, y, d, h, m, s); 
        if (ncal[0] == 0) msg("  %s has no valid cal", 1, sta[ns[0]]);  
        if (ncal[1] == 0) msg("  %s has no valid cal", 1, sta[ns[1]]); 
        return(-1.0); /* Should this be a fatal error? */ 
        }
                                            /* Info Only */
    msg ("Time %d %03d %02d:%02d:%02d, stations '%s' '%s', using %d, %d cals",
                -3, y, d, h, m, s, sta[ns[0]], sta[ns[1]], ncal[0], ncal[1]);
 
                                        /* For each station */ 
    t1 = totcal[0] / (double)ncal[0];
    t2 = totcal[1] / (double)ncal[1];
    msg ("Time %d %03d %02d:%02d:%02d, cal(%s) %.0f, cal(%s) %.0f",
                -3, y, d, h, m, s, sta[ns[0]], t1, sta[ns[1]], t2);

                                        /* SEFD in decakilojanskys (!) */ 
    return(1.0e-4*sqrt(t1*t2)); 
    }

