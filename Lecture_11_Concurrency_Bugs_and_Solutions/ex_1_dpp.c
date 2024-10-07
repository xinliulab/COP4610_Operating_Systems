#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>

#define N 3
sem_t locks[N];

void *Tphilosopher(void *arg) {
  int id = *((int *)arg);
  int lhs = (N + id - 1) % N;
  int rhs = id % N;

  if (id == N) {
    while (1) {
      sem_wait(&locks[rhs]);
      printf("T%d Got %d\n", id, rhs + 1);
      sem_wait(&locks[lhs]);
      printf("T%d Got %d\n", id, lhs + 1);
      sem_post(&locks[rhs]);
      sem_post(&locks[lhs]);
    }
  } else {
    while (1) {
      sem_wait(&locks[lhs]);
      printf("T%d Got %d\n", id, lhs + 1);
      sem_wait(&locks[rhs]);
      printf("T%d Got %d\n", id, rhs + 1);
      sem_post(&locks[lhs]);
      sem_post(&locks[rhs]);
    }
  }

  return NULL;
}

int main() {
  pthread_t threads[N];
  int ids[N];

  // Initialize the semaphores
  for (int i = 0; i < N; i++) {
    sem_init(&locks[i], 0, 1);
  }

  // Create threads
  for (int i = 0; i < N; i++) {
    ids[i] = i + 1;
    pthread_create(&threads[i], NULL, Tphilosopher, &ids[i]);
  }

  // Join threads (not strictly necessary as the philosophers run indefinitely)
  for (int i = 0; i < N; i++) {
    pthread_join(threads[i], NULL);
  }

  // Destroy semaphores (optional, not reachable here due to infinite loop)
  for (int i = 0; i < N; i++) {
    sem_destroy(&locks[i]);
  }

  return 0;
}
