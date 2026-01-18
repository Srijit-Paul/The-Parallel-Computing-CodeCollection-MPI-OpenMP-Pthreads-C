#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <math.h>

void hypercube_bitonic_sort(int *a, int n, int my_rank, int p, MPI_Comm comm) {
    int partner_mask, partner_rank, i, j, temp;
    for (i = 0; i < log2(p); i++) {
        for (j = i; j >= 0; j--) {
            partner_mask = (int) pow(2, j);
            partner_rank = my_rank ^ partner_mask;
            if (my_rank & partner_mask) {
                MPI_Sendrecv(a, n, MPI_INT, partner_rank, 0, &temp, n, MPI_INT, partner_rank, 0, comm, MPI_STATUS_IGNORE);
                if (temp > a[0]) {
                    a[0] = temp;
                }
            } else {
                MPI_Sendrecv(a + (n-1), n, MPI_INT, partner_rank, 0, &temp, n, MPI_INT, partner_rank, 0, comm, MPI_STATUS_IGNORE);
                if (temp < a[n-1]) {
                    a[n-1] = temp;
                }
            }
            MPI_Barrier(comm);
            int k;
            for (k = 0; k < n/2; k++) {
                if (a[k] > a[k + n/2]) {
                    int tmp = a[k];
                    a[k] = a[k + n/2];
                    a[k + n/2] = tmp;
                }
            }
        }
    }
}

int main(int argc, char** argv) {
    int my_rank, p, n;
    int *data = NULL;
    MPI_Comm comm;

    MPI_Init(&argc, &argv);
    comm = MPI_COMM_WORLD;
    MPI_Comm_size(comm, &p);
    MPI_Comm_rank(comm, &my_rank);

    if (my_rank == 0) {
        printf("Enter the number of elements:\n");
        scanf("%d", &n);
        data = (int*) malloc(n * sizeof(int));
        printf("Enter the elements:\n");
        int i;
        for (i = 0; i < n; i++) {
            scanf("%d", &data[i]);
        }
    }

    MPI_Bcast(&n, 1, MPI_INT, 0, comm);
    int chunk_size = n/p;
    int *local_data = (int*) malloc(chunk_size * sizeof(int));
    MPI_Scatter(data, chunk_size, MPI_INT, local_data, chunk_size, MPI_INT, 0, comm);

    hypercube_bitonic_sort(local_data, chunk_size, my_rank, p, comm);

    MPI_Gather(local_data, chunk_size, MPI_INT, data, chunk_size, MPI_INT, 0, comm);

    if (my_rank == 0) {
        printf("Sorted array:\n");
        int i;
        for (i = 0; i < n; i++) {
            printf("%d ", data[i]);
        }
        printf("\n");
        free(data);
    }
    free(local_data);
    MPI_Finalize();
    return 0;
}
