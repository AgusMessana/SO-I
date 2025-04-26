// CAS significa compare-and-swap y funciona de la siguiente forma:
// CAS(&x, y, z), donde x es nuestra variable a modificar, y es el valor esperado y z es el nuevo valor.
// Si la variable tiene en valor esperado, la cambia al nuevo valor, si no, no hace nada. Además, dice si se logró o no.

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define visitantes_moli 40

volatile int contador = 0;

void *molinete(void *proceso) {
  for (int x = 0; x < visitantes_moli / 2; ++x) {
    int viejo = contador;
    // Mientras CAS falle (o sea, no logre actualizar contador), seguimos intentando
    while (__sync_val_compare_and_swap(&contador, viejo, viejo + 1) != viejo) {
      viejo = contador;         // actualizamos "viejo" porque alguien pudo haber cambiado contador
    }
    printf("[%d] molinete - Ha ingresado otro más (%p)\n", contador, proceso);
  }

  return NULL;
}

int main() {
  pthread_t moli_1, moli_2;

  pthread_create(&moli_1, NULL, molinete, NULL);
  pthread_create(&moli_2, NULL, molinete, NULL);

  pthread_join(moli_1, NULL);
  pthread_join(moli_2, NULL);

  printf("[VISITANTES TOTAL] %d\n", contador);

  return 0;
}
