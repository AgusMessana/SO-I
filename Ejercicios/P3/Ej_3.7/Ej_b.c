#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

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
  sleep(1);
}

void *fumador1(void *arg) {
  while (1) {
    sem_wait(&tabaco);
    printf("Fumador 1: tomé el tabaco. Espero el papel.\n");
    sem_wait(&papel);
    printf("Fumador 1: tomé el papel. Fumo.\n");
    fumar(1);
    sem_post(&otra_vez);
  }
}

void *fumador2(void *arg) {
  while (1) {
    sem_wait(&fosforos);
    printf("Fumador 2: tomé los fósforos. Espero el tabaco.\n");
    sem_wait(&tabaco);
    printf("Fumador 2: tomé el tabaco. Fumo.\n");
    fumar(2);
    sem_post(&otra_vez);
  }
}

void *fumador3(void *arg) {
  while (1) {
    sem_wait(&papel);
    printf("Fumador 3: tomé el papel. Espero los fósforos.\n");
    sem_wait(&fosforos);
    printf("Fumador 3: tomé los fósforos. Fumo.\n");
    fumar(3);
    sem_post(&otra_vez);
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

  return 0;
}

/**
 * No, no hay forma de reordenar los ingredientes para eliminar el deadlock. Podemos reordenar los ingredientes para asegurar que uno de los tres fumadores pueda fumar en cierto caso, pero no podemos lograr que todos lo hagan.
 * El problema radica en que los fumadores toman elementos que no le servirían porque no tienen el combo completo, impidiendo que los que sí lo tienen puedan fumar. Básicamente, el problema es la "ceguera" de los fuamdores.
 */
