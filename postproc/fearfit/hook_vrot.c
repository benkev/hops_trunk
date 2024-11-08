/*
 * $Id: hook_vrot.c 1526 2017-01-18 15:22:59Z gbc $
 *
 * Hook which allows experimentation on optimize closure
 */

#define THIS_DID_NOT_WORK 0
#if THIS_DID_NOT_WORK

/* no need to include original function */
#warning "vrot hacking enabled -- see hook_vrot.c"

#include <math.h>
#include <complex.h>
#include "mk4_data.h"
#include "param_struct.h"
#include "pass_struct.h"


complex vrot (int ap, double dr, double mbd, int fr, int sb, struct type_pass* pass)
    {
    double theta, ambig;
    extern struct type_param param;
    extern struct type_status status;
                                        // theta is in rotations

                                        /* fringe rate * time from central epoch */
    theta = pass->pass_data[fr].frequency * dr 
                * (param.acc_period * (ap + 0.5) + status.epoch_err[fr]);

    if (param.mbd_anchor == SBD)        // force mbd to stay near sbd
        {
        ambig = 1.0 / status.freq_space;
        mbd += ambig * floor ((status.sbd_max - mbd) / ambig);
        }
                                        // Residual mbd effect at this freq
    theta += mbd * (pass->pass_data[fr].frequency - param.ref_freq);
                                        // Effect due to offset of lag where max lies
    theta += (param.nlags - status.max_delchan) * 0.125 * sb;
   
    if (pass->control.optimize_closure) // sacrifice mbd fit for less-noisy closure?
        theta += 0.125 * mbd * sb / status.sbd_sep;
       
    else                                /* effect of non-integral sbd iff SSB
                                         * correct phase to dc edge, based on sb delay */
        theta += 0.125 * status.sbd_max * sb / status.sbd_sep;
        
    theta *= (-2.0 * M_PI);             // convert to radians

    return (cexp(I * theta));              // return unit phasor
    }
#else /* THIS_DID_NOT_WORK */
#define orig_vrot vrot
#include "vrot.c"
#endif /* THIS_DID_NOT_WORK */

/*
 * eof
 */
