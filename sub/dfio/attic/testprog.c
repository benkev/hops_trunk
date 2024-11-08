#include <stdio.h>
#include "mk4_data.h"

char progname[] = "testprog";
int msglev = 1;

main (argc, argv)
int argc;
char *argv[];
    {
    struct mk4_corel corel, corel2;
    struct type_000 t000;
    struct type_100 t100;
    struct type_101 *t101a, *t101b;
    struct type_120 *t120[4];
    struct index_tag index;
    int i, j, ap, lag_len, size_101, size_120;

    corel.nalloc = 0;
					/* Init. */
    clear_mk4corel (corel);
    clear_100 (&t100);
					/* test values for type 100 */
    t100.baseline[0] = 'A';
    t100.baseline[1] = 'B';
    t100.filenum = 1;
    strcpy (t100.rootname, "1234/97031-230400/src.abcdef");
    t100.qcode[0] = '9';
    t100.qcode[1] = ' ';
    t100.pct_done = 90.0;
    clear_date (&(t100.start));
    clear_date (&(t100.stop));
    t100.ndrec = 4;
    t100.cflength = 100;
    t100.aplength = 10000;
    t100.mode = COUNTS_PER_LAG;
    t100.lags = 16;
    t100.nindex = 2;
    t100.blocks = 8;
    t100.rectype = 120;
					/* make space for 101, 102 records */
    size_101 = sizeof (struct dummy_101) + t100.blocks*4;
    t101a = (struct type_101 *)malloc (size_101);
    t101b = (struct type_101 *)malloc (size_101);

    if (t100.mode == COUNTS_PER_LAG)     lag_len = 16 * t100.lags;
    else if (t100.mode == COUNTS_GLOBAL) lag_len = 8 * t100.lags + 8;
    else if (t100.mode == AUTO_CORR)     lag_len = 8 * t100.lags;
    clear_101 (t101a);
    clear_101 (t101b);
    size_120 = sizeof (struct dummy_120) + lag_len;
    for (i=0; i<4; i++) 
	{
	t120[i] = (struct type_120 *)malloc (size_120);
	clear_120 (t120[i]);
	}
					/* Allocate space for pointers */
    corel_alloc (&corel, 2, 2);

    t101a->index = 0;
    clear_date (&(t101a->procdate));
    t101a->ref_fgroup[0] = 'X';
    t101a->ref_fgroup[1] = '1';
    t101a->rem_fgroup[0] = 'X';
    t101a->rem_fgroup[1] = '1';
    t101a->polarization[0] = 'R';
    t101a->polarization[1] = 'R';
    t101a->corr_board = 1;
    t101a->corr_slot = 2;
    t101a->ref_chan = 3;
    t101a->rem_chan = 3;
    t101a->delay_off = 0;
    t101a->gate_on = 0.0;
    t101a->gate_off = 0.0;
    t101a->post_mortem = 0;
    for (i=0; i<8; i++) t101a->blocks[i] = i;

    t101b->index = 1;
    clear_date (&(t101b->procdate));
    t101b->ref_fgroup[0] = 'X';
    t101b->ref_fgroup[1] = '1';
    t101b->rem_fgroup[0] = 'X';
    t101b->rem_fgroup[1] = '1';
    t101b->polarization[0] = 'R';
    t101b->polarization[1] = 'R';
    t101b->corr_board = 1;
    t101b->corr_slot = 2;
    t101b->ref_chan = 3;
    t101b->rem_chan = 3;
    t101b->delay_off = 0;
    t101b->gate_on = 0.0;
    t101b->gate_off = 0.0;
    t101b->post_mortem = 0;
    for (i=0; i<8; i++) t101b->blocks[i] = i;

    for (i=0; i<4; i++)
	{
	strncpy (t120[i]->unused2, "  ", 2);
	t120[i]->baseline[0] = 'A';
	t120[i]->baseline[1] = 'B';
	t120[i]->filenum = 1;
	strcpy (t120[i]->rootcode, "abcdef");
	t120[i]->index = i/2 + 1;
	t120[i]->ap = 0;
	t120[i]->flag = 0;
	t120[i]->status = 0;
	t120[i]->bitshift = 0.0;
	t120[i]->fbit = 0.0;
	for (j=0; j<t100.lags; j++)
	    {
	    t120[i]->ld.cpl[i].coscor = j;
	    t120[i]->ld.cpl[i].cosbits = j+100;
	    t120[i]->ld.cpl[i].sincor = j+200;
	    t120[i]->ld.cpl[i].sinbits = j+300;
	    }
	}
					/* set pointers for global recs */
    corel.id = &t000;
    strncpy (corel.id->record_id, "000", 3);
    strncpy (corel.id->version_no, "00", 2);
    strncpy (corel.id->unused1, "   ", 3);
    strncpy (corel.id->date, " 97024-100125 ", 14);
    strcpy (corel.id->name, "1234/97031-230400/AB.1.abcdef");
    corel.t100 = &t100;
					/* Throw in a few duplicated pointers */
    for (i=0; i<2; i++)
	{
	if (i == 0) corel.index[i].t101 = t101a;
	else corel.index[i].t101 = t101b;
	for (ap=0; ap<2; ap++)
	    {
	    j = 2*i + ap;
	    t120[j]->index = i;
	    t120[j]->ap = ap;
	    corel.index[i].t120[ap] = t120[j];
	    }
	}
					/* Fill in some dummy data */

					/* Write the file */
    write_mk4corel (corel, "1234/97031-230400/AB.1.abcdef");
					/* Read it back in */
    read_mk4corel ("/correlator/prog/src/sub/dfio/1234/97031-230400/AB.1.abcdef", &corel2);
					/* Write it out again */
    write_mk4corel (corel2, "1234/97031-230400/AB.1.abcdeg");
    }
