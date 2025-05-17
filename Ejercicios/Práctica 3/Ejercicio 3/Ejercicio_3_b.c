#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define M 5
#define N 5
#define SZ 8

/*
 * El buffer guarda punteros a enteros, los
 * productores los consiguen con malloc() y los
 * consumidores los liberan con free()
 */
int *buffer[SZ];
pthread_mutex_t lock_buff;
pthread_cond_t buffer_no_lleno, buffer_no_vacio;
int ind_esc, ind_lec, cant_elem;

void enviar(int *p) {
  pthread_mutex_lock(&lock_buff);
  while(cant_elem == SZ){
    pthread_cond_wait(&buffer_no_lleno, &lock_buff);
  }
  buffer[ind_esc] = p;
  ind_esc = (ind_esc + 1) % SZ;
  cant_elem++;

  pthread_cond_signal(&buffer_no_vacio);
  pthread_mutex_unlock(&lock_buff);
  return;
}

int *recibir() {
  int *p;
  pthread_mutex_lock(&lock_buff);
  while(cant_elem == 0){
    pthread_cond_wait(&buffer_no_vacio, &lock_buff);
  }
  p = buffer[ind_lec];
  ind_lec = (ind_lec + 1) % SZ;
  cant_elem--;
  pthread_cond_signal(&buffer_no_lleno);
  pthread_mutex_unlock(&lock_buff);
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
  pthread_cond_init(&buffer_no_lleno, NULL);
  pthread_cond_init(&buffer_no_vacio, NULL);

  for (i = 0; i < M; i++)
    pthread_create(&productores[i], NULL, prod_f, i + (void *) 0);

  for (i = 0; i < N; i++)
    pthread_create(&consumidores[i], NULL, cons_f, i + (void *) 0);

  pthread_join(productores[0], NULL);   /* Espera para siempre */
  pthread_mutex_destroy(&lock_buff);
  pthread_cond_destroy(&buffer_no_lleno);
  pthread_cond_destroy(&buffer_no_vacio);
  return 0;
}
