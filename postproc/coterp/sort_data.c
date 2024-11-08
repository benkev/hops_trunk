/************************************************************************/
/*									*/
/* Places the data in time, baseline and segmentation time order, 	*/
/* ready for loop in main routine.					*/
/*									*/
/*	Inputs:		data		It would be tough without this	*/
/*			nrec		number of records to sort	*/
/*									*/
/*	Output:		data		Ready to boogie			*/
/*									*/
/* Created October 5 1995 by CJL, borrowed from average and simplified	*/
/*									*/
/************************************************************************/
#include "coterp.h"

int
sort_data (data, nrec)
fringedata *data;
int nrec;
    {
    int i;
					/* Initialize the sort order */
    for (i=0; i<nrec; i++) data[i].order = i;
					/* Sort by baseline */
    for (i=0; i<nrec; i++)
	    data[i].keyval = data[i].fdata.baseline[1]
                                        + 256 * data[i].fdata.baseline[0];
    sorter (data, nrec);
					/* Sort by time */
    for (i=0; i<nrec; i++)
	data[i].keyval = data[i].fdata.time_tag;
    sorter (data, nrec);
					/* Finally, sort by fgroup */
    for (i=0; i<nrec; i++)
	data[i].keyval = data[i].fdata.freq_code;
    sorter (data, nrec);

    return (0);
    }
