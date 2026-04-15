/**
 * Implemente el algoritmo de Peterson para solucionar el problema del jardín ornamental. Tenga en cuenta lo discutido sobre barreras de memoria.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define CANT_VISITANTES 1000000
int visitantes = 0;
int flag[] = { 0, 0 };          // flag[0] = 1 indica que molinete0 quiere entrar, flag[1] = 1 indica que molinete1 quiere entrar.

int turn = 0;                   // turn = 0 indica que molinete0 debe entrar, turn = 1 indica que molinete1 debe entrar.

void *molinete0(void *arg) {
  for (int i = 0; i < CANT_VISITANTES / 2; i++) {
    // lock
    flag[0] = 1;
    turn = 1;
    asm("mfence");
    while (flag[1] && turn == 1);

    // sección crítica
    visitantes++;
    printf("m0: visitantes hasta ahora: %d.\n", visitantes);

    // unlock
    flag[0] = 0;
  }

  return NULL;
}

void *molinete1(void *arg) {
  for (int i = 0; i < CANT_VISITANTES / 2; i++) {
    // lock
    flag[1] = 1;
    turn = 0;
    asm("mfence");
    while (flag[0] && turn == 0);

    // sección crítica
    visitantes++;
    printf("m1: visitantes hasta ahora: %d.\n", visitantes);
    // unlock
    flag[1] = 0;
  }

  return NULL;
}

int main(void) {
  pthread_t h0, h1;
  pthread_create(&h0, NULL, molinete0, NULL);
  pthread_create(&h1, NULL, molinete1, NULL);

  pthread_join(h0, NULL);
  pthread_join(h1, NULL);

  printf("Cantidad visitantes: %d.\n", visitantes);
  return 0;
}
