#include <iostream>
#include "mpi.h"
#include <fstream>
#include <cstdlib>

int main(int argc, char **argv) {
	std::ofstream fout("lil.txt", std::ios_base::app);
	int n = 1000;
	int i, rank, size;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	n /= size;
	for (i=0; i<n; i++) {
		fout << rand() << ", ";
	}
	if (!rank) {
		fout << rand();
	}
	MPI_Finalize();
	return 0;
}
