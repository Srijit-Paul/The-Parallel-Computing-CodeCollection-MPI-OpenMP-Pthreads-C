#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define ARRAY_SIZE 20

void merge(int *array, int left, int middle, int right) {
    int i, j, k;
    int n1 = middle - left + 1;
    int n2 = right - middle;
    
    int left_array[n1], right_array[n2];

    for (i = 0; i < n1; i++)
        left_array[i] = array[left + i];

    for (j = 0; j < n2; j++)
        right_array[j] = array[middle + 1 + j];

    i = 0;
    j = 0;
    k = left;

    while (i < n1 && j < n2) {
        if (left_array[i] <= right_array[j]) {
            array[k] = left_array[i];
            i++;
        } else {
            array[k] = right_array[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        array[k] = left_array[i];
        i++;
        k++;
    }

    while (j < n2) {
        array[k] = right_array[j];
        j++;
        k++;
    }
}

void merge_sort(int *array, int left, int right) {
    if (left < right) {
        int middle = (left + right) / 2;
        merge_sort(array, left, middle);
        merge_sort(array, middle + 1, right);
        merge(array, left, middle, right);
    }
}

int main(int argc, char *argv[]) {
    int rank, num_processes;
    int array[ARRAY_SIZE];
    int local_array_size, *local_array;
    int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes);

    if (rank == 0) {
        // Generate random array in process 0
        printf("Original Array: ");
        for (i = 0; i < ARRAY_SIZE; i++) {
            array[i] = rand() % 100;
            printf("%d ", array[i]);
        }
        printf("\n");
    }

    // Distribute array across all processes
    MPI_Bcast(array, ARRAY_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    // Divide the array into chunks
    local_array_size = ARRAY_SIZE / num_processes;
    local_array = (int *)malloc(local_array_size * sizeof(int));

    // Scatter the chunks across all processes
    MPI_Scatter(array, local_array_size, MPI_INT, local_array, local_array_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Perform local merge sort
    merge_sort(local_array, 0, local_array_size - 1);

    // Gather the sorted chunks back into the root process
    MPI_Gather(local_array, local_array_size, MPI_INT, array, local_array_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Perform final merge on the sorted chunks
        merge_sort(array, 0, ARRAY_SIZE - 1);

        // Print the sorted array
        printf("Sorted Array: ");
        for (i = 0; i < ARRAY_SIZE; i++)
            printf("%d ", array[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
