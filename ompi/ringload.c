/*
 * $Id$
 *
 * ring sending example from
 * http://mpitutorial.com/tutorials/mpi-send-and-receive/
 // Author: Wes Kendall
 // Copyright 2011 www.mpitutorial.com
 // This code is provided freely with the tutorials on mpitutorial.com. Feel
 // free to modify it for your own use. Any distribution of the code must
 // either provide a link to www.mpitutorial.com or keep this header in tact.
 //
 // Example using MPI_Send and MPI_Recv to pass a message around in a ring.
 //
 // Modified to send a larger quantity of data around the ring
 // and to cycle multiple times.
 *
 * mpirun -bynode -n N -H `hostname`,... ringload count cycles
 */

#include <assert.h>
#include <libgen.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

/* for top level management of work */
typedef struct loop {
    int		    datum;  /* place where current trip is stored */
    int		    cycle;  /* current working cycle */
    int             total;  /* total number of cycles */
    int		    bsize;  /* size of buffer being passed */
    int             paths;  /* number of paths (world_size) */
} Loop;

/* structures to hold per-trip data */
typedef struct info {
    struct timeval  sent;   /* written by sender */
    double	    gbps;   /* calculated by receiver */
} Trip;

/* wrapper union for everything */
typedef union data {
    Loop    loop;	    /* top-level information */
    Trip    trip;	    /* per-trip data */
} Data;

/*
 * Load the current time into the position indicated by the loop datum
 */
void load_data(Data *data, int rank)
{
    struct timeval *tvp;
    data->loop.datum += 1;
    tvp = &data[data->loop.datum].trip.sent;
    gettimeofday(tvp, 0);
}

/*
 * Describe the data contents, and compute & report the rate.
 */
static void describe_data(Data *data, int rank, int from, int wsize)
{
    struct timeval *tvp, *tvo, now;
    double secs, clck;
    gettimeofday(&now, 0);
    tvo = &data[1].trip.sent;
    tvp = &data[data->loop.datum].trip.sent;
    clck = (double)(tvp->tv_sec - tvo->tv_sec);
    clck += (double)(tvp->tv_usec - tvo->tv_usec) * 1e-6;
    secs = (double)(now.tv_sec - tvp->tv_sec);
    secs += (double)(now.tv_usec - tvp->tv_usec) * 1e-6;
    data[data->loop.datum].trip.gbps = 1e-9 * (data->loop.bsize * 8.0) / secs;
    fprintf(stdout,
	"%12.6fs %02d<-%02d at %5d / cycle %5d at %7.3f Gbps (%.3fs)\n",
	clck, rank, from, data->loop.datum, data->loop.cycle,
	data[data->loop.datum].trip.gbps, secs);
}

/*
 * Final report on data
 */
static void report_hostdata(Data *data, char *hn, int step, int n)
{
    int ii, jj;
    double averate, minrate, maxrate, rate;
    char *hp, *ht;
    fprintf(stdout,
	"With buffer size %g MB [world %d = %d loops, ave, (min max)]\n",
	(double)data->loop.bsize * 1e-6, n, data->loop.paths);
    for (ii = 0; ii < data->loop.paths; ii++) {
	averate = 0.0;
	minrate = 1000;
	maxrate = 0.0;
	for (jj = 0; jj <= data->loop.total; jj++) {
	    rate = data[1 + ii + jj * data->loop.paths].trip.gbps;
	    averate += rate;
	    if (rate > maxrate) maxrate = rate;
	    if (rate < minrate) minrate = rate;
	}
	averate /= (data->loop.total + 1);
        hp = hn + step*( (ii+1) % data->loop.paths);
        ht = hn + step*ii;
	fprintf(stdout,
            " %12s]%02d<-%02d[%-12s %7.3f Gbps (%7.3f %7.3f)\n",
            hp, (ii+1) % data->loop.paths, ii, ht, averate, minrate, maxrate);
    }
}

/*
 * Initialize the data buffer.
 */
static void init_data(Data *data,
    char *name, int cycles, int bsize, int world_size)
{
    fprintf(stdout, "Starting %s team of %d for cycles %d data packet %dB\n",
	name, world_size, cycles, bsize);
    data->loop.datum = 0;
    data->loop.bsize = bsize;
    data->loop.paths = world_size;
    data->loop.total = cycles;

    assert(sizeof(Trip) >= sizeof(Loop));
}

/*
 * Main loop
 */
int main(int argc, char** argv)
{
    int count = (argc>1) ? atoi(argv[1]): 1;
    int cycles = (argc>2) ? atoi(argv[2]): 1;
    int icnt, bsize, world_rank, world_size, clen;
    char host[200], *hostnames = NULL;
    Data *data;

    /* help the human */
    if (argc > 1) {
	clen = strlen(argv[1]);
	switch (argv[1][clen-1]) {
	case 'k': count *= 1000; break;
	case 'M': count *= 1000000; break;
	case 'G': count *= 1000000000; break;
	default: break;
	}
    } 

    /* Initialize the MPI environment */
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    /* general data initialization */
    bsize = (world_size + 3) * cycles * sizeof(Data);
    if (count > bsize) bsize = count;
    icnt = bsize / sizeof(int);
    bsize = icnt * sizeof(int);
    data = (Data *)(malloc(bsize));
    if (!data) return(perror("malloc"),1);
    if (world_rank == 0)
	init_data(data, basename(argv[0]), cycles, bsize, world_size);

    gethostname(host, sizeof(host));
    fprintf(stdout, " host at rank %02d is %s\n", world_rank, host);

    if (world_rank == 0) {
        hostnames = malloc(world_size * 200);
        if (!hostnames) return(1);
    }
    MPI_Gather(host, 200, MPI_CHAR, hostnames, 200,
        MPI_CHAR, 0, MPI_COMM_WORLD);
    
    data->loop.cycle = -1;
    while (cycles > data->loop.cycle) {
        if (world_rank != 0) {
	    /* Receive from the lower and send to the higher process. */
            MPI_Recv(data, icnt, MPI_INT, world_rank - 1, 0,
		MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    describe_data(data, world_rank, world_rank - 1, world_size);
        } else {
	    data->loop.cycle += 1;
	}

        load_data(data, world_rank);
        MPI_Send(data, icnt, MPI_INT, (world_rank + 1) % world_size, 0,
            MPI_COMM_WORLD);

        if (world_rank == 0) {
	    /* Now process 0 can receive from the last process. */
            MPI_Recv(data, icnt, MPI_INT, world_size - 1, 0,
		MPI_COMM_WORLD, MPI_STATUS_IGNORE);
	    describe_data(data, world_rank, world_size - 1, world_size);
        }
    }
    MPI_Finalize();

    if (world_rank == 0) report_hostdata(data, hostnames, 200, world_size);
    return(0);
}

/*
 * eof
 */
