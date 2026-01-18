#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>

#define MAX 1000000

void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int *arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quicksort(int *arr, int low, int high) {
    if (low < high) {
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

int main(int argc, char *argv[]) {
    int size, rank, root = 0;
    int arr[MAX];

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    if (rank == root) {
        srand(time(NULL));
        for (int i = 0; i < MAX; i++) {
            arr[i] = rand() % MAX;
        }
    }

    int n = MAX / size;
    int *sub_arr = (int*) malloc(n * sizeof(int));

    MPI_Scatter(arr, n, MPI_INT, sub_arr, n, MPI_INT, root, MPI_COMM_WORLD);

    quicksort(sub_arr, 0, n - 1);

    MPI_Gather(sub_arr, n, MPI_INT, arr, n, MPI_INT, root, MPI_COMM_WORLD);

    if (rank == root) {
        quicksort(arr, 0, MAX - 1);

        // Check if array is sorted
        for (int i = 0; i < MAX - 1; i++) {
            if (arr[i] > arr[i + 1]) {
                printf("Array is not sorted\n");
                break;
            }
        }
    }

    MPI_Finalize();

    return 0;
}
