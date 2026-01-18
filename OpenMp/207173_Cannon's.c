#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 1000
#define BLOCK_SIZE 100

int main()
{
    int i, j, k, l;
    int A[N][N], B[N][N], C[N][N], C_ref[N][N];
    int num_threads, thread_id;
    int block_offset_x, block_offset_y;

    // initialize matrices
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            A[i][j] = rand() % 10;
            B[i][j] = rand() % 10;
            C[i][j] = 0;
            C_ref[i][j] = 0;
        }
    }

    // serial reference implementation
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                C_ref[i][j] += A[i][k] * B[k][j];
            }
        }
    }

    // parallel implementation
    num_threads = omp_get_max_threads();
    omp_set_nested(1);
    #pragma omp parallel shared(A, B, C) private(i, j, k, l, thread_id, block_offset_x, block_offset_y)
    {
        thread_id = omp_get_thread_num();
        int num_blocks = N / BLOCK_SIZE;
        int blocks_per_thread = num_blocks / num_threads;
        int start_block = thread_id * blocks_per_thread;
        int end_block = start_block + blocks_per_thread;
        if (thread_id == num_threads - 1) {
            end_block = num_blocks;
        }
        for (l = 0; l < num_blocks; l++) {
            block_offset_x = l * BLOCK_SIZE;
            block_offset_y = l * BLOCK_SIZE;
            for (i = start_block; i < end_block; i++) {
                block_offset_x = i * BLOCK_SIZE;
                for (j = 0; j < num_blocks; j++) {
                    block_offset_y = j * BLOCK_SIZE;
                    for (k = 0; k < num_blocks; k++) {
                        #pragma omp task shared(A, B, C) private(i, j, k, block_offset_x, block_offset_y)
                        {
                            int ii, jj, kk;
                            for (ii = 0; ii < BLOCK_SIZE; ii++) {
                                for (jj = 0; jj < BLOCK_SIZE; jj++) {
                                    for (kk = 0; kk < BLOCK_SIZE; kk++) {
                                        C[block_offset_x + ii][block_offset_y + jj] += A[block_offset_x + ii][block_offset_x + kk] * B[block_offset_x + kk][block_offset_y + jj];
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        #pragma omp taskwait
    }

    // verify results
    int is_correct = 1;
        for (j = 0; j < N; j++) {
            if (C[i][j] != C_ref[i][j]) {
                is_correct = 0;
                break;
            }
            if (!is_correct) 
            {
                break;
            }
    }
    


if (is_correct) {
    printf("Matrix multiplication is correct.\n");
} else {
    printf("Matrix multiplication is incorrect.\n");
}

    return 0;
}
       
