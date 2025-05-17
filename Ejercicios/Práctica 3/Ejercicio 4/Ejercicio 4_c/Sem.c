#include <semaphore.h>
#include <stdlib.h>
#include <pthread.h>
#include "rwlock.h"


struct rwlock {
  int lec;                      // Lectores activos
  int esc_esp;                  // Lectores esperando
  int esc_act;                  // 1 si hay un escritor activo, 0 si no
  pthread_mutex_t mutex;
  sem_t escOK;                  // Permitir lectura
  sem_t lecOK;                  // Permitir escritura
  sem_t cola;                   // Implementar cola de peticiones
};

struct rwlock *rwlock_init() {
  struct rwlock *rwl = malloc(sizeof(struct rwlock));
  pthread_mutex_init(&rwl->mutex, NULL);
  sem_init(&rwl->escOK, 0, 1);  // Se puede leer (si no hay escritores esperando)
  sem_init(&rwl->lecOK, 0, 1);  // Se puede escribir
  sem_init(&rwl->cola, 0, 1);   // Permite el primer hilo de la cola
  rwl->lec = 0;
  rwl->esc_esp = 0;
  rwl->esc_act = 0;

  return rwl;
}

void rwlock_destroy(struct rwlock *rwl) {
  pthread_mutex_destroy(&rwl->mutex);
  sem_destroy(&rwl->escOK);
  sem_destroy(&rwl->lecOK);
  sem_destroy(&rwl->cola);

  free(rwl);
}

void lock_r(struct rwlock *rwl) {
  sem_wait(&rwl->cola);
  pthread_mutex_lock(&rwl->mutex);
  while (rwl->esc_act || rwl->esc_esp > 0) {
    pthread_mutex_unlock(&rwl->mutex);
    sem_wait(&rwl->lecOK);
    pthread_mutex_lock(&rwl->mutex);
  }
  rwl->lec++;
  pthread_mutex_unlock(&rwl->mutex);
  sem_post(&rwl->cola);
}

void unlock_r(struct rwlock *rwl) {
  pthread_mutex_lock(&rwl->mutex);
  rwl->lec--;
  if (rwl->lec == 0 && rwl->esc_esp > 0) {
    sem_post(&rwl->escOK);
  }
  pthread_mutex_unlock(&rwl->mutex);
}

void lock_w(struct rwlock *rwl) {
  sem_wait(&rwl->cola);
  pthread_mutex_lock(&rwl->mutex);
  while (rwl->lec > 0 || rwl->esc_act) {
    pthread_mutex_unlock(&rwl->mutex);
    sem_wait(&rwl->escOK);
    pthread_mutex_lock(&rwl->mutex);
  }
  rwl->esc_esp--;
  rwl->esc_act = 1;
  pthread_mutex_unlock(&rwl->mutex);
  sem_post(&rwl->cola);
}

void unlock_w(struct rwlock *rwl) {
  pthread_mutex_lock(&rwl->mutex);
  rwl->esc_act = 0;
  if (rwl->esc_esp > 0) {
    sem_post(&rwl->escOK);
  } else {
    sem_post(&rwl->lecOK);
  }
  pthread_mutex_unlock(&rwl->mutex);
}
