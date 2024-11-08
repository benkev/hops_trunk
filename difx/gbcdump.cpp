/*
 * This code fragment can be #include'd in mode.cpp of mpifxcorr
 * to dump out intermediate results for study.  It needs to be
 * mentioned in mode.h:
 *
 @@ -314,6 +314,8 @@
 +  void gbcdump(cf32 *fft, int nchan, int dsi);
 *
 * and used in mode.cpp, e.g:
 @@ -27,6 +27,8 @@
 +#include <gbcdump.cpp>
 @@ -804,6 +806,7 @@
 +            gbcdump(fftd, fftchannels, datastreamindex);
 */

void Mode::gbcdump(cf32 *fft, int nchan, int dsi)
{
  static FILE *fp[2];
  static char fn[256];
  static int cnt[2];
  static cf32 amp[2][1024];
  double  x;
  int ii;
  if (dsi > 1) return;
  if (!fp[dsi]) {
    sprintf(fn, "gbc-%d.txt", dsi);
    fp[dsi] = fopen(fn, "w");
    cnt[dsi] = 0;
  }
  for (ii = 0; ii < nchan; ii++) {
    amp[dsi][ii].re += fft[ii].re;
    amp[dsi][ii].im += fft[ii].im;
  }
  if ((++cnt[dsi] % 20000) == 19999) {
    for (ii = 0; ii < nchan; ii++) {
      x = sqrt(amp[dsi][ii].re*amp[dsi][ii].re +
               amp[dsi][ii].im*amp[dsi][ii].im);
      fprintf(fp[dsi], "%d %g %g %g\n",
        ii, amp[dsi][ii].re, amp[dsi][ii].im, x);
      amp[dsi][ii].re = amp[dsi][ii].im = 0;
    }
    fprintf(fp[dsi], "\n\n");
    fflush(fp[dsi]);
  }
}

