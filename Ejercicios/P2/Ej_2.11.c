/**
 * Usando CAS, implemente una variante del jardín ornamental sin usar locks. Compare la performance de esta variante lock-free con la variante que implementa un mutex via CAS, especialmente al aumentar mucho el número de hilos
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define CANT_VISITANTES 1000000
int visitantes = 0;
int lock = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


/**
 * Para la versión lock-free haremos lo siguiente
 */
void *molinete0_lf(void *arg) {
  for (int i = 0; i < CANT_VISITANTES / 2; i++) {
    int viejo, nuevo;
    while (1) {
      viejo = visitantes;       // 1. leer
      nuevo = viejo + 1;        // 2. calcular
      if (__sync_bool_compare_and_swap(&visitantes, viejo, nuevo))
        break;                  // 3. si el CAS tuvo éxito, salie
      // si no, repetir
    }
  }

  return NULL;
}

void *molinete1_lf(void *arg) {
  for (int i = 0; i < CANT_VISITANTES / 2; i++) {
    int viejo, nuevo;
    while (1) {
      viejo = visitantes;       // 1. leés
      nuevo = viejo + 1;        // 2. calculás
      if (__sync_bool_compare_and_swap(&visitantes, viejo, nuevo))
        break;                  // 3. si el CAS tuvo éxito, salís
      // si no, repetís
    }
  }

  return NULL;
}

/**
 * Para la versión con lock haremos lo siguiente
 */
void *molinete0_l(void *arg) {
  for (int i = 0; i < CANT_VISITANTES / 2; i++) {
    // lock con CAS
    while (!__sync_bool_compare_and_swap(&lock, 0, 1));

    // sección crítica
    visitantes++;

    // unlock
    lock = 0;
  }

  return NULL;
}

void *molinete1_l(void *arg) {
  for (int i = 0; i < CANT_VISITANTES / 2; i++) {
    // lock con CAS
    while (!__sync_bool_compare_and_swap(&lock, 0, 1));

    // sección crítica
    visitantes++;

    // unlock
    lock = 0;
  }

  return NULL;
}

int main(void) {
  pthread_t h0, h1;
  pthread_create(&h0, NULL, molinete0_lf, NULL);
  pthread_create(&h1, NULL, molinete1_lf, NULL);

  printf("MAIN JOINING...\n");
  pthread_join(h0, NULL);
  pthread_join(h1, NULL);

  printf("Cantidad visitantes: %d.\n", visitantes);

  return 0;
}

/**
 * Con muchos hilos, la versión más rápida es la lock-free porque todos los hilos intentan acceder al mismo tiempo. Uno solo lo hace, aumenta el valor y sale, y todos vuelven a intentar.
 * Con lock, uno entra y todos los demás deben esperar a que el hilo que tomó el lock lo suelte, generando pérdidas de tiempo.
 */