#include <iostream>
#include <cstdlib>
#include "mpi.h"

int main(int argc, char** argv) {

	double M = 0;
	int N = 10000000;
	int X, Y, size, rank;
	double *recvbuf;
	recvbuf = (double *)malloc(sizeof(int));
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	int n = N / size;
	for (int i = 0; i < n; i++) {
		X = rand()%2 - 1;
		Y = rand()%2 - 1;
		if (X*X + Y*Y <= 1) {
			M += 1;
		}
	}
	MPI_Reduce(&M, recvbuf, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	N = (double) N;
	if (!rank) {
		*recvbuf += M;
		std::cout << 4.0*(*recvbuf)/N << std::endl;
		//std::cout << *recvbuf << std::endl;
	}
	MPI_Finalize();
	return 0;
}
