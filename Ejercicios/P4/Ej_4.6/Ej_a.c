#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int *a;
  int n;
} paquete;

void *qsort_hilo(void *arg) {
  paquete *datos = (paquete *) arg;
  qsort(datos->a, datos->n);

  return NULL;
}

/* Particion de Lomuto, tomando el primer elemento como pivote */
int particionar(int a[], int n) {
  int i, j = 0;
  int p = a[0];
  swap(&a[0], &a[n - 1]);
  for (i = 0; i < n - 1; i++) {
    if (a[i] <= p)
      swap(&a[i], &a[j++]);
  }
  swap(&a[j], &a[n - 1]);
  return j;
}

void qsort(int a[], int n) {
  if (n < 2)
    return;

  paquete datos_izq;
  pthread_t h;
  int p = particionar(a, n);
  datos_izq.a = a;
  datos_izq.n = p;

  pthread_create(&h, NULL, qsort_hilo, &datos_izq);
  qsort(a + p + 1, n - p - 1);
  pthread_join(h, NULL);
}

/**
 * El problema aparece al momento de crear los hilos.
 * Si el tamaño del arrgelo es pequeño, es más rápido ordenarlo de forma secuencial que crear los hilos para que éstos lo hagan.
 * Si el tamaño del arreglo es grande, la cantidad de hilos a crear y destruir crece exponencialmente. Por ejemplo, para una arrgelo de 1.000.000 de valores, se deberían crear unos 500.000 hilos. Esto realentiza en gran medida el programa, y puede ocasionar que el mismo de rompa.
 */