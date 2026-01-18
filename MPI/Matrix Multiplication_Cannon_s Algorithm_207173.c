#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define N 4

int main(int argc, char** argv) {
    int size, rank, i, j, k, shift_source, shift_dest;
    int A[N][N], B[N][N], C[N][N];
    int A_local[N][N/size], B_local[N/size][N], C_local[N/size][N];
    MPI_Comm comm;
    MPI_Status status;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0) {
        printf("Number of processes = %d\n", size);
        printf("Initial Matrix A:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                A[i][j] = rand() % 10;
                printf("%d ", A[i][j]);
            }
            printf("\n");
        }
        printf("Initial Matrix B:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                B[i][j] = rand() % 10;
                printf("%d ", B[i][j]);
            }
            printf("\n");
        }
    }
    MPI_Barrier(MPI_COMM_WORLD);
    // Broadcast the matrices to all processes
    MPI_Bcast(A, N*N, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, N*N, MPI_INT, 0, MPI_COMM_WORLD);
    // Scatter matrix A to all processes
    MPI_Scatter(A, N*N/size, MPI_INT, A_local, N*N/size, MPI_INT, 0, MPI_COMM_WORLD);
    // Shift the columns of B by process rank
    MPI_Cart_create(MPI_COMM_WORLD, 1, &size, &rank, 1, &comm);
    MPI_Cart_shift(comm, 1, -rank, &shift_source, &shift_dest);
    MPI_Sendrecv_replace(B, N*N/size, MPI_INT, shift_dest, 0, shift_source, 0, comm, &status);
    // Perform local matrix multiplication
    for (i = 0; i < N/size; i++) {
        for (j = 0; j < N; j++) {
            C_local[i][j] = 0;
            for (k = 0; k < N; k++) {
                C_local[i][j] += A_local[i][k] * B[k][j];
            }
        }
    }
    // Gather the results from all processes
    MPI_Gather(C_local, N*N/size, MPI_INT, C, N*N/size, MPI_INT, 0, MPI_COMM_WORLD);
    // Print the resulting matrix
    if (rank == 0) {
        printf("Resulting Matrix C:\n");
        for (i = 0; i < N; i++) {
            for (j = 0; j < N; j++) {
                printf("%d ", C[i][j]);
            }
            printf("\n");
        }
    }
    MPI_Finalize();
    return 0;
}
