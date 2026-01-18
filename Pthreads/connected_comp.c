#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_VERTICES 100
#define THREADS 4

int num_vertices;
int adj_matrix[MAX_VERTICES][MAX_VERTICES];
int visited[MAX_VERTICES];
int components[MAX_VERTICES];
int component_index = 0;

pthread_mutex_t lock;

void *dfs(void *arg) {
    int *v = (int *) arg;

    visited[*v] = 1;
    components[*v] = component_index;

    for (int i = 0; i < num_vertices; i++) {
        if (!visited[i] && adj_matrix[*v][i]) {
            dfs(&i);
        }
    }

    pthread_exit(NULL);
}

void connected_components() {
    pthread_t threads[THREADS];

    for (int i = 0; i < num_vertices; i++) {
        if (!visited[i]) {
            pthread_create(&threads[i % THREADS], NULL, dfs, &i);
            component_index++;
        }
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    num_vertices = 6;

    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            adj_matrix[i][j] = 0;
        }
        visited[i] = 0;
        components[i] = -1;
    }

    adj_matrix[0][1] = 1;
    adj_matrix[1][0] = 1;
    adj_matrix[2][3] = 1;
    adj_matrix[3][2] = 1;
    adj_matrix[3][4] = 1;
    adj_matrix[4][3] = 1;

    connected_components();

    printf("Connected components:\n");
    for (int i = 0; i < num_vertices; i++) {
        printf("%d ", components[i]);
    }
    printf("\n");

    return 0;
}

