//srijitpaulRoll:207173
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MATRIX_SIZE 4
#define BLOCK_SIZE 2

int A[MATRIX_SIZE][MATRIX_SIZE];
int B[MATRIX_SIZE][MATRIX_SIZE];
int C[MATRIX_SIZE][MATRIX_SIZE];

int num_threads = 4;
pthread_t threads[4];

void* worker(void* arg) {
    int tid = *(int*)arg;
    int i, j, k, l, m;
    int x, y;
    int block_x = tid / BLOCK_SIZE;
    int block_y = tid % BLOCK_SIZE;
    int a_start = block_x * BLOCK_SIZE;
    int b_start = block_y * BLOCK_SIZE;
    int c_start = tid;
    
    int a[MATRIX_SIZE][MATRIX_SIZE];
    int b[MATRIX_SIZE][MATRIX_SIZE];
    int c[MATRIX_SIZE][MATRIX_SIZE];
    
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            a[i][j] = A[a_start + i][j];
            b[i][j] = B[i][b_start + j];
            c[i][j] = 0;
        }
    }
    
    for (l = 0; l < BLOCK_SIZE; l++) {
        for (k = 0; k < MATRIX_SIZE; k++) {
            for (i = 0; i < MATRIX_SIZE; i++) {
                c[i][l] += a[i][k] * b[k][l];
            }
        }
    }
    
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < BLOCK_SIZE; j++) {
            C[c_start + i][j] = c[i][j];
        }
    }
    
    pthread_exit(NULL);
}

int main() {
    int i, j;
    int tid[4];
    
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
            C[i][j] = 0;
        }
    }
    
    for (i = 0; i < num_threads; i++) {
        tid[i] = i;
        pthread_create(&threads[i], NULL, worker, &tid[i]);
    }
    
    for (i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    
    printf("Matrix A:\n");
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", A[i][j]);
        }
        printf("\n");
    }
    
    printf("Matrix B:\n");
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", B[i][j]);
        }
        printf("\n");
    }
    
    printf("Matrix C:\n");
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            printf("%d ", C[i][j]);
        }
        printf("\n");
    }
    
    return 0;
}

