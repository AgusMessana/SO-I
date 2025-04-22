#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <math.h>

// Cantidad de puntos a generar por hilo
#define PUNTOS 1000000

// Variable para contar puntos dentro del circulo
long pts_en_circ = 0;

// Mutex para evitar condición de carrera
pthread_mutex_t lock;

void* monte_carlo(void* arg) {
  long cuenta_local = 0;
  unsigned int semilla = time(NULL);

  for(long i = 0; i < PUNTOS; i++) {
    double x = (rand_r(&semilla) / (double)RAND_MAX) * 2.0 - 1.0; //x entre -1 y 1
    double y = (rand_r(&semilla) / (double)RAND_MAX) * 2.0 - 1.0; //y entre -1 y 1
    if(x*x + y*y <= 1.0) {
      cuenta_local++;
    }
  }

  pthread_mutex_lock(&lock);
  pts_en_circ += cuenta_local;
  pthread_mutex_unlock(&lock);

  return NULL;
}

int main() {
  pthread_t threads[4]; //Esto indica la cantidad de hilos que vamos a usar
  pthread_mutex_init(&lock, NULL);

  for(int i = 0; i < 4; i++) {
    pthread_create(&threads[i], NULL, monte_carlo, NULL);
  }

  for(int j = 0; j < 4; j++) {
    pthread_join(threads[j], NULL);
  }

  double pi = (4.0 * pts_en_circ) / (PUNTOS * 4.0);
  printf("Aproximación de pi: %f\n", pi);

  return 0;
}