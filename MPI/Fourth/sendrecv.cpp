#include "mpi.h"
#include <iostream>

int main(int argc, char** argv) {

int rank, size;

MPI_Init(&argc, &argv);
MPI_Comm_size(MPI_COMM_WORLD, &size);
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
MPI_Status status;
int a[10];
if (!rank){
for (int i=0; i<10; i++){
	a[i]=i;
}
MPI_Send(&a, 10, MPI_INT, 1, 100, MPI_COMM_WORLD);
} else if (rank == 1) {
MPI_Recv(&a, 10, MPI_INT, 0, 100, MPI_COMM_WORLD, &status);
for (int i=0;i<10;i++)
std::cout << a[i] << " ";
}
MPI_Finalize();
return 0;
}
