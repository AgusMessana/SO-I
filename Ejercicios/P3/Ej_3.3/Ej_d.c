#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N_FILOSOFOS 5
#define ESPERA 5000000

pthread_mutex_t tenedor[N_FILOSOFOS];
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

void tomar_tenedores_inteligente(int i) {
  int tengo_los_dos = 0;

  while (tengo_los_dos == 0) {
    pthread_mutex_lock(der(i));

    if (pthread_mutex_trylock(izq(i)) == 0) {
      tengo_los_dos = 1;
    } else {
      pthread_mutex_unlock(der(i));
      usleep(random() % ESPERA);
    }
  }
}

void dejar_tenedores(int i) {
  pthread_mutex_unlock(der(i));
  pthread_mutex_unlock(izq(i));
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
  for (i = 0; i < N_FILOSOFOS; i++)
    pthread_create(&filo[i], NULL, filosofo, i + (void *) 0);
  pthread_join(filo[0], NULL);
  return 0;
}

/**
 * La solución funciona pero no es eficiente. Al intentar una y otra vez tomar el tenedor izquierdo se produce un busy-waiting, lo que consume CPU en exceso. A diferencia de la solución con semáforos, donde el proceso quedaba bloqueado si no podía comer, en este caso el proceso esta en constante funcionamiento.
 */
