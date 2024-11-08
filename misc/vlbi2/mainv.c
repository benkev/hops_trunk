#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

extern int process2(char *filename1, char *filename2,
                    char *filenameout, char *filenamedata,
		    int smode, double swidth, double fwidth,int rev,int tbit,
		    double ampfac, int verb);

int main(int argc, char *argv[])
{
    char filenamein1[256], filenamein2[256], buf[80];
    char filenameout[256], filenamedata[256];
    int proctime;
    int rev;
    int tbit;
    int i;
    int verb;
    double swidth;    
    double fwidth;
    double ampfac = 1.0;

    proctime = 100;
    swidth = 3000;
    fwidth = 0;
    rev = 0;
    tbit = 0;
    verb = 2;
    filenamein1[0]=filenamein2[0]=0;
    strcpy(filenameout, "dd1.pos");
    filenamedata[0] = 0;
    if(argc < 2 || (!strcmp(argv[1], "--help"))) { 
      printf("%s file1.m5b file2.m5b -proctime frames\n    "
	     " -rev 0|1 -2bit 0|1 -ampfac 1.0 -verb 0|1|2"
             " -ps plotfile -data datafile\n", argv[0]);
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
    if(strstr(buf, "-ampfac")) sscanf(argv[i+1],"%lf",&ampfac);
    if(strstr(buf, "-verb")) sscanf(argv[i+1],"%d",&verb);
    if(strstr(buf, "-ps")) strncpy(filenameout, argv[i+1], 255);
    if(strstr(buf, "-data")) strncpy(filenamedata, argv[i+1], 255);
     }
    process2(filenamein1,filenamein2,
	filenameout, filenamedata,
        proctime,swidth,fwidth,rev,tbit, ampfac, verb);
}


