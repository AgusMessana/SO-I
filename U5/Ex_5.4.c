#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int NPuntos = 100000;
int puntosDentro = 0;

void *marcar_punto(void *arg) {
  int puntosDentroAux = 0;
  for (int i = 0; i < NPuntos; i++) {
    double x = ((double) rand() / RAND_MAX) * 2.0 - 1.0;
    double y = ((double) rand() / RAND_MAX) * 2.0 - 1.0;        // Para que los puntos estén entre -1 y 1
    if (x * x + y * y <= 1) {
      puntosDentroAux++;
    }
  }

  pthread_mutex_lock(&mutex);
  puntosDentro += puntosDentroAux;
  pthread_mutex_unlock(&mutex);

  return NULL;
}

int main(void) {
  srand(time(NULL));
  pthread_t hilo1;
  int res_create = pthread_create(&hilo1, NULL, marcar_punto, NULL);
  if (res_create != 0) {
    perror("Error en la ejecución de create.\n");
    exit(EXIT_FAILURE);
  }

  printf("MAIN JOINING...\n");
  int res_join = pthread_join(hilo1, NULL);
  if (res_join != 0) {
    perror("Error en la ejecución de join.\n");
    exit(EXIT_FAILURE);
  }

  double aprox_pi = (4.0 * puntosDentro) / NPuntos;
  printf("Aporximación de pi: %lf\n", aprox_pi);

  pthread_mutex_destroy(&mutex);
  return 0;
}
