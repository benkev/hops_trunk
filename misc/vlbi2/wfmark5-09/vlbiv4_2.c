#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

void autocorr(unsigned char *, float *, int);
void xcorr2(unsigned char *, unsigned char *, double *, double *, double, double, double,int,int,int,int);
void xcalc(double *, double *, int, double, double,double,int,int);
void xinit(void);
void getddate(char *, int, int);
double tosecs(int, int, int, int, int);
double gst(double);
void Four(double *, double *, int);
void plot(double *, double *, int, int, char *, char *,int,int);
void WriteData(double *, double *, char *);

#define NFRQSER 32
#define PI 3.1415926536
#define VELC 299792458.0
//#define NLAG  64
#define NLAG  1024
#define NSIZ NLAG*4
#define NSIZ2 NLAG*2
#define NCHAN 16

static unsigned char vblock[625];
static unsigned char vblock2[625];
static float cc[65536];
static double xspecr[NFRQSER*NSIZ2], xspeca[NFRQSER*NSIZ2];


int process2(char *filename1, char *filename2, char *filenameout, int smode, double swidth, double fwidth,int rev,int tbit)
{
    int i, j, k, bn, ist, iseg, iiseg, nist, isec;
    int ii,chan,err1,err2;
    unsigned int kk, v24, dd[2504];       // for speedy sscanf
    int yr, day, hr, mn, sec, ierr, tmode, num, sun, nbins, nchan, sync, syncp;
    char p;
    char buf[32768];
    double secs, av, secc, sec0, sec1, phase, delay0, delayrate, dgha, fsec, freq, secp;
    double gpsLat, gpsLon, gpsHgt, x1, y1, z1, x2, y2, z2, delay, ra, dec, bx, by, bz, gha, ha, b_r, b_east,
        b_north, az, el, sx, sy, sz;
    time_t now;
    FILE *file1, *file2;
    int headercounter;
    char readcount;
    int temp1, temp2, get0, get1,ichan,frame;
    int cnt =0;

 if(filename2[0]==0){ nchan=1; nist=1;}
 else {nchan = NCHAN; nist=2;}
 for(ichan=0;ichan<NCHAN;ichan++){
    headercounter = 0;
    chan=ichan;
    xinit();
    get0 = 1;
    get1 = 1;
    //    smode = 10000;
    sun = 0;
    tmode = 0;                  // set to zero for operation 1=test
    //    swidth = 3000;
    freq = 1420.6e06;           // nominal frequency
    if (tmode)
        freq = 80e06;           // for test

    nbins=1;
    if(nbins > NFRQSER) nbins = NFRQSER;       // limit to prevent array overflow
    k = 0;
    iseg = 0;
    if ((file1 = fopen(filename1, "r")) == NULL)
        return 0;
    if(filename2[0]){
    if ((file2 = fopen(filename2, "r")) == NULL)
        return 0;
    }
    iiseg = 1;
    err1 = err2 = 1;
    num = 0; secp = 0;
    while (iiseg) {
        for (ist = 0; ist < nist; ist++) {
	readcount=0;
	  if (ist == 0 && get0){
	    err1=fread(dd, sizeof(dd), 1, file1);
//	    printf("dd %x %x %x\n",dd[0],dd[1],dd[2]);
	    readcount++;
	  }
	  if (ist ==1 && get1){
            err2=fread(dd, sizeof(dd), 1, file2);
	    readcount++;
	  }
          if(readcount && err1 && err2){
            if ((ist == 0 && get0) || (ist == 1 && get1)) {
//	      printf("station %d ", ist);
//		  if (ist == 0) printf("file: %s ", filename1);
//		  if (ist == 1) printf("file: %s ", filename2);
		  // get the frequency
		  sync=dd[0];
		  frame = dd[1]&0x7fff;
		  freq=1e9;
		  sprintf(buf,"MJD ");
		  for(j=0;j<3;j++){ p=((dd[2] >> (28 - j*4)) & 0xf); sprintf(&buf[4+j],"%d",p); }
		  sprintf(&buf[7]," ");
		  for(j=3;j<8;j++){ p=((dd[2] >> (28 - j*4)) & 0xf); sprintf(&buf[5+j],"%d",p); }
		  sprintf(&buf[13],".");
		  for(j=0;j<4;j++){ p=((dd[3] >> (28 - j*4)) & 0xf); sprintf(&buf[14+j],"%d",p); }
		  sprintf(&buf[18]," secs frame %d\n",frame);
//		  if(strstr(buf,"817")) printf("%s",buf);
//		  else printf("%s ERROR ",buf);
		  //printf("%s",buf);
		  sscanf(buf,"%*s %*s %lf %*s %*s %lf",&secs,&fsec);
                  isec = secs;
		  secs = isec + fsec*5000.0/64e6;
//		  printf("secs %f ns\n",secs*1e09);
//                  printf("err1 %d err2 %d\n",err1,err2);
		  }
		  headercounter++;
                    yr=2006; day=81; hr=mn=sec=0; fsec=0;
//                    secs = tosecs(yr, day, hr, mn, sec) + fsec;
                    if (ist == 0)
                        sec0 = secs;
                    if (ist == 1)
                        sec1 = secs;
                    ierr = 0;
                    if (ist == 1){
                    get0 = get1 = 1;
                    if (sec0 > sec1) {ierr = 1; get0 = 0; }      // read more file2
                    if (sec0 < sec1) {ierr = 1; get1 = 0; }      // read more file1
                    }
//                  printf("%d:%d:%d:%d:%f bn %d ist %d\n",yr,day,hr,mn,secc,bn,ist);
			k = 0;
			 j = v24 = 0;
                        if(!tbit){
			 for(i=0;i<5000;i++){
                           if(!rev){
			   if(i%2==1) v24 += ((dd[4+i/2] >> (16+chan)) & 1) << (7-j);
			   else v24 += ((dd[4+i/2] >> (chan)) & 1) << (7-j);
                           }
                           else{
			   if(i%2==1) v24 += ((dd[4+i/2] >> (31-chan)) & 1) << (7-j);
			   else v24 += ((dd[4+i/2] >> (15-chan)) & 1) << (7-j);
                           }
			   j++;
			   if(j==8) {
                            if(ist==0)vblock[k++] = v24;
                            if(ist==1)vblock2[k++] = v24;
//			    printf("k %d v24 %d\n",k,v24);
			    j = v24 = 0;
			  }
			  }
                          }
                        else {
                  for(i=0;i<2500;i++){
         
                           if(!rev) v24 += ((dd[4+i] >> (chan)*2) & 1) << (7-j); 
                           else v24 += ((dd[4+i] >> (15-chan)*2) & 1) << (7-j); 
			   j++;
			   if(j==8) {
                            if(ist==0)vblock[k++] = v24;
                            if(ist==1)vblock2[k++] = v24;
//			    printf("k %d v24 %d\n",k,v24);
			    j = v24 = 0;
			  }
                         }

                       }
            } else iiseg = 0;
        }
	 if(nist==2){
         if(!ierr){
            delay = 0; // update delay between seconds
//	    delay=10.0;
            phase = 0;
            if (num == 0 && ichan==0){
                for (i = 0; i < NSIZ2; i++)
		for(j=0;j<NCHAN;j++)
                    xspecr[j*NSIZ2+i] = xspeca[j*NSIZ2+i] = 0.0;
	    }
            xcorr2(vblock, vblock2, xspecr, xspeca, delay, phase, fwidth,num,nbins,ichan,tbit);
            num++;
            if (num == smode || err1 == 0) {
                xcalc(xspecr, xspeca, num, secs, swidth, fwidth, nbins,ichan);
	       if(ichan==NCHAN-1) plot(xspecr, xspeca, num, NCHAN,filename1,filename2,rev,tbit);
                num = 0;
		iiseg=0;
            }
	 }
         else {
	   printf("missing or incorrect time %4d:%03d:%02d:%02d:%02d %f %f\n",
                 yr,day,hr,mn,sec,sec0,sec1);
	      }
	 }
           if(err1 == 0 || err2 == 0){
           printf("End of data encountered at frame %d\nExecution will be aborted\n",frame);
            return 0;
           }
    else {
	 if((sync != 0xabaddeed || fabs(secs-secp) > 0.001 || fabs(secs-secp) < 0.0) && secp !=0)
	  printf("ERROR secs %f secd %f sync %x syncp %x\n",secs,secs-secp,sync,syncp);
	 secp=secs;
	 syncp=sync;
	 }
    }
    fclose(file1);
    fclose(file2);
    //printf("ichan %d\n",ichan);
  }
  //WriteData(&xspecr[0], &xspeca[0], filenameout); 
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

void xcorr2(unsigned char vblock[], unsigned char vblock2[], double xspecr[], double xspeca[], double dsteer, double phsteer,double fwidth, int num, int nbins, int chan, int tbit)
// vblock are data in 8-bit bytes,  dsteer in ns
{
    FILE *output;
    int sft, i, j, k, m, m1, lenn, dsft, nn, mm, i1, i2;
    double sum1, sum2, amp, delay, fracsft, re, am, ph, aasin, aacos, bbcos[NFRQSER], bbsin[NFRQSER];
    double aac, aas;
    unsigned int ch, ch1, ch2;
    unsigned int da[160], da2[160];
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
//    for(j=0;j<nbins;j++){
	  j=chan; {
	    ph = 2.0*PI*(j-nbins/2)*512.0e-6*fwidth*num/(double)nbins;
	    bbcos[j] = cos(ph);
	    bbsin[j] = sin(ph);
    }
    for (i = 0; i < NSIZ / 2; i++) {
        ph = 2.0 * PI * (double) i *fracsft / (NSIZ) + phsteer; // need () because of 4 in define
        aacos = cos(ph);
        aasin = sin(ph);
//    for(j=0;j<nbins;j++){
        j=chan; {
	aac = aacos * bbcos[j] - aasin * bbsin[j];
	aas = aasin * bbcos[j] + aacos * bbsin[j];
        re = px[i] * aac - py[i] * aas;
        am = px[i] * aas + py[i] * aac;
        xspecr[j*NSIZ2+i] += re;
        xspeca[j*NSIZ2+i] += am;
       }
    }
}

void xcalc(double xspecr[], double xspeca[], int num, double secs, double swidth, double fwidth, int nbins, int chan)
{
    int i, j, k, m;
    double sumr, sumi, amp, phase, delay, a, maxamp, maxdelay, maxphase, maxrate;
    double px[NSIZ * 2], py[NSIZ * 2], y[3];

    maxamp = -1e99;
//    for(j=0;j<nbins;j++){
    j=chan; {
    for (i = NSIZ / 2; i < NSIZ * 2; i++)
        px[i] = py[i] = 0;      // zero out negative frequencies
    for (i = 0; i < NSIZ / 2; i++) {
        px[i] = xspecr[j*NSIZ2+i] / num;
        py[i] = xspeca[j*NSIZ2+i] / num;
// printf("i %3d amp %10.4f\n",i,sqrt(px[i]*px[i]+py[i]*py[i]));
    }
    Four(px, py, NSIZ * 2);
    sumi = -1e99;
    for (i = 0; i < NSIZ * 2; i++) {
        sumr = px[i] * px[i] + py[i] * py[i];
        if(i < NSIZ)
           delay = 125.0 * 0.5 * i;
        else 
           delay = 125.0 * 0.5 * (i - NSIZ * 2);
        if (sumr > sumi && fabs(delay) <= swidth) {
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
    if(swidth == 0.0 || fabs(a) > 0.5) a = 0;
    delay = 15.625 * 0.5 * (k + a); // delay interpolation for 32 MHz BW
    phase -= a * 45.0;          // phase interpolation
    if (phase > 180.0)
        phase -= 360.0;
    if (phase < -180.0)
        phase += 360.0;
    delay = -delay;             // to make delay positive for late arrival at remote site
    amp = sqrt(y[1] + (y[2] - y[0]) * a / 4.0) / ((double) (NSIZ / 2));
    if(amp>maxamp){maxamp=amp; maxdelay=delay; maxphase=phase; maxrate=(j-nbins/2)*fwidth/(double)nbins; }
    }
    amp=maxamp; delay=maxdelay; phase=maxphase;
//    printf("%10.6f amp %7.4f rate %8.3f delay %4.0f ns k %d fwidth %f nbins %d\n",secs,amp,maxrate,delay,k,fwidth,nbins);

}

void plot(double xspecr[], double xspeca[], int num, int chan,char *filename1,char *filename2,int rev,int tbit)
{
    int x,y,i,hr,mn,sec,iter,ichan;
    double integ,freq,temper,t,pdum,xx1,xx2,yy1,yy2,h,s,b,xoff,yoff;
    double secs,amp,phase,delay,delaymin,delaymax,elev,secss,seclast,secfirst,ampmax, secprev;
    double timediff, count,avamp,namp;
    time_t now;
    int timemode;
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
      yy1 = yoff; yy2=yy1+300.0;
      h=s=b=0;
      fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n %2.1f %2.1f %2.1f sethsbcolor stroke\n"
                    ,xx1, yy1, xx2, yy1, h, s, b);
      fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n %2.1f %2.1f %2.1f sethsbcolor stroke\n"
                    ,xx1, yy1+150.0, xx2, yy1+150.0, h, s, b);
      fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n %2.1f %2.1f %2.1f sethsbcolor stroke\n"
                    ,xx1, yy2, xx2, yy2, h, s, b);
      fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n %2.1f %2.1f %2.1f sethsbcolor stroke\n"
                    ,xx1, yy1, xx1, yy2, h, s, b);
      fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n %2.1f %2.1f %2.1f sethsbcolor stroke\n"
                    ,xx2, yy1, xx2, yy2, h, s, b);

	x = xoff; y = 100;
        fprintf(file3, "/Times-Roman findfont\n 10 scalefont\n setfont\n");
	for(iter=0;iter<2;iter++){
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
        if(iter==0) yy2 = y+amp*75.0;
	else yy2 = phase * 75.0 / PI  + y + 150.0 + 75.0;
	if(iter == 0 && yy2 > y+150.0) yy2=y+150.0;
	if(i==0){xx1=xx2; yy1=yy2;}
	         h=s=b=0;
                  s=b=1;
                  h = ichan*0.67/chan;
                  if (h > 0.67) h = 0.67;
		  if (h < 0.0) h = 0.0;
		 if(iter) h=s=b=0;
	if(!iter || (iter && (i>10 && i<NSIZ2-10)&& fabs(yy2-yy1)<37.5))
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
printf("%f \n",avamp/namp);
	       }
               }
        /* write bottom of page */
        x = 100; y = 60.0;
        now = time(NULL);
	fprintf(file3, "0 0 0 sethsbcolor\n");
        fprintf(file3, "/Times-Roman findfont\n 10 scalefont\n setfont\n");
	fprintf(file3,"100 50 moveto\n (file1: %s file2: %s bits/sam %d) show\n",filename1,filename2,tbit+1);
        if(!rev) fprintf(file3,"220 80 moveto\n (DBE channel number) show\n");
        else fprintf(file3,"220 80 moveto\n (DBE channel number reversed) show\n");
	fprintf(file3,"30 150 moveto\n 90 rotate\n (amplitude) show\n -90 rotate\n");
	fprintf(file3,"30 300 moveto\n 90 rotate\n (phase) show\n -90 rotate\n");
        fprintf(file3, "/Times-Roman findfont\n 8 scalefont\n setfont\n");
        fprintf(file3,"%d %d moveto\n (%s) show\n",x+20+300,20,ctime(&now));
        fprintf(file3, "showpage\n%c%cTrailer\n", '%', '%');
        fclose(file3);
}

void WriteData(double *xspecr, double *xspeca, char *filenameout)
{
	int i;	

	double amp[NFRQSER*NLAG];

	FILE *OutPtr;

	if ((OutPtr = fopen(filenameout,"w")) == NULL){
		printf("\nOutput Filename Not Specified\n");
	}	
	
	for (i=0;i<(NFRQSER*NLAG);i++){
		amp[i] = sqrt(xspecr[i]*xspecr[i]+xspeca[i]*xspeca[i]);
		fprintf(OutPtr,"%4d %8f\n",i,amp[i]);
	}   

	//fclose(OutPtr);
	
}
