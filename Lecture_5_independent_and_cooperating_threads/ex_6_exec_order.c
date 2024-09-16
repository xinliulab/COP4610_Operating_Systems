/*
Steps to compile and run this code:

1.  Open the terminal and navigate to the folder containing this code:
    cd /path/to/your/code

2.  Compile the code using gcc:
    gcc ex_6_exec_order.c

3.  Run the executable and redirect the output to a file:
    ./a.out > output.txt
    (This saves the program output to `output.txt`.)

4.  After running the program multiple times, use the following command to count the occurrences of each unique result:
    sort output.txt | uniq -c
    (This will sort the output, then count and display how many times each unique result appears.)

5.  Note that some outcomes may have very low probabilities of occurring. 
    Therefore, you might need to increase the number of iterations (the loop in `main`) significantly to observe more diverse results. 
    Even so, some outcomes may never appear due to their extremely low likelihood. 
    To exhaustively explore all possible outcomes, more sophisticated software tools may be needed to systematically analyze and verify all possibilities.

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int x = 0; 
int y = 0; 

void *thread1_func(void *arg) {
    x = 1;  
    x = y + 1;
    printf(" Thread 1: x = %d ", x);
    return NULL;
}

void *thread2_func(void *arg) {
    y = 2;  
    y = y * 2;
    printf(" Thread 2: y = %d ", y);
    return NULL;
}

int main() {
    for (int i = 0; i < 1000000; i++) {
        pthread_t thread1, thread2;

        pthread_create(&thread1, NULL, thread1_func, NULL);
        pthread_create(&thread2, NULL, thread2_func, NULL);


        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);

        printf(" Final values: x = %d, y = %d\n", x, y);
    }

    return 0;
}
