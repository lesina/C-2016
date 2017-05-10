/*
 * Николай Хохлов, k_h@inbox.ru, 2012.
 * Реализация алгоритма сортировки слиянием.
 * Изначально массив распределен между процессами, после работы сбор у 0.
 * Сбор всех данных и обработка у одного процесса.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <mpi.h>


int compare(const void* x1, const void* x2)
{
	return (*(int*)x1 - *(int*)x2);
}


int pow2(int i)
{
	return 1 << i;
}

//void mpi_merge_sort(int *a, int na);
//void mpi_merge(int *a, int *b, int *c, int na, int nb);
void merge(int *a, int *b, int *len, MPI_Datatype *dtype)//, int *c, int na, int nb);
void merge_sort(int *a, int na);
void print(int *a, int na);
int check_sort(int *a, int n);
double timer();
int n_count(int n, int rank, int size);

int main(int argc, char *argv[])
{
	int i, n, nlocal, npartner, *a, *b, *c;
	int size, rank;
	MPI_Op op;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Op_create((MPI_User_function*) merge, 0, &op);
	double t;
	if (argc < 2) {
		if (rank == 0) {
			printf("Usage: %s num_elements.\n", argv[0]);
		}
		return 1;
	}
	n = atoi(argv[1]);
	nlocal = n_count(n, rank, size);
	a = (int*)malloc(sizeof(int) * nlocal);
	
	srand(time(NULL));
	for (i = 0; i < nlocal; i++) a[i] = rand() % 10;
	
	t = timer();
	qsort(a, nlocal, sizeof(int), compare);


	/* Начало сбора. */
	/*if (rank) {
		MPI_Send(a, nlocal, MPI_INT, 0, 0, MPI_COMM_WORLD);
	} else {
		for (i = 1; i < size; i++) {
			int ni = n_count(n, i, size);
			b = (int*)malloc(sizeof(int) * ni);
			MPI_Recv(b, ni, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			c = (int*)malloc(sizeof(int) * (ni + nlocal));
			merge(a, b, nlocal, ni);
			free(a);
			a = c;
			nlocal += ni;
			free(b);
		}
	}*/
	MPI_Reduce(&a, &a, nlocal, MPI_INT, op, 0, MPI_COMM_WORLD);
	/* Конец сбора. */

	if (rank == 0) {
		t = timer() - t;
		if (n < 11) print(a, n);
		printf("Time: %f sec, sorted: %d\n", t, check_sort(a, n));
	}
	free(a);
	MPI_Finalize();
	return 0;
}

int n_count(int n, int rank, int size)
{
	return (n / size) + (n % size > rank);
}

double timer()
{
	struct timeval ts;
	gettimeofday(&ts, NULL);
	return (double)ts.tv_sec + 1e-6 * (double)ts.tv_usec;
}

int check_sort(int *a, int n)
{
	int i;
	for (i = 0; i < n - 1; i++) {
		if (a[i] > a[i+1]) {
			return 0;
		}
	}
	return 1;
}

void print(int *a, int na)
{
	int i;
	for (i = 0; i < na; i++) printf("%d ", a[i]);
	printf("\n");
}

/*
 * Процедура слияния массивов a и b в массив c.
 */
void merge(int *a, int *b, int *len, MPI_Datatype *dtype)//, int *c, int na, int nb)
{
	int i = 0, j = 0;
	int tmp;
	//int *c = (int*)malloc(sizeof(a)+sizeof(b));
	while (i < len[0] && j < len[1]) {
		if (a[i] <= b[j]) {
			//a[i + j] = a[i];
			i++;
		} else {
			for (int k=len[0]-1; k>=(i+j); k++)
			{
				tmp = a[k];
				a[k] = a[k-1];
				a[k-1] = tmp;
			}
			len[0] += 1;
			a[i + j] = b[j];
			j++;
		}
	}
	if (i < sizeof(a)) {
		memcpy(c + i + j, a + i, (sizeof(a) - i) * sizeof(int));
	} else {
		memcpy(c + i + j, b + j, (sizeof(b) - j) * sizeof(int));
	}
}

/*
 * Процедура сортировки слиянием.
 */
void merge_sort(int *a, int na)
{
	if(na < 2) return;
	if(na == 2) {
		if(a[0] > a[1]) { int t = a[0]; a[0]=a[1]; a[1]=t; }
		return;
	}
	merge_sort(a, na / 2);
	merge_sort(a + na / 2, na - na / 2);

	int *b = (int*)malloc(sizeof(int) * na);
	
	merge(a, a + na / 2, b, na / 2, na - na / 2);
	
	memcpy(a, b, sizeof(int) * na);
	
	free(b);
}
