/************************************************************************/
/*									*/
/* This routine takes one triangle record, and locates the relevant	*/
/* baseline records.  It transfers the triangle coherence time to these	*/
/* baseline records, and writes the three baselines to a temporary	*/
/* file.  It then invokes fringex on the temporary file, in order to	*/
/* generate segmented data, which it then feeds into an aedit script to	*/
/* form segmented bispectra.  Finally, the segmented bispectra are fed	*/
/* to average, to generate the coherence-corrected bispectrum		*/
/* corresponding to the input triangle record.  This bispectrum is	*/
/* appended to the output file.						*/
/*									*/
/*	Inputs:		triangle	A-file record pointer		*/
/*			data		baseline data array		*/
/*			nf		number of data records		*/
/*			outfile		Name of the output file		*/
/*									*/
/*	Output:		outfile		Filled with bispectrum data	*/
/*			return value	0=OK, 1=bad			*/
/*									*/
/* Created August 23 1996 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "adata.h"
#include "bispec.h"
#include "mk4_util.h"
#include "mk4_afio.h"

int
make_bispec (trianglesum *triangle,
             fringesum *data,
             int nf,
             char *outfile)
    {
    char fxcommand[256], aecommand[256], avcommand[256], catcommand[256];
    char tempname[50], fxoutname[50], aeoutname[50], avoutname[50];
    int i, j, nmatch;
    fringesum *datum;
    FILE  *fptemp;
					/* Scratch file root, reasonably unique */
    sprintf (tempname, "%s_%s", triangle->triangle, triangle->root_id[0]);
					/* Construct various scratch file names */
    sprintf (fxoutname, "%s.fxout", tempname);
    sprintf (aeoutname, "%s.aeout", tempname);
    sprintf (avoutname, "%s.avout", tempname);
					/* Open the temporary data file */
    if ((fptemp = fopen (tempname, "w")) == NULL)
	{
	msg ("Error opening temporary data file '%s'", 2, tempname);
	return (1);
	}
					/* Loop through data looking for records */
					/* which belong to this triangle record */
    nmatch = 0;
    for (i=0; i<nf; i++)
	{
	datum = data + i;
					/* Ignore records which don't match */
	if (datum->time_tag != triangle->time_tag) continue;
	if (datum->freq_code != triangle->freq_code) continue;
	if (strcmp (datum->source, triangle->source) != 0) continue;
	for (j=0; j<3; j++)
	    if (strcmp (datum->root_id, triangle->root_id[j]) == 0) break;
	if (j == 3) continue;
	for (j=0; j<3; j++)
	    if (datum->extent_no == triangle->extent_no[j]) break;
	if (j == 3) continue;
	for (j=0; j<3; j++)
	    if (datum->baseline[0] == triangle->triangle[j]) break;
	if (j == 3) continue;
	for (j=0; j<3; j++)
	    if (datum->baseline[1] == triangle->triangle[j]) break;
	if (j == 3) continue;
					/* If we get this far, we have a match */
	nmatch++;
	if (nmatch > 3)
	    {
	    msg ("Error, more than three baseline records match in make_bispec()", 2);
	    fclose (fptemp);
	    unlink (tempname);
	    return (1);
	    }
					/* Reset coherence time and write to temp file */
	datum->srch_cotime = triangle->cotime;
	write_fsumm (datum, fptemp);
	}
    fclose (fptemp);
					/* Now we construct the various command lines */
    sprintf (fxcommand, "fringex -i search -r %s -d srchpos -c > %s", 
							tempname, fxoutname);
    sprintf (aecommand, "aedit -b \"batch;read %s;close;twrite %s\"", fxoutname, aeoutname);
    sprintf (avcommand, "average -h -o %s %s", avoutname, aeoutname);
    if (strcmp (outfile, "none") == 0)
	sprintf (catcommand, "cat %s", avoutname);
    else
	sprintf (catcommand, "cat %s >> %s", avoutname, outfile);
					/* Execute them */
    if (system (fxcommand) != 0)  { msg ("fxcommand error", 2); return (1); }
    if (system (aecommand) != 0)  { msg ("aecommand error", 2); return (1); }
    if (system (avcommand) != 0)  { msg ("avcommand error", 2); return (1); }
    if (system (catcommand) != 0) { msg ("catcommand error", 2); return (1); }
					/* Clean up temp files */
    unlink (tempname);
    unlink (fxoutname);
    unlink (aeoutname);
    unlink (avoutname);

    return (0);
    }
