/* swab.c - byte reversing procedures to be used for conversion
 * between Big-Endian and Small-Endian formats
/*
 * Created by David E. Flynt, 8/8/94
 * modified by rjc, 2005.9.2, to optimize for speed, by
 * eliminating loops. Probably should be implemented as macro's someday.
 */

#include <stdio.h>

/* short_reverse: reverse the order of bytes in a short integer */
void short_reverse (short *i)
    {
    char *p1 = (char *)i, *p2, temp;
    p2 = p1 + 1;

    temp = *p1;
    *p1  = *p2;
    *p2  = temp;
    }

/* unsig_reverse: reverse the order of bytes in a unsigned short integer */
void unsig_reverse (unsigned short *i)
    {
    char *p1 = (char *)i, *p2, temp;
    p2 = p1 + 1;

    temp = *p1;
    *p1  = *p2;
    *p2  = temp;
    }

/* int_reverse: reverse the order of bytes in an integer */
void int_reverse (int *j)
    {
    char *p = (char *)j, temp;		/* p points to the first byte of j */
  
    temp = p[0];
    p[0] = p[3];
    p[3] = temp;

    temp = p[1];
    p[1] = p[2];
    p[2] = temp;
    }

/* long_reverse: reverse the order of bytes in a 64-bit integer */
void long_reverse (long *j)
    {
    char *p = (char *)j, temp;		/* p points to the first byte of j */
  
    temp = p[0];
    p[0] = p[7];
    p[7] = temp;
  
    temp = p[1];
    p[1] = p[6];
    p[6] = temp;
  
    temp = p[2];
    p[2] = p[5];
    p[5] = temp;
  
    temp = p[3];
    p[3] = p[4];
    p[4] = temp;
    }

/* float_reverse: reverse the order of bytes in a float */
void float_reverse (float *j)
    {
    char *p = (char *)j, temp;		/* p points to the first byte of j */
  
    temp = p[0];
    p[0] = p[3];
    p[3] = temp;

    temp = p[1];
    p[1] = p[2];
    p[2] = temp;
    }

/* double_reverse: reverse the order of bytes in a double */
void double_reverse (double *j)
    {
    char *p = (char *)j, temp;		/* p points to the first byte of j */
  
    temp = p[0];
    p[0] = p[7];
    p[7] = temp;
  
    temp = p[1];
    p[1] = p[6];
    p[6] = temp;
  
    temp = p[2];
    p[2] = p[5];
    p[5] = temp;
  
    temp = p[3];
    p[3] = p[4];
    p[4] = temp;
    }
