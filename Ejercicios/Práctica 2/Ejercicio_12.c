#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

pthread_mutex_t a = PTHREAD_MUTEX_INITIALIZER;

void *foo(void *_arg) {
  pthread_mutex_lock(&a);
  printf("Foo!\n");
  sleep(1);
  pthread_mutex_unlock(&a);
  return NULL;
}

int main() {
  pthread_t th;
  pthread_create(&th, NULL, foo, NULL);
  pthread_mutex_t b = a;

  pthread_mutex_lock(&b);
  printf("Bar!\n");
  sleep(1);
  pthread_mutex_unlock(&b);

  pthread_join(th, NULL);

  return 0;
}

// Este código tiene un problema. Cuando se hace pthread_mutex_t b = a, se copia el el mutex a nivel memoria, no se crea un nuevo mutex independiente. Ambos apuntan a la misma estructura interna de mutex.
// Enctonces, cuando el hilo foo() hace lock(&a) y main() hace lock(&b), ambos están intentando bloquear el mismo mutex.
// Esto trae como consecuencia que se imprima primero Foo! y después Bar!, o primero Bar! y después Foo!, dependiendo que hilo llegue primero.

//! Versión correcta
void *foo(void *_arg) {
  pthread_mutex_t *mutex = (pthread_mutex_t *) _arg;    // El hilo recibe el puntero al mutex
  pthread_mutex_lock(mutex);
  printf("Foo!\n");
  sleep(1);
  pthread_mutex_unlock(mutex);
  return NULL;
}

int main() {
  pthread_t th;

  pthread_create(&th, NULL, foo, &a);   // Pasamos la dirección de 'a' al hilo

  pthread_mutex_lock(&a);
  printf("Bar!\n");
  sleep(1);
  pthread_mutex_unlock(&a);

  pthread_join(th, NULL);

  return 0;
}
