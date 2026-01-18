//srijitpaulRoll:207173
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MATRIX_SIZE 100

double matrix[MATRIX_SIZE][MATRIX_SIZE+1];
int n;

void *eliminate(void *arg) {
  int row = *(int*)arg;
  for (int i = row+1; i < n; i++) {
    double factor = matrix[i][row] / matrix[row][row];
    for (int j = row; j <= n; j++) {
      matrix[i][j] -= factor * matrix[row][j];
    }
  }
  pthread_exit(NULL);
}

int main() {
  pthread_t threads[MATRIX_SIZE];
  printf("Enter matrix size: ");
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j <= n; j++) {
      printf("Enter matrix element [%d][%d]: ", i, j);
      scanf("%lf", &matrix[i][j]);
    }
  }
  for (int i = 0; i < n; i++) {
    int *row_ptr = malloc(sizeof(int));
    *row_ptr = i;
    pthread_create(&threads[i], NULL, eliminate, row_ptr);
  }
  for (int i = 0; i < n; i++) {
    pthread_join(threads[i], NULL);
  }
  printf("Matrix after elimination:\n");
  for (int i = 0; i < n; i++) {
    for (int j = 0; j <= n; j++) {
      printf("%.2lf ", matrix[i][j]);
    }
    printf("\n");
  }
  return 0;
}

