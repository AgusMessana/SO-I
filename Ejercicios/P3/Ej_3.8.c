#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define B 2
#define K (2 * B)

volatile int s, r, buf[B];
static inline int diff() {
  return (K + s - r) % K;
}

void *prod(void *_arg) {
  int cur = 0;
  while (1) {
    while (diff() == B);
    buf[s % B] = cur++;
    s = (s + 1) % K;
  }
}

void *cons(void *_arg) {
  int cur;
  while (1) {
    while (diff() == 0);
    cur = buf[r % B];
    r = (r + 1) % K;
    printf("Leí %d\n", cur);
  }
}

/**
 * Primeramente, la variable s sólo la modifica la función prod, y la variable r solo la modifica la función cons. O sea, no hay hilos "peleando" por la misma variable. Entonces, eso está cubierto.
 * Si bien buf sí es modificada por ambas funciones, prod la modifica cuando diff() == B, o sea, cuando el buffer está lleno. Y cons lo hace cuando diff() == 0. O sea, cuando está vacío. Como no puede estar lleno y vacío a la vez, no puede modificarse la variable por las dos funciones a la vez.
 */

/**
 * No se puede generalizar para n procesos. Habría race condition entre las variables r y s pues las operaciones que las modifican no son atómicas, y a la vez pueden sobreescribir datos en el buffer, perdiendo datos para siempre.
 * Este programa sólo sirve para dos procesos. Uno como productor y otro como consumidor.
 */
