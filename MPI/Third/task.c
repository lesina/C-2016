#include "stdio.h"
#include "mpi.h"
#include "fstream"
#include <stdlib.h>


double startT, stopT;

int *mergeArrays(int *v1, int n1, int *v2, int n2)
{
	int i, j, k;
	int *result;

	result = new int[n1 + n2];
	i = 0;
	j = 0;
	k = 0;

	while (i < n1 && j < n2)
		if (v1[i] < v2[j]) {
			result[k] = v1[i];
			i++;
			k++;
		} else {
			result[k] = v2[j];
			j++;
			k++;
		}

	if (i == n1)
		while (j < n2) {
			result[k] = v2[j];
			j++;
			k++;
		}
	if (j == n2)
		while (i < n1) {
			result[k] = v1[i];
			i++;
			k++;
		}

	return result;
}

void swap(int *v, int i, int j)
{
	int t;
	t = v[i];
	v[i] = v[j];
	v[j] = t;
}

void sort(int *v, int n)
{
	int i, j;

	for (i = n - 2; i >= 0; i--)
		for (j = 0; j <= i; j++)
			if (v[j] > v[j + 1])
				swap(v, j, j + 1);
}

using namespace std;

int main(int argc, char **argv)
{
	int *data;		//Our unsorted array
	int *resultant_array;	//Sorted Array
	int *sub;

	int m, n;
	int id, p;
	int r;
	int s;
	int i;
	int z;
	int move;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &id);
	MPI_Comm_size(MPI_COMM_WORLD, &p);
	//Task Of The Master Processor
	if (id == 0) {
		n = 80000;
		s = n / p;
		r = n % p;
		data = new int[n + s - r];

		ofstream file("input");

		for (i = 0; i < n; i++) {
			data[i] = rand() % 15000;
			file << data[i] << " ";
		}

		file.close();

		if (r != 0) {
			for (i = n; i < n + s - r; i++)
				data[i] = 0;

			s = s + 1;
		}

		startT = MPI_Wtime();	//Start The Time.
		MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
		resultant_array = new int[s];	//Allocating result array
		//Sending data array from master to all other slaves
		MPI_Scatter(data, s, MPI_INT, resultant_array, s, MPI_INT, 0,
			    MPI_COMM_WORLD);
		sort(resultant_array, s);
	} else {
		MPI_Bcast(&s, 1, MPI_INT, 0, MPI_COMM_WORLD);
		//Allocating resultant array
		resultant_array = new int[s];
		MPI_Scatter(data, s, MPI_INT, resultant_array, s, MPI_INT, 0,
			    MPI_COMM_WORLD);
		//Each slave processor will sort according to the array partitioning n/p
		sort(resultant_array, s);	//Sort the array up to index s.
	}

	move = 1;

	//Merging the sub sorted arrays to obtain resultant sorted array
	while (move < p) {
		if (id % (2 * move) == 0) {
			if (id + move < p) {	//Receive
				MPI_Recv(&m, 1, MPI_INT, id + move, 0,
					 MPI_COMM_WORLD, &status);
				sub = new int[m];	//Allocate space for sub array
				MPI_Recv(sub, m, MPI_INT, id + move, 0,
					 MPI_COMM_WORLD, &status);
				//Obtaing resultant array by merging sub sorted array
				resultant_array =
				    mergeArrays(resultant_array, s, sub, m);
				s = s + m;
			}
		} else {	//Send datas to neighbour processors
			int near = id - move;
			MPI_Send(&s, 1, MPI_INT, near, 0, MPI_COMM_WORLD);
			MPI_Send(resultant_array, s, MPI_INT, near, 0,
				 MPI_COMM_WORLD);
			break;
		}

		move = move * 2;
	}

	//Final Part, In this part Master CPU outputs the results.!!!
	if (id == 0) {
		stopT = MPI_Wtime();
		double parallelTime = stopT - startT;
		printf("\n\n\nTime: %f", parallelTime);

		ofstream file("output");
		for (i = 0; i < n; i++) {
			file << resultant_array[i] << " ";
		}

		file.close();
	}

	MPI_Finalize();		//Finalize MPI environment.
}
