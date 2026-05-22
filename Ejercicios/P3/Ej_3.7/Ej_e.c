#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t fum_1, fum_2, fum_3, otra_vez;

void agente() {
  while (1) {
    sem_wait(&otra_vez);
    int caso = random() % 3;
    if (caso == 0) {
      sem_post(&fum_1);
    } else if (caso == 1) {
      sem_post(&fum_2);
    } else {
      sem_post(&fum_3);
    }
  }
}

void fumar(int fumador) {
  printf("Fumador %d: Puf! Puf! Puf!\n", fumador);
  sleep(2);
}

void *fumador1(void *arg) {
  while (1) {
    sem_wait(&fum_1);
    fumar(1);
    sem_post(&otra_vez);
  }
}

void *fumador2(void *arg) {
  while (1) {
    sem_wait(&fum_2);
    fumar(2);
    sem_post(&otra_vez);
  }
}

void *fumador3(void *arg) {
  while (1) {
    sem_wait(&fum_3);
    fumar(3);
    sem_post(&otra_vez);
  }
}

int main() {
  pthread_t s1, s2, s3;

  sem_init(&fum_1, 0, 0);
  sem_init(&fum_2, 0, 0);
  sem_init(&fum_3, 0, 0);
  sem_init(&otra_vez, 0, 1);

  pthread_create(&s1, NULL, fumador1, NULL);
  pthread_create(&s2, NULL, fumador2, NULL);
  pthread_create(&s3, NULL, fumador3, NULL);

  agente();

  sem_destroy(&fum_1);
  sem_destroy(&fum_2);
  sem_destroy(&fum_3);
  sem_destroy(&otra_vez);

  return 0;
}

/**
 * Esta implementación es básicamente un servidor de turnos. Los fumadores están esperando para fumar. El agente es el encargado de cederles el turno a cada fumador con el fin de que no se bloqueen esperando otro elemento.
 * Con la idea del servidor, podemos simplificar ampliament el programa, a la vez que evitamos deadlocks o livelocks.
 */
