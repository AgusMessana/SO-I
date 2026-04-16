/**
 * Analice y explique el comportamiento del siguiente programa.
 */

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
pthread_mutex_t a = PTHREAD_MUTEX_INITIALIZER;

void *foo(void *_arg) {
  // lock
  pthread_mutex_lock(&a);

  // zona crítica
  printf("Foo!\n");
  sleep(1);

  // unlock
  pthread_mutex_unlock(&a);

  return NULL;
}

int main() {
  pthread_t th;
  pthread_create(&th, NULL, foo, NULL);

  pthread_mutex_t b = a;

  // lock
  pthread_mutex_lock(&b);

  // zona crítica
  printf("Bar!\n");
  sleep(1);

  // unlock
  pthread_mutex_unlock(&b);

  pthread_join(th, NULL);

  return 0;
}

/**
 * El programa funciona pero es incorrecto igualar mutex de la forma en que está en el main. Ese mutex b quedará en un estado corrupto. Dicho comportamiento está indefinido por POSIX.
 * La razón del funcionamiento es que al copiar el mutex con = en ese momento exacto (antes de que nadie lo haya tomado), b queda con el mismo estado que a: libre. Entonces pthread_mutex_lock(&b) entra sin problemas.
 * El sleep(1) se "ignora" luego de Bar! ya que mientras se corre ese sleep, el hilo th ya está ejecutando foo, y por ende se imprime Foo! seguido de Bar! sin esperar un segundo.
 */