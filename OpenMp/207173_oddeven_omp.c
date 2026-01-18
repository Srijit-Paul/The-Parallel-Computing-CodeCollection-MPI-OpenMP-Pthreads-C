#include <stdio.h>
#include <omp.h>

#define N 10

void print_array(int a[N]) {
    for (int i = 0; i < N; i++) {
        printf("%d ", a[i]);
    }
    printf("\n");
}

void odd_even_sort(int a[N]) {
    int phase, i, temp;
    #pragma omp parallel private(phase,i,temp)
    for (phase = 0; phase < N; phase++) {
        if (phase % 2 == 0) {
            #pragma omp for schedule(static)
            for (i = 1; i < N; i += 2) {
                if (a[i-1] > a[i]) {
                    temp = a[i-1];
                    a[i-1] = a[i];
                    a[i] = temp;
                }
            }
        } else {
            #pragma omp for schedule(static)
            for (i = 1; i < N-1; i += 2) {
                if (a[i] > a[i+1]) {
                    temp = a[i+1];
                    a[i+1] = a[i];
                    a[i] = temp;
                }
            }
        }
    }
}

int main() {
    int a[N] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3};
    printf("Original array:\n");
    print_array(a);

    odd_even_sort(a);

    printf("Sorted array:\n");
    print_array(a);

    return 0;
}
