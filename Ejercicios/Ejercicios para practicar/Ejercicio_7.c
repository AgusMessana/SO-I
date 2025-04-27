// Completa el siguiente código donde hay dos threads que incrementan una variable global de manera concurrente. Asegúrate de usar un mutex para evitar condiciones de carrera.

#include <pthread.h>
#include <stdio.h>

int contador = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void *incrementar(void *arg) {
  for (int i = 0; i < 1000; i++) {
    // Completa el código aquí para bloquear y desbloquear el mutex
    pthread_mutex_lock(&lock);
    contador++;
    pthread_mutex_unlock(&lock);
  }

  return NULL;
}

int main() {
  pthread_t t1, t2;
  pthread_create(&t1, NULL, incrementar, NULL);
  pthread_create(&t2, NULL, incrementar, NULL);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  printf("Contador final: %d\n", contador);
  return 0;
}
