#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 3

int main(int argc, char** argv) {
    int rank, size, i, j, k;
    double A[N][N+1] = {{1, 1, 1, 6}, {2, 3, 1, 11}, {1, -1, -1, 1}};
    double x[N], temp, pivot, ratio;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size != N) {
        if (rank == 0) {
            printf("This program requires %d processes\n", N);
        }
        MPI_Finalize();
        return EXIT_FAILURE;
    }

    if (rank == 0) {
        printf("Matrix A:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N+1; j++) {
                printf("%.2f\t", A[i][j]);
            }
            printf("\n");
        }
        printf("\n");
    }

    for (k = 0; k < N-1; k++) {
        if (k == rank) {
            pivot = A[k][k];
            for (j = k+1; j < N+1; j++) {
                A[k][j] /= pivot;
            }
            for (i = k+1; i < N; i++) {
                ratio = A[i][k];
                for (j = k+1; j < N+1; j++) {
                    A[i][j] -= ratio * A[k][j];
                }
            }
        }
        MPI_Bcast(&A[k][k+1], N-k-1, MPI_DOUBLE, k, MPI_COMM_WORLD);
        MPI_Bcast(&A[k+1][k], 1, MPI_DOUBLE, k, MPI_COMM_WORLD);
        MPI_Bcast(&A[k+1][k+1], N-k-1, MPI_DOUBLE, k, MPI_COMM_WORLD);
    }

    if (rank == 0) {
        x[N-1] = A[N-1][N];
    }

    for (i = N-2; i >= 0; i--) {
        temp = A[i][N];
        for (j = i+1; j < N; j++) {
            temp -= A[i][j] * x[j];
        }
        x[i] = temp;
    }

    if (rank == 0) {
        printf("Solution x:\n");
        for (i = 0; i < N; i++) {
            printf("%.2f\n", x[i]);
        }
    }

    MPI_Finalize();
    return EXIT_SUCCESS;
}
