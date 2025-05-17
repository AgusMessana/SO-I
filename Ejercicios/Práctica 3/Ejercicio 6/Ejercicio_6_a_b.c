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

void *fumador1(void *arg) {     // tiene fósforos
  while (1) {
    sem_wait(&tabaco);
    sem_wait(&papel);
    fumar(1);
    sem_post(&otra_vez);
  }
}

void *fumador2(void *arg) {     // tiene papel
  while (1) {
    sem_wait(&fosforos);
    sem_wait(&tabaco);
    fumar(2);
    sem_post(&otra_vez);
  }
}

void *fumador3(void *arg) {     // tiene tabaco
  while (1) {
    sem_wait(&papel);
    sem_wait(&fosforos);
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

//! a)
// El deadlock puede ocurrier pues puede haber mala sincronización de los semáforos. Por ejemplo, el agente pone papel y fósforos, por lo que el fumador 1 debería fumar. Pero antes de que haga los dos sem_wait, el fumador 2 hace sem_wait(&fosforos) y el fuamdor 3 hace sem_wait(&papel). Así, al fumador 2 le falta tabaco. Al 3 le faltan fósforos y el 1 no tiene más elementos disponibles. Los 3 se bloquean.

//! b)
// Ordenar los ingredientes no soluciona el problema pues el inconveniente de este ejercicio se basa en que los fuamdores pueden tomar ingredientes que no le corresponden. Por ende, por más que se ordenen los ingredientes, los fuamdores pueden tomar los ingredientes que no le corresponden.