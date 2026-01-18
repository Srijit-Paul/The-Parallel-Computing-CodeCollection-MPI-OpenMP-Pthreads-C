#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 4

int A[N][N], B[N][N], C[N][N];

void fill_matrix(int mat[N][N])
{
    int i, j;
    for (i = 0; i < N; i++)
        for (j = 0; j < N; j++)
            mat[i][j] = rand() % 10;
}

void print_matrix(int mat[N][N])
{
    int i, j;
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
            printf("%d ", mat[i][j]);
        printf("\n");
    }
}

int main(int argc, char* argv[])
{
    int i, j, k, rank, num_procs, block_size, offset, a_offset, b_offset;
    MPI_Status status;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

    block_size = N / num_procs;

    if (rank == 0) {
        fill_matrix(A);
        fill_matrix(B);
        printf("Matrix A:\n");
        print_matrix(A);
        printf("\nMatrix B:\n");
        print_matrix(B);
    }

    MPI_Bcast(B, N * N, MPI_INT, 0, MPI_COMM_WORLD);

    int* A_block = (int*) malloc(sizeof(int) * N * block_size);
    int* C_block = (int*) malloc(sizeof(int) * N * block_size);

    MPI_Scatter(A, N * block_size, MPI_INT, A_block, N * block_size, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = 0; i < block_size; i++)
        for (j = 0; j < N; j++) {
            C_block[i * N + j] = 0;
            for (k = 0; k < N; k++)
                C_block[i * N + j] += A_block[i * N + k] * B[k][j];
        }

    MPI_Gather(C_block, N * block_size, MPI_INT, C, N * block_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("\nResult Matrix:\n");
        print_matrix(C);
    }

    MPI_Finalize();
    return 0;
}
