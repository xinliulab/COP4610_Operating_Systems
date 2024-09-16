#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000000
long sum = 0;


void *Tsum(void *arg) {
    for (int i = 0; i < N; i++) {
        // sum++;  
        asm volatile("lock addq $1, %0": "+m"(sum));
        // objdump -d a.out | less 
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;


    pthread_create(&thread1, NULL, Tsum, NULL);
    pthread_create(&thread2, NULL, Tsum, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);


    printf("sum = %ld\n", sum);

    return 0;
}
