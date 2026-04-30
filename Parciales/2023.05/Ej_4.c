#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int ben = 0;
// 1. Declaramos un Mutex y una Variable de Condición globales
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t c = PTHREAD_COND_INITIALIZER;

void *helper(void *arg) {
  // 2. El hijo toma el lock, modifica ben, avisa y suelta el lock
  pthread_mutex_lock(&m);
  ben += 1;
  pthread_cond_signal(&c);      // Toca la alarma para avisarle al padre
  pthread_mutex_unlock(&m);

  pthread_exit(NULL);
}

void main() {
  pthread_t thread;
  pthread_create(&thread, NULL, &helper, NULL);
  pthread_yield();

  // 3. El padre toma el lock para leer la variable de forma segura
  pthread_mutex_lock(&m);

  // 4. ¡LA CLAVE DE MESA! Usamos un while en lugar de un if.
  // Mientras ben siga siendo 0, el padre se va a dormir soltando el lock.
  while (ben == 0) {
    pthread_cond_wait(&c, &m);
  }

  if (ben == 1)
    printf("Yeet Haw\n");
  else
    printf("Yee Howdy\n");

  // 5. El padre suelta el lock antes de terminar
  pthread_mutex_unlock(&m);
}
