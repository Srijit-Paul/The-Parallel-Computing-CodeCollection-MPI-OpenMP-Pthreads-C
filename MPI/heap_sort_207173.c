#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void swap(int *x, int *y) {
    int temp = *x;
    *x = *y;
    *y = temp;
}

void heapify(int arr[], int n, int i) {
    int largest = i; // Initialize largest as root
    int l = 2*i + 1; // left = 2*i + 1
    int r = 2*i + 2; // right = 2*i + 2

    // If left child is larger than root
    if (l < n && arr[l] > arr[largest])
        largest = l;

    // If right child is larger than largest so far
    if (r < n && arr[r] > arr[largest])
        largest = r;

    // If largest is not root
    if (largest != i) {
        swap(&arr[i], &arr[largest]);

        // Recursively heapify the affected sub-tree
        heapify(arr, n, largest);
    }
}

void heapSort(int arr[], int n) {
    // Build heap (rearrange array)
    for (int i = n / 2 - 1; i >= 0; i--)
        heapify(arr, n, i);

    // One by one extract an element from heap
    for (int i = n - 1; i > 0; i--) {
        // Move current root to end
        swap(&arr[0], &arr[i]);

        // call max heapify on the reduced heap
        heapify(arr, i, 0);
    }
}

int main(int argc, char **argv) {
    int rank, size;
    int *arr, *sub_arr;
    int n = 10; // size of the array to be sorted

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // scatter the array to all processes
    sub_arr = (int*)malloc(n/size*sizeof(int));
    MPI_Scatter(arr, n/size, MPI_INT, sub_arr, n/size, MPI_INT, 0, MPI_COMM_WORLD);

    // sort the subarray using heap sort
    heapSort(sub_arr, n/size);

    // gather the sorted subarrays
    MPI_Gather(sub_arr, n/size, MPI_INT, arr, n/size, MPI_INT, 0, MPI_COMM_WORLD);

    // sort the gathered array using heap sort
    if (rank == 0) {
        heapSort(arr, n);
        printf("Sorted array: ");
        for (int i = 0; i < n; i++)
            printf("%d ", arr[i]);
        printf("\n");
    }

    MPI_Finalize();
    return 0;
}
