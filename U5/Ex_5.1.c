#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *funcion(void *arg) {
  printf("Hilo: Hello World!\n");

  return NULL;
}

int main(void) {
  pthread_t hilo1;
  int res_create = pthread_create(&hilo1, NULL, funcion, NULL);
  if (res_create != 0) {
    perror("Error en la ejecución de pthread_create.\n");
    exit(EXIT_FAILURE);
  }

  printf("MAIN TERMINANDO...\n");

  return 0;
}

/**
 * Hace falta el join para que el programa funcione correctamente
 */
