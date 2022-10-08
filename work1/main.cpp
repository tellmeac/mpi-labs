#include <stdio.h>
#include <mpi.h>
#include <cmath>
#include <time.h>
#include <cstdlib>

const int VECTOR_SIZE = 10000000;

int main(int argc, char **argv) { 
    int rank, size;

    srand((time (NULL)));

    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // Vector part size for a signle process to work with.
    int acquired_size = VECTOR_SIZE / size;

    double* x = new double[VECTOR_SIZE];
    double* y = new double[VECTOR_SIZE];

    // Metrics timestamp
    double start_ts = MPI_Wtime();

    double local_result = 0;
    for (int i = 0; i < acquired_size; i++) {
        x[i] = (double)rand() / RAND_MAX;
        y[i] = (double)rand() / RAND_MAX;

        local_result += x[i] * y[i];
    }

    double recv_result;
    for (int k = 0; k < log2(size); k++) {
        recv_result = 0;

        // Send procedeed
        if ((rank+1) % (int)powf(2.0, k+1) == (int)powf(2.0, k)) {
            int send = rank + (int)powf(2.0, k);
            if (send > size - 1) 
                send = MPI_PROC_NULL;

            MPI_Send(&local_result, 1, MPI_DOUBLE, send, 1, MPI_COMM_WORLD);
        }

        // Recieve compuatations
        if ((rank+1) % (int)powf(2.0, k+1) == 0) {
            int recv = rank - (int)powf(2.0,k);
            if (recv < 0)
                recv = MPI_PROC_NULL;

            MPI_Recv(&recv_result, 1, MPI_DOUBLE, recv, 1, MPI_COMM_WORLD, &status);

            local_result += recv_result;
        }
    }

    double finish_ts = MPI_Wtime();
    double timedelta = finish_ts - start_ts;

    // last element prints result
    if (rank == size - 1) {
        printf("I am the last process!\n");
        printf("There was %d elements to work with\n", VECTOR_SIZE);
        printf("Completed answer is %f\n", local_result);
        printf("Metrics timedelta %f\n", timedelta);
    }
        
    MPI_Finalize();
}