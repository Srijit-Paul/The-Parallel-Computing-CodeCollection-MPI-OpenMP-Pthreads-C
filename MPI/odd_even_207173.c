#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void odd_even_sort(int *a, int n) {
    int phase, i, tmp;
    for (phase = 0; phase < n; phase++) {
        if (phase % 2 == 0) { // even phase
            for (i = 1; i < n; i += 2) {
                if (a[i-1] > a[i]) {
                    tmp = a[i-1];
                    a[i-1] = a[i];
                    a[i] = tmp;
                }
            }
        } else { // odd phase
            for (i = 1; i < n-1; i += 2) {
                if (a[i] > a[i+1]) {
                    tmp = a[i+1];
                    a[i+1] = a[i];
                    a[i] = tmp;
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    int rank, size, i;
    int* local_data = NULL;
    int* recv_buf = NULL;
    int* send_buf = NULL;
    int local_size;
    int n;
    
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (argc != 2) {
        if (rank == 0) {
            fprintf(stderr, "Usage: mpirun -np <num_procs> %s <n>\n", argv[0]);
        }
        MPI_Finalize();
        return 1;
    }

    n = atoi(argv[1]);
    local_size = n / size;
    local_data = (int*)malloc(local_size * sizeof(int));

    if (rank == 0) {
        int* data = (int*)malloc(n * sizeof(int));
        for (i = 0; i < n; i++) {
            data[i] = rand() % n;
        }

        // scatter data
        for (i = 0; i < size; i++) {
            send_buf = data + i * local_size;
            MPI_Send(send_buf, local_size, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }

    // receive local data
    recv_buf = (int*)malloc(local_size * sizeof(int));
    MPI_Recv(recv_buf, local_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // sort local data
    odd_even_sort(recv_buf, local_size);

    // gather sorted data
    if (rank == 0) {
        int* sorted_data = (int*)malloc(n * sizeof(int));
        for (i = 0; i < local_size; i++) {
            sorted_data[i] = recv_buf[i];
        }
        for (i = 1; i < size; i++) {
            recv_buf = sorted_data + i * local_size;
            MPI_Recv(recv_buf, local_size, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
        // sort the final result
        odd_even_sort(sorted_data, n);
        free(sorted_data);
    } else {
        MPI_Send(recv_buf, local_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    free(local_data);
    free(recv_buf);
    MPI_Finalize();
    return 0;
}
