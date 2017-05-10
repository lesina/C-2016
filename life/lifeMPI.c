/*
 * Author: Nikolay Khokhlov <k_h@inbox.ru>, 2016
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

#include "mpi.h"

#define ind(i, j) (((i + l->nx) % l->nx) + ((j + l->ny) % l->ny) * (l->nx))
#define ind2(i, j) (((i + l.nx) % l.nx) + ((j + l.ny) % l.ny) * (l.nx))

typedef struct {
    int nx, ny;
    int *u0;
    int *u1;
    int steps;
    int save_steps;
} life_t;

void life_init(const char *path, life_t *l, int rank);

void life_free(life_t *l);

void life_step(life_t *l, int start, int end);

void life_save_vtk(const char *path, life_t *l);

int main(int argc, char **argv) {

    int size, rank;
    double t0, t1;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    if (argc != 2) {
        printf("Usage: %s input file.\n", argv[0]);
        return 0;
    }
    life_t l;
    life_init(argv[1], &l, rank);

    int i, j, k, start = l.ny / 2 * rank, end = start + l.ny / 2;
    int *friends_array;
    friends_array = (int *) calloc(l.nx * l.ny, sizeof(int));
    char buf[100];
    t0 = MPI_Wtime();
    for (k = 0; k < l.steps; k++) {
        if (rank == 0 && k % l.save_steps == 0) {
            sprintf(buf, "life_%06d.vtk", k);
            printf("Saving step %d to '%s'.\n", k, buf);
            life_save_vtk(buf, &l);
        }
//        printf("Begin step \n");
        life_step(&l, start, end);
//        printf("End step \n");
        if (!rank) {
//            printf("Wait for uo");
            MPI_Recv(friends_array, l.nx * l.ny, MPI_INT, 1, 100, MPI_COMM_WORLD, &status);
            for (j = end; j < end + l.ny / 2; j++)
                for (i = 0; i < l.nx; i++) {
                    l.u0[ind2(i, j)] = friends_array[ind2(i, j)];
                }
//            printf("Wait for u1");
            MPI_Recv(friends_array, l.nx * l.ny, MPI_INT, 1, 101, MPI_COMM_WORLD, &status);
            for (j = end; j < end + l.ny / 2; j++)
                for (i = 0; i < l.nx; i++) {
                    l.u1[ind2(i, j)] = friends_array[ind2(i, j)];
                }
            MPI_Send(l.u0, l.nx * l.ny, MPI_INT, 1, 102, MPI_COMM_WORLD);
            MPI_Send(l.u1, l.nx * l.ny, MPI_INT, 1, 103, MPI_COMM_WORLD);
        } else {
//            printf("Sending for uo");
            MPI_Send(l.u0, l.nx * l.ny, MPI_INT, 0, 100, MPI_COMM_WORLD);
//            printf("Sending for u1");
            MPI_Send(l.u1, l.nx * l.ny, MPI_INT, 0, 101, MPI_COMM_WORLD);
            MPI_Recv(l.u0, l.nx * l.ny, MPI_INT, 0, 102, MPI_COMM_WORLD, &status);
            MPI_Recv(l.u1, l.nx * l.ny, MPI_INT, 0, 103, MPI_COMM_WORLD, &status);
        }
    }

    t1 = MPI_Wtime();
    if (!rank) printf("Time is %lf\n", t1-t0);
    life_free(&l);
    MPI_Finalize();
    return 0;
}

/**
 * Загрузить входную конфигурацию.
 * Формат файла, число шагов, как часто сохранять, размер поля, затем идут координаты заполненых клеток:
 * steps
 * save_steps
 * nx ny
 * i1 j2
 * i2 j2
 */
void life_init(const char *path, life_t *l, int rank) {
    FILE *fd = fopen(path, "r");
    assert(fd);
    assert(fscanf(fd, "%d\n", &l->steps));
    assert(fscanf(fd, "%d\n", &l->save_steps));
    if (!rank) printf("Steps %d, save every %d step.\n", l->steps, l->save_steps);
    assert(fscanf(fd, "%d %d\n", &l->nx, &l->ny));
    if (!rank) printf("Field size: %dx%d\n", l->nx, l->ny);

    l->u0 = (int *) calloc(l->nx * l->ny, sizeof(int));
    l->u1 = (int *) calloc(l->nx * l->ny, sizeof(int));

    int i, j, r, cnt;
    cnt = 0;
    while ((r = fscanf(fd, "%d %d\n", &i, &j)) != EOF) {
        l->u0[ind(i, j)] = 1;
        cnt++;
    }
    if (!rank) printf("Loaded %d life cells.\n", cnt);
    fclose(fd);
}

void life_free(life_t *l) {
    free(l->u0);
    free(l->u1);
    l->nx = l->ny = 0;
}

void life_save_vtk(const char *path, life_t *l) {
    FILE *f;
    int i1, i2, j;
    f = fopen(path, "w");
    assert(f);
    fprintf(f, "# vtk DataFile Version 3.0\n");
    fprintf(f, "Created by write_to_vtk2d\n");
    fprintf(f, "ASCII\n");
    fprintf(f, "DATASET STRUCTURED_POINTS\n");
    fprintf(f, "DIMENSIONS %d %d 1\n", l->nx + 1, l->ny + 1);
    fprintf(f, "SPACING %d %d 0.0\n", 1, 1);
    fprintf(f, "ORIGIN %d %d 0.0\n", 0, 0);
    fprintf(f, "CELL_DATA %d\n", l->nx * l->ny);

    fprintf(f, "SCALARS life int 1\n");
    fprintf(f, "LOOKUP_TABLE life_table\n");
    for (i2 = 0; i2 < l->ny; i2++) {
        for (i1 = 0; i1 < l->nx; i1++) {
            fprintf(f, "%d\n", l->u0[ind(i1, i2)]);
        }
    }
    fclose(f);
}

void life_step(life_t *l, int start, int end) {
    int i, j;
    for (j = start; j < end; j++) {
        for (i = 0; i < l->nx; i++) {
            int n = 0;
            n += l->u0[ind(i + 1, j)];
            n += l->u0[ind(i + 1, j + 1)];
            n += l->u0[ind(i, j + 1)];
            n += l->u0[ind(i - 1, j)];
            n += l->u0[ind(i - 1, j - 1)];
            n += l->u0[ind(i, j - 1)];
            n += l->u0[ind(i - 1, j + 1)];
            n += l->u0[ind(i + 1, j - 1)];
            l->u1[ind(i, j)] = 0;
            if (n == 3 && l->u0[ind(i, j)] == 0) {
                l->u1[ind(i, j)] = 1;
            }
            if ((n == 3 || n == 2) && l->u0[ind(i, j)] == 1) {
                l->u1[ind(i, j)] = 1;
            }
        }
    }
    int *tmp;
    tmp = l->u0;
    l->u0 = l->u1;
    l->u1 = tmp;
}


