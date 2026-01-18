#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MASTER 0
#define ROWS_A 1024
#define COLS_A 1024
#define ROWS_B 1024
#define COLS_B 1024

int main(int argc, char **argv) {
    int rank, num_procs;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    int a[ROWS_A][COLS_A], b[ROWS_B][COLS_B], c[ROWS_A][COLS_B], c_partial[ROWS_A/num_procs][COLS_B];
    int i, j, k, row, col, root, tag;
    double start_time, end_time;

    // Initialize matrices
    if (rank == MASTER) {
        for (i = 0; i < ROWS_A; i++) {
            for (j = 0; j < COLS_A; j++) {
                a[i][j] = rand() % 10;
            }
        }

        for (i = 0; i < ROWS_B; i++) {
            for (j = 0; j < COLS_B; j++) {
                b[i][j] = rand() % 10;
            }
        }
    }

    // Broadcast matrix B to all processes
    MPI_Bcast(b, ROWS_B*COLS_B, MPI_INT, MASTER, MPI_COMM_WORLD);

    // Scatter matrix A to all processes
    MPI_Scatter(a, ROWS_A*num_procs, MPI_INT, a[0], ROWS_A*num_procs, MPI_INT, MASTER, MPI_COMM_WORLD);

    // Perform partial matrix multiplication
    start_time = MPI_Wtime();
    for (row = 0; row < ROWS_A/num_procs; row++) {
        for (col = 0; col < COLS_B; col++) {
            c_partial[row][col] = 0;
            for (k = 0; k < COLS_A; k++) {
                c_partial[row][col] += a[row][k] * b[k][col];
            }
        }
    }
    end_time = MPI_Wtime();

    // Gather partial results from all processes to the root process
    MPI_Gather(c_partial, ROWS_A*num_procs/num_procs, MPI_INT, c, ROWS_A*num_procs/num_procs, MPI_INT, MASTER, MPI_COMM_WORLD);

    // Print result and time
    if (rank == MASTER) {
        printf("Result:\n");
        for (i = 0; i < ROWS_A; i++) {
            for (j = 0; j < COLS_B; j++) {
                printf("%d ", c[i][j]);
            }
            printf("\n");
        }
        printf("Time: %f seconds\n", end_time - start_time);
    }

    MPI_Finalize();
    return 0;
}
