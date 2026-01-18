#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_BUCKETS 10
#define MAX_THREADS 4

int size;
int *arr;
pthread_mutex_t mutex;
pthread_barrier_t barrier;

typedef struct bucket_node {
    int val;
    struct bucket_node *next;
} bucket_node_t;

bucket_node_t *buckets[NUM_BUCKETS];
int counts[NUM_BUCKETS];

void insert(bucket_node_t **head, int val) {
    bucket_node_t *new_node = (bucket_node_t *) malloc(sizeof(bucket_node_t));
    new_node->val = val;
    new_node->next = *head;
    *head = new_node;
}

void *bucket_sort(void *arg) {
    int thread_num = *((int *) arg);
    int start = thread_num * (size / MAX_THREADS);
    int end = (thread_num == MAX_THREADS - 1) ? size : (thread_num + 1) * (size / MAX_THREADS);

    // Insert elements into buckets
    for (int i = start; i < end; i++) {
        int index = arr[i] / (100 / NUM_BUCKETS);
        pthread_mutex_lock(&mutex);
        insert(&buckets[index], arr[i]);
        counts[index]++;
        pthread_mutex_unlock(&mutex);
    }

    pthread_barrier_wait(&barrier);

    // Sort each bucket
    for (int i = thread_num; i < NUM_BUCKETS; i += MAX_THREADS) {
        bucket_node_t *cur = buckets[i];
        for (int j = 0; j < counts[i]; j++) {
            arr[start + j] = cur->val;
            cur = cur->next;
        }
        start += counts[i];
    }

    pthread_exit(NULL);
}

void parallel_bucket_sort() {
    pthread_t threads[MAX_THREADS];
    int thread_nums[MAX_THREADS];
    for (int i = 0; i < MAX_THREADS; i++) {
        thread_nums[i] = i;
        pthread_create(&threads[i], NULL, bucket_sort, &thread_nums[i]);
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

    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barrier, NULL, MAX_THREADS);

    parallel_bucket_sort();

    printf("Sorted array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");

    pthread_mutex_destroy(&mutex);
    pthread_barrier_destroy(&barrier);

    return 0;
}

