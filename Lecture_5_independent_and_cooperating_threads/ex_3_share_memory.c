/*
Steps to compile and run this code:

1.  Open the terminal and navigate to the folder containing this code:
    cd /path/to/your/code

2.  Compile the code using gcc:
    gcc ex_3_share_memory.c

3.  Run the executable:
    ./a.out

4.  Observe the output of `x` when it is declared as a global variable (Line 30). Since `x` is global, all threads share and modify the same `x`, affecting the output order.

5.  Now, comment out the global `int x = 0;` on Line 12, and uncomment the `int x = 0;` inside the `Thello` function (Line 33) to make `x` a local variable. Each thread will then have its own local copy of `x`, and observe how the output differs compared to the global version.

6.  Compare the behavior and output when `x` is global versus when it is local.

*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NTHREAD 10 // 

pthread_t threads[NTHREAD]; 

int x = 0; //Global Variable

void *Thello(void *arg) {
  // int x = 0;//Local Variable
  int id = *(int *)arg; 
  usleep(id * 100000);
  printf("Hello from thread #%c\n", "123456789ABCDEF"[x++]);
  return NULL;
}

int main() {
  int ids[NTHREAD];


  for (int i = 0; i < NTHREAD; i++) {
    ids[i] = i + 1; 
    if (pthread_create(&threads[i], NULL, Thello, &ids[i]) != 0) {
      fprintf(stderr, "Error creating thread %d\n", i);
      return 1;
    }
  }

  for (int i = 0; i < NTHREAD; i++) {
    pthread_join(threads[i], NULL);
  }

  return 0;
}
