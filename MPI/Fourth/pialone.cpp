#include <iostream>
#include <cstdlib>
#include "mpi.h"

int main(int argc, char** argv) {

	double M = 0;
	int N = 100000000;
	int X, Y, size, rank;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	double *recvbuf;
        recvbuf = (double *)malloc(sizeof(int));	
	std::cout << rank << std::endl;
	for (int i = 0; i < (N/size); i++) {
		X = rand()%2 - 1;
		Y = (rand()%2)%size - 1 + (double)rank*2.0/(double)size;
		if (X*X + Y*Y <= 1) {
			M += 1;
		}
	}
	MPI_Reduce(&M, recvbuf, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
	if (!rank) {
		*recvbuf += M;
		N = (double) N;
		std::cout << 4.0*(*recvbuf)/N << std::endl;
	}
	MPI_Finalize();
	return 0;
}
