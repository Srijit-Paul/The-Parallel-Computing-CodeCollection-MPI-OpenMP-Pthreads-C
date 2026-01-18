#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define THREADS 4

int num_items;
int *items;

pthread_mutex_t lock;

void compare(int i, int j, int dir) {
    if (dir == (items[i] > items[j])) {
        int tmp = items[i];
        items[i] = items[j];
        items[j] = tmp;
    }
}

void bitonic_merge(int low, int cnt, int dir) {
    if (cnt > 1) {
        int k = cnt / 2;
        for (int i = low; i < low + k; i++) {
            compare(i, i + k, dir);
        }
        bitonic_merge(low, k, dir);
        bitonic_merge(low + k, k, dir);
    }
}

void bitonic_sort(int low, int cnt, int dir) {
    if (cnt > 1) {
        int k = cnt / 2;
        bitonic_sort(low, k, !dir);
        bitonic_sort(low + k, k, dir);
        bitonic_merge(low, cnt, dir);
    }
}

void *sort(void *arg) {
    int *id = (int *) arg;
    int chunk_size = num_items / THREADS;
    int low = *id * chunk_size;
    bitonic_sort(low, chunk_size, 1);
    pthread_exit(NULL);
}

int main() {
    num_items = 16;
    items = (int *) malloc(num_items * sizeof(int));

    for (int i = 0; i < num_items; i++) {
        items[i] = rand() % 100;
    }

    pthread_t threads[THREADS];
    for (int i = 0; i < THREADS; i++) {
        pthread_create(&threads[i], NULL, sort, &i);
    }
    for (int i = 0; i < THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    bitonic_merge(0, num_items, 1);

    printf("Sorted array: ");
    for (int i = 0; i < num_items; i++) {
        printf("%d ", items[i]);
    }
    printf("\n");

    free(items);

    return 0;
}

