/**
 * Implementar un programa con dos threads que suman los elementos de un arreglo compartido usando pthread_mutex_t para evitar race conditions.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int arreglo[] = { -42, 15, -7, 103, 0, -19, 88, -2, 54, -120, 33, -5, 77, -91 };

int suma = 0;

void *contador1(void *arg) {
  for (int i = 0; i < (sizeof(arreglo) / sizeof(int)) / 2; i++) {
    // lock
    pthread_mutex_lock(&mutex);

    // zona crítica
    suma += arreglo[i];

    // unlock
    pthread_mutex_unlock(&mutex);
  }

  return NULL;
}

void *contador2(void *arg) {
  for (int i = (sizeof(arreglo) / sizeof(int)) / 2;
       i < sizeof(arreglo) / sizeof(int); i++) {
    // lock
    pthread_mutex_lock(&mutex);

    // zona crítica
    suma += arreglo[i];

    // unlock
    pthread_mutex_unlock(&mutex);
  }

  return NULL;
}

/**
 * Otra forma de hacer el contador con un mutex más óptimo sería
 */
void *contador1_1(void *arg) {
  int suma_local = 0;
  int n = sizeof(arreglo) / sizeof(arreglo[0]);
  for (int i = 0; i < n / 2; i++) {
    suma_local += arreglo[i];
  }

  // lock
  pthread_mutex_lock(&mutex);

  // zona crítica
  suma += suma_local;

  // unlock
  pthread_mutex_unlock(&mutex);
  return NULL;
}

int main(void) {
  pthread_t hilo1, hilo2;
  pthread_create(&hilo1, NULL, contador1, NULL);
  pthread_create(&hilo2, NULL, contador2, NULL);

  printf("MAIN JOINING...\n");
  pthread_join(hilo1, NULL);
  pthread_join(hilo2, NULL);

  printf("Suma total del arreglo: %d\n", suma);

  printf("MAIN DESTROYING MUTEX...\n");
  pthread_mutex_destroy(&mutex);

  return 0;
}
