#include <stdio.h>
#include <limits.h>
#include <omp.h>

#define V 5

void print_distances(int dist[V]) {
    printf("Vertex   Distance from Source\n");
    for (int i = 0; i < V; i++) {
        printf("%d \t\t %d\n", i, dist[i]);
    }
}

int min_distance(int dist[V], int visited[V]) {
    int min = INT_MAX;
    int min_index = -1;

    #pragma omp parallel for shared(min, min_index)
    for (int v = 0; v < V; v++) {
        if (visited[v] == 0 && dist[v] <= min) {
            #pragma omp critical
            if (dist[v] < min) {
                min = dist[v];
                min_index = v;
            }
        }
    }

    return min_index;
}

void dijkstra(int graph[V][V], int source) {
    int dist[V];
    int visited[V];

    #pragma omp parallel for
    for (int i = 0; i < V; i++) {
        dist[i] = INT_MAX;
        visited[i] = 0;
    }

    dist[source] = 0;

    for (int count = 0; count < V-1; count++) {
        int u = min_distance(dist, visited);

        visited[u] = 1;

        #pragma omp parallel for
        for (int v = 0; v < V; v++) {
            if (!visited[v] && graph[u][v] && dist[u] != INT_MAX && dist[u] + graph[u][v] < dist[v]) {
                dist[v] = dist[u] + graph[u][v];
            }
        }
    }

    print_distances(dist);
}

int main() {
    int graph[V][V] = {{0, 10, 20, 0, 0},
                       {10, 0, 5, 30, 0},
                       {20, 5, 0, 15, 20},
                       {0, 30, 15, 0, 10},
                       {0, 0, 20, 10, 0}};

    dijkstra(graph, 0);

    return 0;
}
