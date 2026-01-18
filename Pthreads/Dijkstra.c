#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <pthread.h>

#define MAX_VERTICES 100
#define THREADS 4

int num_vertices;
int adj_matrix[MAX_VERTICES][MAX_VERTICES];
int dist[MAX_VERTICES][MAX_VERTICES];

pthread_mutex_t lock;

void *shortest_paths(void *arg) {
    int *source = (int *) arg;
    int u, v, k;

    for (int i = *source; i < num_vertices; i += THREADS) {
        for (int j = 0; j < num_vertices; j++) {
            dist[*source][j] = adj_matrix[*source][j];
        }
        dist[*source][*source] = 0;

        int visited[num_vertices];
        for (int j = 0; j < num_vertices; j++) {
            visited[j] = 0;
        }

        for (int count = 0; count < num_vertices - 1; count++) {
            int min_distance = INT_MAX;
            for (int j = 0; j < num_vertices; j++) {
                if (!visited[j] && dist[*source][j] < min_distance) {
                    min_distance = dist[*source][j];
                    u = j;
                }
            }

            visited[u] = 1;
            for (v = 0; v < num_vertices; v++) {
                if (!visited[v] && adj_matrix[u][v] && dist[*source][u] != INT_MAX
                    && dist[*source][u] + adj_matrix[u][v] < dist[*source][v]) {
                    pthread_mutex_lock(&lock);
                    dist[*source][v] = dist[*source][u] + adj_matrix[u][v];
                    pthread_mutex_unlock(&lock);
                }
            }
        }
    }

    pthread_exit(NULL);
}

void all_pairs_shortest_paths() {
    pthread_t threads[THREADS];
    for (int i = 0; i < num_vertices; i++) {
        pthread_create(&threads[i % THREADS], NULL, shortest_paths, &i);
    }
    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    num_vertices = 6;
    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            adj_matrix[i][j] = INT_MAX;
        }
    }

    adj_matrix[0][1] = 3;
    adj_matrix[0][2] = 8;
    adj_matrix[0][5] = -4;
    adj_matrix[1][3] = 1;
    adj_matrix[1][4] = 7;
    adj_matrix[2][1] = 4;
    adj_matrix[3][2] = -5;
    adj_matrix[3][4] = 2;
    adj_matrix[4][0] = 2;
    adj_matrix[4][2] = 6;

    all_pairs_shortest_paths();

    printf("Shortest path matrix:\n");
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            if (dist[i][j] == INT_MAX) {
                printf("INF ");
            } else {
                printf("%d ", dist[i][j]);
            }
        }
        printf("\n");
    }

    pthread_mutex_destroy(&lock);

    return 0;
}

