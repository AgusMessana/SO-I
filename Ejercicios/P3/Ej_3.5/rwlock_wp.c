#include <stdio.h>
#include <pthread.h>
#include "rwlock_wp.h"

void rwlock_init(rwlock_t * rw) {
  pthread_mutex_init(&rw->lock, NULL);
  pthread_cond_init(&rw->puede_escribir, NULL);
  pthread_cond_init(&rw->puede_leer, NULL);
  rw->act_escr = 0;
  rw->act_lect = 0;
  rw->esp_escrs = 0;
}

void rwlock_read_lock(rwlock_t * rw) {
  pthread_mutex_lock(&rw->lock);
  while (rw->act_escr != 0 || rw->esp_escrs != 0) {
    pthread_cond_wait(&rw->puede_leer, &rw->lock);
  }

  rw->act_lect += 1;

  pthread_mutex_unlock(&rw->lock);
}

void rwlock_read_unlock(rwlock_t * rw) {
  pthread_mutex_lock(&rw->lock);

  rw->act_lect -= 1;

  if (rw->act_lect == 0) {
    pthread_cond_signal(&rw->puede_escribir);
  }
  pthread_mutex_unlock(&rw->lock);
}

void rwlock_write_lock(rwlock_t * rw) {
  pthread_mutex_lock(&rw->lock);

  rw->esp_escrs += 1;

  while (rw->act_lect != 0 || rw->act_escr != 0) {
    pthread_cond_wait(&rw->puede_escribir, &rw->lock);
  }

  rw->esp_escrs -= 1;
  rw->act_escr += 1;
  pthread_mutex_unlock(&rw->lock);
}

void rwlock_write_unlock(rwlock_t * rw) {
  pthread_mutex_lock(&rw->lock);

  rw->act_escr = 0;

  if(rw->esp_escrs != 0) {
    pthread_cond_signal(&rw->puede_escribir);
  } else {
    pthread_cond_broadcast(&rw->puede_leer);
  }
  pthread_mutex_unlock(&rw->lock);
}
