#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv)
{
    int rank, size, a;

    MPI_Status status;
    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int tag = 1;
    if (rank == 0) {
        a = 5;
    } else {
        MPI_Recv(&a, 1, MPI_INT, rank-1, tag, MPI_COMM_WORLD, &status);
    }

    a += rank;
    printf("%d ", a);

    int next_rank = (rank+1 == size) ? 0 : rank + 1;
    MPI_Send(&a, 1, MPI_INT, next_rank, tag, MPI_COMM_WORLD);

    MPI_Finalize();
}
