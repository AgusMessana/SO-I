#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>

volatile int x = 0;
volatile int y = 0;

void *wr(void *arg) {
  x = 123;
  y = 1;
}

void *rd(void *arg) {
  while (!y);
  assert(x == 123);
}


// Sí, puede fallar.
// Aunque rd ve que y == 1, no está garantizado que también vea x == 123. Esto pues el hardware puede reordenar escrituras y lecturas. El volatile sólo le dice al compilador que no optimice ciertas lecturas/escrituras, pero no garantiza orden entre hilos.
// Así, como rd puede ver el cambio en y pero todavía no ver el cambio en x, assert(x == 123) puede fallar.

//! Código arreglado

volatile int x = 0;
volatile int y = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *wr(void *arg) {
  pthread_mutex_lock(&lock);
  x = 123;
  y = 1;
  pthread_mutex_unlock(&lock);

  return NULL;
}

void *rd(void *arg) {
  pthread_mutex_lock(&lock);
  while (!y) {
    pthread_mutex_unlock(&lock);
    sched_yield();              // ceder CPU
    pthread_mutex_lock(&lock);
  }
  assert(x == 123);
  pthread_mutex_unlock(&lock);

  return NULL;
}
