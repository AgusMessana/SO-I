#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#define N_FILOSOFOS 5
#define ESPERA_PENSAR 5000000   // 5 segundos
#define ESPERA_REINTENTO 100000 // 1 segundo

pthread_mutex_t tenedor[N_FILOSOFOS];
pthread_mutex_t estado_filosofo[N_FILOSOFOS];
enum {
  PENSANDO,
  HAMBRIENTO,
  COMIENDO,
} estado[N_FILOSOFOS];

pthread_mutex_t *izq(int i) {
  return &tenedor[i];
}
pthread_mutex_t *der(int i) {
  return &tenedor[(i + 1) % N_FILOSOFOS];
}

void pensar(int i) {
  pthread_mutex_lock(&estado_filosofo[i]);
  estado[i] = PENSANDO;
  printf("Filosofo %d pensando...\n", i);
  pthread_mutex_unlock(&estado_filosofo[i]);
  usleep(random() % ESPERA_PENSAR);
}

void comer(int i) {
  pthread_mutex_lock(&estado_filosofo[i]);
  estado[i] = COMIENDO;
  printf("Filosofo %d comiendo...\n", i);
  pthread_mutex_unlock(&estado_filosofo[i]);
  usleep(random() % ESPERA_PENSAR);
}

int intentar_tomar_tenedores(int i) {
  if (pthread_mutex_trylock(der(i)) == 0) {
    if (pthread_mutex_trylock(izq(i)) == 0) {
      return 1;
    }
  } else {
    pthread_mutex_lock(der(i));
    return 0;
  }
  return 0;
}

void dejar_tenedores(int i) {
  pthread_mutex_unlock(der(i));
  pthread_mutex_unlock(izq(i));
}

void *filosofo(void *arg) {
  int i = arg - (void *) 0;
  while (1) {
    pensar(i);
    pthread_mutex_lock(&estado_filosofo[i]);
    estado[i] = HAMBRIENTO;
    printf("Filosofo %d tiene hambre...\n", i);
    pthread_mutex_unlock(&estado_filosofo[i]);

    while (!intentar_tomar_tenedores(i)) {
      printf
          ("Filosofo %d no pudo tomar ambos tenedores, soltando el derecho (si lo tiene) y reintentando...\n",
           i);
      usleep(random() % ESPERA_REINTENTO);
    }
    comer(i);
    dejar_tenedores(i);
  }
}

int main() {
  pthread_t filo[N_FILOSOFOS];
  int i;
  for (i = 0; i < N_FILOSOFOS; i++) {
    pthread_mutex_init(&tenedor[i], NULL);
    pthread_mutex_init(&estado_filosofo[i], NULL);
  }
  for (i = 0; i < N_FILOSOFOS; i++) {
    pthread_create(&filo[i], NULL, filosofo, i + (void *) 0);
  }
  pthread_join(filo[0], NULL);
  for (i = 0; i < N_FILOSOFOS; i++) {
    pthread_mutex_destroy(&tenedor[i]);
    pthread_mutex_destroy(&estado_filosofo[i]);
  }

  return 0;
}

// Esta variante puede evitar el deadlock. La razón principal es que si un filósofo toma el tenedor derecho y no puede tomar el izquierdo (porque está ocupado), libera el tenedor derecho. Esto rompe la condición de "retención y espera"
// La eficiencia de esta solución es cuestionable. Introduce busy-waiting de una forma controlada. Los filósofos que no pueden obtener ambos tenedores entran en un bucle donde intentan tomar el tenedor derecho, fallan al intentar tomar el izquierdo, liberan el derecho (si lo tomaron) y luego esperan un corto período antes de reintentar. Este reintento constante consume ciclos de CPU aunque el filósofo no esté haciendo un progreso real hacia comer.
