#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 100

void selection_sort(int *array, int n) {
    int i, j, min_index, temp;
    for (i = 0; i < n-1; i++) {
        min_index = i;
        for (j = i+1; j < n; j++) {
            if (array[j] < array[min_index]) {
                min_index = j;
            }
        }
        if (min_index != i) {
            temp = array[i];
            array[i] = array[min_index];
            array[min_index] = temp;
        }
    }
}

int main(int argc, char **argv) {
    int rank, size;
    int i, j, k, temp;
    int array[ARRAY_SIZE];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Status status;
    int chunk_size = ARRAY_SIZE / size;
    int *local_array = malloc(chunk_size * sizeof(int));
    // Initialize the array on the root process
    if (rank == 0) {
        for (i = 0; i < ARRAY_SIZE; i++) {
            array[i] = ARRAY_SIZE - i;
        }
    }
    // Scatter the array across all processes
    MPI_Scatter(array, chunk_size, MPI_INT, local_array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    // Sort the local array
    selection_sort(local_array, chunk_size);
    // Gather the sorted arrays from all processes
    MPI_Gather(local_array, chunk_size, MPI_INT, array, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);
    // Merge the sorted arrays on the root process
    if (rank == 0) {
        for (i = 0; i < size; i++) {
            for (j = i*chunk_size; j < (i+1)*chunk_size; j++) {
                temp = array[j];
                for (k = j-1; k >= i*chunk_size && temp < array[k]; k--) {
                    array[k+1] = array[k];
                }
                array[k+1] = temp;
            }
        }
        printf("Sorted array:\n");
        for (i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", array[i]);
        }
        printf("\n");
    }
    MPI_Finalize();
    return 0;
}
