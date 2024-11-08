/************************************************************************/
/*									*/
/* Calculates the correction factor to be applied to a station-based	*/
/* coherence time based on the observing elevation.  The returned	*/
/* factor is what you have to multiply the zenith coherence time by to	*/
/* get the expected coherence time at the specified elevation.  To	*/
/* convert a measured coherence time to a zenith coherence time, one	*/
/* obviously would need to take the reciprocal of the returned number.	*/
/*									*/
/*	Inputs:		elevation	Degrees above horizon		*/
/*									*/
/*	Output:		return value	Ratio (<= 1.0) -1.0 ==> error	*/
/*									*/
/* Created November 30 1995 by CJL					*/
/*									*/
/************************************************************************/
#include <math.h>

float
elfactor (elevation)
float (elevation);
    {
    int lowind, highind;
    float frac, value;
					/* Lookup table every 5 degrees, from */
					/* program by R. Linfield, implementing */
					/* model by Treuhaft + Lanyi, Radio Science */
					/* vol.22, p251, assuming azimuth 45, */
					/* and "normal" tropospheric conditions. */
					/* Curve is only weakly dependent on latter, */
					/* and former is sensible approximation */
    static float facttab[] = {0.0, 0.242, 0.374, 0.474, 0.562,
			0.635, 0.698, 0.752, 0.799, 0.841, 0.875, 
			0.904, 0.931, 0.951, 0.969, 0.984, 0.993, 0.998, 1.000};

					/* Range-check the input */
    if ((elevation > 90.0) || (elevation <= 0.0)) 
	{
	msg ("Out of range elevation fed to elfactor (%f)", 2, elevation);
	return (-1.0);
	}
					/* Take care of trivial case */
    if (elevation == 90.0) return (1.0);
					/* Do a simple linear interpolation */
    lowind = floor ((double)elevation / 5.0);
    highind = lowind + 1.0;
    frac = elevation - 5.0*lowind;
    value = frac * facttab[highind] + (1.0 - frac) * facttab[lowind];

    return (value);
    }
