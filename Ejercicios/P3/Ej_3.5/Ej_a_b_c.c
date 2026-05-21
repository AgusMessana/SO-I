#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

/**
 * Descomentar el que se quiera según read-preferring, write-preferring o justo.
 */
#include "rwlock_rp.h"
// #include "rwlock_wp.h"
// #include "rwlock_justo.h"

#define M 5
#define N 5
#define ARRLEN 10240

int arr[ARRLEN];
rwlock_t candado;

void *escritor(void *arg) {
  int i;
  int num = arg - (void *) 0;
  while (1) {
    sleep(random() % 3);
    printf("Escritor %d escribiendo\n", num);
    rwlock_write_lock(&candado);
    for (i = 0; i < ARRLEN; i++) {
      arr[i] = num;
    }
    rwlock_write_unlock(&candado);
  }
  return NULL;
}

void *lector(void *arg) {
  int v, i;
  int num = arg - (void *) 0;
  while (1) {
    sleep(random() % 3);
    rwlock_read_lock(&candado);
    v = arr[0];
    for (i = 1; i < ARRLEN; i++) {
      if (arr[i] != v)
        break;
    }
    rwlock_read_unlock(&candado);
    if (i < ARRLEN)
      printf("Lector %d, error de lectura\n", num);
    else
      printf("Lector %d, dato %d\n", num, v);
  }
  return NULL;
}

int main() {
  pthread_t lectores[M], escritores[N];
  rwlock_init(&candado);
  int i;

  for (i = 0; i < M; i++)
    pthread_create(&lectores[i], NULL, lector, i + (void *) 0);

  for (i = 0; i < N; i++)
    pthread_create(&escritores[i], NULL, escritor, i + (void *) 0);

  pthread_join(lectores[0], NULL);      /* Espera para siempre */
  return 0;
}
