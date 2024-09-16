/*
Steps to compile and run this code:

1.  Open the terminal and navigate to the folder containing this code:
    cd /path/to/your/code

2.  Compile the code using gcc:
    gcc ex_2_x++_faulty_lock.c

3.  Run the executable:
    ./a.out

4.  Observe the output of `x`. You may notice incorrect results due to the lock not functioning properly. 
    The custom software lock (`lock_status`) may fail to prevent race conditions when both threads attempt to acquire the lock simultaneously, leading to errors in the final value of `x`.
*/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100000000
#define LOCK 1
#define UNLOCK 0

long x = 0;
int lock_status = UNLOCK;

void *Tsum(void *arg) {
    for (int i = 0; i < N; i++) {
        retry:
            if (lock_status != UNLOCK) {
                goto retry;
            }
            lock_status = LOCK;  // Acquire the lock

            x++;  // Critical section

            lock_status = UNLOCK;  // Release the lock
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

    // Print the final value of x
    printf("x = %ld\n", x);

    return 0;
}
