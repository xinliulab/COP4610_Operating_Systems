/*
Steps to compile and run this code:

1. Open the terminal and navigate to the folder containing this code:
   cd /path/to/your/code

2. Compile the code using gcc (ensure to link the pthread library):
   gcc ex_1_pthread_basics.c

3. Run the executable:
   ./a.out

4. The program will print characters from both threads. To capture and analyze all printed output, redirect it to a file:
   ./a.out > output.txt

5. To count the occurrences of each character in the output file using command-line tools, you can use:
   cat output.txt | fold -w1 | sort | uniq -c
   This command will display the count of each character printed.
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