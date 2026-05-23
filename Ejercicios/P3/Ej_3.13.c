#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  sem_t mutex;
  sem_t sem;
  int contador;
} mi_cond;

void mi_cond_init(mi_cond * cond) {
  sem_init(&cond->mutex, 0, 1);
  sem_init(&cond->sem, 0, 0);
  cond->contador = 0;
}

void 