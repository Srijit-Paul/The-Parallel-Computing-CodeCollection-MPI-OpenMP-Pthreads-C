#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define BUCKET_SIZE 100

int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

int main(int argc, char** argv) {
    int num_elements_per_proc, world_rank, world_size;
    int *sub_list, *bucket, *local_list;
    int i, j, bucket_index, local_bucket_size;
    double start_time, end_time;

    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    num_elements_per_proc = BUCKET_SIZE / world_size;

    sub_list = (int*)malloc(num_elements_per_proc * sizeof(int));
    bucket = (int*)malloc(BUCKET_SIZE * sizeof(int));
    local_list = (int*)malloc(BUCKET_SIZE * sizeof(int));

    for (i = 0; i < num_elements_per_proc; i++) {
        sub_list[i] = rand() % BUCKET_SIZE;
    }

    MPI_Barrier(MPI_COMM_WORLD);
    start_time = MPI_Wtime();

    MPI_Scatter(sub_list, num_elements_per_proc, MPI_INT, local_list, BUCKET_SIZE, MPI_INT, 0, MPI_COMM_WORLD);

    for (i = 0; i < BUCKET_SIZE; i++) {
        bucket[i] = 0;
    }

    for (i = 0; i < num_elements_per_proc; i++) {
        bucket_index = local_list[i];
        bucket[bucket_index]++;
    }

    MPI_Allreduce(bucket, local_list, BUCKET_SIZE, MPI_INT, MPI_SUM, MPI_COMM_WORLD);

    local_bucket_size = 0;
    for (i = 0; i < BUCKET_SIZE; i++) {
        local_bucket_size += local_list[i];
    }

    j = 0;
    for (i = 0; i < BUCKET_SIZE; i++) {
        while (local_list[i] > 0) {
            sub_list[j] = i;
            local_list[i]--;
            j++;
        }
    }

    MPI_Gather(sub_list, num_elements_per_proc, MPI_INT, local_list, num_elements_per_proc, MPI_INT, 0, MPI_COMM_WORLD);

    if (world_rank == 0) {
        qsort(local_list, BUCKET_SIZE, sizeof(int), compare);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    end_time = MPI_Wtime();

    if (world_rank == 0) {
        printf("Sorted list:\n");
        for (i = 0; i < BUCKET_SIZE; i++) {
            printf("%d ", local_list[i]);
        }
        printf("\nTime taken: %f\n", end_time - start_time);
    }

    free(sub_list);
    free(bucket);
    free(local_list);

    MPI_Finalize();
    return 0;
}
