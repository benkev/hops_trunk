/*
 * $Id$
 *
 * A deviant version of ringload that shovels to all in parallel.
 *
 * mpirun -bynode -n N -H `hostname`,... all_load count cycles
 */

#include <libgen.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

/* structures to hold per-trip data */
typedef struct info {
    struct timeval  sent;   /* written by sender  (dir = 0)*/
    struct timeval  recv;   /* written by receiver(dir = 1)*/
    struct timeval  done;   /* written by sender  (dir = 2)*/
    double	    gbps;   /* average for there-and-back  */
} Data;

/*
 * Load the current time into the cycle/dir-dependent position.
 */
static void load_data(Data *data, int dir, int cycle, int cycles, int wsize)
{
    int ii;
    struct timeval tvp;
    gettimeofday(&tvp, 0);
    switch (dir) {
    case 0:
	for (ii = 0; ii < wsize; ii++) data[cycle + ii*cycles].sent = tvp;
	break;
    case 1:
	for (ii = 0; ii < wsize; ii++) data[cycle + ii*cycles].recv = tvp;
	break;
    case 2:
	for (ii = 0; ii < wsize; ii++) data[cycle + ii*cycles].done = tvp;
	break;
    default:
	MPI_Abort(MPI_COMM_WORLD, 1);
	break;
    }
}

/*
 * Calculate the rates (after each cycle) as ave of sent->recv & recv->done.
 * Note that while we've logged recv, we don't use it.
 * And we have to do this for every correspondent.
 */
static void calc_rate(Data *data, int cycle, int cycles, int bsize, int wsize)
{
    struct timeval *tvs, *tvd;
    double secs;
    int ii, jj;
    for (ii = 0, jj = cycle; ii < wsize; ii++, jj += cycles) {
	tvs = &data[jj].sent;
	tvd = &data[jj].done;
	secs = (double)(tvd->tv_sec - tvs->tv_sec);
	secs += (double)(tvd->tv_usec - tvs->tv_usec) * 1e-6;
	data[jj].gbps = 2.0 * (double)bsize / secs;
    }
}

/*
 * Final report on data
 */
static void report_data(Data *data, int cycles, int wrank, int wsize)
{
    int ii, jj;
    double averate, minrate, maxrate, rate;
    fprintf(stdout, "Rank %d: ", wrank);
    for (jj = 0; jj < wsize; jj++) {
	averate = 0.0;
	minrate = 1000;
	maxrate = 0.0;
	for (ii = 0; ii < cycles; ii++) {
	    rate = data[jj*cycles + ii];
	    averate += rate;
	    if (rate > maxrate) maxrate = rate;
	    if (rate < minrate) minrate = rate;
	}
	averate /= (double)cycles;
	fprinf(stdout, " %6.2f<%6.2f<%6.2f", averate, minrate, maxrate);
    }
    fprintf(stdout, "\n");
}

/*
 * Main loop
 */
int main(int argc, char** argv)
{
    int count = (argc>1) ? atoi(argv[1]): 1;
    int cycles = (argc>2) ? atoi(argv[2]): 1;
    int icnt, bsize, world_rank, world_size;
    char host[200];
    Data *data, *back;

    /* Initialize the MPI environment */
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    /* general data initialization */
    bsize = (1 + cycles) * sizeof(Data);
    if (count > bsize) bsize = count;
    icnt = bsize / sizeof(int);
    bsize = icnt * sizeof(int);
    data = (Data *)(malloc(bsize * world_size));
    back = (Data *)(malloc(bsize * world_size));
    if (!data|| !back) return(perror("malloc"),1);

    /* announce start */
    if (world_rank == 0) fprintf(stdout,
	"Starting %s team of %d for cycles %d packet size %dB\n",
	name, world_size, cycles, bsize);
    gethostname(host, sizeof(host));
    fprintf(stdout, " host at rank %02d is %s\n", world_rank, host);
    
    for (icnt = 0; icnt < cycles; icnt++) {
	/* send data out to everyone */
        load_data(data, 0, icnt, cycles, world_size);
	MPI_Alltoall(data, icnt, MPI_INT, back, icnt, MPI_INT, MPI_COMM_WORLD);

	/* send back data to everyone */
        load_data(back, 1, icnt, cycles, world_size);
	MPI_Alltoall(back, icnt, MPI_INT, data, icnt, MPI_INT, MPI_COMM_WORLD);

	/* synchronize */
        load_data(data, 2, icnt, cycles, world_size);
	calc_rate(data, icnt, cycles, bsize, world_size);
	if (world_rank == 0) fprintf(stdout, "Cycle %d complete.\n", icnt);
	MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Finalize();

    report_data(data, cycles, world_rank, world_size);
    return(0);
}

/*
 * eof
 */
