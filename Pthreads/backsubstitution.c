//srijitpaulRoll:207173
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_SIZE 1000

double matrix[MAX_SIZE][MAX_SIZE + 1];
double solution[MAX_SIZE];

int size;

void *back_substitution(void *arg) {
  int row = *(int *)arg;

  solution[row] = matrix[row][size] / matrix[row][row];

  for (int i = row - 1; i >= 0; i--) {
    matrix[i][size] -= matrix[i][row] * solution[row];
  }

  pthread_exit(NULL);
}

int main() {
  pthread_t threads[MAX_SIZE];
  int rows[MAX_SIZE];

  printf("Enter the size of the matrix: ");
  scanf("%d", &size);

  printf("Enter the elements of the matrix (row by row):\n");
  for (int i = 0; i < size; i++) {
    for (int j = 0; j <=size; j++) {
      scanf("%lf", &matrix[i][j]);
    }
  }

  // Perform back-substitution in parallel
  for (int i = size - 1; i >= 0; i--) {
    rows[i] = i;
    pthread_create(&threads[i], NULL, back_substitution, &rows[i]);
  }

  // Wait for all threads to finish
  for (int i = 0; i < size; i++) {
    pthread_join(threads[i], NULL);
  }

  // Print the solution
  printf("The solution is:\n");
  for (int i = 0; i < size; i++) {
    printf("%f\n", solution[i]);
  }

  return 0;
}

