#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

sem_t tabaco, papel, fosforos, otra_vez;
sem_t sem_fumador1, sem_fumador2, sem_fumador3;

pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

int mesa_tabaco = 0;
int mesa_papel = 0;
int mesa_fosforos = 0;

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

void *pushear_tabaco_f(void *arg) {
  while (1) {
    sem_wait(&tabaco);
    pthread_mutex_lock(&mtx);
    if (mesa_papel) {
      mesa_papel = 0;
      sem_post(&sem_fumador1);
    } else if (mesa_fosforos) {
      mesa_fosforos = 0;
      sem_post(&sem_fumador2);
    } else {
      mesa_tabaco = 1;
    }
    pthread_mutex_unlock(&mtx);
  }

  return NULL;
}

void *pushear_papel_f(void *arg) {
  while (1) {
    sem_wait(&papel);
    pthread_mutex_lock(&mtx);
    if (mesa_tabaco) {
      mesa_tabaco = 0;
      sem_post(&sem_fumador1);
    } else if (mesa_fosforos) {
      mesa_fosforos = 0;
      sem_post(&sem_fumador3);
    } else {
      mesa_papel = 1;
    }
    pthread_mutex_unlock(&mtx);
  }

  return NULL;
}

void *pushear_fosforos_f(void *arg) {
  while (1) {
    sem_wait(&fosforos);
    pthread_mutex_lock(&mtx);
    if (mesa_tabaco) {
      mesa_tabaco = 0;
      sem_post(&sem_fumador2);
    } else if (mesa_papel) {
      mesa_papel = 0;
      sem_post(&sem_fumador3);
    } else {
      mesa_fosforos = 1;
    }
    pthread_mutex_unlock(&mtx);
  }

  return NULL;
}

void *fumador1(void *arg) {
  while (1) {
    sem_wait(&sem_fumador1);
    fumar(1);
    sem_post(&otra_vez);
  }

  return NULL;
}

void *fumador2(void *arg) {
  while (1) {
    sem_wait(&sem_fumador2);
    fumar(2);
    sem_post(&otra_vez);
  }

  return NULL;
}

void *fumador3(void *arg) {
  while (1) {
    sem_wait(&sem_fumador3);
    fumar(3);
    sem_post(&otra_vez);
  }

  return NULL;
}

int main() {
  pthread_t s1, s2, s3;
  pthread_t pt, pp, pf;

  sem_init(&tabaco, 0, 0);
  sem_init(&papel, 0, 0);
  sem_init(&fosforos, 0, 0);
  sem_init(&otra_vez, 0, 1);

  sem_init(&sem_fumador1, 0, 0);
  sem_init(&sem_fumador2, 0, 0);
  sem_init(&sem_fumador3, 0, 0);

  pthread_create(&s1, NULL, fumador1, NULL);
  pthread_create(&s2, NULL, fumador2, NULL);
  pthread_create(&s3, NULL, fumador3, NULL);

  pthread_create(&pt, NULL, pushear_tabaco_f, NULL);
  pthread_create(&pp, NULL, pushear_papel_f, NULL);
  pthread_create(&pf, NULL, pushear_fosforos_f, NULL);

  printf("Abriendo la tabaquería...\n");
  agente();

  return 0;
}

/**
 * Esta solución implementa variables de estado booleanas (las mesa_INGREDIENTE) y tres hilos intermediarios (pushers).
 * Estos hilos se encargan de retirar los recursos y evaluar qué ingredientes hay disponibles sobre la mesa.
 * Al proteger esta sección con un mutex, nos aseguramos de que la inspección y actualización de las variables la haga solo un hilo a la vez, logrando que la administración de los recursos sea atómica.
 * Tras verificar el estado global, el intermediario despierta ÚNICAMENTE al fumador que tiene el combo ganador mediante su semáforo privado, evitando colisiones y bloqueos, para luego soltar el mutex y permitir que el programa siga su curso.
 */
