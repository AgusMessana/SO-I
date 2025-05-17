#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#define M 5
#define N 5
#define SZ 8

/*
 * El buffer guarda punteros a enteros, los
 * productores los consiguen con malloc() y los
 * consumidores los liberan con free()
 */
int *buffer[SZ], ind_esc, ind_lec;
pthread_mutex_t lock_buff;
sem_t esp_vac, elem_disp;

void enviar(int *p) {
  sem_wait(&esp_vac);
  pthread_mutex_lock(&lock_buff);
  buffer[ind_esc] = p;
  ind_esc = (ind_esc + 1) % SZ;
  pthread_mutex_unlock(&lock_buff);
  sem_post(&elem_disp);
  return;
}

int *recibir() {
  int *p;
  sem_wait(&elem_disp);
  pthread_mutex_lock(&lock_buff);
  p = buffer[ind_lec];
  ind_lec = (ind_lec + 1) % SZ;
  pthread_mutex_unlock(&lock_buff);
  sem_post(&esp_vac);
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
  pthread_mutex_init(&lock_buff, NULL);
  sem_init(&esp_vac, 0, SZ);
  sem_init(&elem_disp, 0, 0);

  for (i = 0; i < M; i++)
    pthread_create(&productores[i], NULL, prod_f, i + (void *) 0);

  for (i = 0; i < N; i++)
    pthread_create(&consumidores[i], NULL, cons_f, i + (void *) 0);

  pthread_join(productores[0], NULL);   /* Espera para siempre */
  pthread_mutex_destroy(&lock_buff);
  sem_destroy(&esp_vac);
  sem_destroy(&elem_disp);
  return 0;
}
