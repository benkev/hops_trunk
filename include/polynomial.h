#include "mk4_sizes.h"
#define MAX_INTERVALS 200

struct poly_str              /* structure to contain spline poly's for one interval */
    {
    double rot_interval[2];           /* rot start and stop times for these splines */
    double delay_spline [6];          /* contains one delay spline for all channels
                                              units are seconds / sec^n             */
    double phase_spline [MAX_POLY_CHAN] [6];  /* contains one phase spline per channel
                                              units are rotations / sec^n           */
    };


struct poly_array             /* structure to contain all of the spline polynomials */
    {
    long  int num_intervals;     /* total number of intervals containing valid data */
    struct poly_str interval_arrays [MAX_INTERVALS];      /* array of interval data */
    };

