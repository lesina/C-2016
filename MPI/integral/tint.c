#include <stdio.h>
#include <math.h>
#include <mpi.h>

#define A 2.0

double f(double x)
{
	return sqrt(4.0 - x * x);
}

int main(int argc, char *argv[])
{
	if (argc < 2) {
		printf("Usage: %s num_intervals.\n", argv[0]);
		return 1;
	}
	MPI_Init(&argc, &argv);
	int rank;
	int i;
	int N = atoi(argv[1]);
	double h = A / N;
	double S = 0.0;

	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	for (i = 0; i < N; i++) {
		S += h * (f(h * i) + f(h * (i + 1))) / 2.0;
	}
	if (rank == 0) {
		printf("%f\n", S);
	}
	MPI_Finalize();
	return 0;
}
