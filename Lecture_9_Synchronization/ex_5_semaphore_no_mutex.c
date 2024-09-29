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
int n;
sem_t empty_sem;  // Semaphore to track empty slots in the buffer
sem_t full_sem;   // Semaphore to track full slots in the buffer

// Define P and V operations
#define P(sem) sem_wait(sem)
#define V(sem) sem_post(sem)

// Producer thread function
void *Tproduce(void *arg) {
  while (1) {
    P(&empty_sem);            // Wait for an empty slot (P operation)
    printf("O");              // Produce an item (just print 'O')
    V(&full_sem);             // Signal that there is one more full slot (V operation)
  }
  return NULL;
}

// Consumer thread function
void *Tconsume(void *arg) {
  while (1) {
    P(&full_sem);             // Wait for a full slot (P operation)
    printf("X");              // Consume an item (just print 'X')
    V(&empty_sem);            // Signal that there is one more empty slot (V operation)
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