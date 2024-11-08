// bwrite - reads a mk5b format file and overwrites the data with a pcal tone
//
// first version, based on bfind      2010.10.21  rjc
// 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define _FILE_OFFSET_BITS 64    /* This has to be defined for LFS */
#define SYNC 0xabaddeed

#define TWOPI 6.28318530717958;

int main (int   argc, 
          char *argv[])
    {
    int i,
        nfr = 0,
        index,
                                    // transition at 40 degrees corresponds very closely to
                                    // a sampler threshold of 0.91 of the sigma of the sine funtion
//      value[36] = {1, 1, 1, 1, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1, 1, 1, 1,
//                   2, 2, 2, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2, 2};
        value[36] = {1, 2, 1, 2, 1, 2, 1, 2, 2, 3, 2, 1, 2, 1, 2, 1, 2, 1,
                     2, 1, 2, 1, 2, 1, 2, 1, 1, 0, 1, 2, 1, 2, 1, 2, 1, 2};
                     

    
    unsigned int data[2504],
                 d;

    double fs,
           f_tone,
           theta = 0.0,
           dtheta;

    FILE *fin,
         *fout;

    if (argc !=  5)
        {
        printf ("Usage: bwrite <input m5b fname> <output m5b fname> <fs (MHz)> <tone f (KHz)>\n");
        return(1);
        }

    fs = atof (argv[3]);
    f_tone = atof (argv[4]);

    dtheta = 0.36 * f_tone / fs;    // delta theta in degrees

    fin = fopen(argv[1], "r");

    if (fin == NULL)
        {
        printf ("problem opening input file %s\n", argv[1]);
        perror ("bwrite");
        return (1);
        }

    fout = fopen(argv[2], "w");

    if (fout == NULL)
        {
        printf ("problem opening output file %s\n", argv[2]);
        perror ("bwrite");
        return (1);
        }

                                    // loop over mk5b disk frames
    while (fread (data, sizeof(int), 2504, fin) == 2504)
        {
        if (data[0] != SYNC)        //tombstone on bad sync
            {
            printf ("invalid sync word read %8.8X at frame # %d, quitting!\n", data[0], nfr);
            for (i=0; i<16; i++)
                printf ("%x ", data[i]);
            printf ("\n");
            return (1);
            }

        for (i=4; i<2504; i++)
            {
                                    // map theta into 0..35
            index = (int) (0.1 * fmod (theta, 360.0));
            d = value[index];
            data[i] = d | d<<2 | d<<4 | d<<6 | d<<8 | d<<10 | d<<12 | d<<14 |
                      d<<16 | d<<18 | d<<20 | d<<22 | d<<24 | d<<26 | d<<28 | d<<30;
            theta += dtheta;
            }
        fwrite (data, sizeof (int), 2504, fout);
        nfr++;
        }
    }

