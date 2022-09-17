#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv)
{
    int rank, size;

    MPI_Status status;
    MPI_Request request = MPI_REQUEST_NULL;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    const int tag = 0;
    int msg = -1;
    if (rank == 0) {
        msg = rank; // as task require, looks weird
        for (int next = 1; next < size; ++next) {
            MPI_Isend(&msg, 1, MPI_INT, next, next, MPI_COMM_WORLD, &request);
        }
    } else {
        MPI_Irecv(&msg, 1, MPI_INT, 0, rank, MPI_COMM_WORLD, &request);
    }

    MPI_Wait(&request, &status);

    if (rank != 0) {
        printf("id:%d, msg:%d\n", rank, msg);
    }

    MPI_Finalize();
}
