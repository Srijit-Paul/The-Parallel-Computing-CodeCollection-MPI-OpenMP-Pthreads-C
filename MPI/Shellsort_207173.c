#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 10

void shellSort(int arr[], int n) {
    int gap, i, j, temp;

    for (gap = n / 2; gap > 0; gap /= 2) {
        for (i = gap; i < n; i++) {
            for (j = i - gap; j >= 0 && arr[j] > arr[j + gap]; j -= gap) {
                temp = arr[j];
                arr[j] = arr[j + gap];
                arr[j + gap] = temp;
            }
        }
    }
}

void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;

    int L[n1], R[n2];

    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        }
        else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(int arr[], int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);

        merge(arr, l, m, r);
    }
}

int main(int argc, char* argv[]) {
    int size, rank;
    int arr[N] = { 5, 1, 9, 4, 6, 2, 8, 3, 10, 7 };
    int i;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int local_arr[N / size];
    MPI_Scatter(arr, N / size, MPI_INT, local_arr, N / size, MPI_INT, 0, MPI_COMM_WORLD);

    shellSort(local_arr, N / size);

    MPI_Gather(local_arr, N / size, MPI_INT, arr, N / size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        mergeSort(arr, 0, N - 1);

        printf("Sorted array: ");
        for (i = 0; i < N; i++)
            printf("%d ", arr[i]);
        printf("\n");
    }

    MPI_Finalize();

    return 0;
}
