#include <mpi.h>
#include <string.h>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>


#define LIFE 1
#define DEAD 0

int neighborsCount(int** zone, int i, int j, int size) {
    int neighbors = 0;
    if (zone[(i - 1 + size) % size][(j - 1 + size) % size]  == LIFE) neighbors++;
    if (zone[(i - 1 + size) % size][j]                      == LIFE) neighbors++;
    if (zone[(i - 1 + size) % size][(j + 1) % size]         == LIFE) neighbors++;
    if (zone[i][(j - 1 + size) % size]                      == LIFE) neighbors++;
    if (zone[i][(j + 1) % size]                             == LIFE) neighbors++;
    if (zone[(i + 1) % size][(j - 1 + size) % size]         == LIFE) neighbors++;
    if (zone[(i + 1) % size][j]                             == LIFE) neighbors++;
    if (zone[(i + 1) % size][(j + 1) % size]                == LIFE) neighbors++;

//    std::cout << "[" << i << "]" << "[" << j << "]: " << neighbors << " "<< zone[i][j] <<std::endl;

    return neighbors;
}

auto checkCell(int** zone, int i, int j, int size) {
    int neighbors = neighborsCount(zone, i, j, size);



    if (neighbors == 3 && (zone[i][j] == DEAD)) {
        std::cout << "LIFE TO " << i << ' ' << j << std::endl;
        return LIFE;
    }

    if ((neighbors == 3 || neighbors == 2) && zone[i][j] == LIFE)
        return LIFE;

    return DEAD;

}

void giveLife(int** zone, int i, int j, int size) {

}

void printZone(int** zone, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            std::cout << zone[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << std::endl;
}

int main(int argc, char *argv[]) {
    int SIZE = 10;
    int** gameZone = new int*[SIZE];
    for (int i = 0; i < SIZE; i++)
        gameZone[i] = new int[SIZE];

    gameZone[0][1] = LIFE;
    gameZone[0][3] = LIFE;
    gameZone[1][2] = LIFE;

    gameZone[2][1] = LIFE;
    gameZone[2][3] = LIFE;
    gameZone[3][2] = LIFE;
    gameZone[4][1] = LIFE;
    gameZone[4][3] = LIFE;
    gameZone[5][2] = LIFE;
    gameZone[6][1] = LIFE;
    gameZone[6][3] = LIFE;
    gameZone[7][2] = LIFE;

    MPI_Init(&argc, &argv);
    int size = 0, rank = 0;
    int start = 0, end = 0;
    MPI_Status status;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

//    std::cout <<rank << ": PID: " << getpid() << std::endl;

    int offset = SIZE % size;
    if (offset == 0) {
        start = rank * SIZE / size;
        end = (rank + 1) * SIZE / size - 1;
    } else {
        if (rank < offset) {
            start = rank * (SIZE / size + 1);
            end = (rank + 1) * (SIZE / size + 1) - 1;
        } else {
            start = rank * (SIZE / size) + offset;
            end = (rank + 1) * (SIZE / size) + offset - 1;
        }
    }

    int starts[size] = {};
    int ends[size] = {};

    if (rank == 0) {
        starts[0] = start;
        ends[0] = end;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&starts[i], 1, MPI_INT, i, 1, MPI_COMM_WORLD, &status);
            MPI_Recv(&ends[i], 1, MPI_INT, i, 2, MPI_COMM_WORLD, &status);
        }
    } else {
        MPI_Send(&start, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
        MPI_Send(&end, 1, MPI_INT, 0, 2, MPI_COMM_WORLD);
    }

    // SEND & RECEIVE
    for(int loop = 0; loop < 10; loop++) {

        if(rank == 0) {
            printZone(gameZone, SIZE);
        }

//        MPI_Barrier(MPI_COMM_WORLD);

        if (rank % 2) {
//            std::cout << "SEND: from " << rank << " to " <<  (rank + 1) % size << std::endl;
            MPI_Send(gameZone[end], SIZE, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
//            std::cout << "RECV: to " << rank << " from " <<  (rank - 1 + size) % size << std::endl;
            MPI_Recv(gameZone[(start - 1 + SIZE) % SIZE], SIZE, MPI_INT, (rank - 1 + size) % size, MPI_ANY_TAG,
                     MPI_COMM_WORLD, &status);
//            std::cout << "SEND: from " << rank << " to " <<  (rank - 1 + size) % size << std::endl;
            MPI_Send(gameZone[start], SIZE, MPI_INT, (rank - 1 + size) % size, 0, MPI_COMM_WORLD);
//            std::cout << "RECV: to " << rank << " from " <<  (rank + 1) % size << std::endl;
            MPI_Recv(gameZone[(end + 1) % SIZE], SIZE, MPI_INT, (rank + 1) % size, MPI_ANY_TAG, MPI_COMM_WORLD,
                     &status);
        } else {
//            std::cout << getpid() << " RECV: to " << rank << " from " <<  (rank - 1 + size) % size << std::endl;
            MPI_Recv(gameZone[(start - 1 + SIZE) % SIZE], SIZE, MPI_INT, (rank - 1 + size) % size, MPI_ANY_TAG,
                     MPI_COMM_WORLD, &status);
            MPI_Send(gameZone[end], SIZE, MPI_INT, (rank + 1) % size, 0, MPI_COMM_WORLD);
//            std::cout << getpid() << " RECV: to " << rank << " from " <<  (rank + 1) % size << std::endl;
            MPI_Recv(gameZone[(end + 1) % SIZE], SIZE, MPI_INT, (rank + 1) % size, MPI_ANY_TAG, MPI_COMM_WORLD,
                     &status);
            MPI_Send(gameZone[start], SIZE, MPI_INT, (rank - 1 + size) % size, 0, MPI_COMM_WORLD);
        }


        int nextgenerationZone[SIZE][SIZE] = {};

        for (int i = start; i < (end + 1); i++) {
            for(int j = 0; j < SIZE; j++) {
                int neighbors = neighborsCount(gameZone, i, j, SIZE);
                if (neighbors == 3 and gameZone[i][j] == DEAD) {
                    nextgenerationZone[i][j] = LIFE;
                    continue;
                }
                if ((neighbors == 3 || neighbors == 2) && gameZone[i][j] == LIFE) {
                    nextgenerationZone[i][j] = LIFE;
                } else {
                    nextgenerationZone[i][j] = DEAD;
                }
            }
        }

        for (int i = start; i < (end + 1); i++) {
//            for(int j = 0; j < SIZE; j++) {
//                gameZone[i][j] = nextgenerationZone[i][j];
//            }
            memcpy(gameZone[i], nextgenerationZone[i], SIZE*sizeof(int));
        }


//        MPI_Barrier(MPI_COMM_WORLD);

        if (rank == 0) {
            for (int i = 1; i < size; i++) {
                for (int j = starts[i]; j < ends[i] + 1; j++) {
                    MPI_Recv(gameZone[j], SIZE, MPI_INT, i, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                }
            }
        } else {
            for (int i = start; i < end + 1; i++) {
//                std::cout << i << " FROM: " << rank << std::endl;
//                for (int k = 0; k < SIZE; k++)
//                    std::cout << gameZone[i % size][k];
//                std::cout << std::endl << std::endl;
                MPI_Send(gameZone[i], SIZE, MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }
    }

    MPI_Finalize();
    return 0;
}

