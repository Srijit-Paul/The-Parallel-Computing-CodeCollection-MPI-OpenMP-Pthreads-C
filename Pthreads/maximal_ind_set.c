#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

#define MAX_VERTICES 100
#define THREADS 4

int num_vertices;
int adj_matrix[MAX_VERTICES][MAX_VERTICES];
bool visited[MAX_VERTICES];
bool independent_set[MAX_VERTICES];

pthread_mutex_t lock;

void *find_independent_set(void *arg) {
    int *start = (int *) arg;
    int i, j;

    for (i = *start; i < num_vertices; i += THREADS) {
        if (!visited[i]) {
            bool is_adjacent = false;
            for (j = 0; j < num_vertices; j++) {
                if (visited[j] && adj_matrix[i][j]) {
                    is_adjacent = true;
                    break;
                }
            }
            if (!is_adjacent) {
                pthread_mutex_lock(&lock);
                visited[i] = true;
                independent_set[i] = true;
                pthread_mutex_unlock(&lock);
            }
        }
    }
    pthread_exit(NULL);
}

void maximal_independent_set() {
    for (int i = 0; i < num_vertices; i++) {
        visited[i] = false;
        independent_set[i] = false;
    }
    visited[0] = true;
    independent_set[0] = true;

    pthread_t threads[THREADS];
    for (int i = 0; i < THREADS; i++) {
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&threads[i], NULL, find_independent_set, arg);
    }

    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    for (int i = 0; i < num_vertices; i++) {
        if (independent_set[i]) {
            printf("%d ", i);
        }
    }
}

int main() {
    num_vertices = 7;
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            adj_matrix[i][j] = 0;
        }
    }

    adj_matrix[0][1] = 1;
    adj_matrix[1][0] = 1;
    adj_matrix[1][2] = 1;
    adj_matrix[2][1] = 1;
    adj_matrix[2][3] = 1;
    adj_matrix[3][2] = 1;
    adj_matrix[3][4] = 1;
    adj_matrix[4][3] = 1;
    adj_matrix[5][6] = 1;
    adj_matrix[6][5] = 1;

    maximal_independent_set();

    return 0;
    }

