// dumpvis - dump of Swinburne file quantities  - rjc
// updated to automatically detect #vis   rjc 2018.11.8
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#if HAVE_DIFX2MARK4_H
/* which we probably don't */
#include "difx2mark4.h"
#else /* HAVE_DIFX2MARK4_H */
/* just what we need */
#define MAX_VIS 2048
#define SYNC_WORD 0xff00ff00
struct vis_record
    {
    int sync;
    int version;
    int baseline;
    int mjd;
    double iat;
    int config_index;
    int source_index;
    int freq_index;
    char pols[2];
    int pulsar_bin;
    double weight;
    double uvw[3];
    struct components
        {
        float real;
        float imag;
        } comp[MAX_VIS];
    };
#endif

int main (int argc,
          char **argv)
    {
    int i,
        n = 0,
        numch,                      // number of fine channels per record
        testsync;
    unsigned char *p;
    struct vis_record rec;

    FILE *fin;
    
    if (argc != 2)
        {
        printf ("usage: dumpvis <vis_file>\n");
        exit (0);
        }
    fin = fopen (argv[1], "r");
    if (fin == NULL)
        {
        printf ("problem opening %s\n", argv[1]);
        exit (0);
        }

    while (fread (&rec.sync, sizeof (int), 1, fin) > 0)
        {
        fread (&rec.version, sizeof (int), 1, fin);
        fread (&rec.baseline, sizeof (int), 1, fin);
        fread (&rec.mjd, sizeof (int), 1, fin);
        fread (&rec.iat, sizeof (double), 1, fin);
        fread (&rec.config_index, sizeof (int), 1, fin);
        fread (&rec.source_index, sizeof (int), 1, fin);
        fread (&rec.freq_index, sizeof (int), 1, fin);
        fread (&rec.pols[0], sizeof (char), 2, fin);
        fread (&rec.pulsar_bin, sizeof (int), 1, fin);
        fread (&rec.weight, sizeof (double), 1, fin);
        fread (&rec.uvw[0], sizeof (double), 3, fin);
                                    // read visibilities until sync encountered or EOF
        numch = 0;
        while (fread (&testsync, sizeof (int), 1, fin) > 0)
            {
                                    // backspace over test read
            fseek (fin, - sizeof (int), SEEK_CUR);
            if (testsync == SYNC_WORD)
                break;
                
            fread (&rec.comp[numch], sizeof (float), 2, fin);
            numch++;
            }
           
        printf ("\nrecord %d\n", n);
        printf ("sync %08x version %08x baseline %08x mjd %d\n", 
                rec.sync, rec.version, rec.baseline, rec.mjd);
        printf ("iat %le config_index %d source_index %d freq_index %d\n",
                rec.iat, rec.config_index, rec.source_index, rec.freq_index);
        printf ("pols %c%c pulsar_bin %d weight %le\n", 
                rec.pols[0], rec.pols[1], rec.pulsar_bin, rec.weight);
        printf ("uvw %lf %lf %lf\nvisibilities:\n", rec.uvw[0], rec.uvw[1], rec.uvw[2]);
        for (i=0; i<numch; i++)
            printf ("%d: %f %f\n", i, rec.comp[i].real, rec.comp[i].imag);
        n++;
        }
    printf ("\nrecord count %d\n", n);
    exit (0);
    }
