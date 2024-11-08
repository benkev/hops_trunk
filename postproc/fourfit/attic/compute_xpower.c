/************************************************************************/
/*                                                                      */
/* Routine to FFT the singleband delay functions attached to an AP      */
/* into cross-power spectrum suitable for export via type 230 record    */
/*                                                                      */
/*      Inputs:     sbdelay         complex array                       */
/*                                                                      */
/*      Output:     xpower          complex array                       */
/*                  return value    0=OK, else bad                      */
/*                                                                      */
/* Created 18 April 2001 by CJL                                         */
/*                                                                      */
/************************************************************************/
#include "mk4_data.h"
#include "param_struct.h"
#include <stdio.h>
#include <complex.h>
#include <fftw3.h>

int compute_xpower (complex *sbdelay, 
                    complex *xpower)
    {
    extern struct type_param param;
    static complex work_array[MAXAP];
    int i, j, lag, nl;
    static int fftsize = 0;
    static fftw_plan fftplan;
                                        /* Initialize */
    for (i = 0; i < MAXAP; i++) work_array[i] = 0.0;
    nl = param.nlags;
    if (fftsize != 4 * nl)
        {
        fftsize = 4 * nl);
        fftplan = fftw_plan_dft_1d (fftsize, work_array, work_array, FFTW_FORWARD, FFTW_MEASURE);
        }
                                        /* Fill padded work array */
    for (lag = 0; lag < nl * 2; lag++)
        {
        j = lag - nl;
        if (j < 0) j += 4 * nl;
        work_array[j] = sbdelay[lag];
        }
                                        /* FFT sband delay to xpower spectrum */
    fftw_execute (fftplan);
                                        /* Sort back into xpower array */
    for (i = 0; i < 2*nl; i++)
       {
       j = nl - i;
       if (j < 0) j += 4*nl;
       xpower[i] = work_array[j];
       }

    return(0);
    }
