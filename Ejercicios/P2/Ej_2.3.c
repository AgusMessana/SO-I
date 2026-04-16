/**
 * ¿Puede fallar la siguiente aserción? ¿Bajo qué condiciones? Explique. Si puede fallar, arregle el programa.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <assert.h>

int x = 0, y = 0, a = 0, b = 0;
void *foo(void *arg) {
  x = 1;
  asm("mfence");
  a = y;
  return NULL;
}
void *bar(void *arg) {
  y = 1;
  asm("mfence");
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
}

/**
 * El assert puede fallar si el compilador y/o el procesador ejecutan primero las instrucciones a = y y b = x antes de realizar las instrucciones x = 1 e y = 1. De esa forma quedaría assert(0 || 0), fallando.
 * Lo podemos corregir con una barrera de memoria como se muestra en el código.
 */