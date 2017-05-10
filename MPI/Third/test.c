#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi.h"

int main( int argc, char* argv[] ) {
        const int N = 100;
        double x[N], TotalSum, ProcSum = 0.0;
        int ProcRank, ProcNum, k, i1, i2;
        MPI_Status Status;

        // Инициализация
        MPI_Init( &argc, &argv );
        MPI_Comm_size( MPI_COMM_WORLD, &ProcNum );
        MPI_Comm_rank( MPI_COMM_WORLD, &ProcRank );

        // Подготовка данных
        if ( ProcRank == 0 ) {
                for( i1 = 0; i1 < N; ++i1 ) {
                        x[i1] = i1;
                }
        }

        // Рассылка данных на все процессы
        MPI_Bcast( x, N, MPI_DOUBLE, 0, MPI_COMM_WORLD );

        // Вычисление частичной суммы на каждом из процессов
        // на каждом процессе суммируются элементы вектора x от i1 до i2
        k = N / ProcNum;
        i1 = k * ProcRank;
        i2 = k * ( ProcRank + 1 );
        if ( ProcRank == ProcNum-1 ) i2 = N;
        for ( int i = i1; i < i2; i++ ) {
                ProcSum = ProcSum + x[i];
        }

        MPI_Reduce( &ProcSum, &TotalSum, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD );

        // Вывод результата
        if ( ProcRank == 0 ) {
                printf("\nTotal Sum = %10.2f",TotalSum);
        }
        MPI_Finalize();
        return 0;
}
