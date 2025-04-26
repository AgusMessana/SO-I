#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define MAX_ELEM 4

int suma = 0;
int arreglo[MAX_ELEM] = { 1, 2, 6, -3 };

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *suma_pares(void *arg) {
  for (int i = 0; i < MAX_ELEM; i += 2) {
    pthread_mutex_lock(&lock);
    int num = arreglo[i];
    suma += num;
    pthread_mutex_unlock(&lock);
  }

  return NULL;
}

void *suma_impares(void *arg) {
  for (int i = 1; i < MAX_ELEM; i += 2) {
    pthread_mutex_lock(&lock);
    int num = arreglo[i];
    suma += num;
    pthread_mutex_unlock(&lock);
  }

  return NULL;
}

int main() {
  pthread_mutex_init(&lock, NULL);
  pthread_t t1, t2;

  pthread_create(&t1, NULL, suma_pares, NULL);
  pthread_create(&t2, NULL, suma_impares, NULL);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  pthread_mutex_destroy(&lock);

  printf("La suma de los elementos del arreglo es: %d\n", suma);

  return 0;
}
