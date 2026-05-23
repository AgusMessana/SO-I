#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int contador;
} mi_sem;

void mi_sem_init(mi_sem * sem, int value) {
  pthread_mutex_init(&sem->mutex, NULL);
  pthread_cond_init(&sem->cond, NULL);
  sem->contador = value;
}

void mi_sem_wait(mi_sem * sem) {
  pthread_mutex_lock(&sem->mutex);
  while (sem->contador == 0) {
    pthread_cond_wait(&sem->cond, &sem->mutex);
  }
  sem->contador -= 1;
  pthread_mutex_unlock(&sem->mutex);
}

void mi_sem_post(mi_sem * sem) {
  pthread_mutex_lock(&sem->mutex);
  pthread_cond_signal(&sem->cond);
  sem->contador += 1;
  pthread_mutex_unlock(&sem->mutex);
}
