#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>

#define N 1000000
long x = 0;

atomic_int flag[2];
atomic_int turn;

void *Tsum(void *arg) {
    int id = (int)(intptr_t)arg; // Thread ID: 0 or 1
    int other = 1 - id;          // The other thread's ID

    for (int i = 0; i < N; i++) {
        // Entry Section: Peterson's Algorithm
        atomic_store_explicit(&flag[id], 1, memory_order_release);
        atomic_store_explicit(&turn, other, memory_order_release);
        while (atomic_load_explicit(&flag[other], memory_order_acquire) &&
               atomic_load_explicit(&turn, memory_order_acquire) == other) {
            // Busy wait
        }

        // Critical Section
        x++;

        // Exit Section
        atomic_store_explicit(&flag[id], 0, memory_order_release);
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // Initialize flags and turn
    atomic_init(&flag[0], 0);
    atomic_init(&flag[1], 0);
    atomic_init(&turn, 0);

    // Create threads, passing their IDs (0 or 1)
    pthread_create(&thread1, NULL, Tsum, (void *)(intptr_t)0);
    pthread_create(&thread2, NULL, Tsum, (void *)(intptr_t)1);

    // Wait for threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    // Print the final value of x
    printf("x = %ld\n", x);

    return 0;
}
