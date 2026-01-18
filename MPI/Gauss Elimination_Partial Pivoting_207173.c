#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 5

int main(int argc, char** argv) {
    int my_rank, comm_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    
    double A[N][N+1] = {{2.0, 1.0, -1.0, 2.0, 3.0, 8.0},
                        {-3.0, -1.0, 2.0, -1.0, -2.0, -11.0},
                        {-2.0, 1.0, 2.0, 1.0, -1.0, -3.0},
                        {2.0, 2.0, 4.0, 1.0, -3.0, -1.0},
                        {1.0, 1.0, 1.0, 1.0, 1.0, 2.0}};

    int i, j, k;
    for (k = 0; k < N-1; k++) {
        int max_row = k;
        double max_val = A[k][k];
        for (i = k+1; i < N; i++) {
            if (A[i][k] > max_val) {
                max_row = i;
                max_val = A[i][k];
            }
        }
        
        if (my_rank == (max_row % comm_size)) {
            double temp[N+1];
            for (j = k; j <= N; j++) {
                temp[j] = A[k][j];
                A[k][j] = A[max_row][j];
                A[max_row][j] = temp[j];
            }
        }

        MPI_Bcast(&A[k][k], N+1-k, MPI_DOUBLE, max_row % comm_size, MPI_COMM_WORLD);
        
        for (i = k+1; i < N; i++) {
            if (i % comm_size == my_rank) {
                double factor = A[i][k] / A[k][k];
                for (j = k; j <= N; j++) {
                    A[i][j] = A[i][j] - factor * A[k][j];
                }
            }
        }
    }

    double x[N];
    for (i = N-1; i >= 0; i--) {
        x[i] = A[i][N];
        for (j = i+1; j < N; j++) {
            if (j % comm_size == my_rank) {
                x[i] = x[i] - A[i][j] * x[j];
            }
        }
        x[i] = x[i] / A[i][i];
        MPI_Bcast(&x[i], 1, MPI_DOUBLE, i % comm_size, MPI_COMM_WORLD);
    }

    if (my_rank == 0) {
        printf("Solution:\n");
        for (i = 0; i < N; i++) {
            printf("x[%d] = %.2f\n", i, x[i]);
        }
    }

    MPI_Finalize();
    return 0;
}
