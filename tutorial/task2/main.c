#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mpi.h"

int main(int argc, char **argv)
{
    int rank, size;

    const int tag = 1;

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int result = 0;
    srand(time(NULL));

    int privateVal = rank + rand() % 10;

    if (rank == 0) {
        printf("[main] initial value=%d\n", privateVal);
    }

    result += privateVal;
    if (rank + 1 < size) {
        MPI_Send(&result, 1, MPI_INT, rank + 1, tag, MPI_COMM_WORLD);
    } else {
        MPI_Send(&result, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        MPI_Recv(&result, 1, MPI_INT, size-1, tag, MPI_COMM_WORLD, &status);
    } else {
        MPI_Recv(&result, 1, MPI_INT, rank-1, tag, MPI_COMM_WORLD, &status);
    }

    if (rank == 0) {
        printf("[main]  result value=%d\n", result);
    } else {
        printf("[other] id=%d with received value=%d\n", rank, result);
    }

    MPI_Finalize();
}
