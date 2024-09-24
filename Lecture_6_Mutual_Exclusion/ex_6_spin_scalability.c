/*
Steps to compile and run this code:

1. Open the terminal and navigate to the folder containing this code:
   cd /path/to/your/code

2. Compile the code using gcc:
   gcc ex_6_spin_scalability.c 

3. Run the executable, specifying the number of threads as a command-line argument:
   ./a.out <number_of_threads>
   For example:
   ./a.out 2    (to run with 2 threads)
   ./a.out 32   (to run with 32 threads)

4. You can also switch to the MUTEX mode:
   - Comment out the line `#define USE_SPINLOCK`.
   - Uncomment the line `#define USE_MUTEX`.
   - Recompile the code and run the program with different numbers of threads.
     For example:
     ./a.out 2
     ./a.out 32

5. Measure and compare the performance for different thread counts using the `time` command:
   
   time ./a.out 2
   time ./a.out 4
   time ./a.out 8
   time ./a.out 16
   time ./a.out 32
   time ./a.out 64

6. Use `strace -fc` to observe the number of system calls made by each locking mechanism (spinlock vs mutex). For example:

   strace -fc ./a.out 64

   This will give you a detailed breakdown of system calls and their usage for both types of locks.

7. You can also compare the performance of locking with atomic instructions:
   - Comment out the `Txpp` function.
   - Uncomment the atomic instruction-based `Txpp` function (if available).
   - Recompile and rerun the code to see which version performs faster with different numbers of threads.
   
   Compare how the runtime and system call usage changes between spinlock, mutex, and atomic instruction implementations.

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>
#include <stdatomic.h>  // Add this for atomic operations

// Choose between spinlock and mutex
// Uncomment one of the following lines to select the locking mechanism
#define USE_SPINLOCK
// #define USE_MUTEX

// Shared variables
#define NUM_INCREMENTS 1000000
long x = 0;

// Lock Implementation
#ifdef USE_SPINLOCK
int lock = 0;  // Spinlock (0 means unlocked, 1 means locked)
#else
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
#endif


// Atomic xchg function (swap values atomically)
int xchg(int *addr, int newval) {
    int result;
    asm volatile (
        "lock xchg %0, %1"
        : "+m" (*addr), "=a" (result)
        : "1" (newval)
        : "cc"
    );
    return result;
}

// Acquire the lock using xchg (spinlock)
void acquire_lock(int *lock) {
    while (xchg(lock, 1)) {  // Keep spinning until the lock is available (lock == 0)
        // Busy-wait (spin) until the lock becomes available
    }
}

// Release the lock
void release_lock(int *lock) {
    xchg(lock, 0);  // Set the lock back to 0 (unlocked)
}



void *Txpp(void *arg) {
    for (int i = 0; i < NUM_INCREMENTS; i++) {
    #ifdef USE_SPINLOCK
        acquire_lock(&lock);  // Using the new xchg-based spinlock implementation
    #else
        pthread_mutex_lock(&lock);
    #endif

        x++;

    #ifdef USE_SPINLOCK
        release_lock(&lock);  // Release the xchg-based spinlock
    #else
        pthread_mutex_unlock(&lock);
    #endif
    }
    return NULL;
}

// void *Txpp(void *arg) {
//   for (int i = 0; i < NUM_INCREMENTS; i++) {
//     atomic_fetch_add(&x, 1);  // Use atomic increment instead of locking
//   }
//   return NULL;
// }

int main(int argc, char *argv[]) {
    int num_threads = 4; // Default number of threads
    if (argc > 1) {
        num_threads = atoi(argv[1]);
    }

    pthread_t threads[num_threads];

    // Create threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, Txpp, NULL);
    }

    // Wait for threads to finish
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Final counter value: %ld\n", x);

    return 0;
}
