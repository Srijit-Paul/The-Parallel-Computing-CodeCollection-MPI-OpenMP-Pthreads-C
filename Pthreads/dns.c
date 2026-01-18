//srijitpaulRoll:207173
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MATRIX_SIZE 4
#define NUM_THREADS 4

int matrix_a[MATRIX_SIZE][MATRIX_SIZE];
int matrix_b[MATRIX_SIZE][MATRIX_SIZE];
int matrix_c[MATRIX_SIZE][MATRIX_SIZE];

struct thread_data {
  int thread_id;
  int start_row;
  int start_col;
  int end_row;
  int end_col;
};

void *multiply(void *threadarg) {
  struct thread_data *my_data;
  my_data = (struct thread_data *) threadarg;
  int tid = my_data->thread_id;
  int start_row = my_data->start_row;
  int start_col = my_data->start_col;
  int end_row = my_data->end_row;
  int end_col = my_data->end_col;

  int block_size = MATRIX_SIZE / NUM_THREADS;

  for (int i = start_row; i < end_row; i++) {
    for (int j = start_col; j < end_col; j++) {
      int sum = 0;
      for (int k = 0; k < MATRIX_SIZE; k++) {
        sum += matrix_a[i][k] * matrix_b[k][j];
      }
      matrix_c[i][j] += sum;
    }
  }

  pthread_exit(NULL);
}

int main() {
  pthread_t threads[NUM_THREADS];
  struct thread_data thread_data_array[NUM_THREADS];
  int rc;
  long t;

  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      matrix_a[i][j] = rand() % 10;
      matrix_b[i][j] = rand() % 10;
      matrix_c[i][j] = 0;
    }
  }

  for (t = 0; t < NUM_THREADS; t++) {
    int start_row = t * (MATRIX_SIZE / NUM_THREADS);
    int end_row = (t + 1) * (MATRIX_SIZE / NUM_THREADS);

    thread_data_array[t].thread_id = t;
    thread_data_array[t].start_row = start_row;
    thread_data_array[t].start_col = 0;
    thread_data_array[t].end_row = end_row;
    thread_data_array[t].end_col = MATRIX_SIZE;

    rc = pthread_create(&threads[t], NULL, multiply, (void *) &thread_data_array[t]);
    if (rc) {
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }
  }

  for (t = 0; t < NUM_THREADS; t++) {
    pthread_join(threads[t], NULL);
  }

  printf("Matrix C:\n");
  for (int i = 0; i < MATRIX_SIZE; i++) {
    for (int j = 0; j < MATRIX_SIZE; j++) {
      printf("%d ", matrix_c[i][j]);
    }
    printf("\n");
  }

  pthread_exit(NULL);
}

