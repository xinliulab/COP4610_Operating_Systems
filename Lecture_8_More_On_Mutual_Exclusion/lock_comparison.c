// File: lock_comparison.c

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>

// Spinlock implementation
typedef volatile int spinlock_t;

void spin_lock(spinlock_t *lock) {
    while (__sync_lock_test_and_set(lock, 1)) {
        // Busy-wait (spin) until the lock becomes available
    }
}

void spin_unlock(spinlock_t *lock) {
    __sync_lock_release(lock);
}

// Shared variables
#define NUM_INCREMENTS 1000000
long counter = 0;

// Choose between spinlock and mutex
// Uncomment one of the following lines to select the locking mechanism
#define USE_SPINLOCK
// #define USE_MUTEX

#ifdef USE_SPINLOCK
spinlock_t lock = 0;
#else
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
#endif

void *increment(void *arg) {
    for (int i = 0; i < NUM_INCREMENTS; i++) {
    #ifdef USE_SPINLOCK
        spin_lock(&lock);
    #else
        pthread_mutex_lock(&lock);
    #endif

        counter++;

    #ifdef USE_SPINLOCK
        spin_unlock(&lock);
    #else
        pthread_mutex_unlock(&lock);
    #endif
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    int num_threads = 4; // Default number of threads
    if (argc > 1) {
        num_threads = atoi(argv[1]);
    }

    pthread_t threads[num_threads];

    struct timeval start_time, end_time;
    gettimeofday(&start_time, NULL);

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, increment, NULL);
    }

    // Wait for threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    gettimeofday(&end_time, NULL);

    // Calculate elapsed time
    double elapsed_time = (end_time.tv_sec - start_time.tv_sec) +
                          (end_time.tv_usec - start_time.tv_usec) / 1e6;

    printf("Final counter value: %ld\n", counter);
    printf("Elapsed time: %.6f seconds\n", elapsed_time);

    return 0;
}
