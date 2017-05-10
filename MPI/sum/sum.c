#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"


void merge(int *arr, int l, int m, int r) {
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;
	int L[n1], R[n2];
	for (i=0; i<n1; i++)
		L[i] = arr[l+i];
	for (j=0; j<n2; j++)
		R[j] = arr[m+1+j];
	
	i = 0;
	j = 0;
	k = l;
	while (i<n1 && j<n2) {
		if (L[i] <= R[j]) {
			arr[k] = 
		}
	}
}


void mergeSortMPI(double *A) {
	
}


int main(int argc, char* argv[])
{
	int N = 100;
	double x[N], TotalSum, ProcSum = 0.0;
	int ProcRank, ProcNum, k, i1,i2,i;
	MPI_Status status;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &ProcNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &ProcRank);
	
	if (ProcRank == 0 )
	{
		for(i1=0;i1<N;++i1)
		{
			x[i1]=rand();
		}
	}
	MPI_Bcast(x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD );
	k = N / ProcNum;
        i1 = k * ProcRank;
        i2 = k * ( ProcRank + 1 );
        if ( ProcRank == ProcNum-1 ) i2 = N;
	
	/*for ( i = i1; i < i2; i++ )
	 {
                ProcSum = ProcSum + x[i];
        }
	MPI_Reduce( &ProcSum, &TotalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );
	if ( ProcRank == 0 ) {
                printf("\nTotal Sum = %10.2f",TotalSum);
        }*/
	MPI_Finalize();
        return 0;

}
