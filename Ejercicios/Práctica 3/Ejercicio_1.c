#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

bool P = false;
bool M = false;
pthread_mutex_t lk = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

// Lo que hace el proceso B
void *proceso_b(void *arg) {
  pthread_mutex_lock(&lk);
  P = true;
  pthread_cond_signal(&cv);
  M = true;
  pthread_mutex_unlock(&lk);
  return NULL;
}

int main() {
  if (pthread_mutex_init(&lk, NULL) != 0) {
    perror("Mutex init failed\n");
    exit(EXIT_FAILURE);
  }
  if (pthread_cond_init(&cv, NULL) != 0) {
    perror("Condition variable init failed\n");
    exit(EXIT_FAILURE);
  }
  pthread_t thread_b;
  if (pthread_create(&thread_b, NULL, proceso_b, NULL) != 0) {
    perror("Thread creation failed\n");
    exit(EXIT_FAILURE);
  }
  pthread_join(thread_b, NULL);
  printf("Proceso B terminó de ejecutarse\n");

  pthread_mutex_destroy(&lk);
  pthread_cond_destroy(&cv);

  return 0;
}

// Esta implementación no es correcta y puede traer errores.
// El proceso A toma el mutex lk y ve que P y M son falsos. Por lo tanto llama a pthread_cond_wait(&cv) y libera el mutex lk. Esto hace que el proceso A se ponga a dormir hasta que se de la señal cv.
// El proceso B toma el mutex lk y pone a P en true. Luego da la señal cv, lo que despierta el proceso A. Por lo tanto, el proceso A verá que P es true y M es false, lo que ocasiona que A destruya la casa. Luego se establece m en true y se suelta el mutex lk.