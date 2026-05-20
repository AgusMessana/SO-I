#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

sem_t listo_a1, listo_b1;

void *Thread_A(void *arg) {
  int a1;
  printf("Se realizó a1.\n");
  sem_post(&listo_a1);
  sem_wait(&listo_b1);
  int a2;
  printf("Se realizó a2.\n");

  return NULL;
}

void *Thread_B(void *arg) {
  int b1;
  printf("Se realizó b1.\n");
  sem_post(&listo_b1);
  sem_wait(&listo_a1);
  int b2;
  printf("Se realizó b2.\n");

  return NULL;
}

int main(void) {
  pthread_t thA, thB;
  sem_init(&listo_a1, 0, 0);
  sem_init(&listo_b1, 0, 0);
  pthread_create(&thA, NULL, Thread_A, NULL);
  pthread_create(&thB, NULL, Thread_B, NULL);

  pthread_join(thA, NULL);
  pthread_join(thB, NULL);

  sem_destroy(&listo_a1);
  sem_destroy(&listo_b1);

  return 0;
}
