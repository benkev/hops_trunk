#include <math.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
    double a = 24110.54841;
    double b = 8640184.812866;
    double c = 0.093104;
    double d = -6.2e-06;
    double du, T, GMST, ha, hi;

    while (--argc >= 1) {

        du = atof(*++argv); /* days since JD 2451545.0 UT1 */
        T = du/36525.0;

        GMST = a + T * (b + T * (c + T * d));   /* seconds */
        GMST /= 3600.0;                         /* hours */

        ha = 24.0 * modf(GMST / 24.0, &hi);
        fprintf(stdout, "%s gmst %lg hrs = %d days %.9lf hrs\n",
            argv[0], GMST, (int)rint(hi), ha);

    }
    return(0);
}
