#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define MAX_THREADS 4

int *arr;
int size;
pthread_barrier_t barrier;

void *shellsort(void *arg) {
    int thread_num = *((int *) arg);
    int start = thread_num * (size / MAX_THREADS);
    int end = (thread_num == MAX_THREADS - 1) ? size : (thread_num + 1) * (size / MAX_THREADS);

    // Shell sort
    for (int gap = (end - start) / 2; gap > 0; gap /= 2) {
        for (int i = start + gap; i < end; i++) {
            int temp = arr[i];
            int j;
            for (j = i; j >= start + gap && arr[j - gap] > temp; j -= gap) {
                arr[j] = arr[j - gap];
            }
            arr[j] = temp;
        }
        pthread_barrier_wait(&barrier);
    }
    pthread_exit(NULL);
}

void parallel_shellsort() {
    pthread_t threads[MAX_THREADS];
    int thread_nums[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++) {
        thread_nums[i] = i;
        pthread_create(&threads[i], NULL, shellsort, &thread_nums[i]);
    }
    for (int i = 0; i < MAX_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    size = 20;
    arr = (int *) malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100;
    }

    pthread_barrier_init(&barrier, NULL, MAX_THREADS);
    parallel_shellsort();

    printf("Sorted array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    pthread_barrier_destroy(&barrier);
    free(arr);

    return 0;
}

