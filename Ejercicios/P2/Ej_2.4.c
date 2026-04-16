/**
 * ¿Puede fallar la siguiente aserción? (wr y rd corren en un thread cada uno). Explique. Si puede fallar, arregle el programa.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

volatile int x = 0;
volatile int y = 0;

void *wr(void *arg) {
  x = 123;
  asm("mfence");
  y = 1;
}

void *rd(void *arg) {
  while (!y);
  printf("x = %d.\n", x);
  assert(x == 123);
}

int main(void) {
  pthread_t h1, h2;
  pthread_create(&h1, NULL, wr, NULL);
  pthread_create(&h2, NULL, rd, NULL);

  pthread_join(h1, NULL);
  pthread_join(h1, NULL);

  return 0;
}

/**
 * El programa puede fallar.
 * Aunque volatile le indica al compilador que no optimice el acceso a las variables x e y, no se garantiza la atomicidad ni orden de escritura. Podría pasar que wr escriba x = 123 e y = 1 pero el procesador podría reordenar las escrituras, provocando un assert(0).
 * Para solucionarlo usamos una barrera de memoria.
 */
