#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define SILLAS 5
#define TOTAL_CLIENTES 10

typedef struct {
  pthread_mutex_t mutex;
  sem_t clientes;
  sem_t barbero;
  sem_t pago;
  sem_t recibo;
  int sillas_ocupadas;
} barberia_t;

void barberia_init(barberia_t * b) {
  pthread_mutex_init(&b->mutex, NULL);
  sem_init(&b->barbero, 0, 0);
  sem_init(&b->clientes, 0, 0);
  sem_init(&b->pago, 0, 0);
  sem_init(&b->recibo, 0, 0);
  b->sillas_ocupadas = 0;
}

void barberia_destr(barberia_t * b) {
  pthread_mutex_destroy(&b->mutex);
  sem_destroy(&b->clientes);
  sem_destroy(&b->barbero);
  sem_destroy(&b->pago);
  sem_destroy(&b->recibo);
}

void me_cortan() {
  printf("Cliente: me están corando el pelo...\n");
  sleep(1);
}

void cortando() {
  printf("Barbero: estoy cortando el pelo...\n");
  sleep(1);
}

void pagando() {
  printf("Cliente: estoy pagando el corte de pelo...\n");
  sleep(1);
}

void me_pagan() {
  printf("Barbero: me pagan el corte de pelo...\n");
  sleep(1);
}

void cliente_a_cortarse(barberia_t * b) {
  pthread_mutex_lock(&b->mutex);
  if (b->sillas_ocupadas == SILLAS) {
    pthread_mutex_unlock(&b->mutex);
    return;
  }
  b->sillas_ocupadas += 1;
  pthread_mutex_unlock(&b->mutex);

  sem_post(&b->clientes);
  sem_wait(&b->barbero);

  pthread_mutex_lock(&b->mutex);
  b->sillas_ocupadas -= 1;
  pthread_mutex_unlock(&b->mutex);

  me_cortan();

  pagando();
  sem_post(&b->pago);
  sem_wait(&b->recibo);
  printf("Cliente: me dieron el recibo. Me voy de la barbería...\n");
}

void barbero_trabajando(barberia_t * b) {
  while (1) {
    sem_wait(&b->clientes);
    sem_post(&b->barbero);

    cortando();

    sem_wait(&b->pago);
    me_pagan();
    sem_post(&b->recibo);
  }
}

void *hilo_cliente(void *arg) {
  barberia_t *b = (barberia_t *) arg;
  cliente_a_cortarse(b);
  return NULL;
}

void *hilo_barbero(void *arg) {
  barberia_t *b = (barberia_t *) arg;
  barbero_trabajando(b);
  return NULL;
}

int main(void) {
  barberia_t la_barberia;
  barberia_init(&la_barberia);

  pthread_t barbero;
  pthread_t clientes[TOTAL_CLIENTES];

  pthread_create(&barbero, NULL, hilo_barbero, &la_barberia);
  printf("Barbero: abro la barbería y me voy a dormir...\n");

  for (int i = 0; i < TOTAL_CLIENTES; i++) {
    pthread_create(&clientes[i], NULL, hilo_cliente, &la_barberia);

    sleep(1);
    // Comentar el sleep si se quiere que lleguen todos de una.
  }

  for (int i = 0; i < TOTAL_CLIENTES; i++) {
    pthread_join(clientes[i], NULL);
  }

  pthread_cancel(barbero);
  pthread_join(barbero, NULL);

  barberia_destr(&la_barberia);
  printf("Barbero: se cierra la barbería por hoy...\n");

  return 0;
}
