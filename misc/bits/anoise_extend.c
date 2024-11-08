/*
 * $Id: anoise_extend.c 776 2012-08-29 22:03:23Z gbc $
 *
 * A deviant form of bnoise/vnoise which makes vdif formed data simulating
 * VDIF data in a variety of flavors.  The goal here is to simulate ALMA
 * data and correlate it against Mark5b-like or other flavors of data.
 *
 * This file contains routines to extend the "observation" by
 * replicating data already generated.  This is useful since DiFX
 * really expects a few seconds of data in every scan.
 */

#include "anoise.h"

#include <fcntl.h>
#if HAVE_MREMAP
/* the man page says we need the first of these,
 * and practice tells us we need the second of these
 * just in order to correctly define MREMAP_MAYMOVE */
#define _GNU_SOURCE
#define __USE_GNU
#endif /* HAVE_MREMAP */
#include <sys/mman.h>
#include <sys/stat.h>

/* both work if you have mremap */
#define USE_MREMAP  1

static double   extn = 0.0;     /* extended duration of observation */

/*
 * Support function for option seting from main()
 */
void set_extn(double d)
{
    extn = d;
    if (verb>0) fprintf(stdout, "Extending to %g secs\n", extn);
}

/*
 * The new length in bytes is:
 *   bytes/octect * octets/packet * packets/second * seconds
 * returning 0 if there is a problem.  Since we checked that
 * we *are* extending, the new length should exceed the old.
 */
static size_t new_length(size_t osize, VdifConf *vdcp)
{
    size_t bytes = (size_t)rint(8.0 * vdcp->opp * vdcp->pps * extn);
    size_t oldby = (size_t)rint(8.0 * vdcp->opp * vdcp->pps * dura);
    if (verb>0) fprintf(stdout,
        "Extension increases size from %ld to %ld\n",(long)oldby,(long)bytes);
    if (bytes <= oldby) return(0);  /* new length should be larger */
    if (verb>0) fprintf(stdout,
        "Old size is %d calculated as %d\n", osize, oldby);
    if (oldby != osize) return(0);    /* old should be as calculated */
    return(bytes);
}

/*
 * Clone packets from 0..(len-1) bytes repeatedly until "new" bytes have
 * been added.  The data bytes are directly memcpy'd; the headers are
 * adjusted for the seconds and data frame number within each second.
 * For debugging, the seqh/seql pair is also incremented continously.
 */
profile int clone_packets(VdifConf *vdcp,uint64_t *this,size_t len,size_t new)
{
    uint64_t *first = this, *final, *what, *done = this + new / 8;
    uint64_t pkts = len / (8 * vdcp->opp);
    uint32_t seql, seqh;
    int secre, frame;
    VdifExt *h = (VdifExt*)(what = first);
    if (verb>0) fprintf(stdout,
        "First pkt %p at %d fr %05d\n", first, h->secs_inre, h->df_num_insec);
    this += (pkts-1) * vdcp->opp;
    h = (VdifExt*)(final = this);
    if (verb>0) fprintf(stdout,
        "Final pkt %p at %d fr %05d\n", final, h->secs_inre, h->df_num_insec);
    secre = h->secs_inre;
    frame = h->df_num_insec;
    seql = h->seql;
    seqh = h->seqh;
    if (verb>2) fprintf(stdout,
        "Cloning %p(%x)..%p(%x)\n", first, (int)len, done, (int)new);
    this += vdcp->opp;
    while (this < done) {
        /* update header counters */
        if (++frame >= vdcp->pps) { frame = 0; secre++; }
        if (++seql == 0) { seqh++; }
        if (verb>3) fprintf(stdout,
            "Dup %p to %p now %d fr %05d\n", what, this, secre, frame);
        memcpy((h = (VdifExt*)this), what, 8 * vdcp->opp);
        h->df_num_insec = frame;
        h->secs_inre = secre;
        h->status = h->seql;
        h->seql = seql;
        h->seqh = seqh;
        this += vdcp->opp;
        what += vdcp->opp;
        if (what > final) what = first;
    }
    if (verb>0) fprintf(stdout,
        "Clone pkt %p at %d fr %05d\n", this, h->secs_inre, h->df_num_insec);
    return(0);
}

#if HAVE_MREMAP && USE_MREMAP
#warning "Using HAVE_MREMAP && USE_MREMAP flavor"
/*
 * Memory map the data file already written.  Add new packets.
 */
profile int extend_station(VdifConf *vdcp)
{
    int mmfd;
    off_t result;
    void *start;
    struct stat stb;
    size_t len, new;
    if (stat(vdcp->file, &stb)) return(perror("stat"),1);
    mmfd = open(vdcp->file, O_RDWR);
    if (mmfd < 0) return(perror("open"),2);
    len = stb.st_size;
    new = new_length(len,vdcp);
    if (!new) return(fputs("new length failed\n", stderr),3);

    /*
     * The mremap call generates a bus error if the underlying file
     * doesn't get fully extended before we start changing the memory.
     * It appears that this extension can be anytime after the open()
     * but before the next page of memory of the remap is accessed.
     */
    fprintf(stderr,"seeking to %ld\n", new - 1);
    result = lseek(mmfd, new - 1, SEEK_SET);
    if (result == (off_t)-1) return(perror("lseek"),4);
    result = write(mmfd, "", 1);              /* write a null */
    if (result != 1) return(perror("write"),5);
    
    /* map the file data into memory */
    start = mmap(0, len, PROT_READ|PROT_WRITE, MAP_SHARED, mmfd, 0);
    if (start == MAP_FAILED) return(perror("mmap"),6);
    start = mremap(start, len, new, MREMAP_MAYMOVE);
    if (start == MAP_FAILED) return(perror("mremap"),7);

    /* at this point we're ready to dup packets */
    if (clone_packets(vdcp, (uint64_t*)start, len, new)) return(10);

    if (msync(start, new, MS_SYNC)) return(perror("msync"),11);
    if (munmap(start, new)) return(perror("munmap"),12);
    if (close(mmfd)) return(perror("close"),13);
    return(0);
}
#else /* HAVE_MREMAP */
#warning "Not using HAVE_MREMAP && USE_MREMAP flavor"
/*
 * Plan B.  The mmap() call initialized memory with len bytes from
 * the file, the mremap() call changes the map.  If we don't have
 * mremap, this version tells the kernel to initialize the memory
 * with all the bytes we haven't written (which it probably fills
 * with zero, but you never know, and you don't know if it is more
 * work or not).  In any case these both seem to work.
 */
profile int extend_station(VdifConf *vdcp)
{
    int mmfd;
    off_t result;
    void *start;
    struct stat stb;
    size_t len, new;
    if (stat(vdcp->file, &stb)) return(perror("stat"),1);
    mmfd = open(vdcp->file, O_RDWR);
    if (mmfd < 0) return(perror("open"),2);
    len = stb.st_size;
    new = new_length(len,vdcp);
    if (!new) return(fputs("new length failed\n", stderr),3);
    result = lseek(mmfd, new - 1, SEEK_SET);
    if (result == (off_t)-1) return(perror("lseek"),4);
    result = write(mmfd, "", 1);              /* write a null */
    if (result != 1) return(perror("write"),5);

    /* map the file data into memory */
    start = mmap(0, new, PROT_READ|PROT_WRITE, MAP_SHARED, mmfd, 0);
    if (start == MAP_FAILED) return(perror("mmap"),6);

    /* at this point we're ready to dup packets */
    if (clone_packets(vdcp, (uint64_t*)start, len, new)) return(10);

    if (msync(start, new, MS_SYNC)) return(perror("msync"),11);
    if (munmap(start, new)) return(perror("munmap"),12);
    if (close(mmfd)) return(perror("close"),13);
    return(0);
}
#endif /* HAVE_MREMAP */

/*
 * Data has been generated.  Here then, for each station,
 * extend the data through simple packet replication.
 */
int extend_the_data(void)
{
    int ns, errs = 0;
    if (extn <= dura) return(0);    /* no work */
    if (verb>0) fprintf(stdout, "Cloning from %g .. %g secs\n", dura, extn);
    if (verb>1) report_processing(0,0,"extn",1);
    for (ns = 0; (ns < nstn) && !errs; ns++)
        errs += extend_station(&vcnf[ns]);
    if (verb>1) report_processing(0,0,"extn",1);
    return(0);
}

/*
 * eof
 */
