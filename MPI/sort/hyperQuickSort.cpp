#include <iostream>
#include <cstdlib>
#include "mpi.h"


int compare(const void * x1, const void * x2)   // функция сравнения элементов массива
{
	return ( *(int*)x1 - *(int*)x2 );              // если результат вычитания равен 0, то числа равны, < 0: x1 < x2; > 0: x1 > x2
}


int main(int argc, char **argv) {
		
	int rank, size;
	int i;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	int *global_array;
	global_array = (int*)malloc(sizeof(int)*100);
	int *array = global_array;
	// std::cout << "I am here!!!!11!1111";
	if (!rank) {
		for (i = 0; i < 100; i++) {
			global_array[i] = std::rand()%100;
		}
		for (i = 1; i < size; i++) {
			MPI_Send(&global_array[i*(100/size)], 100/size, MPI_INT, i, 100, MPI_COMM_WORLD);
		}
		for (i = 0; i < 100/size; i++)
			array[i] = global_array[i];
		// std::cout << (array[0]==global_array[0]) << std::endl;
	} else {
		MPI_Recv(array, 100/size, MPI_INT, 0, 100, MPI_COMM_WORLD, &status);
	}

	std::qsort(array, 100/size, sizeof(int), compare);
	int boss;
	if (!rank) {
		boss = array[100/(2*size)];
	}
	MPI_Bcast(&boss, 1, MPI_INT, 0, MPI_COMM_WORLD);

	
	std::cout << rank << " of " << size << " has " << array[0] << " and "<< boss << std::endl;
	MPI_Finalize();
	return 0;
}
