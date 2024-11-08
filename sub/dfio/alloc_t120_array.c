/************************************************************************/
/*									*/
/* The type-120 records which contain variable-length lag data are	*/
/* declared with stubbed lag data arrays.  As far as the compiler is	*/
/* concerned, therefore, these records have a definite, but short,	*/
/* length.  In fact, the lag data occupy a possibly very large space at	*/
/* the end of the type-120 record.  When allocating a memory array	*/
/* of type-120 records to hold data, we need to figure out just how big	*/
/* the records are based on correlation type and number of lags, then	*/
/* set record pointers to the start of each record.  These pointers	*/
/* cannot participate in pointer arithmetic.  This routine handles the	*/
/* allocation of a data array within a mk4_corel structure.  Because	*/
/* of the complicated messing around that is going on, this routine	*/
/* must only be called once for each requested array, and therefore	*/
/* index->ap_space must be big enough to cope with any anticipated 	*/
/* need.  The allocated type-120 records are not initialized.		*/
/*									*/
/*	Inputs:		type		determines data structure	*/
/*			nlags		power of 2			*/
/*			index		contains ap_space element	*/
/*					specifying # of t120 records to	*/
/*					be allocated			*/
/*									*/
/*	Output:		index		index struct with type-120	*/
/*					pointers filled in		*/
/*									*/
/* Created 18 December 1996 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include "mk4_data.h"
#include "mk4_dfio.h"

int
alloc_t120_array (int type,
                  int nlags,
                  struct index_tag *index)
    {
    int size, lag_len, i;
    char *array;
					/* Check that no allocation has */
					/* already been done (as best we can) */
    if (index->t120[0] != NULL)
	{
	msg ("Error, looks like an attempt at multiple allocation of", 2);
	msg ("type-120 record memory space", 2);
	return (-1);
	}
					/* Figure out real size of records */
    if (type == COUNTS_PER_LAG)     lag_len = 16 * nlags;
    else if (type == COUNTS_GLOBAL) lag_len = 8 * nlags + 8;
    else if (type == AUTO_CORR)     lag_len = 8 * nlags;
    size = sizeof (struct dummy_120) + lag_len;
					/* Allocate the memory space */
    array = (char *)malloc (index->ap_space * size);
    if (array == NULL)
	{
	msg ("Error allocating memory for lag data records", 2);
	return (-1);
	}
					/* Set pointers */
    for (i=0; i<index->ap_space; i++)
	index->t120[i] = (struct type_120 *)(array + i*size);

    return (0);
    }
