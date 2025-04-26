#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>

int x = 0, y = 0, a = 0, b = 0;
void *foo(void *arg) {
  x = 1;
  a = y;
  return NULL;
}
void *bar(void *arg) {
  y = 1;
  b = x;
  return NULL;
}
int main() {
  pthread_t t0, t1;
  pthread_create(&t0, NULL, foo, NULL);
  pthread_create(&t1, NULL, bar, NULL);
  pthread_join(t0, NULL);
  pthread_join(t1, NULL);
  assert(a || b);
  return 0;
}

// Sí, puede fallar.
// La falla se produce si foo ejecuta primero a = y antes de que bar haya hecho y = 1 y al mismo tiempo bar ejecuta b = x antes de que foo haya hecho x = 1.
// Si sucede eso, y todavía era 0 cuando foo hizo a = y, y x todavía era 0 cuando bar hizo b = x. Por ende, a == 0 y b == 0, lo que causa que a || b valga 0 y el assert falle, terminando el programa.
// Esto pasa pues los dos threads acceden y modifican variables compartidas sin sincronización. Esto es una race condition.

//! Código arreglado

int x = 0, y = 0, a = 0, b = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *foo(void *arg) {
  pthread_mutex_lock(&lock);
  x = 1;
  a = y;
  pthread_mutex_unlock(&lock);
  return NULL;
}
void *bar(void *arg) {
  pthread_mutex_lock(&lock);
  y = 1;
  b = x;
  pthread_mutex_unlock(&lock);
  return NULL;
}
int main() {
  pthread_t t0, t1;
  pthread_create(&t0, NULL, foo, NULL);
  pthread_create(&t1, NULL, bar, NULL);
  pthread_join(t0, NULL);
  pthread_join(t1, NULL);
  assert(a || b);
  return 0;
}