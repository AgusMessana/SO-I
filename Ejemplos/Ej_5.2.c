#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* func(void* arg) {
  printf("Hello world.\n");
  return NULL;
}

int main() {
  pthread_t thread;

  // El último NULL indica que argumentos toma el hilo.
  pthread_create(&thread, NULL, func, NULL); 

  //Funcioón para que se espere hasta que el hilo termine.
  // El NULL indica que no importa lo que retorne el thread.
  pthread_join(thread, NULL);

  return 0;
}