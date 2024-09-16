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

#define N 10000000
#define SPIN

#ifdef SPIN
spinlock_t lock = SPIN_INIT();
#else
mutex_t lock = MUTEX_INIT();
#endif

long n, sum = 0;

void Tsum() {
  for (int i = 0; i < n; i++) {
#ifdef SPIN
    spin_lock(&lock);
#else
    mutex_lock(&lock);
#endif
    sum++;
#ifdef SPIN
    spin_unlock(&lock);
#else
    mutex_unlock(&lock);
#endif
  }
}

int main(int argc, char *argv[]) {
  assert(argc == 2);
  int nthread = atoi(argv[1]);
  n = N / nthread;
  for (int i = 0; i < nthread; i++) {
    create(Tsum);
  }
  join();
  assert(sum == n * nthread);
}
