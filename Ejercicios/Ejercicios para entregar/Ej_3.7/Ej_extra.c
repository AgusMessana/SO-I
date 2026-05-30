#include <stdio.h>
#include <semaphore.h>
sem_t tabaco, papel, fosforos, otra_vez;

// Definimos una función para que no dé error, pero no serían sólo dos semáforos, serían n.
void P_atomico(sem_t * a, sem_t * b);

void *fumador1(void *arg) {
  while (1) {
    P_atomico(&tabaco, &papel);
    fumar(1);
    sem_post(&otra_vez);
  }
}

void *fumador2(void *arg) {
  while (1) {
    P_atomico(&fosforos, &tabaco);
    fumar(2);
    sem_post(&otra_vez);
  }
}

void *fumador3(void *arg) {
  while (1) {
    P_atomico(&papel, &fosforos);
    fumar(3);
    sem_post(&otra_vez);
  }
}

#include <pthread.h>
pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
int mesa_tabaco = 0;
int mesa_papel = 0;
int mesa_fosforos = 0;

// Hacemos el ejemplo del tabaco y fumador 1. Funciona para todos
sem_t sem_fumadores[3];

void *pushear_tabaco_f(void *arg) {
  while (1) {
    sem_wait(&tabaco);
    pthread_mutex_lock(&mtx);
    if (mesa_papel) {
      mesa_papel = 0;
      sem_post(&sem_fumadores[0]);
    } else if (mesa_fosforos) {
      mesa_fosforos = 0;
      sem_post(&sem_fumadores[1]);
    } else {
      mesa_tabaco = 1;
    }
    pthread_mutex_unlock(&mtx);
  }
  return NULL;
}

void *fumador1(void *arg) {
  while (1) {
    sem_wait(&sem_fumadores[0]);
    fumar(1);
    sem_post(&otra_vez);
  }
}

/**
 * El problema de los fumadores expone dos enfoques históricos.
 * Por un lado, Patil propuso usar un P n-ario (P_atomico), una operación teórica que toma múltiples recursos de forma atómica. Esto elimina la necesidad de intermediarios porque evita que un hilo retenga un recurso mientras espera otro, previniendo el deadlock directamente.
 * Por otro lado, la solución de Parnas refutó a Patil demostrando que sí se podía resolver usando un arreglo de semáforos (sem_t sem_fumadores[3]). Parnas agrupó lógicamente las señales privadas de cada fumador, logrando sincronizarlos de forma segura con las primitivas clásicas de Dijkstra sin modificar al agente.
 */
