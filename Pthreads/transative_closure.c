#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_VERTICES 100
#define THREADS 4

int num_vertices;
int adj_matrix[MAX_VERTICES][MAX_VERTICES];
int trans_closure[MAX_VERTICES][MAX_VERTICES];

pthread_mutex_t lock;

void *transitive_closure(void *arg) {
    int *source = (int *) arg;

    for (int k = *source; k < num_vertices; k += THREADS) {
        for (int i = 0; i < num_vertices; i++) {
            for (int j = 0; j < num_vertices; j++) {
                pthread_mutex_lock(&lock);
                trans_closure[i][j] |= (trans_closure[i][k] & trans_closure[k][j]);
                pthread_mutex_unlock(&lock);
            }
        }
    }

    pthread_exit(NULL);
}

void compute_transitive_closure() {
    pthread_t threads[THREADS];
    for (int i = 0; i < THREADS; i++) {
        pthread_create(&threads[i], NULL, transitive_closure, &i);
    }
    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    num_vertices = 4;
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            adj_matrix[i][j] = 0;
            trans_closure[i][j] = 0;
        }
    }

    adj_matrix[0][1] = 1;
    adj_matrix[1][2] = 1;
    adj_matrix[2][3] = 1;
    adj_matrix[3][0] = 1;

    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            trans_closure[i][j] = adj_matrix[i][j];
        }
    }

    compute_transitive_closure();

    printf("Transitive closure matrix:\n");
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            printf("%d ", trans_closure[i][j]);
        }
        printf("\n");
    }

    return 0;
}

