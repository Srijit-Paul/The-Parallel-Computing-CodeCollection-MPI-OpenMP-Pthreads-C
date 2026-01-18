#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE 10

void bubble_sort(int *arr, int n) {
    int i, j, temp;
    for (i = 0; i < n-1; i++) {
        for (j = 0; j < n-i-1; j++) {
            if (arr[j] > arr[j+1]) {
                temp = arr[j];
                arr[j] = arr[j+1];
                arr[j+1] = temp;
            }
        }
    }
}

int main(int argc, char *argv[]) {
    int rank, size;
    int i;
    int arr[ARRAY_SIZE];
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Generate random integers for the array
        printf("Unsorted array: ");
        for (i = 0; i < ARRAY_SIZE; i++) {
            arr[i] = rand() % 100;
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    // Scatter the array to all processes
    MPI_Scatter(arr, ARRAY_SIZE/size, MPI_INT, arr, ARRAY_SIZE/size, MPI_INT, 0, MPI_COMM_WORLD);

    // Sort the local portion of the array
    bubble_sort(arr, ARRAY_SIZE/size);

    // Gather the sorted local portions into a single array
    MPI_Gather(arr, ARRAY_SIZE/size, MPI_INT, arr, ARRAY_SIZE/size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Sorted array: ");
        for (i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
