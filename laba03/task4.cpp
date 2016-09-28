#include<iostream>

int **dynamic_array_alloc(int N, int M)
{
	int **A = new int *[N];	// две строки в массиве
	for (int i = 0; i < N; i++)
		A[i] = new int[M];
	return A;
}

void dynamic_array_free(int **A, int N)
{
        for(int i = 0; i < N; i++) {
                delete[] A[i];
        }
        delete[] A;
}

void dynamic_array_fill(int **A, int M, int N) {
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
			std::cin >> A[i][j];
}

void dynamic_array_print(int **A, int N, int M)
{
        for(int i = 0; i < N; i++) {
                for(int j = 0; j < M; j++) {
                        std::cout << A[i][j] << "\t";
                }
                std::cout << std::endl;
        }
}

int main()
{
	int N, M;
	std::cin >> N >> M;
	int **arr = dynamic_array_alloc(N, M);
	dynamic_array_fill(arr, M, N);
	dynamic_array_print(arr, N, M);
	dynamic_array_free(arr, N);
	return 0;
}
