#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>


int main(int argc, char *argv[])
{
    char filenamein1[256], filenamein2[256], filenameout[256], buf[80];
    int proctime;
    int rev;
    int tbit;
    int i;
    double swidth;    
    double fwidth;

	proctime = 100;
    swidth = 3000;
    fwidth = 0;
    rev = 0;
    tbit = 0;
    filenamein1[0]=filenamein2[0]=0;


	if(argc < 2) { 
      printf("vlbi file1 file2 fileout -proctime proctime -rev rev -2bit 2bit\n");
      return 0;
     }

	if (argc > 1)
        sscanf(argv[1], "%255s", filenamein1);
    if (argc > 2)
        sscanf(argv[2], "%255s", filenamein2);
		if (argc > 3)
        sscanf(argv[3], "%255s", filenameout);

    for(i=0;i<argc;i++){
    sscanf(argv[i], "%79s",buf);
    if(strstr(buf, "-proctime")) sscanf(argv[i+1],"%d",&proctime);
    if(strstr(buf, "-rev")) sscanf(argv[i+1],"%d",&rev);
    if(strstr(buf, "-2bit")) sscanf(argv[i+1],"%d",&tbit);
     }

process2(filenamein1,filenamein2, filenameout, proctime,swidth,fwidth,rev,tbit);

}


