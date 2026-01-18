#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>

#define MAX_VERTICES 100
#define MAX_EDGES (MAX_VERTICES * (MAX_VERTICES - 1) / 2)
#define THREADS 4

struct Edge {
    int u;
    int v;
    int weight;
};

struct Graph {
    int num_vertices;
    int num_edges;
    struct Edge edges[MAX_EDGES];
};

int dist[MAX_VERTICES];
pthread_mutex_t lock;

void *relax(void *arg) {
    struct Edge *e = (struct Edge *) arg;
    int u = e->u;
    int v = e->v;
    int weight = e->weight;

    pthread_mutex_lock(&lock);
    if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
        dist[v] = dist[u] + weight;
    }
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

void bellman_ford(struct Graph *graph, int source) {
    for (int i = 0; i < graph->num_vertices; i++) {
        dist[i] = INT_MAX;
    }
    dist[source] = 1;

    int i = 0, j = 0;
    pthread_t threads[THREADS];
    while (i < graph->num_vertices - 1) {
        for (j = 0; j < THREADS && i < graph->num_vertices - 1; j++, i++) {
            for (int k = 0; k < graph->num_edges; k++) {
                struct Edge e = graph->edges[k];
                pthread_create(&threads[k % THREADS], NULL, relax, &e);
            }
            for (int k = 0; k < graph->num_edges && k < THREADS; k++) {
                pthread_join(threads[k], NULL);
            }
        }
    }
}

int main() {
    struct Graph graph = {6, 9, {
            {0, 1, 4}, {0, 2, 4}, {1, 2, 2},
            {1, 0, 4}, {2, 0, 4}, {2, 1, 2},
            {2, 3, 3}, {2, 5, 2}, {3, 4, 2}
    }};
    int source = 0;
    bellman_ford(&graph, source);

    printf("Shortest distances from vertex %d:\n", source);
    for (int i = 0; i < graph.num_vertices; i++) {
        if (dist[i] == INT_MAX) {
            printf("%d: Infinity\n", i);
        } else {
            printf("%d: %d\n", i, dist[i]);
        }
    }

    return 0;
}

