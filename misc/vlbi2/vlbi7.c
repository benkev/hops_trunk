#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

int process2(char *, char *, int, int,int,double, double, int);

int main(int argc, char *argv[])
{
    char filenamein1[256], filenamein2[256], buf[80];
    int proctime;
    int rev;
    int tbit;
    int i;
    int chann = -1;
    double delayns;
    double freqoff;

    proctime = 500;
    rev = 0;
    tbit = 1;
    delayns = 0;
    filenamein1[0]=filenamein2[0]=0;
    if(argc < 2) { 
      printf("vlbi6 file1 file2 -proctime proctime -del apriori_del_ns -frq lo_offset_Hz\n");
      return 0;
     }
    if (argc > 1)
        sscanf(argv[1], "%255s", filenamein1);
    if (argc > 2)
        sscanf(argv[2], "%255s", filenamein2);

    for(i=0;i<argc;i++){
    sscanf(argv[i], "%79s",buf);
    if(strstr(buf, "-proctime")) sscanf(argv[i+1],"%d",&proctime);
    if(strstr(buf, "-rev")) sscanf(argv[i+1],"%d",&rev);
    if(strstr(buf, "-2bit")) sscanf(argv[i+1],"%d",&tbit);
    if(strstr(buf, "-del")) sscanf(argv[i+1],"%lf",&delayns);
    if(strstr(buf, "-frq")) sscanf(argv[i+1],"%lf",&freqoff);
    if(strstr(buf, "-chan")) sscanf(argv[i+1],"%d",&chann);
     }
process2(filenamein1,filenamein2, proctime,rev,tbit,delayns,freqoff,chann);
return 0;
}

void xcorr2(unsigned char *, unsigned char *, double *, double *,  double, double,int,int);
void xcalc(double *, double *, int, double,int);
void xinit(void);
void Four(double *, double *, int);
void plot(double *, double *, double *,int, int, char *, char *,int,int,double,double*,double*,double,double);
double delaysrc(double *, double *, double, int, int);

#define PI 3.1415926536
#define VELC 299792458.0
#define NLAG  64   //original
//#define NLAG  128
//#define NLAG  512
//#define NLAG  1024
//#define NLAG  64
#define NSIZ NLAG*4
#define NSIZ2 NLAG*2
#define NCHAN 16

static unsigned char vblock[625];
static unsigned char vblock2[625];
static float cc[65536];
static double xspecr[16*NSIZ2], xspeca[16*NSIZ2],xamp[16*NSIZ2],xpha[16*NSIZ2],delp[16*NSIZ2];

int process2(char *filename1, char *filename2, int smode, int rev,int tbit,double delayns, double freqoff, int chann)
{
    int i, j, k, iseg, iiseg, isec;
    int chan,err1,err2;
    unsigned int v24, mag, sync, dd[2504], dd1[2504];       // for speedy sscanf
    int ierr, num, nchan;
    char p;
    char buf[32768];
    double secs, phase, fsec,mxdel, delay;
    double bstate[32],bcount[32];
    FILE *file1, *file2;
    int ichan,frame;

 frame = 0;
 ierr = sync = 0;
 nchan = NCHAN;
 if(chann >= 0) nchan = 1;
    xinit();
    k = 0;
    iseg = 0;
    if ((file1 = fopen(filename1, "r")) == NULL)
        return 0;
    if ((file2 = fopen(filename2, "r")) == NULL)
        return 0;
    iiseg = 1;
    err1 = err2 = 1;
    num = 0; 
    while (iiseg) {
	    err1=fread(dd, sizeof(dd), 1, file1);
//	    printf("dd %x %x %x\n",dd[0],dd[1],dd[2]);
            err2=fread(dd1, sizeof(dd1), 1, file2);
          if(err1 && err2){
		  sync=dd[0];
		  frame = dd[1]&0x7fff;
		  sprintf(buf,"MJD ");
		  for(j=0;j<3;j++){ p=((dd[2] >> (28 - j*4)) & 0xf); sprintf(&buf[4+j],"%d",p); }
		  sprintf(&buf[7]," ");
		  for(j=3;j<8;j++){ p=((dd[2] >> (28 - j*4)) & 0xf); sprintf(&buf[5+j],"%d",p); }
		  sprintf(&buf[13],".");
		  for(j=0;j<4;j++){ p=((dd[3] >> (28 - j*4)) & 0xf); sprintf(&buf[14+j],"%d",p); }
		  sprintf(&buf[18]," secs frame %d\n",frame);
		  sscanf(buf,"%*s %*s %lf %*s %*s %lf",&secs,&fsec);
                  isec = secs;
		  secs = isec + fsec*2500.0/64e6;
		  if(!(frame%100)) printf("secs %12.6f frame %d\n",secs,frame);
//                  printf("err1 %d err2 %d\n",err1,err2);
                  frame = dd1[1]&0x7fff;
                  sprintf(buf,"MJD ");
                  for(j=0;j<3;j++){ p=((dd1[2] >> (28 - j*4)) & 0xf); sprintf(&buf[4+j],"%d",p); }
                  sprintf(&buf[7]," ");
                  for(j=3;j<8;j++){ p=((dd1[2] >> (28 - j*4)) & 0xf); sprintf(&buf[5+j],"%d",p); }
                  sprintf(&buf[13],".");
                  for(j=0;j<4;j++){ p=((dd1[3] >> (28 - j*4)) & 0xf); sprintf(&buf[14+j],"%d",p); }
                  sprintf(&buf[18]," secs frame %d\n",frame);
                  sscanf(buf,"%*s %*s %lf %*s %*s %lf",&secs,&fsec);
                  isec = secs;
                  secs = isec + fsec*2500.0/64e6;
                  if(!(frame%100)) printf("secs %12.6f frame %d\n",secs,frame);
//                  printf("err1 %d err2 %d\n",err1,err2);
		  }
        for(ichan=0;ichan<nchan;ichan++){
                chan=ichan;
               if(chann >= 0) chan = chann;
                      bstate[ichan] = bcount[ichan]=0;
                      bstate[ichan+16] = bcount[ichan+16]=0;
			k = 0;
			 j = v24 = 0;
                  for(i=0;i<2500;i++){
                           v24 += ((dd[4+i] >> (chan)*2) & 1) << (7-j); 
                           bcount[ichan]++;
                           mag = ((dd[4+i] >> (chan)*2) & 3);
                           if(mag == 3 || mag == 0) bstate[ichan]++;
			   j++;
			   if(j==8) {
                            vblock[k++] = v24;
//			    printf("k %d v24 %d\n",k,v24);
			    j = v24 = 0;
			  }
                         }
			k = 0;
			 j = v24 = 0;
                  for(i=0;i<2500;i++){
                           v24 += ((dd1[4+i] >> (chan)*2) & 1) << (7-j);         
                           bcount[ichan+16]++;
                           mag = ((dd1[4+i] >> (chan)*2) & 3);
                           if(mag == 3 || mag == 0) bstate[ichan+16]++;
                           j++;
                           if(j==8) {
                            vblock2[k++] = v24;
//                          printf("k %d v24 %d\n",k,v24);
                            j = v24 = 0;
                          }
                         }
         if(!ierr){
            delay = delayns;
            phase = freqoff * 2.0 * PI * secs;
// printf("phase/(2.0*PI) %f\n",phase/(2.0*PI));
            if (num == 0 && ichan==0){
                for (i = 0; i < NSIZ2; i++)
		for(j=0;j<NCHAN;j++)
                    xspecr[j*NSIZ2+i] = xspeca[j*NSIZ2+i] = 0.0;
	    }
            xcorr2(vblock, vblock2, xspecr, xspeca, delay, phase,ichan,tbit);
	 }
         else {
	   printf("missing or incorrect time %f\n", secs);
	      }
	 }
           if(err1 == 0 || err2 == 0)
           printf("end of data encountered at frame %d\n",frame);
    else {
	 if(sync != 0xabaddeed)
	  printf("ERROR secs %f\n",secs);
	 }
         num++;
        if (num == smode || err1 == 0) iiseg=0;
    }
    for(ichan=0;ichan<nchan;ichan++) xcalc(xspecr, xspeca, num, secs,ichan);
                mxdel = delaysrc(xspecr, xspeca, delayns, num, NCHAN);
                plot(xspecr, xspeca, delp, num, NCHAN,filename1,filename2,rev,tbit,delayns,bstate,bcount,mxdel,freqoff);
    
    fclose(file1);
    fclose(file2);
    return 0;
}


void xinit(void)
{
    int i, j;
    double sum;
    for (i = 0; i < 65536; i++) {
        sum = 0;
        for (j = 0; j < 16; j++)
            if ((i >> j) & 1)
                sum++;
        cc[i] = (8 - sum) / 8.0;
    }

}

void xcorr2(unsigned char vblock[], unsigned char vblock2[], double xspecr[], double xspeca[], double dsteer, double phsteer,int chan, int tbit)
// vblock are data in 8-bit bytes,  dsteer in ns
{
    int sft, i, j, k, m, m1, lenn, dsft, nn, mm, i1, i2;
    double sum1, sum2, fracsft, re, am, ph, aasin, aacos;
    double aac, aas;
    unsigned int ch, ch1, ch2;
    unsigned int da[NSIZ], da2[NSIZ];
    double aut[NSIZ], px[NSIZ * 2], py[NSIZ * 2];

//    lenn = 512;
    lenn = 624;
    if(tbit) lenn=312;
    if (dsteer > 0.0)
        dsft = dsteer / 15.625 + 0.5; // nearest integer shift
    else
        dsft = dsteer / 15.625 - 0.5;  // 32 MHz 64 ms/s sample
    if (dsft < 0)
        nn = 1 - dsft / 32;     // number of 32 bit shifts to make positive
    else
        nn = 0;
    nn = nn * 32;               // 
    fracsft = dsteer / 15.625 - dsft; // fractional delay shift needed to steer delay

// printf("cc 255 %f\n",cc[255]);
    for (i = 0; i < NSIZ; i++)
        px[i] = py[i] = 0.0;
    for (i = 0; i < lenn; i += 4)
        da[(i >> 2)] = (vblock[i] << 24) | (vblock[i + 1] << 16) | (vblock[i + 2] << 8) | vblock[i + 3];
    for (i = 0; i < lenn; i += 4)
        da2[(i >> 2)] = (vblock2[i] << 24) | (vblock2[i + 1] << 16) | (vblock2[i + 2] << 8) | vblock2[i + 3];
    mm = (NLAG / 2 + nn) / 32;  // offset for first bit stream
    for (sft = 0; sft < NLAG; sft++) {
        aut[sft] = 0.0;
        sum1 = sum2 = 0.0;
        m = sft + dsft + nn;
        k = m / 32;             // number of 32 bit int shifts
        j = m - k * 32;
        i2 = (lenn / 4) - mm;
        if(!j)
          m = (lenn / 4) - k;
        else
          m = (lenn / 4) - k - 1;
        if (m < i2) i2 = m;
//        i1 = 2 - mm;     // for SRT blank first 8 bytes
        i1 = 0 - mm;
        if (2 - k > i1) i1 = 2 - k;
        for (i = i1; i < i2; i++) { 
            m = i + k;
            m1 = m + 1;
            ch1 = da[i + mm];
            if(!j)
               ch2 = (da2[m] << j);
            else 
               ch2 = (da2[m] << j) | (da2[m1] >> (32 - j));
            ch = ch1 ^ ch2;
// if(sft==64)    printf("i %d m %d ch ch1 ch2 %x %x %x da %x %x\n",i,m,ch,ch1,ch2,da[i+2],da2[m]);
            sum1 += cc[(ch >> 16)] + cc[(ch & 0xffff)];
        }
        sum2 = 2 * (i2 - i1);
        aut[sft] = sin(PI * 0.5 * sum1 / sum2);
        if (sft >= NLAG / 2)
            px[sft - NLAG / 2] = aut[sft];
        else
            px[NSIZ + sft - NLAG / 2] = aut[sft];
//   if(sft > NLAG/2 - 5 && sft < NLAG/2 + 5)printf("xcorr sft %d %f\n",sft,aut[sft]);
    }
    Four(px, py, NSIZ);         // FFT to cross-spectral function
    for (i = 0; i < NSIZ / 2; i++) {
        ph = 2.0 * PI * (double) i *fracsft / (NSIZ) + phsteer; // need () because of 4 in define
        ph += chan * dsteer * 1e-9 * 32e06 * 2.0 * PI;
        aacos = cos(ph);
        aasin = sin(ph);
        j=chan; 
	aac = aacos;
	aas = aasin;
        re = px[i] * aac - py[i] * aas;
        am = px[i] * aas + py[i] * aac;
        xspecr[j*NSIZ2+i] += re;
        xspeca[j*NSIZ2+i] += am;
    }
}

void xcalc(double xspecr[], double xspeca[], int num, double secs, int chan)
{
    int i, j, k;
    double amp, sumr, sumi, a, phase, delay, maxamp, maxdelay, maxphase;
    double px[NSIZ * 2], py[NSIZ * 2], y[3];

    maxamp = -1e99;
    maxdelay = 0;
    j=chan; 
    for (i = NSIZ / 2; i < NSIZ * 2; i++)
        px[i] = py[i] = 0;      // zero out negative frequencies
    for (i = 0; i < NSIZ / 2; i++) {
        px[i] = xspecr[j*NSIZ2+i] / num;
        py[i] = xspeca[j*NSIZ2+i] / num;
// printf("i %3d amp %10.4f\n",i,sqrt(px[i]*px[i]+py[i]*py[i]));
    }
    Four(px, py, NSIZ * 2);
    sumi = -1e99;
    k = 0;
    for (i = 0; i < NSIZ * 2; i++) {
        sumr = px[i] * px[i] + py[i] * py[i];
        if(i < NSIZ)
           delay = 125.0 * 0.5 * i;
        else
           delay = 125.0 * 0.5 * (i - NSIZ * 2);
        if (sumr > sumi && fabs(delay) <= 8000.0) {
            sumi = sumr;
            k = i;
        }
    }
    phase = atan2(py[k], px[k]) * 180.0 / PI;
    y[1] = px[k] * px[k] + py[k] * py[k];
    i = k + 1;
    if (i >= NSIZ * 2)
        i -= NSIZ * 2;
    y[2] = px[i] * px[i] + py[i] * py[i];
    i = k - 1;
    if (i < 0)
        i += NSIZ * 2;
    y[0] = px[i] * px[i] + py[i] * py[i];
    a = (2.0 * (2.0 * y[1] - y[0] - y[2]));
    if (k > NSIZ)
        k -= NSIZ * 2;
    if (a != 0.0)
        a = (y[2] - y[0]) / a;
    a = 0;
    delay = 15.625 * 0.5 * (k + a); // delay interpolation for 32 MHz BW
    phase -= a * 45.0;          // phase interpolation
    if (phase > 180.0)
        phase -= 360.0;
    if (phase < -180.0)
        phase += 360.0;
    delay = -delay;             // to make delay positive for late arrival at remote site
    amp = sqrt(y[1] + (y[2] - y[0]) * a / 4.0) / ((double) (NSIZ / 2));
    if(amp>maxamp){maxamp=amp; maxdelay=delay; maxphase=phase; }
    amp=maxamp; delay=maxdelay; phase=maxphase;
    printf("%10.6f amp %7.4f delay %4.0f ns k %d chan %d\n",secs,amp,delay,k,chan);

}


double delaysrc(double xspecr[], double xspeca[], double delayns, int num, int chan)
{
  int ichan,i,j;
  double max,maxdel,suma,sumr,ph,del,phase,amp,aa,delw;
  max = -1e99;
  maxdel = 0;
  aa = NSIZ2;
  delw = 512.0*NLAG/64.0;

   for(ichan=1;ichan<chan;ichan++){
   for(i=0;i<NSIZ2;i++){
      xamp[ichan*NSIZ2+i] = sqrt(xspecr[ichan*NSIZ2+i]*xspecr[ichan*NSIZ2+i]
		+xspeca[ichan*NSIZ2+i]*xspeca[ichan*NSIZ2+i]);
      xpha[ichan*NSIZ2+i] = atan2(xspeca[ichan*NSIZ2+i],xspecr[ichan*NSIZ2+i]);
        }}
   j = 0;
   for(j=0;j<NSIZ2*16;j++){
        del = (j-NSIZ2*8)*delw/(NSIZ2*8);
        sumr = suma = 0;
	for(ichan=1;ichan<chan;ichan++){
	for(i=0;i<NSIZ2;i++){
        amp = xamp[ichan*NSIZ2+i];
        phase = xpha[ichan*NSIZ2+i];
        ph = del*1e-9*(ichan*NSIZ2+i)*2.0*PI*32e06/aa;
        sumr += amp * cos(phase - ph);
        suma += amp * sin(phase - ph); 
        }
      }
        amp = sqrt(sumr*sumr+suma*suma);
        if(amp > max) {max = amp; maxdel = del;}
        delp[j] = amp/(num*NSIZ2*(chan-1));
      }
        for(ichan=1;ichan<chan;ichan++){
        for(i=0;i<NSIZ2;i++){
        amp = xamp[ichan*NSIZ2+i];
        phase = xpha[ichan*NSIZ2+i];
        ph = maxdel*1e-9*(ichan*NSIZ2+i)*2.0*PI*32e06/aa;
        xspecr[ichan*NSIZ2+i] = amp * cos(phase - ph);
        xspeca[ichan*NSIZ2+i] = amp * sin(phase - ph);
        }
      }
    printf("maxdel %f maxamp %f SNR %5.1f\n",delayns-maxdel,max/(num*NSIZ2*(chan-1)),max/(num*NSIZ2*(chan-1))*sqrt(num*2500.0*(chan-1)));
    return maxdel;
}

void plot(double xspecr[], double xspeca[], double delp[], int num, int chan,char *filename1,char *filename2,int rev,int tbit,double delayns,double bstate[],double bcount[],double maxdel,double freqoff)
{
    int x,y,i,iter,ichan;
    double xx1,xx2,yy1,yy2,h,s,b,xoff,yoff;
    double amp,phase;
    double avamp,namp,scale,mxd,snr;
    time_t now;
    FILE *file3;

       if ((file3 = fopen("dd1.pos", "w")) == NULL) {
            printf(" output error\n");
            return;
       }

        fprintf(file3, "%%!PS-Adobe-\n%c%cBoundingBox:  0 0 612 792\n%c%cEndProlog\n", '%', '%', '%', '%');
        fprintf(file3, "1 setlinewidth\n");
        fprintf(file3, "/Times-Roman findfont\n 8 scalefont\n setfont\n");
      xoff = 40; yoff = 100;

      xx1 = xoff; xx2=xx1+500.0;
      yy1 = yoff; yy2=yy1+300.0+150.0;
      h=s=b=0;
      fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n %2.1f %2.1f %2.1f sethsbcolor stroke\n"
                    ,xx1, yy1, xx2, yy1, h, s, b);
      fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n %2.1f %2.1f %2.1f sethsbcolor stroke\n"
                    ,xx1, yy1+150.0, xx2, yy1+150.0, h, s, b);
      fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n %2.1f %2.1f %2.1f sethsbcolor stroke\n"
                    ,xx1, yy1+300.0, xx2, yy1+300.0, h, s, b);
      fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n %2.1f %2.1f %2.1f sethsbcolor stroke\n"
                    ,xx1, yy2, xx2, yy2, h, s, b);
      fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n %2.1f %2.1f %2.1f sethsbcolor stroke\n"
                    ,xx1, yy1, xx1, yy2, h, s, b);
      fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n %2.1f %2.1f %2.1f sethsbcolor stroke\n"
                    ,xx2, yy1, xx2, yy2, h, s, b);

	x = xoff; y = 100;
        fprintf(file3, "/Times-Roman findfont\n 10 scalefont\n setfont\n");
        namp=avamp=0.0;
        mxd = -1e99;
        for(ichan=0;ichan<chan;ichan++){
        for(i=0;i<NSIZ2;i++){
                h=s=b=0;
        amp = sqrt(xspecr[ichan*NSIZ2+i]*xspecr[ichan*NSIZ2+i]
                        +xspeca[ichan*NSIZ2+i]*xspeca[ichan*NSIZ2+i]);
        phase = atan2(xspeca[ichan*NSIZ2+i],xspecr[ichan*NSIZ2+i]);
        if(delp[i+ichan*NSIZ2] > mxd) mxd = delp[i+ichan*NSIZ2];
        amp = amp/num;
        avamp += amp;
        namp++;
        }}
        scale = 0.5/(avamp/namp);
	for(iter=0;iter<3;iter++){
	for(ichan=0;ichan<chan;ichan++){
        namp=avamp=0.0;
	for(i=0;i<NSIZ2;i++){
                h=s=b=0;
	amp = sqrt(xspecr[ichan*NSIZ2+i]*xspecr[ichan*NSIZ2+i]
			+xspeca[ichan*NSIZ2+i]*xspeca[ichan*NSIZ2+i]);
	phase = atan2(xspeca[ichan*NSIZ2+i],xspecr[ichan*NSIZ2+i]);
	amp = amp/num;
        avamp += amp;
        namp++;
        xx2=x+(i+ichan*NSIZ2)*500.0/(NSIZ2*16.0);
//        if(iter==0) yy2 = y+amp*75.0*100.0;
        if(iter==0) yy2 = y+amp*75.0*scale;
	if(iter == 1) yy2 = phase * 75.0 / PI  + y + 150.0 + 75.0;
	if(iter == 2) yy2 = y + delp[i+ichan*NSIZ2]*150.0/mxd + 300.0;
	if(iter == 0 && yy2 > y+150.0) yy2=y+150.0;
	if(i==0){xx1=xx2; yy1=yy2;}
	         h=s=b=0;
                  s=b=1;
                  h = ichan*0.67/chan;
                  if (h > 0.67) h = 0.67;
		  if (h < 0.0) h = 0.0;
		 if(iter) h=s=b=0;
	if(!iter || (iter && (i>10 && i<NSIZ2-10)&& fabs(yy2-yy1)<37.5) || iter == 2)
          fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n %2.1f %2.1f %2.1f sethsbcolor stroke\n"
                    ,xx1, yy1, xx2, yy2, h, s, b);
                xx1 = xx2; yy1 = yy2;
		}
	xx1=xx2=x+(ichan*NSIZ2)*500.0/(NSIZ2*16.0);
	yy1=yoff; yy2=yoff+300.0;
	fprintf(file3, "0 0 0 sethsbcolor\n");
	fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n 0 0 0 sethsbcolor stroke\n"
			,xx1, yy1, xx2, yy2);
	fprintf(file3,"%5.1f %5.1f moveto\n (%2d) show\n",xx1+10,yoff-10,ichan);
	fprintf(file3,"%5.1f %5.1f moveto\n (%5.3f) show\n",xx1+10,yoff-35,avamp/namp);
        if(iter < 2){
        if(tbit && ichan==0) fprintf(file3,"%5.1f %5.1f moveto\n (bstate%d) show\n",10.0,40.0-iter*10,iter);
        if(tbit) fprintf(file3,"%5.1f %5.1f moveto\n (%4.2f) show\n",xx1+10,40.0-iter*10,2.0*bstate[ichan+iter*16]/bcount[ichan+iter*16]);
        }
	       }
               }
        /* write bottom of page */
        x = 100; y = 60.0;
        now = time(NULL);
	fprintf(file3, "0 0 0 sethsbcolor\n");
        fprintf(file3, "/Times-Roman findfont\n 10 scalefont\n setfont\n");
	fprintf(file3,"50 52 moveto\n (file1: %s file2: %s delay_ns %5.2f frq %7.2f) show\n",filename1,filename2,delayns-maxdel,freqoff);
        snr = mxd*sqrt(num*2500.0*(chan-1));
	fprintf(file3,"490 560 moveto\n (corr %5.1e SNR  %5.1f) show\n",mxd,snr);
        if(!rev) fprintf(file3,"220 80 moveto\n (DBE channel number) show\n");
        else fprintf(file3,"220 80 moveto\n (DBE channel number reversed) show\n");
	fprintf(file3,"30 150 moveto\n 90 rotate\n (amplitude) show\n -90 rotate\n");
	fprintf(file3,"30 300 moveto\n 90 rotate\n (phase) show\n -90 rotate\n");
	fprintf(file3,"30 400 moveto\n 90 rotate\n (amp vs delay %4.0f to %4.0f ns) show\n -90 rotate\n",-NLAG*1000.0/128.0,NLAG*1000.0/128.0);
        fprintf(file3, "/Times-Roman findfont\n 8 scalefont\n setfont\n");
        fprintf(file3,"%d %d moveto\n (%s) show\n",x+20+300,20,ctime(&now));
        fprintf(file3, "showpage\n%c%cTrailer\n", '%', '%');
        fclose(file3);
}


#define PI 3.1415926536


void
Four (double *fft_r, double *fft_i, int nn)
{
  int n, mmax, m, j, istep, i, ii, jj;
  double wtemp, wr, wpr, wpi, wi, theta;
  double tempr, tempi;

  n = nn << 1;
  j = 1;
  for (i = 1; i < n; i += 2)
    {
      if (j > i)
	{
	  jj = (j - 1) / 2;
	  ii = (i - 1) / 2;
	  tempr = fft_r[jj];
	  tempi = fft_i[jj];
	  fft_r[jj] = fft_r[ii];
	  fft_i[jj] = fft_i[ii];
	  fft_r[ii] = tempr;
	  fft_i[ii] = tempi;
	}
      m = n >> 1;
      while (m >= 2 && j > m)
	{
	  j -= m;
	  m >>= 1;
	}
      j += m;
    }
  mmax = 2;
  while (n > mmax)
    {
      istep = mmax << 1;

      theta = -(6.28318530717959 / mmax);
      wtemp = sin (0.5 * theta);
      wpr = -2.0 * wtemp * wtemp;
      wpi = sin (theta);
      wr = 1.0;
      wi = 0.0;
      for (m = 1; m < mmax; m += 2)
	{
	  for (i = m; i <= n; i += istep)
	    {
	      j = i + mmax;
	      jj = (j - 1) / 2;
	      ii = (i - 1) / 2;
	      tempr = wr * fft_r[jj] - wi * fft_i[jj];
	      tempi = wr * fft_i[jj] + wi * fft_r[jj];
	      fft_r[jj] = fft_r[ii] - tempr;
	      fft_i[jj] = fft_i[ii] - tempi;
	      fft_r[ii] += tempr;
	      fft_i[ii] += tempi;
	    }
	  wr = (wtemp = wr) * wpr - wi * wpi + wr;
	  wi = wi * wpr + wtemp * wpi + wi;
	}
      mmax = istep;
    }

}
