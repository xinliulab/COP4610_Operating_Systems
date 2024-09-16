/*
Steps to compile and run this code:

1.  Open the terminal and navigate to the folder containing this code:
    cd /path/to/your/code

2.  Compile the code using gcc:
    gcc ex_4_x++.c

3.  Run the executable:
    ./a.out

4.  Observe the output of `x` when `N` is set to 100. The result will be correct since there are fewer increments.

5.  Now, change the value of `N` to 1000000 (Line 6).

6.  Compile and run the code again, and observe the output of `x`. 
    You will likely notice incorrect results due to a race condition, where both threads are incrementing `x` simultaneously without proper synchronization, leading to errors in the final value of `x`.
*/


#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 100
long x = 0;


void *Tsum(void *arg) {
    for (int i = 0; i < N; i++) {
        x++;  
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;


    pthread_create(&thread1, NULL, Tsum, NULL);
    pthread_create(&thread2, NULL, Tsum, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);


    printf("x = %ld\n", x);

    return 0;
}
