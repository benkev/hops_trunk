// bfind - searches a mk5b format file for a given pattern
//
// first version, based on b2a      2006.3.13   rjc
// 
#include <stdio.h>
#include <stdlib.h>

#define _FILE_OFFSET_BITS 64    /* This has to be defined for LFS */
#define SYNC 0xabaddeed

int main (int   argc, 
          char *argv[])
    {
    int i,
        j,
        nfr = 0,
        nwords;

    
    unsigned int data[2504],
                 mask,
                 word[10];

    
    FILE *fb;

    unsigned int atoh (char *);
    
    if (argc < 4)
        {
        printf ("Usage: bfind <input m5b fname> <mask> <word0> <word1> ...\n");
        return(1);
        }

    mask = atoh (argv[2]);

    for (i=3; i<argc; i++)
        word[i-3] = atoh (argv[i]) & mask;

    nwords = argc - 3;
    
    fb = fopen(argv[1], "r");

    if (fb == NULL)
        {
        printf ("problem opening input file %s\n", argv[1]);
        perror ("bfind");
        return (1);
        }

                                    // loop over mk5b disk frames
    while (fread (data, sizeof(int), 2504, fb) == 2504)
        {
        if (data[0] != SYNC)        //tombstone on bad sync
            {
            printf ("invalid sync word read %8.8X at frame # %d, quitting!\n", data[0], nfr);
            for (i=0; i<16; i++)
                printf ("%x ", data[i]);
            printf ("\n");
            return (1);
            }

        for (i=4; i<2504-nwords; i++)
            {
            for (j=0; j<nwords; j++)
                if ((data[i+j] & mask) != word[j])
                    break;

            if (j == nwords)
                {
                printf ("found pattern starting at word# %d\n", nfr * 2500 + i);
                printf ("%x %x %x %x %x\n", data[i], data[i+1], data[i+2], data[i+3], data[i+4]);
                printf ("%x %x %x %x %x\n", data[i+5], data[i+6], data[i+7], data[i+8], data[i+9]);
                return (0);
                }
            }
        nfr++;
        }
    }



    unsigned int atoh (char *s)  // Converts up to 8 hex chars to binary.
                             // Stops on non-numeric characters.
                             // s points to null-terminated input string
                     
   {
   unsigned val;
   short int i,
             cval;
   char c;

   val = 0;
 
   for (i=0; i<8; i++)
      {
      c = *(s+i);
      if (c >= '0'  &&  c <= '9')
         cval = c - '0';
 
      else if (c >= 'a'  &&  c <= 'f')
         cval = c - 'a' + 10;
 
      else if (c >= 'A'  &&  c <= 'F')
         cval = c - 'A' + 10;
 
      else
         return (val);     // after decoding 0-7 chars
 
      val = (val << 4) + cval;
      }
      return (val);        // Decoded a full eight chars
   }
 


