#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

struct barrier {
  pthread_spinlock_t my_spin;
  pthread_cond_t cond;
  int contador;
  int n;
  int ciclo;
};

void barrier_init(struct barrier *b, int n) {
  pthread_mutex_init(&b->mutex, NULL);
  pthread_cond_init(&b->cond, NULL);
  b->contador = 0;
  b->n = n;
  b->ciclo = 0;
}

void barrier_wait(struct barrier *b) {
  pthread_mutex_lock(&b->mutex);
  int mi_ciclo = b->ciclo;
  b->contador += 1;
  if (b->contador == b->n) {
    b->ciclo += 1;
    b->contador = 0;
    pthread_cond_broadcast(&b->cond);
  } else {
    while (mi_ciclo == b->ciclo) {
      pthread_cond_wait(&b->cond, &b->mutex);
    }
  }

  pthread_mutex_unlock(&b->mutex);
}