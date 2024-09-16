#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NTHREAD 10 // 


pthread_t threads[NTHREAD]; 

void *Thello(void *arg) {
  int x = 0;
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
