#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define M 5
#define N 5
#define SZ 8

/*
 * El buffer guarda punteros a enteros, los
 * productores los consiguen con malloc() y los
 * consumidores los liberan con free()
 */
int *buffer[SZ];
int pos_ahora;
sem_t hay_lugar, hay_elementos;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void enviar(int *p) {
  sem_wait(&hay_lugar);
  pthread_mutex_lock(&mutex);

  buffer[pos_ahora] = p;
  pos_ahora += 1;

  sem_post(&hay_elementos);
  pthread_mutex_unlock(&mutex);
  return;
}

int *recibir() {
  sem_wait(&hay_elementos);
  pthread_mutex_lock(&mutex);

  pos_ahora -= 1;
  int *p = buffer[pos_ahora];

  sem_post(&hay_lugar);
  pthread_mutex_unlock(&mutex);
  return p;
}

void *prod_f(void *arg) {
  int id = arg - (void *) 0;
  while (1) {
    sleep(random() % 3);

    int *p = malloc(sizeof *p);
    *p = random() % 100;
    printf("Productor %d: produje %p->%d\n", id, p, *p);
    enviar(p);
  }
  return NULL;
}

void *cons_f(void *arg) {
  int id = arg - (void *) 0;
  while (1) {
    sleep(random() % 3);

    int *p = recibir();
    printf("Consumidor %d: obtuve %p->%d\n", id, p, *p);
    free(p);
  }
  return NULL;
}

int main() {
  pthread_t productores[M], consumidores[N];
  sem_init(&hay_lugar, 0, SZ);
  sem_init(&hay_elementos, 0, 0);
  int i;

  for (i = 0; i < M; i++)
    pthread_create(&productores[i], NULL, prod_f, i + (void *) 0);

  for (i = 0; i < N; i++)
    pthread_create(&consumidores[i], NULL, cons_f, i + (void *) 0);

  pthread_join(productores[0], NULL);   /* Espera para siempre */

  sem_destroy(&hay_elementos);
  sem_destroy(&hay_lugar);

  return 0;
}
