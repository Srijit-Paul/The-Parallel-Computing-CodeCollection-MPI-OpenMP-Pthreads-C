#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define MAX_VERTICES 1000

int adj_matrix[MAX_VERTICES][MAX_VERTICES], visited[MAX_VERTICES];

void dfs(int rank, int vertices, int vertex) {
    visited[vertex] = 1;
    for (int i = 0; i < vertices; i++) {
        if (adj_matrix[vertex][i] && !visited[i]) {
            printf("Process %d: Vertex %d is connected to vertex %d.\n", rank, vertex, i);
            dfs(rank, vertices, i);
        }
    }
}

int main(int argc, char **argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    int vertices, edges;
    if (rank == 0) {
        printf("Enter the number of vertices and edges: ");
        scanf("%d %d", &vertices, &edges);
        for (int i = 0; i < edges; i++) {
            int u, v;
            printf("Enter edge %d: ", i + 1);
            scanf("%d %d", &u, &v);
            adj_matrix[u][v] = adj_matrix[v][u] = 1;
        }
    }
    MPI_Bcast(&vertices, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(&adj_matrix[0][0], MAX_VERTICES * MAX_VERTICES, MPI_INT, 0, MPI_COMM_WORLD);
    for (int i = 0; i < vertices; i++) {
        visited[i] = 0;
    }
    int start_vertex = rank;
    while (start_vertex < vertices) {
        if (!visited[start_vertex]) {
            printf("Process %d: Starting DFS from vertex %d.\n", rank, start_vertex);
            dfs(rank, vertices, start_vertex);
        }
        start_vertex += size;
    }
    MPI_Finalize();
    return 0;
}
