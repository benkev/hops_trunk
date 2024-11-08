// bstate - make state count of input Mk5B file
//
// first version, based on b2a      2006.3.13   rjc
// 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define _FILE_OFFSET_BITS 64    /* This has to be defined for LFS */
#define SYNC 0xabaddeed

int main (int   argc, 
          char *argv[])
    {
    int i,
        j,
        nfr = 0,
        nfrtot,
        ns = 0,
        n[16][4];

    
    unsigned int data[2504];

    double pcts[16][4],
           gfact[16],
           pi = 3.14159265358979,
           a,
           x;
    
    FILE *fb;

    a = 8 * (pi - 3) / (3 * pi * (4 - pi));
    
    if (argc != 3)
        {
        printf ("Usage: bstate <input m5b fname> <# frames>\n");
        return(1);
        }

    nfrtot = atoi (argv[2]);
    
    fb = fopen(argv[1], "r");

    if (fb == NULL)
        {
        printf ("problem opening input file %s\n", argv[1]);
        perror ("bfind");
        return (1);
        }

                                    // initialize counters
    for (i=0; i<16; i++)
        for (j=0; j<4; j++)
            n[i][j] = 0;
            
                                    // loop over mk5b disk frames
    while (fread (data, sizeof(int), 2504, fb) == 2504)
        {
        if (data[0] != SYNC)        //tombstone on bad sync
            {
            printf ("invalid sync word read %8.8X, quitting!\n", data[0]);
            return (1);
            }

        for (i=4; i<2504; i++)
            for (j=0; j<16; j++)
                                    // 2nd index in order {--, +, -, ++}
                                    // due to MS order of sample data on disk
                ++n[j][data[i] >> (2*j) & 3];   
        nfr++;
        ns += 2500;

        if (nfr == nfrtot)
            {
                                    // calculate percentages and gain corrections
            for (i=0; i<16; i++)
                {
                for (j=0; j<4; j++)
                    pcts[i][j] = 100.0 * n[i][j] / ns;
                                    // x is fraction of samples in low state
                x = (double) (n[i][1] + n[i][2]) / ns;

                                    // gain factor to be applied before resampling
                gfact[i] = sqrt (-4 / (pi * a) - log (1 - x*x)
                           + 2 * sqrt (pow (2 / (pi * a) + log (1 - x*x) / 2, 2) 
                           - log (1-x*x)/a)) / 0.91;
                }
                
            printf ("\nch    --     -      +      ++      --     -     +    ++   gfact\n");
            for (i=0; i<16; i++)
                printf ("%2d %6d %6d %6d %6d   %5.1f %5.1f %5.1f %5.1f  %5.2f\n",
                        i, n[i][0], n[i][2], n[i][1], n[i][3],
                        pcts[i][0], pcts[i][2], pcts[i][1], pcts[i][3], gfact[i]);
            return (0);
            }
        }
    }



