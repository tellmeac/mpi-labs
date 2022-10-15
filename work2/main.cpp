#include <stdio.h>
#include <mpi.h>
#include <math.h>

// Variant 14
double f(double x) {
    double x2 = x * x;
    // approx integral from 0 to 1: ~2.505512
    return (sqrt(2 + x2)) / ((1+cos(2 * x)) * sqrt(1 - x2));
}

int main(int argc, char** argv) {
	int rank, size;

	MPI_Status status;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	double start_ts = MPI_Wtime();

	const int precision = 1000000000;

	const double a = 0, b = 1;

	// Step width
	double height = (b - a) / precision;

	double ax = a + rank * (b - a) / size;

	double sum = 0;
	double beforeF = f(ax), current = f(ax+height);
	for (int i = 0; i < precision / size - 1; ++i) {   
		sum += beforeF + current;

		beforeF = current;
		current = f(ax+height);
		
		ax += height;
	}
		
	double result;
	MPI_Reduce(&sum, &result, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

	// Simple optimization: ((fa + fb) + (fa1 + fb1) + ...) * height / 2
	result *= height / 2; 
	
	if (rank == 0)
		printf("Result = %f; timedelta = %f\n", result, MPI_Wtime() - start_ts);

	MPI_Finalize();
}