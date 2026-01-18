#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void insertion_sort(int arr[], int n) {
    int i, j, temp;
    for (i = 1; i < n; i++) {
        temp = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > temp) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = temp;
    }
}

int main(int argc, char *argv[]) {
    int rank, size, *arr = NULL, *recv_buf = NULL, i, n;
    double start_time, end_time;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (rank == 0) {
        // Get the array size from user input
        printf("Enter the number of elements: ");
        fflush(stdout);
        scanf("%d", &n);

        // Generate the array
        arr = (int*)malloc(n * sizeof(int));
        for (i = 0; i < n; i++) {
            arr[i] = rand() % n;
        }
        printf("\nUnsorted array:\n");
        for (i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n");

        // Start the timer
        start_time = MPI_Wtime();
    }

    // Broadcast the array size to all processes
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Calculate the chunk size for each process
    int chunk_size = n / size;
    int remainder = n % size;

    // Allocate memory for the receive buffer
    recv_buf = (int*)malloc(chunk_size * sizeof(int));

    // Scatter the array to all processes
    MPI_Scatter(arr, chunk_size, MPI_INT, recv_buf, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    // Sort the local chunk of the array
    insertion_sort(recv_buf, chunk_size);

    // Gather the sorted chunks into a single array
    MPI_Gather(recv_buf, chunk_size, MPI_INT, arr, chunk_size, MPI_INT, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        // Sort the remaining elements of the array
        if (remainder != 0) {
            insertion_sort(&arr[size * chunk_size], remainder);
        }

        // End the timer
        end_time = MPI_Wtime();

        // Print the sorted array and the time taken to sort it
        printf("Sorted array:\n");
        for (i = 0; i < n; i++) {
            printf("%d ", arr[i]);
        }
        printf("\n\nTime taken to sort the array: %f seconds\n", end_time - start_time);

        // Free the memory
        free(arr);
    }

    free(recv_buf);
    MPI_Finalize();
    return 0;
}
