//srijitpaulRoll:207173
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MATRIX_SIZE 100
#define NUM_THREADS 4

double matrix[MATRIX_SIZE][MATRIX_SIZE+1];

void *eliminate(void *arg) {
  int pivot = *(int*)arg;
  for (int row = pivot+1; row < MATRIX_SIZE; row++) {
    double multiplier = matrix[row][pivot] / matrix[pivot][pivot];
    for (int col = pivot; col <= MATRIX_SIZE; col++) {
      matrix[row][col] -= multiplier * matrix[pivot][col];
    }
  }
  pthread_exit(NULL);
}

int main() {
  pthread_t threads[NUM_THREADS];
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j <= MATRIX_SIZE; j++) {
      matrix[i][j] = rand() % 10;
    }
  }
  for (int pivot = 0; pivot < MATRIX_SIZE; pivot++) {
    // partial pivoting
    double max = matrix[pivot][pivot];
    int max_row = pivot;
    for (int i = pivot+1; i < MATRIX_SIZE; i++) {
      if (matrix[i][pivot] > max) {
        max = matrix[i][pivot];
        max_row = i;
      }
    }
    if (max_row != pivot) {
      for (int i = pivot; i <= MATRIX_SIZE; i++) {
        double temp = matrix[pivot][i];
        matrix[pivot][i] = matrix[max_row][i];
        matrix[max_row][i] = temp;
      }
    }
    // elimination
    for (int i = 0; i < NUM_THREADS; i++) {
      int *arg = malloc(sizeof(int));
      *arg = pivot;
      pthread_create(&threads[i], NULL, eliminate, arg);
    }
    for (int i = 0; i < NUM_THREADS; i++) {
      pthread_join(threads[i], NULL);
    }
  }
  printf("Solution:\n");
  for (int i = 0; i < MATRIX_SIZE; i++) {
    printf("%f\n", matrix[i][MATRIX_SIZE] / matrix[i][i]);
  }
  return 0;
}

