/************************************************************************/
/*									*/
/* This is the routine that writes the type-2 data out to the stream    */
/* passed as fp.  Each call writes 1 line.                              */
/* Differences in the							*/
/* way C and Fortran format numbers necessitate some fiddling with a	*/
/* couple of characters in the output of sprintf.  Essentially, C will	*/
/* expand a field width to accomodate the number, where Fortran will	*/
/* not (it prints asterisks for out of range values).  Combine this	*/
/* with the fact that C insists on a 0 before the decimal point for	*/
/* floating point numbers less than 1, and the Fortran format F4.3 	*/
/* cannot be easily mimicked.  Affected are sbresid and ambiguity	*/
/*									*/
/*	Inputs:		data		Filled in frngesum structure	*/
/*              	fp		Regular A-format output file	*/
/*									*/
/* Created as write_data() April 5 1989 by CJL				*/
/* Modified April 23 1991 by CJL -- fix bug for snr >= 1000		*/
/* Modified March 11 1991 by CJL -- Separate type 0, 1, 2 output	*/
/* Separated into afio library from aedit by CJL, 23 September 1992     */
/* This involved alteration to write only one line at a time.           */
/* modified by AEER for fringex  		         		*/
/* -modified by SSD for fold output for CALTECH package                 */
/************************************************************************/

#include <stdio.h>
#include <math.h>
#include <sys/types.h>
#include <errno.h>
#include "adata.h"

int
write_vsumm (data,fp)
fringesum *data;
FILE *fp;
    {
    char *wformat2f,*wformat2g,*wformat2h,*wformat2i,*wformat2j;
    char buf[210], buf2[210], format[220];
    int pyear, pday, phour, pmin, psec, syear, sday, shour, smin, ssec;

    wformat2f = "<%-5s %1d %2d %4d %4d %2d%03d%c%02d%02d %2d %03d-%02d%02d\
 %-8s %-2s%c %02d.0 00 %9.2f %c %02d %5.2f %4.1f %5.1f %7.1f %7.1f %5.1f %5.1f %3d \n";
    wformat2g = "<%-5s %1d %2d %4d %4d %2d%03d%c%02d%02d %2d %03d-%02d%02d\
 %-8s %-2s%c %02d.0 00 %9.2f %c %02d %5.1f %4.1f %5.1f %7.1f %7.1f %5.1f %5.1f %3d \n";
    wformat2h = "<%-5s %1d %2d %4d %4d %2d%03d%c%02d%02d %2d %03d-%02d%02d\
 %-8s %-2s%c %02d.0 00 %9.2f %c %02d %5.2f %3.0f. %5.1f %7.1f %7.1f %5.1f %5.1f %3d \n";
    wformat2i = "<%-5s %1d %2d %4d %4d %2d%03d%c%02d%02d %2d %03d-%02d%02d\
 %-8s %-2s%c %02d.0 00 %9.2f %c %02d %5.1f %3.0f. %5.1f %7.1f %7.1f %5.1f %5.1f %3d \n";

    if (data->snr < 100) 
      {
	strcpy (format, wformat2f);
	if (data->amp > 99.999) strcpy (format, wformat2g);
      }
    else 
      {
	strcpy (format, wformat2h);
	if (data->amp > 99.999) strcpy (format, wformat2i);
      }
    
    int_to_time(data->procdate,&pyear,&pday,&phour,&pmin,&psec);
    int_to_time(data->time_tag,&syear,&sday,&shour,&smin,&ssec);
    
    sprintf(buf,format,
	data->fname,
	2,
	data->extent_no,
	data->length,
	data->expt_no,
	pyear,pday,
        '-',
        phour,pmin,
	syear,sday,shour,smin,
	data->source,
	data->baseline,
        data->quality,
        ssec,
        data->ref_freq,
        data->freq_code,
        data->no_freq,
	data->amp,
	data->snr,
	data->resid_phas,
        data->u,
        data->v,
        99.0,
        98.0,
        data->length);
        
      
    if(fputs(buf,fp) == EOF) 
	{
	msg("Error writing to output file",2);
	return(-1);
	}

    return(0);
    }







