#include <pthread.h>
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
int pos_ahora, cant_elems;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t no_vacio = PTHREAD_COND_INITIALIZER;
pthread_cond_t no_lleno = PTHREAD_COND_INITIALIZER;

void enviar(int *p) {
  pthread_mutex_lock(&mutex);
  while (cant_elems == SZ) {
    pthread_cond_wait(&no_lleno, &mutex);
  }

  buffer[pos_ahora] = p;
  cant_elems += 1;
  pos_ahora += 1;

  pthread_cond_signal(&no_vacio);
  pthread_mutex_unlock(&mutex);
  return;
}

int *recibir() {
  pthread_mutex_lock(&mutex);
  while (cant_elems == 0) {
    pthread_cond_wait(&no_vacio, &mutex);
  }

  pos_ahora -= 1;
  int *p = buffer[pos_ahora];
  cant_elems -= 1;

  pthread_cond_signal(&no_lleno);
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
  int i;

  for (i = 0; i < M; i++)
    pthread_create(&productores[i], NULL, prod_f, i + (void *) 0);

  for (i = 0; i < N; i++)
    pthread_create(&consumidores[i], NULL, cons_f, i + (void *) 0);

  pthread_join(productores[0], NULL);   /* Espera para siempre */

  return 0;
}
