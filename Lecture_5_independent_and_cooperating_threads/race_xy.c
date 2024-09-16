#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int x = 0; 
int y = 0; 

void *thread1_func(void *arg) {
    x = 1;
    usleep(100);  
    x = y + 1;
    printf(" Thread 1: x = %d ", x);
    return NULL;
}

void *thread2_func(void *arg) {
    y = 2;
    usleep(100);  
    y = y * 2;
    printf(" Thread 2: y = %d ", y);
    return NULL;
}

int main() {
    for (int i = 0; i < 10000; i++) {
        pthread_t thread1, thread2;

        pthread_create(&thread1, NULL, thread1_func, NULL);
        pthread_create(&thread2, NULL, thread2_func, NULL);


        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        printf(" Final values: x = %d, y = %d\n", x, y);
    }

    return 0;
}
