#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <sys/wait.h>
#include <sys/types.h>
#define visitantes_moli 40

int contador = 0;
int flag[2] = { 0, 0 };         // 0 -> sin intencion de entrar a RC, 1 -> con intencion

int turno = 0;                  // A quién le toca

void *molinete0(void *proceso) {
  for (int x = 0; x < visitantes_moli / 2; ++x) {
    // LOCK
    flag[0] = 1;
    turno = 1;                  // Cedo el turno al otro
    while (flag[1] == 1 && turno == 2) {
      ;                         // Esperar
    }

    // REGIÓN CRÍTICA
    contador++;

    // UNLOCK
    flag[0] = 0;                // ya terminé

    printf("[%d] m1 - Ha ingresado otro más (%p) \n", contador, proceso);
  }

  return NULL;
}

void *molinete1(void *proceso) {
  for (int x = 0; x < visitantes_moli / 2; ++x) {
    // LOCK
    flag[1] = 1;
    turno = 0;
    while (flag[0] == 1 && turno == 1) {
      ;                         // ESPERAR
    }

    // REGIÓN CRÍTICA
    contador++;

    // UNLOCK
    flag[1] = 0;

    printf("[%d] m2 - Ha ingresado otro más (%p) \n", contador, proceso);
  }

  return NULL;
}

int main() {
  pthread_t moli_1, moli_2;

  pthread_create(&moli_1, NULL, molinete0, NULL);
  pthread_create(&moli_2, NULL, molinete1, NULL);

  pthread_join(moli_1, NULL);
  pthread_join(moli_2, NULL);

  printf("[VISITANTES TOTAL] %d \t", contador);

  return 0;
}
