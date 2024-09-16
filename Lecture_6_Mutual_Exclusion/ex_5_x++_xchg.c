/*
Steps to compile and run this code:

1.  Open the terminal and navigate to the folder containing this code:
    cd /path/to/your/code

2.  Compile the code using gcc:
    gcc ex_5_x++_xchg.c 
    
3.  Run the executable:
    ./a.out

4.  Now, increase the value of `N` to a large number, for example, 1000000 or higher.
    This will demonstrate the **inefficiency** of spinlocks using atomic instructions, especially with frequent context switching between threads.

6.  Compile and run the code again with the higher `N` value.
    You will likely observe slower execution as the threads compete for the lock, showing the **performance overhead** of atomic operations.

7.  To further explore, use `objdump` to inspect the generated assembly instructions:
    objdump -d a.out
    (This command disassembles the executable, allowing you to see the actual `lock` and `xchg` instructions used by the CPU to perform atomic operations.)
*/



#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000000
long x = 0;
int lock = 0;  // Lock variable for synchronization (0 means unlocked, 1 means locked)

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

void *Tsum(void *arg) {
    for (int i = 0; i < N; i++) {
        acquire_lock(&lock);  // Acquire the lock before modifying x
        x++;  // Critical section: safely increment x
        release_lock(&lock);  // Release the lock after modifying x
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    // Create two threads
    pthread_create(&thread1, NULL, Tsum, NULL);
    pthread_create(&thread2, NULL, Tsum, NULL);

    // Wait for both threads to finish
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("x = %ld\n", x);  // Print the final value of x

    return 0;
}
