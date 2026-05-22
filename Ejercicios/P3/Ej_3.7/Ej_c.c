#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

sem_t tabaco, papel, fosforos, otra_vez;

void agente() {
  while (1) {
    sem_wait(&otra_vez);
    int caso = random() % 3;
    if (caso != 0)
      sem_post(&fosforos);
    if (caso != 1)
      sem_post(&papel);
    if (caso != 2)
      sem_post(&tabaco);
  }
}

void fumar(int fumador) {
  printf("Fumador %d: Puf! Puf! Puf!\n", fumador);
  sleep(2);
}

void *fumador1(void *arg) {
  while (1) {
    sem_wait(&tabaco);
    printf("Fumador 1: tomé el tabaco. Espero el papel.\n");
    if (sem_trywait(&papel) == 0) {
      printf("Fumador 1: tomé el papel. Fumo.\n");
      fumar(1);
      sem_post(&otra_vez);
    } else {
      sem_post(&tabaco);

      // El usleep es para evitar livelock
      usleep(10000);
    }
  }
}

void *fumador2(void *arg) {
  while (1) {
    sem_wait(&fosforos);
    printf("Fumador 2: tomé los fósforos. Espero el tabaco.\n");
    if (sem_trywait(&tabaco) == 0) {
      printf("Fumador 2: tomé el tabaco. Fumo.\n");
      fumar(2);
      sem_post(&otra_vez);
    } else {
      sem_post(&fosforos);
      usleep(10000);
    }
  }
}

void *fumador3(void *arg) {
  while (1) {
    sem_wait(&papel);
    printf("Fumador 3: tomé el papel. Espero los fósforos.\n");
    if (sem_trywait(&fosforos) == 0) {
      printf("Fumador 3: tomé los fósforos. Fumo.\n");
      fumar(3);
      sem_post(&otra_vez);
    } else {
      sem_post(&papel);
      usleep(10000);
    }
  }
}

int main() {
  pthread_t s1, s2, s3;

  sem_init(&tabaco, 0, 0);
  sem_init(&papel, 0, 0);
  sem_init(&fosforos, 0, 0);
  sem_init(&otra_vez, 0, 1);

  pthread_create(&s1, NULL, fumador1, NULL);
  pthread_create(&s2, NULL, fumador2, NULL);
  pthread_create(&s3, NULL, fumador3, NULL);

  agente();

  sem_destroy(&tabaco);
  sem_destroy(&papel);
  sem_destroy(&fosforos);
  sem_destroy(&otra_vez);

  return 0;
}
