#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#define TWOPI 6.28318530717958
#define PI 3.1415926536
#define VELC 299792458.0
#define NLAG  64   //original
#define NSIZ 256   // NLAG*4
#define NSIZ2 128
#define NSIZF 4096
#define NCHAN 16
#define NRATE 128
#define NACC 100

typedef struct {
   double ra,dec,xyz1[3],xyz2[3],ras,decs,freq,foffset,delayns,delayns2,
                 srhz,lat1,lat2,lon1,lon2,hgt1,hgt2,lat1s,lat2s,lon1s,lon2s,bx,by,bz,refel,secst,simdelay;
   int rah,ram,decd,decm,usb,numframes,lat1d,lat1m,lon1d,lon1m,lat2d,lat2m,lon2d,lon2m,aut;
   int nrate,fxcor,sigsim,joff,rev,dbemode,ver,numframstart,dchan,chanst,nnchan;
} d1type;

d1type d1;

static double frqq[32],freqq0,delaya;
static int usb,moff;
static char startt[256];
static unsigned char vblock[625];
static unsigned char vblock2[625];
static unsigned char mblock[625];
static unsigned char mblock2[625];
static float cc[65536];
static double xspecr[NCHAN*NSIZ2*NRATE], xspeca[NCHAN*NSIZ2*NRATE],xamp[NCHAN*NSIZ2],xpha[NCHAN*NSIZ2],delp[NCHAN*NSIZ2*NRATE],ratep[NCHAN*NSIZ2];
static double xxspecr[NCHAN*NSIZ2], xxspeca[NCHAN*NSIZ2], xcos[NCHAN*NSIZ2], xsin[NCHAN*NSIZ2];

int process2(char *, char *, int, int,double, double);
void xcorr2(unsigned char *, unsigned char *, double *, double *, double *, double *,  double, double,int,double,int);
void fxcorr2(double *, double *, double *, double *,  double, double,int,double,int);
void xcalc(double *, double *, int, double,int,int);
void xinit(void);
void Four(double *, double *, int,int);
void plot(double *, double *, double *,double *, int, int, char *, char *,int,double,double*,double*,double,double,double,double);
double delaysrc(double *, double *, double, int, int,double *,double *);
void getddate(char *, int, int);
double tosecs(int, int, int, int, double);
double gst(double);
void latlon2xyz(double,double,double,double*,double*,double*);
void radec_azel(double, double, double, double *, double *);
void precess(double,double,double*,double*,double,double);
double gst(double);
double vgeom(int,int,int,int,double,double*);
void catfile(void);
void sigsim(unsigned int *,unsigned int *,double);
double gauss(void);
void mjdate(int,int *, int *);
void simpoly(unsigned int *,unsigned int *, double);
void toyrday (double, int*, int*, int*, int*, int*);


int main(int argc, char *argv[])
{
    char filenamein1[256], filenamein2[256], buf[80];
    int numframes,numframs;
    int tbit;
    int i;
    double delayns,srhzs;
    double freqoff,fspac,cycb;

    d1.ver = 6;   // vlbi0 version 3 added dbemode=1 3Mar10  ver 4 25Mar10    
    d1.numframes = 400;  // default for short autocorr  240000 for 10 seconds 
    tbit = 1;
    d1.delayns = 0;   
    d1.foffset = 0;
    d1.freq = 230.0;   // frequency of SMA corresponding to 1.024 GHz in the I.F.
    d1.freq = 0.512;
    d1.srhz = 0.1/128.0;
    d1.usb = 1;
    d1.aut = 0;
    d1.fxcor = 1;
    d1.rev = 0;
    d1.sigsim = 0;
    d1.simdelay = 0;
    d1.nrate = NRATE;
    d1.dbemode = 0;   // Original mode channel zero not useable mode 1 use chan 0
// dbemode = 0 ignore dbe ch 0
// dbemode = 1 include channel 0
// dbemode = 2 correlate odd vs even dbe channels
// dbemode = 3 use only even dbe channels
// dbemode = 4 use only odd dbe channels
    catfile();
    numframes = d1.numframes;   
    delayns = d1.delayns;
    freqoff = d1.foffset;
    freqq0 = d1.freq;   // frequency of SMA corresponding to 1.024 GHz in the I.F.
    usb = d1.usb;
    filenamein1[0]=filenamein2[0]=0;
    moff=0;
    if(argc < 2) { 
      printf("vlbi0 version %d file1 file2 -numframes numframes -delayns apriori_del_ns -foffset lo_offset_Hz\n",d1.ver);
      printf("-1bit to do 1-bit correlation and get Van Vleck correction -dbemode 1 for channel dbe channel zero\n");
      printf("program reads vlbi0.cat to get source and baseline information\n");
      printf("output plot is in vlbi0.pos\n");
      printf("vlbi0 file1 does autocorrelation\n");
      return 0;
     }
    if (argc > 1)
        sscanf(argv[1], "%255s", filenamein1);
    if (argc > 2) {
            if(argv[2][0] != '-') sscanf(argv[2], "%255s", filenamein2);
                  }
    numframs = 0;
    srhzs = 0;
    d1.numframstart=0;
    for(i=0;i<argc;i++){
    sscanf(argv[i], "%79s",buf);
    if(strstr(buf, "-numframes")) sscanf(argv[i+1],"%d",&numframs);
    if(strstr(buf, "-rev")) sscanf(argv[i+1],"%d",&d1.rev);
    if(strstr(buf, "-1bit")) { tbit = 0; d1.fxcor = 0; }
    if(strstr(buf, "-sigsim")) sscanf(argv[i+1],"%d",&d1.sigsim);
    if(strstr(buf, "-delayns")) sscanf(argv[i+1],"%lf",&delayns);
    if(strstr(buf, "-foffset")) sscanf(argv[i+1],"%lf",&freqoff);
    if(strstr(buf, "-freq")) sscanf(argv[i+1],"%lf",&freqq0);
    if(strstr(buf, "-simdelay")) sscanf(argv[i+1],"%lf",&d1.simdelay);
    if(strstr(buf, "-usb")) sscanf(argv[i+1],"%d",&usb);
    if(strstr(buf, "-dbemode")) sscanf(argv[i+1],"%d",&d1.dbemode);
    if(strstr(buf, "-numframstart")) sscanf(argv[i+1],"%d",&d1.numframstart);
    if(strstr(buf, "-srhz")) sscanf(argv[i+1],"%lf",&srhzs);
     }

    if(filenamein2[0]==0) {
         strcpy(filenamein2,filenamein1);
         d1.aut = 1;    // autocorrelation
         delayns=d1.delayns=0;
         d1.nrate = 1;
         numframes = 100;
     }
    if(numframs) numframes = numframs;  // allow override
    d1.numframes = numframes;
    fspac = 1.0/(NSIZF * 15.625e-9);
    if(freqoff >= 0) d1.joff = freqoff / fspac + 0.5;
    else  d1.joff = freqoff / fspac - 0.5;
    d1.foffset = freqoff;
    cycb = 2500.0*15.625e-9*freqoff;
    i = cycb;
    cycb = cycb - i;
    freqoff = cycb / (2500.0*15.625e-9);
//    printf("d1.joff %d freqoff %f\n",d1.joff,freqoff);
    for(i=0;i<16;i++) frqq[i]= freqq0 + usb*(i-1)*32e-3;   // LSB DBE goes down with DBE ch - ch1 for start 

     d1.srhz = 0.1 / (numframes * 2500.0 * 15.625e-9);
    if(d1.srhz > 1000.0) d1.srhz = 1000.0;
    if(srhzs != 0.0) d1.srhz = srhzs;   // allow overide
    if(fabs(delayns) > 100.0){
    if(delayns > 0) {
       moff = delayns / 62.5 + 0.5;   // 4 samples
       delayns -= moff*62.5;
    }
    else {
       moff = delayns / 62.5 - 0.5;
       delayns -= moff*62.5;
    }
    }
    moff = moff * 4;
    d1.delayns2 = moff*15.625;
    d1.dchan = 1;
    d1.chanst = 0;
    d1.nnchan = 16;
    if(d1.dbemode == 0) {d1.chanst = 1; d1.nnchan = 15;}
    if(d1.dbemode == 3) {d1.dchan = 2; d1.chanst = 0; d1.nnchan = 8;}
    if(d1.dbemode == 4) {d1.dchan = 2; d1.chanst = 1; d1.nnchan = 8;}
process2(filenamein1,filenamein2, numframes,tbit,delayns,freqoff);
return 0;
}

  void catfile(void)  
{
     FILE * file1;
     char buf[256];
     if ((file1 = fopen("vlbi0.cat", "r")) == NULL)
          {
         printf("Since prg cannot open vlbi0.cat it will");
         printf(" assume all defaults - use vlbi0.cat or command line arguments\n");
         return;
         }
     while (fgets(buf, 256, file1) != 0)
          {
         if (buf[0] != '*' && buf[0] != '#')
              {
             if (strstr(buf, "XYZ1 "))
                 sscanf(buf, "%*s %lf %lf %lf", &d1.xyz1[0],&d1.xyz1[1],&d1.xyz1[2]);
             if (strstr(buf, "XYZ2 "))
                 sscanf(buf, "%*s %lf %lf %lf", &d1.xyz2[0],&d1.xyz2[1],&d1.xyz2[2]);
             if (strstr(buf, "RA ")) {
                 sscanf(buf, "%*s %d:%2d:%lf",&d1.rah,&d1.ram,&d1.ras);
                 d1.ra = (d1.rah+d1.ram/60.0+d1.ras/3600.0)*PI/12.0;
                 }
             if (strstr(buf, "DEC ")) {
                 sscanf(buf, "%*s %d:%2d:%lf",&d1.decd,&d1.decm,&d1.decs);
                 d1.dec = (fabs(d1.decd)+d1.decm/60.0+d1.decs/3600.0)*PI/180.0;
                 if(d1.decd < 0.0) d1.dec = -d1.dec;
                 }
             if (strstr(buf, "LATLON1 ")) {
                 d1.hgt1 = 0;
                 sscanf(buf, "%*s %d:%2d:%lf %d:%2d:%lf %lf",
                     &d1.lat1d,&d1.lat1m,&d1.lat1s,&d1.lon1d,&d1.lon1m,&d1.lon1s,&d1.hgt1);
                 d1.lat1 = (fabs(d1.lat1d)+d1.lat1m/60.0+d1.lat1s/3600.0);
                 if(d1.lat1d < 0.0) d1.lat1 = -d1.lat1;
                 d1.lon1 = (fabs(d1.lon1d)+d1.lon1m/60.0+d1.lon1s/3600.0);
                 if(d1.lon1d < 0.0) d1.lon1 = -d1.lon1;
                 latlon2xyz(d1.lat1, d1.lon1, d1.hgt1, &d1.xyz1[0],&d1.xyz1[1],&d1.xyz1[2]);
                 }
             if (strstr(buf, "LATLON2 ")) {
                 d1.hgt2 = 0;
                 sscanf(buf, "%*s %d:%2d:%lf %d:%2d:%lf %lf",
                     &d1.lat2d,&d1.lat2m,&d1.lat2s,&d1.lon2d,&d1.lon2m,&d1.lon2s,&d1.hgt2);
                 d1.lat2 = (fabs(d1.lat2d)+d1.lat2m/60.0+d1.lat2s/3600.0);
                 if(d1.lat2d < 0.0) d1.lat2 = -d1.lat2;
                 d1.lon2 = (fabs(d1.lon2d)+d1.lon2m/60.0+d1.lon2s/3600.0);
                 if(d1.lon2d < 0.0) d1.lon2 = -d1.lon2;
                 latlon2xyz(d1.lat2, d1.lon2, d1.hgt2, &d1.xyz2[0],&d1.xyz2[1],&d1.xyz2[2]);
                 }
             if (strstr(buf, "LATLONR1 ")) {
                 d1.hgt1 = 0;
                 sscanf(buf, "%*s %lf %lf %lf",
                     &d1.lat1,&d1.lon1,&d1.hgt1);
                 d1.lat1 = d1.lat1*180.0/PI;
                 d1.lon1 = d1.lon1*180.0/PI;
                 latlon2xyz(d1.lat1, d1.lon1, d1.hgt1, &d1.xyz1[0],&d1.xyz1[1],&d1.xyz1[2]);
                 }
             if (strstr(buf, "LATLONR2 ")) {
                 d1.hgt2 = 0;
                 sscanf(buf, "%*s %lf %lf %lf",
                     &d1.lat2,&d1.lon2,&d1.hgt2);
                 d1.lat2 = d1.lat2*180.0/PI;
                 d1.lon2 = d1.lon2*180.0/PI;
                 latlon2xyz(d1.lat2, d1.lon2, d1.hgt2, &d1.xyz2[0],&d1.xyz2[1],&d1.xyz2[2]);
                 printf("xyz1 %f %f %f xyz2 %f %f %f\n",d1.xyz1[0],d1.xyz1[1],d1.xyz1[2],d1.xyz2[0],d1.xyz2[1],d1.xyz2[2]);
                 printf("dxyz1 %f %f %f\n",d1.xyz1[0]-d1.xyz2[0],d1.xyz1[1]-d1.xyz2[1],d1.xyz1[2]-d1.xyz2[2]);
                 }
               if (strstr(buf, "FREQ "))
                   sscanf(buf, "%*s %lf",&d1.freq);
               if (strstr(buf, "FOFFSET "))
                   sscanf(buf, "%*s %lf",&d1.foffset);
               if (strstr(buf, "DELAYNS "))
                   sscanf(buf, "%*s %lf",&d1.delayns);
               if (strstr(buf, "SRHZ "))
                   sscanf(buf, "%*s %lf",&d1.srhz);
               if (strstr(buf, "NUMFRAMES "))
                   sscanf(buf, "%*s %d",&d1.numframes);
               if (strstr(buf, "USB")) d1.usb = 1;
               if (strstr(buf, "LSB")) d1.usb = -1;
               if (strstr(buf, "DBEMODE"))
                   sscanf(buf, "%*s %d",&d1.dbemode);
             }
         }
     fclose(file1);
 }

int process2(char *filename1, char *filename2, int smode, int tbit,double delayns, double freqoff)
{
    int i, j, k, m, iseg, iiseg, isec, isync, isync1, ii;
    int err1,err2;
    unsigned int v24, m24, mag, sync, dd[2504], dd1[2504];       // for speedy sscanf
    int num, nchan, numm;
    char p;
    char buf[32768];
    double secs, secs2, phase, fsec,mxdel, delay,delayg,rate,secss;
    double bstate[2*NCHAN],bcount[2*NCHAN];
    double maxrate,max,ratea,maxamp;
    FILE *file1, *file2;
    int ichan,frame,frame2,jmax;
    int mjd,yr,day,hr,mn;

 frame = 0;
 sync = 0;
 nchan = NCHAN;
 ratea = 0;
    xinit();
    k = 0;
    iseg = 0;
    if ((file1 = fopen(filename1, "r")) == NULL){
        return 0;
    }
    if ((file2 = fopen(filename2, "r")) == NULL)
        return 0;
    iiseg = 1;
    err1 = err2 = 1;
    num = numm = 0; 
    for (k = 0; k < d1.nrate; k++) {
      for (i = 0; i < NSIZ2; i++) {
         for(j=0;j<NCHAN;j++) xspecr[k*NSIZ2*NCHAN+j*NSIZ2+i] = xspeca[k*NSIZ2*NCHAN+j*NSIZ2+i] = 0.0;
     } }
     for (i = 0; i < NSIZ2; i++) {
      for(j=0;j<NCHAN;j++) xxspecr[j*NSIZ2+i] = xxspeca[j*NSIZ2+i] = 0.0;
     }
    j=0;
    while (iiseg) {
            err1=fread(dd, 4, 1, file1);
            j++;
            if(dd[0]  ==  0xabaddeed) iiseg = 0;
            }
            if(j < 2504) iiseg = 1;   
            isync = 0;
    j=0;
    while (iiseg) {
            err1=fread(dd1, 4, 1, file2);
            j++;
            if(dd1[0]  ==  0xabaddeed) iiseg = 0;
            }
            if(j < 2504) iiseg = 1;     
            isync1 = 0;
            if(moff < 0){ 
                isync = 2500;
                    err1=fread(dd, 12, 1, file1);
                for(j=0;j<-moff;j++){
                    err1=fread(dd, 4, 1, file1);
                    isync--;
                    if(isync == 0) {err1=fread(dd, 16, 1, file1); isync=2500;}   // read over sync word
                    } 
            }
            if(moff > 0) { 
                           isync1 = 2500;
                           err1=fread(dd1, 12, 1, file2);   // read the rest of the header
                           for(j=0;j<moff;j++) {
                           err1=fread(dd1, 4, 1, file2);
                           isync1--;
                           if(isync1 == 0) {err1=fread(dd1, 16, 1, file2); isync1=2500;}
                        }
                  }
// printf("moff %d isync %d isync1 %d\n",moff,isync,isync1);
    while (iiseg) {
           if(numm == 0){
	    if(isync==0) err1=fread(&dd[1], sizeof(dd)-4, 1, file1);
            else err1=fread(dd, sizeof(dd), 1, file1);
//	    for(j=0;j<2504;j++) {printf("%x ",dd[j]); if(j%10 == 0) printf("\n");}
            if(isync1==0)err2=fread(&dd1[1], sizeof(dd1)-4, 1, file2);
            else err2=fread(dd1, sizeof(dd1), 1, file2);
//	    for(j=2400;j<2504;j++) {printf("j %d %x ",j,dd1[j]); printf("\n");}
          }
          else {
            err1=fread(dd, sizeof(dd), 1, file1);
            err2=fread(dd1, sizeof(dd1), 1, file2);
          }
          if(dd[isync] ==  0xabaddeed && dd1[isync1] ==  0xabaddeed){
          if(numm >= d1.numframstart){
          if(err1 && err2){
                  k=isync;
		  sync=dd[k];
		  frame = dd[k+1]&0x7fff;
		  sprintf(buf,"MJD ");
		  for(j=0;j<3;j++){ p=((dd[k+2] >> (28 - j*4)) & 0xf); sprintf(&buf[4+j],"%d",p); }
		  sprintf(&buf[7]," ");
		  for(j=3;j<8;j++){ p=((dd[k+2] >> (28 - j*4)) & 0xf); sprintf(&buf[5+j],"%d",p); }
		  sprintf(&buf[13],".");
		  for(j=0;j<4;j++){ p=((dd[k+3] >> (28 - j*4)) & 0xf); sprintf(&buf[14+j],"%d",p); }
		  sprintf(&buf[18]," secs frame %d\n",frame);
		  sscanf(buf,"%*s %*s %lf %*s %*s %lf",&secs,&fsec);
                  isec = secs;
		  secs = isec + fsec*2500.0/64e6;
//                  printf("secs %lf isync %d\n",secs,isync);
                  if(moff < 0) secs += -isync*15.625e-9;
//		  if(!(frame%400)) printf("ref %s secs %12.6f frame %d\n",buf,secs,frame);
//                  printf("err1 %d err2 %d\n",err1,err2);
                  k=isync1;
                  frame2 = dd1[k+1]&0x7fff;
                  sprintf(buf,"MJD ");
                  for(j=0;j<3;j++){ p=((dd1[k+2] >> (28 - j*4)) & 0xf); sprintf(&buf[4+j],"%d",p); }
                  sprintf(&buf[7]," ");
                  for(j=3;j<8;j++){ p=((dd1[k+2] >> (28 - j*4)) & 0xf); sprintf(&buf[5+j],"%d",p); }
                  sprintf(&buf[13],".");
                  for(j=0;j<4;j++){ p=((dd1[k+3] >> (28 - j*4)) & 0xf); sprintf(&buf[14+j],"%d",p); }
                  sprintf(&buf[18]," secs frame %d\n",frame2);
                  sscanf(buf,"%*s %d %lf %*s %*s %lf",&mjd,&secs2,&fsec);
                  isec = secs2;
                  secs2 = isec + fsec*2500.0/64e6;
//                  printf("secs2 %lf isync1 %d d1.delayns2 %f\n",secs2,isync1,d1.delayns2);
                  if(moff) secs2 -= d1.delayns2*1e-9;
                  if(moff > 0) secs2 += -isync1*15.625e-9;
                  if(fabs(secs-secs2) > 1e-10) printf("secs %f secs2 %f diff %e frame %d frame2 %d\n",
                                secs,secs2,secs-secs2,frame,frame2);
//                  if(!(frame%400)) printf("rem %s secs %12.6f frame %d\n",buf,secs,frame);
//                  printf("err1 %d err2 %d\n",err1,err2);
		  }
              delayg=rate=0;
              yr = day = 0;
              mjdate(mjd,&yr,&day);
              hr = (int)(secs/3600.0);
//              yr = (dd[1]>>28) + 2000;  // not yet implemented
//              printf("yr %d day %d dd1 %x %x mjd %d\n",yr,day,dd[1],dd1[1],mjd);
              mn = (int)(secs/60.0) - hr*60;
              secss = secs - hr*3600 - mn*60;
              delayg = vgeom(yr,day,hr,mn,secss,&rate); // for cm cm2
              delay = delayns+delayg;
              if(num==0) { delaya = delay; ratea = rate; d1.secst = secs;}   // Apriori
              if(d1.sigsim==1) sigsim(dd,dd1,secs-d1.secst);
              if(d1.sigsim==2) simpoly(dd,dd1,d1.simdelay);
        for(ichan=0;ichan<nchan;ichan++){
                      bstate[ichan] = bcount[ichan]=0;
                      bstate[ichan+16] = bcount[ichan+16]=0;
           }
        for(ichan=d1.chanst;ichan<nchan;ichan+=d1.dchan){
                      bstate[ichan] = bcount[ichan]=0;
                      bstate[ichan+16] = bcount[ichan+16]=0;
			k = 0;
			j = v24 = m24 = 0;
                        if(d1.rev) m = 15-ichan; 
                        else m = ichan;
                           ii = 0;
                  for(i=0;i<2500;i++){
                           if(ii == isync) ii += 4;
                           v24 += ((dd[ii] >> m*2) & 1) << (7-j); 
                           m24 += ((dd[ii] >> (m*2+1)) & 1) << (7-j); 
                           bcount[ichan]++;
                           mag = ((dd[ii] >> m*2) & 3);
                           if(mag == 3 || mag == 0) bstate[ichan]++;
			   j++;
			   if(j==8) {
                            mblock[k] = m24;
                            vblock[k++] = v24;
//			    printf("k %d v24 %d\n",k,v24);
			    j = v24 = m24 = 0;
			  }
                            ii++;
                         }
			k = 0;
			 j = v24 = m24 = 0;
                        if(d1.dbemode == 2) {
                        if(d1.rev) m = 15-ichan+1;
                        else m = ichan+1;
                          }
                           ii = 0;
                  for(i=0;i<2500;i++){
                           if(ii == isync1) ii += 4;
                           v24 += ((dd1[ii] >> m*2) & 1) << (7-j);         
                           m24 += ((dd1[ii] >> (m*2+1)) & 1) << (7-j);         
                           bcount[ichan+16]++;
                           mag = ((dd1[ii] >> m*2) & 3);
                           if(mag == 3 || mag == 0) bstate[ichan+16]++;
                           j++;
                           if(j==8) {
                            mblock2[k] = m24;
                            vblock2[k++] = v24;
//                          printf("k %d v24 %d\n",k,v24);
                            j = v24 = m24 = 0;
                          }
                           ii++;
                         }
            phase = freqoff * 2.0 * PI * secs;
            phase += delay * frqq[ichan] * 2.0 * PI;
            phase = usb*phase;
            if(!(frame%NACC) && ichan==d1.chanst) printf("%4d:%03d:%02d:%02d:%f delay %f ns rate %f ns/s %f Hz refel %5.2f num %d %d smode %d\n",yr,day,hr,mn,secss,delay,rate,rate*frqq[0],d1.refel,num,numm,smode);
            if(frame == 0 && ichan==d1.chanst)  sprintf(startt,"%4d:%03d:%02d:%02d:%f",yr,day,hr,mn,secss);
            if(ichan >= d1.chanst) {
                 if(d1.fxcor)  fxcorr2(xspecr, xspeca, xxspecr, xxspeca, delay, phase,ichan,secs,num);
                 else xcorr2(vblock, vblock2, xspecr, xspeca, xxspecr, xxspeca, delay, phase,ichan,secs,num);
                 }
	 }
           if(err1 == 0 || err2 == 0)
           printf("end of data encountered at frame %d\n",frame);
         num++;
        if (num == smode || err1 == 0) iiseg=0;
      }
    }
    else { printf("couldn't find sync word %x %x\n",dd[0],dd1[0]);
//           return 0;
         }
        numm++;
    }
                mxdel = delaysrc(xspecr, xspeca, delaya, num, NCHAN, &maxrate,&maxamp);
    for(ichan=d1.chanst;ichan<nchan;ichan+=d1.dchan) 
                xcalc(xspecr, xspeca, num, secs,ichan,(int)maxrate);
                max = -1e99; jmax = 0;
                for(i=0;i<NSIZ2*NCHAN;i++) if(delp[(int)maxrate*NSIZ2*NCHAN+i] > max) {max = delp[(int)maxrate*NSIZ2*NCHAN+i];jmax = i;}
                for(i=0;i<d1.nrate;i++) ratep[i] = delp[i*NSIZ2*NCHAN+jmax];
                plot(&xspecr[(int)maxrate*NSIZ2*NCHAN], &xspeca[(int)maxrate*NSIZ2*NCHAN], 
                     &delp[(int)maxrate*NSIZ2*NCHAN],ratep, num, NCHAN,filename1,filename2,tbit,delaya,bstate,bcount,mxdel,freqoff,maxrate,maxamp);
    
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

void xcorr2(unsigned char vblock[], unsigned char vblock2[], double xspecr[], double xspeca[], double xxspecr[], double xxspeca[], double dsteer, double phsteer,int chan, double sec, int accum)
// vblock are data in 8-bit bytes,  dsteer in ns
{
    int sft, i, j, k, m, m1, lenn, dsft, nn, mm, i1, i2;
    double sum1, sum2, fracsft, ph, aasin, aacos, daasin, daacos;
    double aac, aas;
    unsigned int ch, ch1, ch2;
    unsigned int da[NSIZ], da2[NSIZ];
    double aut[NSIZ], px[NSIZ * 2], py[NSIZ * 2];
    double rsin[NRATE],rcos[NRATE];

    lenn=312;
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
//   if(sft==32)    printf("i+mm %d m %d dsft %d\n",i+mm,m,dsft);
            sum1 += cc[(ch >> 16)] + cc[(ch & 0xffff)];
        }
        sum2 = 2 * (i2 - i1);
        aut[sft] = sin(PI * 0.5 * sum1 / sum2);   // Apply Van Vleck correction for 1-bit/sam
        if (sft >= NLAG / 2)
            px[sft - NLAG / 2] = aut[sft];
        else
            px[NSIZ + sft - NLAG / 2] = aut[sft];
//   if(sft > NLAG/2 - 5 && sft < NLAG/2 + 5)printf("xcorr sft %d %f\n",sft,aut[sft]);
    }

    Four(px, py, NSIZ, 1);         // FFT to cross-spectral function

        aacos =  cos(phsteer);
        aasin =  sin(phsteer);
      
//        ph = usb * 2.0 * PI *fracsft / (NSIZ); // need () because of 4 in define
        ph = 2.0 * PI *fracsft / (NSIZ); // need () because of 4 in define
        daacos = cos(ph);
        daasin = sin(ph);

    for (i = 0; i < NSIZ / 2; i++) {
//        ph = usb * 2.0 * PI * (double) i *fracsft / (NSIZ) + phsteer; // need () because of 4 in define
//        aacos = cos(ph);
//        aasin = sin(ph);
        m = chan*NSIZ2+i;
        xxspecr[m] += px[i] * aacos - py[i] * aasin;
        xxspeca[m] += px[i] * aasin + py[i] * aacos;
        aac = aacos * daacos - aasin * daasin;
        aasin = aacos * daasin + aasin * daacos;
        aacos = aac;
        }
 
    if((accum%NACC) == NACC - 1 || ((accum == d1.numframes - 1) && d1.numframes < NACC)) {
    for (k = 0; k < d1.nrate; k++) {
//        ph = usb * (k-NRATE/2)*sec*srhz*2.0*PI;
        ph = usb * (k-d1.nrate/2)*(sec-d1.secst)*d1.srhz*2.0*PI*frqq[chan]/frqq[1];
        rcos[k] = cos(ph);
        rsin[k] = sin(ph);
        }
    for (i = 0; i < NSIZ / 2; i++) {
        m = chan*NSIZ2+i;
    for (k = 0; k < d1.nrate; k++) {
        aac = rcos[k];
        aas = rsin[k];
        xspecr[m] += xxspecr[chan*NSIZ2+i] * aac - xxspeca[chan*NSIZ2+i] * aas;
        xspeca[m] += xxspecr[chan*NSIZ2+i] * aas + xxspeca[chan*NSIZ2+i] * aac;
        m += NSIZ2*NCHAN;
      }
        xxspecr[chan*NSIZ2+i] = xxspeca[chan*NSIZ2+i] = 0;
     }
    }
}


void fxcorr2(double xspecr[], double xspeca[], double xxspecr[], double xxspeca[], double dsteer, double phsteer,int chan, double sec, int accum)
// vblock are data in 8-bit bytes,  dsteer in ns
{
    int i, j, k, m, lenn, dsft;
    double sum2, fracsft, ph, aasin, aacos, daasin, daacos;
    double aac, aas, rre, rre2, aam, aam2, pwr, pwr2;
    double rsin[NRATE],rcos[NRATE];
    static double ddar[NSIZF],ddar2[NSIZF],px[NSIZF],py[NSIZF];

    lenn=312;
    if (dsteer > 0.0)
        dsft = dsteer / 15.625 + 0.5; // nearest integer shift
    else
        dsft = dsteer / 15.625 - 0.5;  // 32 MHz 64 ms/s sample
    fracsft = dsteer / 15.625 - dsft; // fractional delay shift needed to steer delay

    for (i = 0; i < NSIZF; i ++) ddar[i]=ddar2[i]=0; 
    pwr = pwr2 = 0;
    for (i = 0; i < lenn*8; i ++) {
           k = i >> 3;
           m = 7-(i%8);
           if((vblock[k] >> m) & 1) aac = 1; else aac = 0;
           if((mblock[k] >> m) & 1) aas = 1; else aas = 0;
             aac = aac*2.0 + aas -1.5;
           if(dsft >= 0) j=i+dsft;
           else j=i;
           if(j < NSIZF) ddar[j] = aac;
           pwr += aac*aac;
           if((vblock2[k] >> m) & 1) aac = 1; else aac = 0;
           if((mblock2[k] >> m) & 1) aas = 1; else aas = 0;
           aac = aac*2.0 + aas -1.5;
           if(dsft < 0) j=i-dsft;
           else j=i;
           if(j < NSIZF) ddar2[j] = aac;
           pwr2 += aac*aac;
     }
     Four(ddar, ddar2, NSIZF, -1);  // transform 2-reals with complex FFT
     for (i = 0; i < NSIZ; i++)
        px[i] = py[i] = 0.0;

     for (i = 0; i < NSIZF/2; i++) {
     j = i - d1.joff;
     if (i >= 1) {
               rre = ddar[i] + ddar[NSIZF - i];
               aam = ddar2[i] - ddar2[NSIZF - i];
                  } else {
               rre = ddar[i] + ddar[0];
               aam = ddar2[i] - ddar2[0];
                  }
      if (j >= 1 && j < NSIZF/2) {
               aam2 = -ddar[j] + ddar[NSIZF - j];
               rre2 = ddar2[j] + ddar2[NSIZF - j];
                  } else {
               aam2 = 0;
               rre2 = 0;
                  }
         px[i>>4] += (rre*rre2 + aam*aam2);    // average over NSIZF/NSIZ = 16 freqs
         py[i>>4] += (aam*rre2 - rre*aam2);
      }
     sum2 = sqrt(pwr*pwr2)*64.0;
     for(i = 0; i < NSIZ/2; i++) {
          px[i] = px[i] / sum2;
          py[i] = py[i] / sum2;
     }
        aacos =  cos(phsteer);
        aasin =  sin(phsteer);
      
//        ph = usb * 2.0 * PI *fracsft / (NSIZ); // need () because of 4 in define
        ph = 2.0 * PI *fracsft / (NSIZ); // need () because of 4 in define
        daacos = cos(ph);
        daasin = sin(ph);

    for (i = 0; i < NSIZ / 2; i++) {
//        ph = usb * 2.0 * PI * (double) i *fracsft / (NSIZ) + phsteer; // need () because of 4 in define
//        aacos = cos(ph);
//        aasin = sin(ph);
        m = chan*NSIZ2+i;
        xxspecr[m] += px[i] * aacos - py[i] * aasin;
        xxspeca[m] += px[i] * aasin + py[i] * aacos;
        aac = aacos * daacos - aasin * daasin;
        aasin = aacos * daasin + aasin * daacos;
        aacos = aac;
        }
 
    if((accum%NACC) == NACC - 1 || ((accum == d1.numframes - 1) && d1.numframes < NACC)) {
    for (k = 0; k < d1.nrate; k++) {
//        ph = usb * (k-NRATE/2)*sec*srhz*2.0*PI;
        ph = usb * (k-d1.nrate/2)*(sec-d1.secst)*d1.srhz*2.0*PI*frqq[chan]/frqq[1];
        rcos[k] = cos(ph);
        rsin[k] = sin(ph);
        }
    for (i = 0; i < NSIZ / 2; i++) {
        m = chan*NSIZ2+i;
    for (k = 0; k < d1.nrate; k++) {
        aac = rcos[k];
        aas = rsin[k];
        xspecr[m] += xxspecr[chan*NSIZ2+i] * aac - xxspeca[chan*NSIZ2+i] * aas;
        xspeca[m] += xxspecr[chan*NSIZ2+i] * aas + xxspeca[chan*NSIZ2+i] * aac;
        m += NSIZ2*NCHAN;
      }
        xxspecr[chan*NSIZ2+i] = xxspeca[chan*NSIZ2+i] = 0;
     }
    }
}

double gauss(void)
{
double v1,v2,r,fac,amp,vv1;
r=0.0;
while(r>1.0 || r==0.0){
v1=2.0*(rand()/2147483648.0)-1.0;v2=2.0*(rand()/2147483648.0)-1.0;
r=v1*v1+v2*v2;
}
fac=sqrt(-2.0*log(r)/r);vv1=v1*fac;
amp=vv1;
return(amp);
}


void sigsim(unsigned int dd[], unsigned int dd1[],double secs)
{ int i,j,asn,mag,asn1,mag1,foff;
  double a,b,f,phase[16],pwr,pwr1,aa[5000],bb[5000],thres,noise;
  thres = 0.8; noise = 0*10.0; foff = 400e3;
  f = 0.5/16.0; pwr=pwr1=0;
  for(i=4; i< 2504; i++){
  if(i%16 == 0) for(j=0;j<16;j++) phase[j] = (rand()/2147483648.0)*2.0*PI;

    a = b = 0;
    for(j=1;j<15;j++) {
       a += cos(i*j*f*2.0*PI+ phase[j]);
      }
    a += noise*gauss();
    for(j=1;j<15;j++) {
       b += cos(i*j*f*2.0*PI+ phase[j]+PI/2.0+foff*(i*15.625e-9 + secs)*2.0*PI);
      }
     b += noise*gauss();
     aa[i] = a;
     bb[i] = b;
     pwr += a*a;
     pwr1 += b*b;
     }
  pwr = sqrt(pwr/2500.0);
  pwr1 = sqrt(pwr1/2500.0);
  for(i=4; i< 2504; i++){
    a = aa[i]/pwr;
    j = i + 0;
  if(j >= 4 && j < 2504) b = bb[j]/pwr1; else b = 0;
    if(a > 0.0)  asn=1; else asn=0;
    if(asn) mag = 0; else mag = 1;
    if(asn && a > thres) mag = 1;
    if(!asn && a < -thres) mag = 0;

    if(b > 0.0)  asn1=1; else asn1=0;
    if(asn1) mag1 = 0; else mag1 = 1;
    if(asn1 && b > thres) mag1 = 1;
    if(!asn1 && b < -thres) mag1 = 0;

//     mag=asn; mag1=asn1;

       dd[i]=dd1[i]=0;
    for(j=0;j<16;j++) {
       dd[i] |= (asn << 2*j) + (mag << (2*j+1));
       dd1[i] |= (asn1 << 2*j) + (mag1 << (2*j+1));
    }
   }
}


void simpoly(unsigned int dd[], unsigned int dd1[], double dely)
{ double fft_r[2048],fft_i[2048],h[1024],hi[1024];
  double w,re,im,rre,iim,firscale,adcscale;
  double reout[16][2500],imout[16][2500],rgauss[3000*32];
  double fftf_r,ff,ff0;
  int simdat[3000*32];
  int iter,i,j,kk,m,nn,mm,ij,nh,ch,n,asn,mag,idelay;
 idelay = dely*1.024;   // 1024 MHz sample rate
 ff0=ff=0;
 for(iter=0;iter<2; iter++){
 n=16; mm = 8*1; nn = 2*n;  nh=16*1/2; firscale=32768; adcscale=128;
  for(i=0;i<mm*nn;i++) { w = 0.95*(i-mm*nn/2)*PI/nn+1e-6; h[i] = firscale*(sin(w)/w)*(0.54-0.46*cos(2.0*PI*i/(mm*nn-1))); }
  for(i=0;i<nh;i++) { w = (i-nh/2)*PI+1e-6+PI/2.0; hi[i] = firscale*(0.7-0.3*cos(2.0*PI*i/(nh-1)))*(sin(w)/w); 
//   printf("i %d hi %f\n",i,hi[i]);
  }
 if(iter==0) for(j=0;j<3000*32;j++) rgauss[j] = gauss();
 rre = iim = re = im = 0.0;
 for(j=0;j<2500;j++){
  if(j%2==0){
  for(i=0;i<nn;i++){
   rre = iim = 0.0;
   for(m=0; m < mm; m++){
   kk = i + j*nn + nn*m + iter*idelay;
   fftf_r = gauss();
   if(fftf_r > 1.4) {kk += 4; ff++;}
   if(fftf_r < -1.4) {kk -= 4; ff++;}
   ff0++;
   re = adcscale*rgauss[kk];
   im = adcscale*rgauss[kk+nn];
   rre += re*h[i+m*nn];
   iim += im*h[i+m*nn];
   }
   fft_r[i] = rre/firscale; fft_i[i] = iim/firscale;
  }
  Four(fft_r,fft_i,nn,-1);
  }
for(i=0;i<n;i++){
if(j%2==0){
if(i >= 1){ rre = fft_r[i] + fft_r[nn-i]; iim = fft_i[i] - fft_i[nn-i]; }
  else { rre = fft_r[i] + fft_r[0]; iim = fft_i[i] - fft_i[0]; }
 }
if(j%2==1){
if(i >= 1){ iim = -fft_r[i] + fft_r[nn-i]; rre = fft_i[i] + fft_i[nn-i]; }
  else { iim = -fft_r[i] + fft_r[0]; rre = fft_i[i] + fft_i[0]; }
 }
  reout[i][j] = rre;
  imout[i][j] = iim;
 }
 }
for(ch=0;ch<n;ch++){
 for(j=0;j<2500;j++){
  ij = j/2 + nh/2;
  if(j%2==0)re=reout[ch][ij];
  if(j%2==1) {
   im=0;
  for(m=0;m<nh;m++){
   im += imout[ch][ij+m-nh/2+1]*hi[m]; 
 }
 im = im/firscale;
 }
  fftf_r = 0;
  if(j%4==0) fftf_r=re;
  if(j%4==1) fftf_r=im;
  if(j%4==2) fftf_r=-re;
  if(j%4==3) fftf_r=-im;
// if(ch==1)  printf("j %d re %f im %f fft %f\n",j,re,im,fftf_r);
 if(fftf_r>0.0) simdat[j+ch*2500]=1;
 else simdat[j+ch*2500]=0;
 }
}
  for(i=4; i< 2504; i++){
    if(iter==0) dd[i]=0;
    else dd1[i]=0;
    for(j=0;j<16;j++) {
       asn = simdat[i-4+j*2500];
       mag = 0;
       if(iter==0) dd[i] |= (asn << 2*j) + (mag << (2*j+1));
       else dd1[i] |= (asn << 2*j) + (mag << (2*j+1));
    }
   }
 }
printf("fraction %f\n",ff/ff0);
}


void xcalc(double xspecr[], double xspeca[], int num, double secs, int chan, int irate)
{
    int i, j, k;
    double amp, sumr, sumi, a, phase, delay, maxamp, maxdelay, maxphase;
    double px[NSIZ * 2], py[NSIZ * 2], y[3];

    maxamp = -1e99;
    maxphase = 0;
    maxdelay = 0;
    j=chan; 
    for (i = NSIZ / 2; i < NSIZ * 2; i++)
        px[i] = py[i] = 0;      // zero out negative frequencies
    for (i = 0; i < NSIZ / 2; i++) {
        px[i] = xspecr[irate*NSIZ2*NCHAN+j*NSIZ2+i] / num;
        py[i] = xspeca[irate*NSIZ2*NCHAN+j*NSIZ2+i] / num;
// printf("i %3d amp %10.4f\n",i,sqrt(px[i]*px[i]+py[i]*py[i]));
    }
    Four(px, py, NSIZ * 2, 1);
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
    printf("%10.6f amp %8.6f delay %4.0f ns phase %4.0f k %d chan %d\n",secs,amp,delay,phase,k,chan);

}


double delaysrc(double xspecr[], double xspeca[], double delaya, int num, int chan, double *maxrate, double *maxamp)
{
  int ichan,i,j,k,maxdelj;
  double max,maxdel,suma,sumr,ph,del,phase,amp,aa,bb,delw,ccos,ssin,cc,ss,eff;
  double aacos[NSIZ2],aasin[NSIZ2],y[3];
  max = -1e99;
  maxdel = 0;
  maxdelj = 0;
  *maxrate = 0;
  aa = NSIZ2;
  delw = 512.0*NLAG/128.0;   // was 64

   for(k=0;k<d1.nrate;k++){
   for(ichan=d1.chanst;ichan<chan;ichan+=d1.dchan){
   for(i=0;i<NSIZ2;i++){
      xamp[ichan*NSIZ2+i] = sqrt(xspecr[k*NSIZ2*NCHAN+ichan*NSIZ2+i]*xspecr[k*NSIZ2*NCHAN+ichan*NSIZ2+i]
		+xspeca[k*NSIZ2*NCHAN+ichan*NSIZ2+i]*xspeca[k*NSIZ2*NCHAN+ichan*NSIZ2+i]);
      xpha[ichan*NSIZ2+i] = atan2(xspeca[k*NSIZ2*NCHAN+ichan*NSIZ2+i],xspecr[k*NSIZ2*NCHAN+ichan*NSIZ2+i]);
      xcos[ichan*NSIZ2+i] = cos(xpha[ichan*NSIZ2+i]);
      xsin[ichan*NSIZ2+i] = sin(xpha[ichan*NSIZ2+i]);
        }}
   j = 0;
   for(j=0;j<NSIZ2*NCHAN;j++){
        del = (j-NSIZ2*8)*delw/(NSIZ2*8);
        sumr = suma = 0;
        bb = del*2.0*PI*32e-3/aa;
        for(i=0;i<NSIZ2;i++) {
           aacos[i] = cos(i * bb);
           aasin[i] = sin(i * bb);
        }
	for(ichan=d1.chanst;ichan<chan;ichan+=d1.dchan){
        ph = usb*del*2.0*PI*frqq[ichan];
        cc = cos(ph);
        ss = sin(ph);
	for(i=0;i<NSIZ2;i++){
        amp = xamp[ichan*NSIZ2+i];
//        phase = xpha[ichan*NSIZ2+i];
//        ph = usb*del*2.0*PI*(frqq[ichan]+usb*i*32e-3/aa);
        ccos = cc * aacos[i] - ss * aasin[i];
        ssin = -(cc * aasin[i] + ss * aacos[i]);
        bb = xcos[ichan*NSIZ2+i] * ccos - xsin[ichan*NSIZ2+i] * ssin;  
        ssin = xcos[ichan*NSIZ2+i] * ssin + xsin[ichan*NSIZ2+i] * ccos;
        sumr += amp * bb;
        suma += amp * ssin;
        }
      }
        amp = sqrt(sumr*sumr+suma*suma);
        if(amp > max) {max = amp; maxdel = del; *maxrate = k; maxdelj = j;}
        delp[k*NSIZ2*NCHAN+j] = amp/((double)num*NSIZ2*(d1.nnchan));
      }
        for(ichan=d1.chanst;ichan<chan;ichan+=d1.dchan){
        for(i=0;i<NSIZ2;i++){
        amp = xamp[ichan*NSIZ2+i];
        phase = xpha[ichan*NSIZ2+i];
        ph = usb*maxdel*2.0*PI*(frqq[ichan]+usb*i*32e-3/aa);
        xspecr[k*NSIZ2*NCHAN+ichan*NSIZ2+i] = amp * cos(phase - ph);
        xspeca[k*NSIZ2*NCHAN+ichan*NSIZ2+i] = amp * sin(phase - ph);
        }
      }
     }
    k = *maxrate; j = maxdelj;
    max = delp[k*NSIZ2*NCHAN+j];
    if(j > 0 && j < NSIZ2*NCHAN - 1) {
    y[1] =  delp[k*NSIZ2*NCHAN+j];
    y[0] = delp[k*NSIZ2*NCHAN+j-1];
    y[2] = delp[k*NSIZ2*NCHAN+j+1];
//    printf("y0 y1 y2 %f %f %f k %d j %d\n",y[0],y[1],y[2],k,j);
    aa = (2.0*y[1]-y[0]-y[2])/2.0;
    bb = (y[2]-y[0])/(4.0*aa);
    max = y[1] + aa*bb*bb;
//    printf("maxdel %f bb %f\n",maxdel,bb);
    if(fabs(bb) < 1.0) maxdel += bb*delw/(NSIZ2*8);
    }
    if(k > 0 && k < d1.nrate - 1) {
    y[1] =  delp[k*NSIZ2*NCHAN+j];
    y[0] = delp[(k-1)*NSIZ2*NCHAN+j];
    y[2] = delp[(k+1)*NSIZ2*NCHAN+j];
    aa = (2.0*y[1]-y[0]-y[2])/2.0;
    bb = (y[2]-y[0])/(4.0*aa);
    max = max * (y[1] + aa*bb*bb)/y[1];
//    printf("y0 y1 y2 %f %f %f\n",y[0],y[1],y[2]);
    }
    *maxamp = max;
    if(d1.fxcor) eff = 0.9;
    else eff = 2.0/PI;
    printf("%s maxdel %8.2f res %8.4f maxrate %8.4f Hz maxamp %5.2f SNR %5.1f\n",startt, moff*15.625+delaya-maxdel, -maxdel,  (*maxrate-d1.nrate/2)*d1.srhz, 1e4*max,max*sqrt(num*2500.0*(d1.nnchan))*eff);
    return maxdel;
}


double vgeom(int yr, int day, int hr, int mn, double sec, double *rate)
{
double gpsLat,gpsLon,x1,y1,z1,x2,y2,z2,bx,by,bz,secs,ra,dec,gha,ha,az,el,b_r,b_east,b_north,sx,sy,sz,delay0,delayrate;
double dgha,raout,decout;


                        x1 = d1.xyz1[0]; y1 = d1.xyz1[1]; z1 = d1.xyz1[2];
                        x2 = d1.xyz2[0]; y2 = d1.xyz2[1]; z2 = d1.xyz2[2];
                        bx = x2 - x1;
                        by = y2 - y1;
                        bz = z2 - z1;
                        if(d1.aut) bx=by=bz=0;
                        d1.bx=bx; d1.by=by; d1.bz=bz;
                        secs = tosecs(yr, day, hr, mn, sec);
                        ra = d1.ra; dec = d1.dec;
                        precess(ra,dec,&raout,&decout,2000,yr);
                        ra = raout, dec = decout;
                        gha = gst(secs) - ra; // gha  = Greenwich hour angle
                        gpsLon = atan2(y1,x1)*180.0/PI;
                        gpsLat = atan2(z1,sqrt(x1*x1+y1*y1))*180.0/PI;
                        ha = gha + gpsLon * PI / 180.0; // gst = Greenwich sidereal time, ha = hour angle
                        radec_azel(ha, dec, gpsLat * PI / 180.0, &az, &el);
                        d1.refel = el*180.0/PI;
//                        if(day==0 || fabs(sec -(int)sec) < 1e-5)    printf("bx %f by %f bz %f az %f el %f ra %f dec %f\n",bx,by,bz,az*180.0/PI,el*180.0/PI,ra*12.0/PI,dec*180.0/PI);
                        // convert from bx,by,bz to projected baselines b_north,b_east
                        b_r = bx * cos(gha) - by * sin(gha);
                        b_east = by * cos(gha) + bx * sin(gha);
                        b_north = bz * cos(dec) - b_r * sin(dec);
                        // sx,sy,sy = source direction vector
                        sx = cos(dec) * cos(gha);
                        sy = -cos(dec) * sin(gha);
                        sz = sin(dec);
                        delay0 = -(bx * sx + by * sy + bz * sz); // in meters from dot product of vectors
                        dgha = 2.0 * PI / 86164.09053; // sidereal rate
                        delayrate = (bx * cos(dec) * sin(gha) + by * cos(dec) * cos(gha)) * dgha;
                        delay0 = delay0 * 1e09 / VELC; // in nanoseconds +ve means late at second site
                        delayrate = delayrate * 1e09 / VELC; //nanoseconds/second
//                        printf("%04d:%03d:%02d:%02d:%f delay0 %f delayrate %f %f Hz xyz1 %f %f %f xyz2 %f %f %f\n",
//                                      yr,day,hr,mn,sec,delay0,delayrate,delayrate*d1.freq,x1,y1,z1,x2,y2,z2);
      *rate = delayrate;
      return delay0;
}

/**============================================================================
 ** routine to convert latitude and longitude to x,y,z coords
 ** lat,lon in degrees, lon +v east hgt in meters, outputs x,y,z in meters
 **==========================================================================*/
void latlon2xyz(double lat, double lon, double hgt, double *x, double *y, double *z)
{
    double deg_to_rad, a, e, f, n;
    deg_to_rad = PI / 180.0;
    f = 1.0 / 298.257223563;    /* WGS 84 */
    e = 2.0 * f - f * f;
    a = 6378137.0;              /* WGS 84 earth radius */

    n = a / sqrt(1.0 - e * sin(lat * deg_to_rad) * sin(lat * deg_to_rad));
    *x = (n + hgt) * cos(lat * deg_to_rad) * cos(lon * deg_to_rad);
    *y = (n + hgt) * cos(lat * deg_to_rad) * sin(lon * deg_to_rad);
    *z = (n * (1.0 - e) + hgt) * sin(lat * deg_to_rad);

}

/* Approximate Precession */
void precess(double rain, double decin, double *praout, double *pdecout,
	 double epin, double epout)
{
  *pdecout = decin + 0.0000972 * cos (rain) * (epout - epin);
  *praout = rain +
    ((0.000223 + 0.0000972 * sin (rain) * tan (decin)) * (epout - epin));
}

void radec_azel(double ha, double dec, double latt, double *azs, double *elevs)
{
/* convert from sky to antenna coords (azel mount) */
/* input: ha,dec,latt
   output: azs=azimuth of source
   elevs=elevation of source
 */
  double p, w, r, zen, north;
  p = sin (dec);
  w = sin (ha) * cos (dec);
  r = cos (ha) * cos (dec);
  zen = r * cos (latt) + p * sin (latt);
  north = -r * sin (latt) + p * cos (latt);
  *elevs = atan2 (zen, sqrt (north * north + w * w));
  *azs = atan2 (-w, north);
  if (*azs < 0)
    *azs = *azs + TWOPI;
}



void mjdate(int mjd, int *yr, int *day)
{int jd,hr,min,sc;
 double sec;
 jd = 2455927 - 2400000;  // mjd for 1 Jan 2012
 if(mjd < 927) mjd += 1000; // update in 2014
 jd = (jd / 1000)*1000+mjd;
 sec = (jd-40587)*24.0*3600.0;  // 2440587 for 1 Jan 1970
 toyrday(sec,yr,day,&hr,&min,&sc);
// if(mjd >= 832 ||  mjd <= 196) {*yr = 2009; *day = mjd - 831;}   //54832
//   if(mjd >= 197 &&  mjd <= 561) {*yr = 2010; *day = mjd - 196;}   //55197
//   if(mjd >= 562 && mjd < 832) {*yr = 2011; *day = mjd - 561;}   //55562
// if(*day < 0) *day += 1000;
}


/* Convert Seconds to Yr/Day/Hr/Min/Sec */
void
toyrday (double secs, int *pyear, int *pday, int *phr, int *pmin, int *psec)
{
  double days, day, sec;
  int i;

  day = floor (secs / 86400.0);
  sec = secs - day * 86400.0;
  for (i = 1970; day > 365; i++)
    {
      days = ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0) ? 366.0 : 365.0;
      day -= days;
    }
  *phr = sec / 3600.0;
  sec -= *phr * 3600.0;
  *pmin = sec / 60.0;
  *psec = sec - *pmin * 60;
  *pyear = i;
  day = day + 1;
  *pday = day;
  if (day == 366)               // fix for problem with day 366
    {
      days = ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0) ? 366 : 365;
      if (days == 365)
        {
          day -= 365;
          *pday = day;
          *pyear = i + 1;
        }
    }
}


/* Convert to Seconds since New Year 1970 */
double tosecs(int yr, int day, int hr, int min, double sec)
  {
    int i;
    double secs;
      secs = (yr - 1970) * 31536000.0 + (day - 1) * 86400.0
    + hr * 3600.0 + min * 60.0 + sec;
    for (i = 1970; i < yr; i++)
    {
      if ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0)
        secs += 86400.0;
    }
    if (secs < 0.0)
        secs = 0.0;
    return secs;
  }

double gst(double ttime)
{
  double secs, pdum;
  int i;
//  secs = (1999 - 1970) * 31536000.0 + 17.0 * 3600.0 + 16.0 * 60.0 + 20.1948;
  secs = (2009 - 1970) * 31536000.0 + 17.0 * 3600.0 + 14.0 * 60.0 + 03.807;
//  for (i = 1970; i < 1999; i++)
  for (i = 1970; i < 2009; i++)
    {
      if ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0)
        secs += 86400.0;
    }

  return (modf ((ttime - secs) / 86164.09053, &pdum) * TWOPI);
/* 17 16 20.1948 UT at 0hr newyear1999 */
}




void plot(double xspecr[], double xspeca[], double delp[], double ratep[], int num, int chan,char *filename1,char *filename2,
                    int tbit,double delaya,double bstate[],double bcount[],double maxdel,double freqoff,double maxrate,double maxamp)
{
    int x,y,i,iter,ichan,k;
    double xx1,xx2,yy1,yy2,h,s,b,xoff,yoff;
    double amp,phase;
    double avamp,namp,scale,mxd,snr;
    time_t now;
    FILE *file3;

       if ((file3 = fopen("vlbi0.pos", "w")) == NULL) {
            printf(" output error\n");
            return;
       }

        fprintf(file3, "%%!PS-Adobe-\n%c%cBoundingBox:  0 0 612 792\n%c%cEndProlog\n", '%', '%', '%', '%');
        fprintf(file3, "1 setlinewidth\n");
        fprintf(file3, "/Times-Roman findfont\n 8 scalefont\n setfont/\n");
      xoff = 40; yoff = 100;

      xx1 = xoff; xx2=xx1+500.0;
      yy1 = yoff; yy2=yy1+300.0+150.0+150.0;
      h=s=b=0;
      fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n %2.1f %2.1f %2.1f sethsbcolor stroke\n"
                    ,xx1, yy1, xx2, yy1, h, s, b);
      fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n %2.1f %2.1f %2.1f sethsbcolor stroke\n"
                    ,xx1, yy1+150.0, xx2, yy1+150.0, h, s, b);
      fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n %2.1f %2.1f %2.1f sethsbcolor stroke\n"
                    ,xx1, yy1+300.0, xx2, yy1+300.0, h, s, b);
      fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n %2.1f %2.1f %2.1f sethsbcolor stroke\n"
                    ,xx1, yy1+450.0, xx2, yy1+450.0, h, s, b);
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
	for(iter=0;iter<4;iter++){
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
        if(usb == 1) xx2=x+(i+ichan*NSIZ2)*500.0/(NSIZ2*16.0);
        else xx2 = x-(i+ichan*NSIZ2)*500.0/(NSIZ2*16.0)+(NSIZ2-1+(chan-1)*NSIZ2)*500.0/(NSIZ2*16.0);
//        if(iter==0) yy2 = y+amp*75.0*100.0;
        if(iter==0) yy2 = y+amp*75.0*scale;
	if(iter == 1) yy2 = phase * 75.0 / PI  + y + 150.0 + 75.0;
	if(iter == 2) yy2 = y + delp[i+ichan*NSIZ2]*150.0/mxd + 300.0;
	if(iter == 3) {
             k = (((double)i+ichan*NSIZ2)*d1.nrate)/((double)chan*NSIZ2) + 0.5;
             if(k >= d1.nrate) k = d1.nrate - 1;
             yy2 = y + ratep[k]*150.0/mxd + 450.0;
        }
	if(iter == 0 && yy2 > y+150.0) yy2=y+150.0;
	if(i==0){xx1=xx2; yy1=yy2;}
	         h=s=b=0;
                  s=b=1;
                  h = ichan*0.67/chan;
                  if (h > 0.67) h = 0.67;
		  if (h < 0.0) h = 0.0;
		 if(iter) h=s=b=0;
	if(!iter || (iter && (i>10 && i<NSIZ2-10)&& fabs(yy2-yy1)<37.5) || iter >= 2)
          fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n %2.1f %2.1f %2.1f sethsbcolor stroke\n"
                    ,xx1, yy1, xx2, yy2, h, s, b);
                xx1 = xx2; yy1 = yy2;
		}
        if(usb == 1) xx1=xx2=x+(ichan*NSIZ2)*500.0/(NSIZ2*16.0);
        else xx1=xx2=x-(ichan*NSIZ2)*500.0/(NSIZ2*16.0)+((chan-1)*NSIZ2)*500.0/(NSIZ2*16.0);
	yy1=yoff; yy2=yoff+300.0;
	fprintf(file3, "0 0 0 sethsbcolor\n");
	fprintf(file3, "newpath\n %5.1f %5.1f moveto \n %5.1f %5.1f lineto\n 0 0 0 sethsbcolor stroke\n"
			,xx1, yy1, xx2, yy2);
	fprintf(file3,"%5.1f %5.1f moveto\n (%2d) show\n",xx1+10,yoff-10,ichan);
	fprintf(file3,"%5.1f %5.1f moveto\n (%6.4f) show\n",xx1+10,yoff-35,avamp/namp);
        if(iter < 2){
        if(tbit && ichan==0) fprintf(file3,"%5.1f %5.1f moveto\n (bstate%d) show\n",12.0,40.0-iter*10,iter);
        if(tbit) fprintf(file3,"%5.1f %5.1f moveto\n (%4.2f) show\n",xx1+10,40.0-iter*10,2.8*bstate[ichan+iter*16]/(bcount[ichan+iter*16]+1e-6));
// opt threshold ~ 0.93 sigma for which cumulative normal disi 0.18
        }
	       }
               }
        /* write bottom of page */
        x = 100; y = 60.0;
        now = time(NULL);
	fprintf(file3, "0 0 0 sethsbcolor\n");
        fprintf(file3, "/Times-Roman findfont\n 10 scalefont\n setfont\n");
	fprintf(file3,"290 80 moveto\n (bits/sam %d delay_ns %12.2f res %5.2f frq %4.0f rate %4.2f Hz) show\n",
                                           tbit+1,delaya+moff*15.625-maxdel,-maxdel,freqoff,(maxrate-d1.nrate/2)*d1.srhz);
	if(d1.joff) fprintf(file3,"20 80 moveto\n (offset %6.0f Hz) show\n",d1.foffset);
        if(d1.fxcor)   snr = mxd*sqrt(num*2500.0*(chan-1))*0.9;
        else snr = mxd*sqrt(num*2500.0*(chan-1))*2.0/PI;
	fprintf(file3,"490 710 moveto\n (corr %5.1e SNR  %5.1f) show\n",maxamp,snr);
        if(usb == 1) fprintf(file3,"40 710 moveto\n (duration %8.3f sec USB) show\n",num*2500.0/64e6);
        if(usb == -1) fprintf(file3,"40 710 moveto\n (duration %8.3f sec LSB) show\n",num*2500.0/64e6);
        fprintf(file3,"155 710 moveto\n (%7.3f GHz moff %d RA %02d:%02d:%05.2f DEC %3d:%02d:%05.2f bxyz %6.2f %6.2f %6.2f) show\n",
                        freqq0,moff,d1.rah,d1.ram,d1.ras,d1.decd,d1.decm,d1.decs,d1.bx,d1.by,d1.bz);
        fprintf(file3,"180 80 moveto\n (DBE channel number) show\n");
	fprintf(file3,"20 52 moveto\n (file1: %s file2: %s) show\n",filename1,filename2);
	fprintf(file3,"30 150 moveto\n 90 rotate\n (amplitude) show\n -90 rotate\n");
	fprintf(file3,"30 300 moveto\n 90 rotate\n (phase) show\n -90 rotate\n");
	fprintf(file3,"30 400 moveto\n 90 rotate\n (amp vs delay %4.0f to %4.0f ns) show\n -90 rotate\n",-NLAG*512.0/128.0,NLAG*512.0/128.0);
	fprintf(file3,"30 550 moveto\n 90 rotate\n (amp vs rate %5.2f  to %5.2f  Hz) show\n -90 rotate\n",-d1.nrate*d1.srhz/2.0,d1.nrate*d1.srhz/2.0);
        fprintf(file3, "/Times-Roman findfont\n 8 scalefont\n setfont\n");
        fprintf(file3,"%d %d moveto\n (%s) show\n",20,20,startt);
        fprintf(file3,"%d %d moveto\n (%s) show\n",x+20+300,20,ctime(&now));
        fprintf(file3,"%d %d moveto\n (vlbi0 program ver %d) show\n",x+250,20,d1.ver);
        fprintf(file3, "showpage\n%c%cTrailer\n", '%', '%');
        fclose(file3);
}



void
Four (double *fft_r, double *fft_i, int nn, int sgn)
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
      theta = -(6.28318530717959 / mmax)*sgn;
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
