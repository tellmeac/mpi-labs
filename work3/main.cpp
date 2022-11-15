#include <stdio.h>
#include <mpi.h>
#include <cstdlib>
#include <ctime>

double f(double x, double y)
{
    return y * (x + y);
};

int main(int argc, char **argv)
{
    int rank, size, rc;
    const int n = 10000;
    double begin, end;
    double integral_total;
    double a0 = 0, b0 = 2, c0 = 0, d0 = 2;
    double integral = 0.0;
    int i, j;
    double x0, y0, x1, y1;
    double a, b, c, d, hx, hy, hx0, hy0;
    MPI_Comm comm;
    rc = MPI_Init(&argc, &argv);
    comm = MPI_COMM_WORLD;
    rc = MPI_Comm_size(comm, &size);
    rc = MPI_Comm_rank(comm, &rank);

    begin = MPI_Wtime();
    hx0 = (b0 - a0) / size;
    hy0 = (d0 - c0);
    a = a0 + rank * hx0;
    c = c0;
    b = a + hx0;
    d = d0;
    hx = (b - a) / (n / size);
    hy = (d - c) / n;

    for (i = 1, x0 = a; i < n / size + 1; i++)
    {
        x1 = x0 + hx;
        for (j = 1, y0 = c; j < n + 1; j++)
        {
            y1 = y0 + hy;
            if (x0 > 0 && x1 < 2 && y0 > 0 && y1 < 2)
            {
                integral += f((x0 + x1) / 2., (y0 + y1) / 2.);
            }
            y0 = y1;
        }
        x0 = x1;
    }
    integral *= hx * hy;

    MPI_Reduce(&integral, &integral_total, 1, MPI_DOUBLE, MPI_SUM, 0, comm);
    end = MPI_Wtime();
    if (rank == 0)
    {
        printf("rank = %d, size = %d, integral = %f, \n", rank, size, integral_total);
        printf("time = %f\n", end - begin);
    }
    rc = MPI_Finalize();
}