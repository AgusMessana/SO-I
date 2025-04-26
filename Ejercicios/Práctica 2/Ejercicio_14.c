// void charsof(char *s, int len, bool r[256]) {
//   int i;
//   for (i = 0; i < 256; i++)
//     r[i] = false;
//   parallel for (i = 0; i < len; i++)
//     r[s[i]] = true;
// }

// Este código tiene race condition porque varios hilos podrían tratar de escribir al mismo tiempo en la misma posición de r.
// Si s tiene dos letras 'a', y dos hilos distintos hacen r['a'] = true al mismo tiempo, escribir en memoria simultáneamente, incluso si el valor es el mismo (true), es una condición de carrera. El problema no es el valor sino el acceso concurrente no controlado.

//! Versión corregida

#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#define MAX_LEN 256

bool r[MAX_LEN];
char s[MAX_LEN];
int len;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *marcar_caracter(void *arg) {
  int i = *(int *) arg;
  char c = s[i];

  pthread_mutex_lock(&lock);
  r[(unsigned char) c] = true;
  pthread_mutex_unlock(&lock);

  return NULL;
}

void charsof(char *s_param, int len_param) {
  len = len_param;
  for (int i = 0; i < 256; i++) {
    r[i] = false;
  }

  pthread_t threads[len];
  int indices[len];

  for (int i = 0; i < len; i++) {
    indices[i] = i;
    pthread_create(&threads[i], NULL, marcar_caracter, &indices[i]);
  }

  for (int i = 0; i < len; i++) {
    pthread_join(threads[i], NULL);
  }
}
