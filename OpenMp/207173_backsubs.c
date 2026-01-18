#include <stdio.h>
#include <omp.h>

#define N 4

void print_vector(double x[N]) {
    for (int i = 0; i < N; i++) {
        printf("%.2f ", x[i]);
    }
    printf("\n");
}

int main() {
    double U[N][N] = {{3, 1, -1, 2},
                      {0, -2, 8, -5},
                      {0, 0, 2, 3},
                      {0, 0, 0, 1}};
    double b[N] = {7, 3, 8, 1};
    double x[N];

    printf("Upper triangular matrix:\n");
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            printf("%.2f ", U[i][j]);
        }
        printf("\n");
    }

    printf("Right-hand side vector:\n");
    print_vector(b);

    // Back-substitution
    double sum;
    #pragma omp parallel for shared(U,b,x) private(sum) schedule(static)
    for (int i = N - 1; i >= 0; i--) {
        sum = 0.0;
        for (int j = i + 1; j < N; j++) {
            sum += U[i][j] * x[j];
        }
        x[i] = (b[i] - sum) / U[i][i];
    }

    printf("Solution vector:\n");
    print_vector(x);

    return 0;
}
