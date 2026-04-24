#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

void *escribir_string(void *arg) {
  char *texto = (char *) arg;
  for (int i = 0; texto[i] != '\0'; i++) {
    putc(texto[i], stdout);
    usleep(rand() % 2000);
  }

  return NULL;
}

int main(void) {
  srand(time(NULL));
  setbuf(stdout, NULL);

  pthread_t hilo1, hilo2;

  int res_create1 = pthread_create(&hilo1, NULL, escribir_string, "HOLA ");
  int res_create2 = pthread_create(&hilo2, NULL, escribir_string, "CHAU");
  if (res_create1 != 0 || res_create2 != 0) {
    perror("Error en la ejecución de create.\n");
    exit(EXIT_FAILURE);
  }

  printf("MAIN JOINING...\n");
  int res_join1 = pthread_join(hilo1, NULL);
  int res_join2 = pthread_join(hilo2, NULL);
  if (res_join1 != 0 || res_join2 != 0) {
    perror("Error en la ejecución de join.\n");
    exit(EXIT_FAILURE);
  }

  printf("TERMINANDO...\n");

  return 0;
}
