#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

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

int parent[MAX_VERTICES];
pthread_mutex_t lock;

void *find_set(void *arg) {
    int v = *(int *) arg;
    while (parent[v] != v) {
        v = parent[v];
    }
    pthread_exit(NULL);
}

void *union_set(void *arg) {
    struct Edge e = *(struct Edge *) arg;
    pthread_mutex_lock(&lock);
    int set_u = parent[e.u];
    int set_v = parent[e.v];
    parent[set_v] = set_u;
    pthread_mutex_unlock(&lock);
    pthread_exit(NULL);
}

int compare_edges(const void *a, const void *b) {
    struct Edge *e1 = (struct Edge *) a;
    struct Edge *e2 = (struct Edge *) b;
    return (e1->weight - e2->weight);
}

void kruskal(struct Graph *graph, struct Edge *mst) {
    for (int i = 0; i < graph->num_vertices; i++) {
        parent[i] = i;
    }
    qsort(graph->edges, graph->num_edges, sizeof(struct Edge), compare_edges);

    int i = 0, j = 0;
    pthread_t threads[THREADS];
    while (i < graph->num_vertices - 1 && j < graph->num_edges) {
        struct Edge e = graph->edges[j++];
        int *u = malloc(sizeof(int));
        int *v = malloc(sizeof(int));
        *u = e.u;
        *v = e.v;

        pthread_create(&threads[i % THREADS], NULL, find_set, u);
        pthread_create(&threads[(i + 1) % THREADS], NULL, find_set, v);

        pthread_join(threads[i % THREADS], NULL);
        pthread_join(threads[(i + 1) % THREADS], NULL);

        if (parent[*u] != parent[*v]) {
            mst[i++] = e;

            pthread_create(&threads[i % THREADS], NULL, union_set, &e);
            pthread_join(threads[i % THREADS], NULL);
        }

        free(u);
        free(v);
    }
}

int main() {
    struct Graph graph = {6, 9, {
            {0, 1, 4}, {0, 2, 4}, {1, 2, 2},
            {1, 0, 4}, {2, 0, 4}, {2, 1, 2},
            {2, 3, 3}, {2, 5, 2}, {3, 4, 2}
    }};
    struct Edge mst[MAX_VERTICES - 1];
    kruskal(&graph, mst);

    printf("Minimum Spanning Tree:\n");
int total_weight = 0;
for (int i = 0; i < graph.num_vertices - 1; i++) {
    printf("(%d, %d) weight=%d\n", mst[i].u, mst[i].v, mst[i].weight);
    total_weight += mst[i].weight;
}
printf("Total weight: %d\n", total_weight);

return 0;
}
