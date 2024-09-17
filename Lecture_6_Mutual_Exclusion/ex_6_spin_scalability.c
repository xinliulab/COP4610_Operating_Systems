/*
Steps to compile and run this code:

1.  Open the terminal and navigate to the folder containing this code:
    cd /path/to/your/code

2.  Compile the code using gcc:
    gcc ex_6_spin_scalability.c -Iinclude -pthread
    (We are using the -pthread option to enable multi-threading.)

3.  Run the executable, specifying the number of threads as a command-line argument:
    ./a.out <number_of_threads>
    For example:
    ./a.out 2    (to run with 2 threads)
    ./a.out 32   (to run with 32 threads)

4.  Observe the runtime of the program with different numbers of threads (e.g., 2, 4, 8, 16, 32, etc.).
    You can use the `time` command to measure execution time:
    
    time ./a.out 2
    time ./a.out 32

    Compare how the runtime changes as you increase the number of threads.

*/


#include "thread.h"
#include "thread-sync.h"
#include <stdatomic.h>

#define N 10000000
#define SPIN //Choose Spin lock or mutex lock

#ifdef SPIN
spinlock_t lock = SPIN_INIT();
#else
mutex_t lock = MUTEX_INIT();
#endif

long n, x = 0;

void Tsum() {
  for (int i = 0; i < n; i++) {
#ifdef SPIN
    spin_lock(&lock);
#else
    mutex_lock(&lock);
#endif
    x++;
#ifdef SPIN
    spin_unlock(&lock);
#else
    mutex_unlock(&lock);
#endif
  }
}

// void Tsum() {
//   for (int i = 0; i < n; i++) {
//     atomic_fetch_add(&x, 1);  // Use atomic increment instead of locking
//   }
// }

int main(int argc, char *argv[]) {
  assert(argc == 2);
  int nthread = atoi(argv[1]);
  n = N / nthread;
  for (int i = 0; i < nthread; i++) {
    create(Tsum);
  }
  join();
  assert(x == n * nthread);
  
  printf("x = %ld\n", x);  // Print the final value of x

  return 0;
}
