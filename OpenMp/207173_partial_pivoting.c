#include <stdio.h>
#include <omp.h>

#define N 4

void swap_rows(double A[][N], int i, int j) {
    double temp;
    #pragma omp parallel for shared(A) private(temp)
    for (int k = 0; k < N; k++) {
        temp = A[i][k];
        A[i][k] = A[j][k];
        A[j][k] = temp;
    }
}

void print_matrix(double A[][N]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", A[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    double A[N][N] = {{3, 1, -1, 2},
                      {2, -1, 7, -2},
                      {6, 2, -5, 1},
                      {1, -5, 2, 4}};

    printf("Original matrix:\n");
    print_matrix(A);

    int pivot_row;
    double max_value, pivot_value;

    // Partial pivoting
    for (int k = 0; k < N - 1; k++) {
        pivot_row = k;
        max_value = A[k][k];

        #pragma omp parallel for shared(A) reduction(max:max_value) schedule(static)
        for (int i = k + 1; i < N; i++) {
            if (A[i][k] > max_value) {
                max_value = A[i][k];
                pivot_row = i;
            }
        }

        if (pivot_row != k) {
            swap_rows(A, k, pivot_row);
        }

        #pragma omp parallel for shared(A) private(pivot_value) schedule(static)
        for (int i = k + 1; i < N; i++) {
            pivot_value = A[i][k] / A[k][k];
            for (int j = k; j < N; j++) {
                A[i][j] = A[i][j] - pivot_value * A[k][j];
            }
        }
    }

    printf("Upper triangular matrix:\n");
    print_matrix(A);

    return 0;
}
