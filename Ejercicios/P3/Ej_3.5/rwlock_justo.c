#include <stdio.h>
#include <pthread.h>
#include "rwlock_justo.h"

void rwlock_init(rwlock_t * rw) {
  pthread_mutex_init(&rw->lock, NULL);
  pthread_cond_init(&rw->puede_escribir, NULL);
  pthread_cond_init(&rw->puede_leer, NULL);
  rw->act_escr = 0;
  rw->act_lect = 0;
  rw->numero = 0;
  rw->turno = 0;
}

void rwlock_read_lock(rwlock_t * rw) {
  pthread_mutex_lock(&rw->lock);
  int mi_numero = rw->numero;
  rw->numero += 1;
  while (mi_numero != rw->turno) {
    pthread_cond_wait(&rw->puede_leer, &rw->lock);
  }

  rw->act_lect += 1;
  rw->turno += 1;

  pthread_cond_broadcast(&rw->puede_leer);
  pthread_cond_broadcast(&rw->puede_escribir);
  pthread_mutex_unlock(&rw->lock);
}

void rwlock_read_unlock(rwlock_t * rw) {
  pthread_mutex_lock(&rw->lock);

  rw->act_lect -= 1;

  if (rw->act_lect == 0) {
    pthread_cond_broadcast(&rw->puede_escribir);
  }
  pthread_mutex_unlock(&rw->lock);
}

void rwlock_write_lock(rwlock_t * rw) {
  pthread_mutex_lock(&rw->lock);
  int mi_numero = rw->numero;
  rw->numero += 1;
  while (rw->act_lect != 0 || rw->act_escr != 0 || mi_numero != rw->turno) {
    pthread_cond_wait(&rw->puede_escribir, &rw->lock);
  }

  rw->act_escr += 1;

  pthread_mutex_unlock(&rw->lock);
}

void rwlock_write_unlock(rwlock_t * rw) {
  pthread_mutex_lock(&rw->lock);

  rw->act_escr = 0;
  rw->turno += 1;

  pthread_cond_broadcast(&rw->puede_leer);
  pthread_cond_broadcast(&rw->puede_escribir);
  pthread_mutex_unlock(&rw->lock);
}
