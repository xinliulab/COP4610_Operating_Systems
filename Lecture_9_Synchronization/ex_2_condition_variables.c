/*
 * How to run this program:
 *
 * 1. Save this code to a file named 'ex_1_producer_consumer.c'.
 * 2. Open a terminal and navigate to the directory where the file is saved.
 * 3. Compile the program using the following command:
 *    gcc ex_2_condition_variables.c
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
int n, count = 0;
pthread_mutex_t lk = PTHREAD_MUTEX_INITIALIZER;      // Mutex to protect shared variable 'count'
pthread_cond_t not_full = PTHREAD_COND_INITIALIZER;  // Condition variable for when the buffer is not full
pthread_cond_t not_empty = PTHREAD_COND_INITIALIZER; // Condition variable for when the buffer is not empty

// Producer thread function
void *Tproduce(void *arg) {
  while (1) {
    pthread_mutex_lock(&lk);  // Lock the mutex to access shared variable 'count'
    while (count == n) {      // If buffer is full, wait for not_full condition
      pthread_cond_wait(&not_full, &lk);
    }
    count++;                  // Produce an item
    printf("O");              // Print 'O' to indicate a produced item
    pthread_cond_signal(&not_empty);  // Signal that the buffer is no longer empty
    pthread_mutex_unlock(&lk); // Unlock the mutex
  }
  return NULL;
}

// Consumer thread function
void *Tconsume(void *arg) {
  while (1) {
    pthread_mutex_lock(&lk);  // Lock the mutex to access shared variable 'count'
    while (count == 0) {      // If buffer is empty, wait for not_empty condition
      pthread_cond_wait(&not_empty, &lk);
    }
    count--;                  // Consume an item
    printf("X");              // Print 'X' to indicate a consumed item
    pthread_cond_signal(&not_full);  // Signal that the buffer is no longer full
    pthread_mutex_unlock(&lk); // Unlock the mutex
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
