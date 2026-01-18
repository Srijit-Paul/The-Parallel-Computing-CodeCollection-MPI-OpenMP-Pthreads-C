#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 4

int n;
int *arr;

pthread_mutex_t lock;

void* odd_even_sort(void* thread_id) {
    int tid = *(int*)thread_id;
    int start = tid * (n / NUM_THREADS);
    int end = (tid + 1) * (n / NUM_THREADS) - 1;
    int i, temp;
    for (i = start; i <= end; i += 2) {
        if (i < n - 1 && arr[i] > arr[i + 1]) {
            temp = arr[i];
            arr[i] = arr[i + 1];
            arr[i + 1] = temp;
        }
    }
    for (i = start + 1; i <= end; i += 2) {
        if (i < n - 1 && arr[i] > arr[i + 1]) {
            pthread_mutex_lock(&lock);
            temp = arr[i];
            arr[i] = arr[i + 1];
            arr[i + 1] = temp;
            pthread_mutex_unlock(&lock);
        }
    }
    pthread_exit(NULL);
}

void odd_even_transposition_sort() {
    int i, j, temp;
    for (i = 0; i < n; i++) {
        for (j = 0; j < NUM_THREADS; j++) {
            pthread_t tid;
            pthread_create(&tid, NULL, odd_even_sort, &j);
            pthread_join(tid, NULL);
        }
    }
}

int main() {
    n = 10;
    arr = (int*) malloc(n * sizeof(int));

    printf("Before sorting:\n");
    for (int i = 0; i < n; i++) {
        arr[i] = rand() % 100;
        printf("%d ", arr[i]);
    }

    odd_even_transposition_sort();

    printf("\nAfter sorting:\n");
    for (int i = 0; i < n; i++) {
        printf("%d ", arr[i]);
    }

    printf("\n");

    free(arr);

    return 0;
}

