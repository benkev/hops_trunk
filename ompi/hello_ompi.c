/*
 * $Id$
 *
 * Simple starter example cribbed from 
 * http://mpitutorial.com/tutorials/mpi-hello-world
 // Author: Wes Kendall
 // Copyright 2011 www.mpitutorial.com
 // This code is provided freely with the tutorials on mpitutorial.com. Feel
 // free to modify it for your own use. Any distribution of the code must
 // either provide a link to www.mpitutorial.com or keep this header in tact.
 * mpirun -bynode -n 2 -H `hostname`,`hostname` hello_ompi
 */
#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d"
           " out of %d processors\n",
           processor_name, world_rank, world_size);

    // Finalize the MPI environment.
    MPI_Finalize();
}

/*
 * eof
 */
