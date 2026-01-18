#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

#define NUM_THREADS 4

void *radix_sort(void *arg);

int *arr, *sorted_arr;
int size, max_num_digits;
pthread_barrier_t barrier;

int main() {
    // Generate random array
    size = 20;
    arr = (int *) malloc(sizeof(int) * size);
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 1000;
    }

    // Find maximum number of digits in array
    int max_num = arr[0];
    for (int i = 1; i < size; i++) {
        if (arr[i] > max_num) {
            max_num = arr[i];
        }
    }
    max_num_digits = 0;
    while (max_num > 0) {
        max_num /= 10;
        max_num_digits++;
    }

    // Initialize sorted array
    sorted_arr = (int *) malloc(sizeof(int) * size);

    // Initialize barrier
    pthread_barrier_init(&barrier, NULL, NUM_THREADS);

    // Create threads and sort
    pthread_t threads[NUM_THREADS];
    for (int i = 0; i < NUM_THREADS; i++) {
        int *thread_num = (int *) malloc(sizeof(int));
        *thread_num = i;
        pthread_create(&threads[i], NULL, radix_sort, (void *) thread_num);
    }

    // Wait for threads to finish
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    // Print sorted array
    printf("Sorted array: ");
    for (int i = 0; i < size; i++) {
        printf("%d ", sorted_arr[i]);
    }
    printf("\n");

    // Clean up
    free(arr);
    free(sorted_arr);
    pthread_barrier_destroy(&barrier);

    return 0;
}

void *radix_sort(void *arg) {
    int thread_num = *((int *) arg);
    int start = thread_num * (size / NUM_THREADS);
    int end = (thread_num == NUM_THREADS - 1) ? size : (thread_num + 1) * (size / NUM_THREADS);

    int *count = (int *) calloc(10, sizeof(int));

    // Sort each digit
    for (int d = 0; d < max_num_digits; d++) {
        // Count occurrences of each digit
        for (int i = start; i < end; i++) {
            int digit = (arr[i] / (int) pow(10, d)) % 10;
            count[digit]++;
        }
        pthread_barrier_wait(&barrier);

        // Calculate prefix sum of counts
        int sum = 0;
        for (int i = 0; i < 10; i++) {
            int temp = count[i];
            count[i] = sum;
            sum += temp;
        }
        pthread_barrier_wait(&barrier);

        // Rearrange array based on prefix sums
        for (int i = start; i < end; i++) {
            int digit = (arr[i] / (int) pow(10, d)) % 10;
            sorted_arr[start + count[digit]] = arr[i];
            count[digit]++;
        }
        pthread_barrier_wait(&barrier);

        // Copy sorted array back to original array
        for (int i = start; i < end; i++) {
            arr[i];
                 }
    pthread_barrier_wait(&barrier);

    // Reset count array for next digit
    for (int i = 0; i < 10; i++) {
        count[i] = 0;
    }
    pthread_barrier_wait(&barrier);
}

free(count);
free(arg);
pthread_exit(NULL);
}

