#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define INF 9999999

int min(int a, int b) {
  return (a < b ? a : b);
}

int main(int argc, char *argv[]) {
  int rank, size, n;
  int *graph, *result;
  int i, j, k, offset;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);

  if (rank == 0) {
    printf("Enter number of vertices: ");
    fflush(stdout);
    scanf("%d", &n);

    graph = (int *) malloc(n * n * sizeof(int));
    result = (int *) malloc(n * n * sizeof(int));

    printf("Enter the graph:\n");
    for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        scanf("%d", &graph[i*n + j]);
      }
    }

    // send graph to all processors
    for (i = 1; i < size; i++) {
      MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
      MPI_Send(&graph[0], n*n, MPI_INT, i, 0, MPI_COMM_WORLD);
    }
  } else {
    MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    graph = (int *) malloc(n * n * sizeof(int));
    result = (int *) malloc(n * n * sizeof(int));

    MPI_Recv(&graph[0], n*n, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }

  // initialize result matrix
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
      if (i == j) {
        result[i*n + j] = 0;
      } else {
        result[i*n + j] = graph[i*n + j];
      }
    }
  }

  // compute shortest paths
  for (k = 0; k < n; k++) {
    for (i = rank; i < n; i += size) {
      for (j = 0; j < n; j++) {
        offset = i*n + k;
        result[i*n + j] = min(result[i*n + j], result[offset] + graph[offset + j]);
      }
    }

    // gather results from all processors
    MPI_Allgather(&result[rank*n], n, MPI_INT, &result[0], n, MPI_INT, MPI_COMM_WORLD);
  }

  if (rank == 0) {
    printf("\nThe shortest paths are:\n");
    for (i = 0; i < n; i++) {
      for (j = 0; j < n; j++) {
        if (result[i*n + j] == INF) {
          printf("INF ");
        } else {
          printf("%d ", result[i*n + j]);
        }
      }
      printf("\n");
    }

    free(graph);
    free(result);
  }

  MPI_Finalize();
  return 0;
}
