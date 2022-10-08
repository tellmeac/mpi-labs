#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "mpi.h"

const int length = 100;

int A[length][length];
int X[length];

int* multiplie(int (*a)[100], int x[length]) {
    
}

int main(int argc, char **argv)
{
    int rank, size;

    srand(time(NULL));

    MPI_Status status;

    MPI_Init(&argc, &argv);

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    

    MPI_Finalize();
}
