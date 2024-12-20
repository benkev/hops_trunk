#include <math.h>
#include "nrutil.h"
#include "nr.h"

float pythag(float a, float b)
{
	float absa,absb;
	absa=fabs(a);
	absb=fabs(b);
	if (absa > absb) return absa*sqrt(1.0+SQR(absb/absa));
	else return (absb == 0.0 ? 0.0 : absb*sqrt(1.0+SQR(absa/absb)));
}
/* (C) Copr. 1986-92 Numerical Recipes Software *1(."<,. */
