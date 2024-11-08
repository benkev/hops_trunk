/************************************************************************/
/*									*/
/* This routine takes a root filename, and returns a filled-in mk4_root	*/
/* memory structure, ready for use by the caller.			*/
/*									*/
/*	Inputs:		filename	Better be a valid root file	*/
/*									*/
/*	Output:		root		Filled in mk4_root struct	*/
/*			return value	0 means OK, !=0 means error	*/
/*									*/
/* Created 18 August 1995 by CJL					*/
/*									*/
/************************************************************************/
#include <stdio.h>
#include "mk4_data.h"
#include "mk4_sizes.h"
#include "mk4_dfio.h"

int
read_mk4root (char *filename,
              struct mk4_root *root)
    {
    int n, totbytes, type, bytes, size, rec_id, version;
    int nspline, nstat, nchan;
    void *alloc_ptr;
    char *ptr;
    FILE *fp;
					/* Standard Mk4 file open call */
    if (open_datafile (filename, &type, &fp) != 0)
	{
	msg ("Cannot open '%s'", 3, filename);
	return (1);
	}
					/* Check for major goof */
    if (type != 0)
	{
	msg ("File '%s' fed to read_mk4root:", 3, filename);
	msg ("This not a type 0 file (type = %d)", 3, type);
	fclose (fp);
	return (1);
	}
					/* Read entire file into memory */
					/* read_mk4file() takes care */
					/* of allocating file_image */
    clear_mk4root (root);
    bytes = read_mk4file (fp, &(root->file_image));
    fclose (fp);
    if (bytes <= 0)
	{
	msg ("Error reading file '%s'", 3, filename);
	return (1);
	}
    root->allocated[root->nalloc] = root->file_image;
    root->nalloc++;
					/* Loop through memory image of */
					/* file, setting pointers to records */
					/* in root structure */
    ptr = root->file_image;
    totbytes = 0; nstat = 0;
    while (totbytes < bytes)
	{
	n = sscanf (ptr, "%3d%2d", &rec_id, &version);
	if (n != 2)
	    {
	    msg ("Unrecognized record type in root file, '%5s'", 2, ptr);
	    break;
	    }
					/* Decode each record */
	alloc_ptr = NULL;
	switch (rec_id)
	    {
	    case 000:
		root->id = (struct type_000 *)addr_000 (version, ptr, &size);
		if (root->id != (struct type_000 *)ptr) alloc_ptr = root->id;
		break;

	    case 010:
		root->t010 = (struct type_010 *)addr_010 (version, ptr, &size);
		if (root->t010 != (struct type_010 *)ptr) alloc_ptr = root->t010;
		break;

	    case 011:
		root->t011 = (struct type_011 *)addr_011 (version, ptr, &size);
		if (root->t011 != (struct type_011 *)ptr) alloc_ptr = root->t011;
		break;

	    case 020:
		if (nstat == MAXSTATIONS)
		    {
		    msg ("Too many station blocks in root file", 3);
		    free (root->file_image);
		    return (1);
		    }
		root->stations[nstat].t020 = 
				(struct type_020 *)addr_020 (version, ptr, &size);
		if (root->stations[nstat].t020 != (struct type_020 *)ptr) 
		    alloc_ptr = root->stations[nstat].t020;
		nstat++;
		nchan = 0;
		break;

	    case 021:
		root->stations[nstat].t021 = 
			(struct type_021 *)addr_021 (version, ptr, &size);
		if (root->stations[nstat].t021 != (struct type_021 *)ptr) 
		    alloc_ptr = root->stations[nstat].t021;
		break;

					/* This assumes records in order */
	    case 022:
		if (nchan == MAX_CHAN)
		    {
		    msg ("Too many channel records in root file", 3);
		    free (root->file_image);
		    return (1);
		    }
		nspline = root->stations[nstat].t020->nsplines;
		root->stations[nstat].t022[nchan] = 
			(struct type_022 *)addr_022 (version, ptr, nspline, &size);
		if (root->stations[nstat].t022[nchan] != (struct type_022 *)ptr) 
		    alloc_ptr = root->stations[nstat].t022[nchan];
		nchan++;
		break;

	    default:
		msg ("Inappropriate record type %d in root file", 2, rec_id);
		totbytes = bytes;
		size = 0;
		break;
	    }
					/* Keep track of allocated pointers */
	if (alloc_ptr != NULL)
	    {
	    root->allocated[root->nalloc] = alloc_ptr;
	    root->nalloc++;
	    }
					/* Adjust pointer and get next rec. */
	ptr += size;
	totbytes += size;
	}

    if (totbytes != bytes)
	msg ("Last record in '%s' truncated", 2, filename);

    return (0);
    }
