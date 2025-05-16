#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#define N_FILOSOFOS 5
#define ESPERA 5000000          // 5 segundos

pthread_mutex_t tenedor[N_FILOSOFOS];
sem_t permiso_comer;
pthread_mutex_t *izq(int i) {
  return &tenedor[i];
}
pthread_mutex_t *der(int i) {
  return &tenedor[(i + 1) % N_FILOSOFOS];
}

void pensar(int i) {
  printf("Filosofo %d pensando...\n", i);
  usleep(random() % ESPERA);
}

void comer(int i) {
  printf("Filosofo %d comiendo...\n", i);
  usleep(random() % ESPERA);
}

void tomar_tenedores(int i) {
  sem_wait(&permiso_comer);
  pthread_mutex_lock(der(i));
  pthread_mutex_lock(izq(i));
}

void dejar_tenedores(int i) {
  pthread_mutex_unlock(izq(i));
  pthread_mutex_unlock(der(i));
  sem_post(&permiso_comer);
}

void *filosofo(void *arg) {
  int i = arg - (void *) 0;
  while (1) {
    tomar_tenedores(i);
    comer(i);
    dejar_tenedores(i);
    pensar(i);
  }
}

int main() {
  pthread_t filo[N_FILOSOFOS];
  int i;
  for (i = 0; i < N_FILOSOFOS; i++)
    pthread_mutex_init(&tenedor[i], NULL);
  sem_init(&permiso_comer, 0, N_FILOSOFOS - 1);
  for (i = 0; i < N_FILOSOFOS; i++)
    pthread_create(&filo[i], NULL, filosofo, i + (void *) 0);
  pthread_join(filo[0], NULL);
  sem_destroy(&permiso_comer);
  for (i = 0; i < N_FILOSOFOS; i++)
    pthread_mutex_destroy(&tenedor[i]);
  return 0;
}

// Como el semáforo se inicializa en N_FILOSOFOS - 1, se asegura de que a lo sumo 4 filósofos puedan pasar el wait. De esta manera, el quinto filósofo no podrá tomar tenedores, y deberá esperar a que se libere alguno para tomarlo. De esta forma, se rompe la espera circular.
