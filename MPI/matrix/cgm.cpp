#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <fstream>
#include "mpi.h"


int main(int argc, char **argv)
{
	int procSize, dest, source, rows, rank, averow, extra, offset;
	double trash;
	std::ifstream fin1, fin2;
	fin1.open("A1.txt");
	fin2.open("A2.txt");
	int size[4];
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &procSize);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Status status;
	if (!rank) {
		fin1 >> size[0];
		fin1 >> size[1];
		fin2 >> size[2];
		fin2 >> size[3];
	}

	MPI_Bcast(&size, 4, MPI_INT, 0, MPI_COMM_WORLD);

	//fin1 >> trash;
	//fin1 >> trash;
	//fin2 >> trash;
	//fin2 >> trash;

	while (!(size[0]/(procSize-1))) procSize--;

	//averow = size[0]/(procSize-1);
	//extra = size[0]%(procSize-1);

	double a[size[0]][size[1]]; // FIRST MATRIX
	double b[size[2]][size[3]]; // SECOND MATRIX
	double c[size[0]][size[3]]; // RESULT!!!!

	if (!rank) {
		
		int i, j;
		for (i=0; i<size[0]; i++)
			for (j=0; j<size[1]; j++)
				fin1 >> a[i][j];
		for (i=0; i<size[2]; i++)
			for (j=0; j<size[3]; j++)
				fin2 >> b[i][j];
		
		averow = size[0]/(procSize-1);
		extra = size[0]%(procSize-1);
		offset = 0;
		for (dest=1; dest<procSize; dest++) {
        		rows = (dest <= extra) ? averow+1 : averow;
        		//printf("...sending %d rows to task %d\n", rows, dest);
        		MPI_Send(&offset, 1, MPI_INT, dest, 101, MPI_COMM_WORLD);
        		MPI_Send(&rows, 1, MPI_INT, dest, 102, MPI_COMM_WORLD);
        		MPI_Send(&a[offset][0], rows*size[1], MPI_DOUBLE, dest, 103, MPI_COMM_WORLD);
        		MPI_Send(&b, size[2]*size[3], MPI_DOUBLE, dest, 104, MPI_COMM_WORLD);
        		offset += rows;
    		}

		for (source=1; source<procSize; source++) {
		        MPI_Recv(&offset, 1, MPI_INT, source, 105, MPI_COMM_WORLD, &status);
        		MPI_Recv(&rows, 1, MPI_INT, source, 106, MPI_COMM_WORLD, &status);
		        MPI_Recv(&c[offset][0], rows*size[3], MPI_DOUBLE, source, 107, MPI_COMM_WORLD, &status);
    		}

		for (i=0; i<size[0]; i++) {
			for (j=0; j<size[3]; j++) {
				std::cout << "\t" << c[i][j];
			}
			std::cout << std::endl;
		}
	
	} else if (rank < procSize) {
		
		int i, j, k;

		MPI_Recv(&offset, 1, MPI_INT, 0, 101, MPI_COMM_WORLD, &status);
    		MPI_Recv(&rows, 1, MPI_INT, 0, 102, MPI_COMM_WORLD, &status);
    		MPI_Recv(&a, rows*size[1], MPI_DOUBLE, 0, 103, MPI_COMM_WORLD, &status);
    		MPI_Recv(&b, size[2]*size[3], MPI_DOUBLE, 0, 104, MPI_COMM_WORLD, &status);

		for (k=0; k<size[3]; k++) {
			for (i=0; i<rows; i++) {
        			c[i][k] = 0.0;
        			for (j=0; j<size[2]; j++) {
			        	c[i][k] += a[i][j] * b[j][k];
        			}
      			}
    		}
		MPI_Send(&offset, 1, MPI_INT, 0, 105, MPI_COMM_WORLD);
		MPI_Send(&rows, 1, MPI_INT, 0, 106, MPI_COMM_WORLD);
		MPI_Send(&c, rows*size[3], MPI_DOUBLE, 0, 107, MPI_COMM_WORLD);

	}
	MPI_Finalize();
	return 0;
}
