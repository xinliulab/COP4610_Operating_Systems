/*
 * How to run this program:
 *
 * 1. Save this code to a file named 'ex_1_producer_consumer.c'.
 * 2. Open a terminal and navigate to the directory where the file is saved.
 * 3. Compile the program using the following command:
 *    gcc ex_4_semaphore.c
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
#include <semaphore.h>
#include <assert.h>

// Global variables
int n, count = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex to protect shared variable 'count'
sem_t empty_sem;  // Semaphore to count empty slots in the buffer
sem_t full_sem;   // Semaphore to count full slots in the buffer

// P operation: Wait (decrement the semaphore)
void P(sem_t *sem) {
    sem_wait(sem);  // P operation waits if the semaphore is zero, otherwise decrements the semaphore
}

// V operation: Signal (increment the semaphore)
void V(sem_t *sem) {
    sem_post(sem);  // V operation increments the semaphore, potentially waking up a waiting thread
}

// Producer thread function
void *Tproduce(void *arg) {
  while (1) {
    P(&empty_sem);          // P operation on empty_sem: Wait for an empty slot
    pthread_mutex_lock(&mutex);  // Lock the mutex to access shared variable 'count'
    count++;                // Produce an item
    printf("O");            // Print 'O' to indicate a produced item
    pthread_mutex_unlock(&mutex); // Unlock the mutex
    V(&full_sem);           // V operation on full_sem: Signal that a full slot is available
  }
  return NULL;
}

// Consumer thread function
void *Tconsume(void *arg) {
  while (1) {
    P(&full_sem);           // P operation on full_sem: Wait for a full slot
    pthread_mutex_lock(&mutex);  // Lock the mutex to access shared variable 'count'
    count--;                // Consume an item
    printf("X");            // Print 'X' to indicate a consumed item
    pthread_mutex_unlock(&mutex); // Unlock the mutex
    V(&empty_sem);          // V operation on empty_sem: Signal that an empty slot is available
  }
  return NULL;
}

int main(int argc, char *argv[]) {
  assert(argc == 2);           // Ensure the program is run with a buffer size argument
  n = atoi(argv[1]);           // Set the buffer size to the value passed as a command-line argument
  setbuf(stdout, NULL);        // Disable output buffering to see real-time output in the console

  // Initialize semaphores
  sem_init(&empty_sem, 0, n);  // Initially, the buffer has 'n' empty slots
  sem_init(&full_sem, 0, 0);   // Initially, the buffer has 0 full slots

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

  // Destroy semaphores (optional in this case since the program runs infinitely)
  sem_destroy(&empty_sem);
  sem_destroy(&full_sem);

  return 0;
}