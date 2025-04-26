#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define N 1000000

int visitantes = 0;

//! Sin sincronizaación
void *sin_sinc(void *arg) {
  for (int i = 0; i < N; i++) {
    int c = visitantes;
    visitantes = c + 1;
  }

  return NULL;
}

//! Algoritmo de Peterson
int flag[2] = { 0, 0 };

int turno = 0;

void *peterson(void *arg) {
  int id = (int) (long) arg;
  for (int i = 0; i < N; i++) {
    flag[id] = 1;
    turno = 1 - id;
    while (flag[1 - id] == 1 && turno == 1 - id) {
      ;
    }

    int c = visitantes;
    visitantes = c + 1;

    flag[id] = 0;
  }

  return NULL;
}

//! Usando incl
static inline void incl(int *p) {
asm("incl %0": "+m"(*p): :"memory");
}

void *con_incl(void *arg) {
  for (int i = 0; i < N; i++) {
    // Incrementa de forma atómica
    incl(&visitantes);
  }

  return NULL;
}

//! usando un pthread_mutex_t
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *sin_sinc(void *arg) {
  for (int i = 0; i < N; i++) {
    pthread_mutex_lock(&lock);
    int c = visitantes;
    visitantes = c + 1;
    pthread_mutex_unlock(&lock);
  }

  return NULL;
}

int main() {
  // pthread_mutex_init(&lock, NULL);
  pthread_t molinete1, molinete2;
  pthread_create(&molinete1, NULL, sin_sinc /* algún algoritmo */ , NULL);
  pthread_create(&molinete2, NULL, sin_sinc, NULL);

  pthread_join(molinete1, NULL);
  pthread_join(molinete2, NULL);
  // pthread_mutex_destroy(&lock);

  // Sin multithreading
  // for(int i = 0; i < N; i++) {
  // visitantes++;
  // }

  printf("Visitantes finales: %d\n", visitantes);

  return 0;
}

//! Diferencias
//? Sin sincronización
// Problema: dos hilos pueden leer visitantes al mismo tiempo y perder incrementos.
// Resultado: visitantes finales menos que lo esperado (data race clásica).
// Ventaja: es rápido, porque no hay trabas ni bloqueos.

//? Algoritmo de Peterson
// Soluciona el problema de race conditions.
// Más lento que sin sincronización porque hay pausas activas (while) para respetar la prioridad.
// Es un método por software (sin hardware especial).

//? Usando incl (sin lock)
//incl es una instrucción atómica: hace el ++ directamente en memoria de forma indivisible.
// Muy rápido, porque no hay mutex ni barreras.
// Sin embargo, sin lock delante de incl, si hay más de un procesador, puede haber problemas.

//? Usando pthread_mutex_lock
// Garantiza exclusión mutua usando primitivas de la biblioteca pthread.
// Soluciona race conditions correctamente.
// Más lento que incl, porque adquiere y libera un mutex en cada iteración.
// Es más portable y seguro: funciona bien en cualquier cantidad de procesadores.

//? Un solo molinete, sin multithreading
// No hay competencia: todo lo hace un único hilo.
// Es el más rápido de todos, porque no hay sincronización, no hay switches de contexto, nada.
// No usa threads, así que no escala: si se quiere usar varios núcleos, no sirve.
