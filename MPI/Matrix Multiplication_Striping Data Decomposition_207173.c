#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MATSIZE 1000

int main(int argc, char** argv) {
    int rank, size, i, j, k;
    double starttime, endtime;
    double a[MATSIZE][MATSIZE], b[MATSIZE][MATSIZE], c[MATSIZE][MATSIZE];
    int num_rows, start_row, end_row, dest, tag = 1;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Initialize matrices
        for (i = 0; i < MATSIZE; i++) {
            for (j = 0; j < MATSIZE; j++) {
                a[i][j] = i + j;
                b[i][j] = i - j;
                c[i][j] = 0.0;
            }
        }
        // Start timer
        starttime = MPI_Wtime();
    }

    // Distribute the data
    num_rows = MATSIZE / size;
    start_row = rank * num_rows;
    end_row = start_row + num_rows - 1;
    MPI_Scatter(a, num_rows * MATSIZE, MPI_DOUBLE, a[start_row], num_rows * MATSIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(b, MATSIZE * MATSIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Perform the matrix multiplication
    for (i = start_row; i <= end_row; i++) {
        for (j = 0; j < MATSIZE; j++) {
            for (k = 0; k < MATSIZE; k++) {
                c[i][j] += a[i][k] * b[k][j];
            }
        }
    }

    // Gather the results
    MPI_Gather(c[start_row], num_rows * MATSIZE, MPI_DOUBLE, c, num_rows * MATSIZE, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // End timer
        endtime = MPI_Wtime();
        printf("Time elapsed: %.2f seconds\n", endtime - starttime);
    }

    MPI_Finalize();
    return 0;
}
