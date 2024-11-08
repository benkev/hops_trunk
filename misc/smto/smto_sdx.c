/*
 * $Id$
 *
 * Program to take apart SMTO sdd files;
 * Written by AEER, hacked into oblivion by GBC.
 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

/* plots/dumps something */
void dumpd(char *,char *,int,double *, double *);
void plotf(char *,char *,int,double *, double *);

/* do something with saturn */
double saturn(double *,double);

/* some sort of file type */
typedef struct {
    int numrec,numdata,bytes_per_rec,bytes_per_index,
	num_used,counter,sddtyp,sddver,pad[512];
} SddType;

/* some sort of scan record */
typedef struct {
    int scanstart,scanstop;
    float hor,vert;
    char scan[16];
    float scannum,fres;
    double restfreq;
    float lst,utdate;
    short mode,recnum,poscode,pad[512];
} SddItype;

/* some sort of class type */
typedef struct {
     short numclass,classstart[15];
} SddCtype;

static double tim[100000],pdata[100000];
// static int tstart, tstop, ttstart, ttstop;

static char *plotfile = "sdd-plot-%d.ps";
static int noplot = 0;
static int verb = 0;
static int min_record = 0, max_record = 99999;

/*
 * Parser of the SDD file.
 */
int parse_file(char *fname, char *pfile)
{
    int i,j,line,line2,np,hr,min,hlen,dlen;
    double dy,scan,tsat,h,tsatt;
    double r,tamb,tsys,freq;
    double pvane;
    double pmax,pmin;
    double sonn,nss;
    SddType bt;
    SddItype in;
    SddCtype sc;
    double data[1024];
    float sdata[2048];
    FILE *file1;
    int bo = 0, tbo = 0;

    if ((file1 = fopen(fname, "r")) == NULL || (bo=0)) {
        printf("cannot open file:%s\n", fname);
        return 0;
    }

    line = line2 = 0; np=0;
    pmax = -1e99; pmin = 1e99;
    pvane=0;
    bt.numrec=0;
    fread(&bt,512,1,file1);	bo+=512;

    printf("file %s\n", fname);
    printf("numrec %d numdata %d bytes_per_rec %d bytes_per_index %d "
	   "num_used %d counter %d sddtyp %d sddver %d pad %d\n",
	bt.numrec,bt.numdata,bt.bytes_per_rec,bt.bytes_per_index,
	bt.num_used,bt.counter,bt.sddtyp,bt.sddver,bt.pad[0]);

    /*
     * Loop over data entries
     *  There appear to be numrec*512-byte-blocks for "index"
     *  entries that take 64 bytes.  What happens after 16384
     *  entries is beyond me--hopefully we don't get that far.
     */
    i=0;
    while(!feof(file1) && i < bt.num_used) {

	if (i < min_record) goto bail;
	if (i > max_record) goto bail;

	printf("### Byte offset: %d %d %d\n", bo, i, tbo);

	fread(&in,64,1,file1);	bo+=64;
	printf("i %d start %d stop %d source %.6s obscode %d\n",
		i,in.scanstart,in.scanstop,in.scan,in.mode-256);
	fclose(file1);
	if ((file1 = fopen(fname, "r")) == NULL || (bo=0)) {
	    printf("cannot open file:%s\n", fname);
	    return 0;
	}

	for(j=0;j<in.scanstart-1;j++) {
	    fread(data,512,1,file1);	bo+=512;
	}
	fread(&sc,32,1,file1);		bo+=32;
	fread(data,8,1,file1);		bo+=8;
	hlen = data[0];
	if (hlen-8-32 < 0) {
	    printf("### !!! Illegal read length %d\n", hlen-8-32);
	    goto bail;
	}
	printf("hlen %d sc.classstart[0] %d\n",
		hlen,sc.classstart[0]);
	fread(&data[1],hlen-8-32,1,file1);  bo+=(hlen-8-32);

	printf("### Byte offset: %d %6.6s\n", bo, in.scan);

	hr = (int)(data[sc.classstart[2]-5+1]);
	min = (data[sc.classstart[2]-5+1] - hr)*60.0;
	dlen = data[sc.classstart[0]-5+1];
	printf("%9.4f %02d:%02d %.6s scan "
		"%3.0f tsys %3.0f tamb %3.0f opac %5.3f el %3.0f"
		" %.8s %.8s\n",
	    data[sc.classstart[2]-5],
	    hr,min, in.scan,
	    data[sc.classstart[0]-5+2],
	    data[sc.classstart[11]-5+6],
	    data[sc.classstart[4]-5+0],
	    data[sc.classstart[11]-5+16],
	    data[sc.classstart[3]-5+10],
	    (char*)&data[sc.classstart[0]-5+8+2],
	    (char*)&data[sc.classstart[0]-5+9+2]
	 // (char*)&data[sc.classstart[11]-5+22]
	);



	// if...SATURN
	if((strstr(in.scan,"SATURN") || (strstr(in.scan,"aturn")))) {

	printf("###>>Saturn case\n");

//      for(j=0;j<25;j++)
//	    printf("class6 %d %f\n",j,data[sc.classstart[5]-5+j]);

	    fread(sdata,dlen,1,file1);	bo+=dlen;
	    pmax = -1e99; pmin = 1e99;

	    for (j=0;j<dlen/4;j++) {
		if (fabs(data[sc.classstart[0]-5+2] - 44)< 1.0) {
		    printf("j %d data %e\n",j,sdata[j]);
		    tim[np] = j;
		    pdata[np] = sdata[j]*1e-3;     
		    if(sdata[j] > 0.0) np++;
		}
		if (strstr((char*)&data[sc.classstart[0]-5+8+2],"CONTSEQ"))
		    printf("CONTSEQ j %d data %8.1f scan %3.0f\n",
			    j,sdata[j]*1e-3,data[sc.classstart[0]-5+2]);
		if (sdata[j] > pmax) pmax = sdata[j];
		if (sdata[j] < pmin && sdata[j] > 0.0) pmin = sdata[j];
	    }

	    sonn = 0; nss=0;
	    for (j=0;j<dlen/4;j++){
		if (strstr((char*)&data[sc.classstart[0]-5+8+2],"CONTSEQ")) {
		    if (j%8==1 || j%8==2 || j%8==4 || j%8==7) {
			sonn += sdata[j]; nss++;
		    } else {
			sonn += -sdata[j];
		    }
		}
		if (strstr((char*)&data[sc.classstart[0]-5+8+2],"CONTFIVE") &&
		    j >=16 && j<= 23) {
		    if (j%8==1 || j%8==2 || j%8==4 || j%8==7) {
			sonn += sdata[j]; nss++;
		    } else {
			sonn += -sdata[j];
		    }
		}
	    }

	    freq = data[sc.classstart[11]-5]*1e-3;
	    tsat = saturn(&h,freq);
	    tsatt = (pmax-pmin)*1e-3;

	    if (nss > 0) {
		tsatt = sonn*1e-3 / nss;
		if (pmax > 50e3 && pmax < 1000e3 &&
		    (!strstr((char*)&data[sc.classstart[0]-5+8+2],"CONTSTIP")))
			printf("SATURN %9.4f %02d:%02d "
				"scan %3.0f flux %5.0f freq %3.0f eff "
				"%5.0f %.8s %.8s tsys %3.0f el %3.0f\n",
			    data[sc.classstart[2]-5],hr,min,
			    data[sc.classstart[0]-5+2],
			    h,freq,tsatt*50.0/tsat,
			    (char*)&data[sc.classstart[0]-5+8+2],
			    (char*)&data[sc.classstart[0]-5+9+2],
			    data[sc.classstart[11]-5+6],
			    data[sc.classstart[3]-5+10]
		);
	    }

	}
	// if...SATURN

bail:
	tbo = bo;
	fclose(file1);
	if ((file1 = fopen(fname, "r")) == NULL || (bo=0)) {
	    printf("cannot open file:%s\n", fname);
	    return 0;
        }
	for(j=0;j<i+8;j++) {
	    fread(&in,64,1,file1);	    bo+=64;
	}
	i++;
      
    }	// end of while loop over records

    printf("### Byte offset: %d %d %d\n", bo, i, tbo);
    fclose(file1);
    printf("### Have %d points to plot\n", np);
    if(np > 0) {
	if (noplot) dumpd(fname,pfile,np,tim,pdata);
	else        plotf(fname,pfile,np,tim,pdata);
    }
}


/*
 * Computes the temperature of Saturn?
 */
double saturn(double *flux,double freq)
{
    double bw,sd,a,bfc,h,tsat,wav;
    wav = 300.0*1e-3/freq;
    bw = 1.22 * wav / 10;
    sd = 9.7/(57.3*3600.0);	    // semidiam Saturn
    a = 4.0*log(2.0)*sd*sd/(bw*bw); // Argument of exp()
    bfc = a/(1.0-exp(-a));	    // Beam filling correction, dimensionless
    h = 2.0*1.38e-23*100.0*3.14*sd*sd /
	(1.3e-3*1.3e-3);	    // 100 K brightness
    h = h / bfc;
    tsat = 0.5 * 3.1415 * 5.0 * 5.0 * h /
	(2.0 * 1.38e-23);	    // 50% eff 10m dish
    *flux = h*1e26;
    return tsat; 
}


/*
 * Just dump the data to a file
 */
void dumpd(char *fname, char *pfile, int np, double tim[], double data[])
{
    FILE *file;
    int	n;
    if ((file = fopen(pfile, "w")) == NULL) {
	printf("cannot open %s:\n", pfile);
	return;
    }
    fprintf(file, "### Data from %s\n", fname);
    for (n = 0; n < np; n++)
	fprintf(file, "%6d\t%f\t%f\n", n, tim[n], data[n]);
    fclose(file);
}

/*
 * Plots something
 */
void plotf(char *fname, char *pfile, int np, double tim[], double data[])
{
    char txt[256];
    int k, iter;
    double h,s,b, x, y, dmax, dmin, dd, f, totpp, err;
    double xoffset,yoffset;
    double tsat;
    FILE *file;

    if ((file = fopen(pfile, "w")) == NULL) {
        printf("cannot open %s:\n", pfile);
        return;
    }

    /* start the plot */
    fprintf(file, "%%!PS-Adobe-\n%%%%BoundingBox: "
		  " 0 0 612 792\n%%%%EndProlog\n");
    fprintf(file, "1 setlinewidth\n");
    fprintf(file, "/Times-Roman findfont\n 12 scalefont\n setfont\n");

    xoffset = 80.0;
    yoffset = 100.0;
    dmax = -1.0e99;
    dmin = 1e99;
    for (k = 0; k < np; k++) {
          if(data[k] > dmax) dmax = data[k];
          if(data[k] < dmin) dmin = data[k];
    }
    b = (1.2 * 1.3e-3 /10.0) * 57.3 * 3600.0;
    s = 10.0/(57.3*3600.0);
    h = 2.0*1.38e-23*150.0*3.14*s*s/(1.3e-3*1.3e-3);
    tsat = 0.5 * 3.1415 * 5.0 * 5.0 * h / (2.0 * 1.38e-23);
//  printf("ratio %f %f b %f %e\n",dmax/dmin,tsat,b,h);
//  dd = 0.0;
//  j1 = 0;
//  j2 = np;
    dmax *= 1.1;
    dmin *= 0.9;

    /* plot box(es) */
    for(iter=0;iter<1;iter++){
    for (y = 0; y < 2; y++) 
    fprintf(file, "newpath\n %6.2f %6.2f moveto\n"
	"%6.2f %6.2f lineto\n 0 0 0 sethsbcolor stroke\n",
	 xoffset, y * 200 + yoffset+iter*200,
	 xoffset + 400.0, y * 200 + yoffset+iter*200);
    fprintf(file, "newpath\n %6.2f %6.2f moveto\n"
	"%6.2f %6.2f lineto\n 0 0 0 sethsbcolor stroke\n",
	xoffset, yoffset+iter*200,
	xoffset, 200.0 + yoffset+iter*200);
    fprintf(file, "newpath\n %6.2f %6.2f moveto\n"
	"%6.2f %6.2f lineto\n 0 0 0 sethsbcolor stroke\n",
	xoffset + 400.0, yoffset+iter*200,
	xoffset + 400.0, 200.0+yoffset+iter*200);
    fprintf(file, "newpath\n %6.2f %6.2f moveto\n"
	"%6.2f %6.2f lineto\n 0 0 0 sethsbcolor stroke\n",
	xoffset + 0.0, yoffset+iter*200,
	xoffset + 0.0, 200.0+yoffset+iter*200);
    }

    h=s=b=0;
//  yp = 0;
//  xp = 0;
    totpp = 0;
    err = 0.1;

    /* plot points */
    for(iter=0;iter<1;iter++) {
	for (k = 0; k < np; k++) {
	    x = (tim[k]-tim[0]) * 400.0/(tim[np-1]-tim[0]);
	    totpp = data[k];
	    y = (totpp-dmin) * 200.0/ (dmax-dmin);
	    if (y > 200)
		y = 200;
		y += iter*200;
//		yp=y;
		fprintf(file, "newpath\n %5.1f %5.1f %5.1f 0 360 arc\n"
		    "closepath\n stroke\n",
		    x + xoffset,y + yoffset,1.0);
		fprintf(file, "newpath\n %6.2f %6.2f moveto\n"
		    "%6.2f %6.2f lineto\n"
		    "%5.3f %5.3f %5.3f sethsbcolor stroke\n",
		    x + xoffset, y+err + yoffset,
		    x + xoffset, y-err + yoffset, h,s,b);
		fprintf(file, "newpath\n %6.2f %6.2f moveto\n"
		    "%6.2f %6.2f lineto\n"
		    "%5.3f %5.3f %5.3f sethsbcolor stroke\n",
		    x - 4 + xoffset, y+err + yoffset,
		    x + 4 + xoffset, y+err + yoffset, h,s,b);
		fprintf(file, "newpath\n %6.2f %6.2f moveto\n"
		    "%6.2f %6.2f lineto\n"
		    "%5.3f %5.3f %5.3f sethsbcolor stroke\n",
		    x - 4 + xoffset, y-err + yoffset,
		    x + 4 + xoffset, y-err + yoffset, h,s,b);
	} 

    }

    /* x ticks and labels */
    for (f = tim[0]; f <= tim[np-1]; f += (tim[np-1]-tim[0])*0.2) {
        x = (f-tim[0]) * 400.0 / (tim[np-1]-tim[0]);
        y = 0.0;
        fprintf(file, "newpath\n %6.2f %6.2f moveto\n"
	    "%6.2f %6.2f lineto\n 0 0 0 sethsbcolor stroke\n",
	    x + xoffset,y+yoffset,
	    x + xoffset,y - 10.0+yoffset);
        sprintf(txt, "%.4g", f);
        fprintf(file, "%6.2f %6.2f moveto\n (%s) show\n",
	    x + xoffset - 10.0,y - 20.0+yoffset, txt);
    }

    /* y ticks and labels */
    for(iter=0;iter<1;iter++){
	for (f = dmin; f < dmax; f += (dmax-dmin)*0.2) {
	    x = 0;
	    y = (f-dmin) * 200.0 / (dmax-dmin);
	    fprintf(file, "newpath\n %6.2f %6.2f moveto\n"
		"%6.2f %6.2f lineto\n 0 0 0 sethsbcolor stroke\n",
		x + xoffset, y+yoffset,
		x + xoffset + 10, y+yoffset);
	    sprintf(txt, "%4.2f", f);
	    fprintf(file, "%6.2f %6.2f moveto\n (%s) show\n",
		x + xoffset - 36.0, y -4.0+yoffset, txt);
	}
	sprintf(txt, "power?");
	fprintf(file, "%6.2f %6.2f moveto\n 90 rotate\n"
	    "(%s) show\n -90 rotate\n",
	    xoffset -40.0, 265.0 - 100.0, txt);
    }

    // sprintf(txt, "fraction of day - scan");
    sprintf(txt, "some sort of time axis");
    fprintf(file, "%6.2f %6.2f moveto\n (%s) show\n",
	xoffset + 180.0, 60.0, txt);
    sprintf(txt, "file: %s", fname);
    fprintf(file, "%6.2f %6.2f moveto\n (%s) show\n",
	380.0+xoffset, 50.0, txt);
    fprintf(file, "showpage\n%%%%Trailer\n");
    fclose(file);
}

int recmm(char *arg)
{
    int mn,mx;
    if (2 == sscanf(arg, "%d,%d", &mn, &mx)) {
	min_record = mn;
	max_record = mx;
	return(0);
    }
    return(1);
}

/*
 * Command line parsing and other boilerplate...
 */
int cmdline(int *argc, char ***argv)
{
    int	c, errs = 0;
    while ((c = getopt(*argc, *argv, "vr:d")) > 0) switch(c) {
    case 'v': verb++;			    break;
    case 'd': noplot++;			    break;
    case 'r': if (recmm(optarg))	    return(2);
	      else			    break;
    default:				    return(1);
    }
    *argc -= optind;
    *argv += optind;
    return(*argc > 0 ? 0 : 1);
}

int usage(char *name)
{
    printf("Usage: %s [options] sddfile[s]\n\n"
	"where the options are:\n"
	"  -v          make it more verbose\n"
	"  -r min,max  records to process\n"
	"  -d          dump data to file, not a plot\n"
	"\n"
	"\n"
	, name);
    return(1);
}

int main(int argc, char **argv)
{
    int errs = 0, pc = 0;
    char pf[1024], *n = *argv;
    if (cmdline(&argc, &argv)) return(usage(n));
    while (argc-- && !errs) {
	snprintf(pf, 1024, plotfile, pc++);
	errs += parse_file((n = *argv++), pf);
	printf("### Parsed [%d] %s -> %s with %d errs.\n",
	    pc, n, pf, errs);
    }
    return(errs);
}

/*
 * eof
 */
