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
    int *k = (int *) arg;
    int i, j;

    for (*k = 0; *k < num_vertices; (*k)++) {
        for (i = 0; i < num_vertices; i++) {
            for (j = 0; j < num_vertices; j++) {
                pthread_mutex_lock(&lock);
                if (dist[i][*k] != INT_MAX && dist[*k][j] != INT_MAX && dist[i][*k] + dist[*k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][*k] + dist[*k][j];
                }
                pthread_mutex_unlock(&lock);
            }
        }
    }

    pthread_exit(NULL);
}

void all_pairs_shortest_paths() {
    pthread_t threads[THREADS];
    int k[THREADS];
    for (int i = 0; i < THREADS; i++) {
        k[i] = i;
        pthread_create(&threads[i], NULL, shortest_paths, &k[i]);
    }
    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    num_vertices = 6;
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            adj_matrix[i][j] = INT_MAX;
        }
        dist[i][i] = 0;
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

    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            dist[i][j] = adj_matrix[i][j];
        }
    }

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

    return 0;
}

