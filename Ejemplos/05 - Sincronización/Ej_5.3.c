#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* func(void* arg) {
  printf("Hola desde el nuevo hilo.\n");
  return NULL;
}

int main() {
  pthread_t thread;
  pthread_create(&thread, NULL, func, NULL);
  pthread_join(thread, NULL);

  printf("Hola desde el hilo principal\n");

  return 0;
}