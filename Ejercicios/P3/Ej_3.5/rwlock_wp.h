#ifndef __RWLOCK_RP_H__
#define __RWLOCK_RP_H__
#include <stdio.h>
#include <pthread.h>

typedef struct {
  pthread_mutex_t lock;
  pthread_cond_t puede_escribir;
  pthread_cond_t puede_leer;
  int act_lect;
  int act_escr;
  int esp_escrs;
} rwlock_t;

/**
 * Inicializa las variables de la estructura.
 */
void rwlock_init(rwlock_t * rw);

/**
 * Simula la entrada de lectores.
 */
void rwlock_read_lock(rwlock_t * rw);

/**
 * Simula la salida de lectores.
 */
void rwlock_read_unlock(rwlock_t * rw);

/**
 * Simula la entrada de escritores.
 */
void rwlock_write_lock(rwlock_t * rw);

/**
 * Simula la salida de escritores.
 */
void rwlock_write_unlock(rwlock_t * rw);

#endif                          /* __RWLOCK_RP_H__ */
