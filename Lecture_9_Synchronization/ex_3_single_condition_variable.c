/*
 * How to run this program:
 *
 * 1. Save this code to a file named 'ex_1_producer_consumer_single_cv.c'.
 * 2. Open a terminal and navigate to the directory where the file is saved.
 * 3. Compile the program using the following command:
 *    gcc ex_3_single_condition_variable.c
 * 
 * 4. Run the program with an argument that represents the buffer size (n), for example:
 *    ./a.out 1
 *    This sets the buffer size to 1.
 *    Or you can run it with a larger buffer size, for example:
 *    ./a.out 2
 *    ./a.out 3
 *    ./a.out 4
 *
 * 5. Observing Deadlock Probability:
 *    - The program uses a single condition variable to coordinate producers and consumers. 
 *    - When the buffer size (n) is set to 1 (a very small buffer), the chance of deadlock increases significantly.
 *      Deadlock may happen because producers and consumers may end up waiting on each other without the correct 
 *      signal being sent, especially with small buffer sizes.
 *    - With larger buffer sizes (e.g., n = 4), the chance of deadlock is reduced because producers and consumers 
 *      have more flexibility to proceed without blocking each other immediately.
 *
 * 6. How to Test for Deadlock:
 *    - Run the program multiple times with different buffer sizes (e.g., n = 1, n = 2, n = 3, etc.)
 *    - Observe if the program stalls (deadlocks). Deadlock may not occur immediately every time you run the program 
 *      due to thread scheduling, so try running the program multiple times to observe deadlock behavior.
 *    - Example command:
 *      ./a.out 1    // Higher likelihood of deadlock with n = 1
 *      ./a.out 4    // Lower likelihood of deadlock with n = 4
 *
 * 7. Expected Behavior:
 *    - With a small buffer size (e.g., n = 1), deadlock is more likely because producers and consumers will frequently 
 *      hit full and empty conditions, causing them to wait on the same condition variable.
 *    - As the buffer size increases (e.g., n = 4 or greater), the deadlock probability decreases because there is 
 *      more buffer space for producers to produce and consumers to consume before they need to wait.
 *    
 * 8. Note:
 *    - This program uses a single condition variable (`pthread_cond_t`) to handle both "full" and "empty" conditions, 
 *      which is why deadlock can occur with smaller buffer sizes. Using separate condition variables for producers 
 *      and consumers would reduce the chance of
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

// Global variables
int n, count = 0;
pthread_mutex_t lk = PTHREAD_MUTEX_INITIALIZER;      // Mutex to protect shared variable 'count'
pthread_cond_t buffer_change = PTHREAD_COND_INITIALIZER;  // Single condition variable to manage both full and empty conditions

// Producer thread function
void *Tproduce(void *arg) {
  while (1) {
    pthread_mutex_lock(&lk);  // Lock the mutex to access shared variable 'count'
    while (count == n) {      // If buffer is full, wait for buffer_change condition
      pthread_cond_wait(&buffer_change, &lk);
    }
    count++;                  // Produce an item
    printf("O");              // Print 'O' to indicate a produced item
    pthread_cond_signal(&buffer_change);  // Signal that the buffer state has changed (could be not empty)
    pthread_mutex_unlock(&lk); // Unlock the mutex
  }
  return NULL;
}

// Consumer thread function
void *Tconsume(void *arg) {
  while (1) {
    pthread_mutex_lock(&lk);  // Lock the mutex to access shared variable 'count'
    while (count == 0) {      // If buffer is empty, wait for buffer_change condition
      pthread_cond_wait(&buffer_change, &lk);
    }
    count--;                  // Consume an item
    printf("X");              // Print 'X' to indicate a consumed item
    pthread_cond_signal(&buffer_change);  // Signal that the buffer state has changed (could be not full)
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
