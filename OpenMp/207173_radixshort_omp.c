#include <stdlib.h>
#include <string.h>
#include <omp.h>
#define INITIAL_SHIFT (((sizeof (long long)) - 1) * 010)
#define NTHREADS 64
#define SWITCH_TO_INSERTION 16
#define SWITCH_TO_QSORT 8192
#define DO_NOT_RADIX 2626144


static inline unsigned long long max(unsigned long long, unsigned long long);
static void q_sort(unsigned long long [], size_t);
static void _radix_sort(unsigned long long [], unsigned long long [],
        register size_t, unsigned long long);
static inline void radix_sort(long long [], size_t, unsigned long long);
void psort(long long [], int);

static inline unsigned long long max(unsigned long long a, unsigned long long b) {
    /* return the max a and b */
    return a > b ? a : b;
}

static void q_sort(unsigned long long a[], size_t size) {
    /*
     * sort in place the array a of size size using quicksort while
     * the array size is > SWITCH_TO_INSERTION, otherwise switch
     * to an insertion sort (faster on small arrays due to better
     * caching)
     */

    if(size < SWITCH_TO_INSERTION) {
        /*
         * embedded insertion sort to avoid function call
         * (we may also have used an inline function)
         */

        // use gcc __builtin_expect to optimize pipelining
        if(__builtin_expect(size < 2, 0)) {
            return;
        }

        register unsigned long long v;
        register size_t i, j;

        // ^= faster than = 0
        for(i ^= i; i < size; ++i) {
            v = a[i];

            for(j = i; j && v < a[j - 1]; --j) {
                a[j] = a[j - 1];
            }

            a[j] = v;
        }

        return;
    }

    register unsigned long long
        mid = a[size >> 1],
            *l = a,
            *r = a + size - 1,
            t,
            /*
             * take the pivot as the middle value of 3 items
             * avoid to have a degenerated qsort (why not
             * take the average?)
             */
            p = *l < mid ?
                (mid < *r ? mid : max(*r, *l))
                : (*l < *r ? *l : max(mid, *r));

    while(l <= r) {
        // while well "ordered"
        while(*l < p) {
            l++;
        }

        // while well "ordered"
        while(*r > p) {
            r--;
        }

        // if we haven't finish the partition, swap elements
        if(l <= r) {
            t = *l;
            *l++ = *r;
            *r-- = t;
        }
    }

    // sort sub-arrays
    q_sort(a, (r - a) + 1);
    q_sort(l, (a - l) + size);
}

static void _radix_sort(unsigned long long a[], unsigned long long *buffer,
        register size_t size, unsigned long long shift) {

    // stop condition if we only use radix
    // if(size < 2) {
    //     return;
    // }

    // switch to a quick sort if array is small or shift == 0
    if(size < SWITCH_TO_QSORT || !shift) {
        q_sort(a, size);
        return;
    }

    register size_t i, radix;

    size_t buckets_size[0x100] = {0}, // size needed in each bucket
           buckets_index[0x100] = {0}, // first address of each bucket
           buckets_cindex[0x101] = {0}; // 0x101 because of next loop
                                        // add all indexes

    if(__builtin_expect(shift == INITIAL_SHIFT, 0)) {
        size_t local_buckets_size[0x100] = {0}; // size needed in each bucket/thread

#pragma omp parallel num_threads(NTHREADS) firstprivate(local_buckets_size) private(radix)
        {
#pragma omp for nowait
            for(i = 0; i < size; ++i) {
                // increment the number of number in the bucket
                radix = (a[i] >> shift) & 0xff;
                ++local_buckets_size[radix];
            }

#pragma omp critical
            // decreasing loop *may* be faster on some architectures
            for(i = 0x100; i--;) {
                buckets_size[i] += local_buckets_size[i];
            }
        }
    }

    else {
        for(i = size; i;) {
            radix = (a[--i] >> shift) & 0xff;
            ++buckets_size[radix];
        }
    }

    unsigned long long *buckets[0x100];
    register unsigned long long *ptr = buffer;

    for(i ^= i; i < 0x100; ++i) {
        buckets[i] = ptr;
        ptr += (unsigned long long)buckets_size[i];
        buckets_cindex[i + 1] = buckets_cindex[i] + buckets_size[i];
    }

    // add the numbers in the correct buckets
    for(i = size; i--;) {
        radix = (a[i] >> shift) & 0xff;
        buckets[radix][buckets_index[radix]++] = a[i];
    }

    // in the next level of recursion we shift the shift to the right
    const register unsigned long long new_shift = shift - 010;

   
    if(__builtin_expect(shift == INITIAL_SHIFT, 0)) {
#pragma omp parallel for num_threads(NTHREADS)
        for(i = 0; i < 0x100; ++i) {
            _radix_sort(buckets[i], a + buckets_cindex[i], buckets_size[i], new_shift);
            memcpy(a + buckets_cindex[i], buckets[i], buckets_size[i] * sizeof (unsigned long long));
        }
    }

    else {
        // already sorted if shift == 0
        // if(shift) {
        for(i = 0; i < 0x100; ++i) {
            _radix_sort(buckets[i], a + buckets_cindex[i], buckets_size[i], new_shift);
        }
        // }
        memcpy(a, buffer, size * sizeof (unsigned long long));
    }
}

static inline void radix_sort(long long a[], size_t size,
        unsigned long long shift) {
    const register unsigned long long
        xor = ((unsigned long long)0x80) << shift;
    register size_t i;

    long long *copy = a;
#pragma omp parallel for num_threads(NTHREADS)
    for(i = 0; i < size; ++i) {
       
        copy[i] ^= xor;
    }

    unsigned long long *buffer = malloc(sizeof (unsigned long long) * size);
    // the magic happens here
    _radix_sort((unsigned long long *)a, buffer, size, shift);
    free(buffer);

#pragma omp parallel for num_threads(NTHREADS)
    for(i = 0; i < size; ++i) {
        // restore the true value of numbers
        a[i] ^= xor;
    }
}

void psort(long long a[], int size) {
    /* sort a in place */

    // switch to q_sort if size < DO_NOT_RADIX
    if(__builtin_expect(size < DO_NOT_RADIX, 0)) {
        register size_t i;
        const register unsigned long long
            xor = ((unsigned long long)0x80) << INITIAL_SHIFT;

        for(i = 0; i < size; ++i) {
            a[i] ^= xor;
        }

        q_sort((unsigned long long *)a, size);

        for(i = 0; i < size; ++i) {
            a[i] ^= xor;
        }
    }

    else {
        
        radix_sort(a, size, INITIAL_SHIFT);
    }
}

