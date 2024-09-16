/*
Steps to compile and run this code:

1. Open the terminal and navigate to the folder containing this code:
   cd /path/to/your/code

2. Compile the code using gcc:
   gcc ex_1_pthread_basics.c

3. Run the executable:
   ./a.out

4. Open a new terminal and type the following to observe CPU usage:
   top
   (Observe the high CPU usage)
*/



#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *Ta(void *arg) {
    while (1) {
      printf("a") ;
    }
    return NULL;
}

void *Tb(void *arg) {
    while (1) {
        printf("b");
    }
    return NULL;
}

int main() {
    pthread_t thread1, thread2;

    if (pthread_create(&thread1, NULL, Ta, NULL) != 0) {
        fprintf(stderr, "Failed to create thread1\n");
        exit(1);
    }

    if (pthread_create(&thread2, NULL, Tb, NULL) != 0) {
        fprintf(stderr, "Failed to create thread2\n");
        exit(1);
    }

 
    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    return 0;
}
