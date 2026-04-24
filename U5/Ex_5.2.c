#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *funcion(void *arg) {
  printf("Hilo: el hilo creador espera a que termine para finalizar.\n");

  return NULL;
}

int main(void) {
  pthread_t hilo1;
  int res_create = pthread_create(&hilo1, NULL, funcion, NULL);
  if (res_create != 0) {
    perror("Error en la ejecución de pthread_create.\n");
    exit(EXIT_FAILURE);
  }

  printf("MAIN JOINING...\n");
  int res_join = pthread_join(hilo1, NULL);
  if (res_join != 0) {
    perror("Error en la ejecución de join.\n");
    exit(EXIT_FAILURE);
  }

  return 0;
}
