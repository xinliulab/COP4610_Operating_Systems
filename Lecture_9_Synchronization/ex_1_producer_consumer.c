/*
 * How to run this program:
 *
 * 1. Save this code to a file named 'ex_1_producer_consumer.c'.
 * 2. Open a terminal and navigate to the directory where the file is saved.
 * 3. Compile the program using the following command:
 *    gcc ex_1_producer_consumer.c
 * 
 * 4. Run the program with an argument that represents the buffer size (n), for example:
 *    ./a.out 1
 *    This sets the buffer size to 1.
 *    Or you can run it with a larger buffer size, for example:
 *    ./a.out 32
 *
 * 5. To check the result using a pipe and an external Python script (checker.py):
 *    ./a.out 2 | python3 pc_checker.py 2
 *    Here, '2' is the buffer size passed to both the C program and the Python checker.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Global variables
// n: The buffer size (maximum number of items the producer can produce before the buffer is full)
// count: The current number of items in the buffer, shared between producers and consumers
int n, count = 0;
pthread_mutex_t lk = PTHREAD_MUTEX_INITIALIZER; // Mutex to protect access to the shared 'count' variable

// Producer thread function
void *Tproduce(void *arg) {
  while (1) {
retry:
    pthread_mutex_lock(&lk);  // Lock the mutex to access shared variable 'count'
    if (count == n) {         // If buffer is full, release the lock and retry
      pthread_mutex_unlock(&lk);
      goto retry;
    }
    count++;                  // Increase the buffer count (producing an item)
    printf("O");              // Print 'O' to indicate a produced item
    pthread_mutex_unlock(&lk); // Unlock the mutex after modifying the shared variable
  }
  return NULL;
}

// Consumer thread function
void *Tconsume(void *arg) {
  while (1) {
retry:
    pthread_mutex_lock(&lk);  // Lock the mutex to access shared variable 'count'
    if (count == 0) {         // If buffer is empty, release the lock and retry
      pthread_mutex_unlock(&lk);
      goto retry;
    }
    count--;                  // Decrease the buffer count (consuming an item)
    printf("X");              // Print 'X' to indicate a consumed item
    pthread_mutex_unlock(&lk); // Unlock the mutex after modifying the shared variable
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  assert(argc == 2);           // Ensure the program is run with a buffer size argument
  n = atoi(argv[1]);           // Set the buffer size to the value passed as a command-line argument
  setbuf(stdout, NULL);        // Disable output buffering to see real-time output in the console

  pthread_t producers[8], consumers[8]; // Create arrays for 8 producer and 8 consumer threads

  // Create 8 producer and 8 consumer threads
  for (int i = 0; i < 8; i++) {
    pthread_create(&producers[i], NULL, Tproduce, NULL); // Create a producer thread
    pthread_create(&consumers[i], NULL, Tconsume, NULL); // Create a consumer thread
  }

  // Wait for all producer and consumer threads to finish (they won't finish in this case due to infinite loop)
  for (int i = 0; i < 8; i++) {
    pthread_join(producers[i], NULL);
    pthread_join(consumers[i], NULL);
  }

  return 0;
}
