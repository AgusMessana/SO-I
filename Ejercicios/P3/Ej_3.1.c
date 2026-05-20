#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

pthread_mutex_t lk = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv = PTHREAD_COND_INITIALIZER;

void funcion(void *arg) {
  pthread_mutex_lock(&lk);
  bool P = true;
  pthread_cond_signal(&cv);
  bool M = true;
  pthread_mutex_unlock(&lk);
}

/**
 * Si usamos semántica MESA con sólo dos hilos, el programa funciona correctamente.
 * Como el lock está tomado por B, al momento de mandar la señal, A no puede tomar el lock, por lo que no sale del while. Una vez que B suelta el lock, A lo toma y ve que P y M son true.
 * En semántica HOARE, el programa fallaría porque al haber un if, A podría despertarse antes de que M sea true.
 * El problema podría aparecer si hay un tercer hilo (llamémoslo C) el cual tome uno de los ingredientes al momento que B suelta el lock. Puede que A se despierte y no vea el ingrediente.
 */
