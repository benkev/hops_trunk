/*************************/
/* Converts from deg. min */
/* sec to decimal deg.    */
/*************************/

double dms_to_deg(x)
double x;
   {
   int deg,min;
   double sec,dms;

   deg = (int)(x/1.E4);
   min = (int)((x- 1.E4 * deg)/ 1.E2);
   sec = x - 1.E4 * deg - min * 1.E2;
   dms = deg + min / 60. + sec / 3600.;
   return(dms);

   }

