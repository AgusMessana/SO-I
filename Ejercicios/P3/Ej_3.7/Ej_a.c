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
 * Supongamos que el agente saca el caso = 0. O sea, hace un post de papel y un post de tabaco.
 * Supongamos que el fumador 3 se despierta. Ve el papel y lo toma. Como no hay fósforos, se queda esperando. Luego, se despierta el fumador 1, ve el tabaco y lo toma.
 * De esta forma, el fumador 1 se queda bloqueado esperando el papel que posee el fumador 3. El fumador 2 está bloqueado porque no hay fósforos, y el fumador 3 tiene el papel pero está bloqueado esperando los fósforos. Luego, ninguno llega al agente, y se produce el deadlock al no poder repartir los demás elementos nuevamente.
 */