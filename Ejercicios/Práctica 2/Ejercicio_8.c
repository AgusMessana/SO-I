#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#define N 10000

int visitantes = 0;

void *proc(void *arg) {
  int i;
  int id = arg - (void *) 0;
  for (i = 0; i < N; i++) {
    int c;
    c = visitantes;
    usleep(10);                 // usleep() == sleep() pero para microsegundos
    visitantes = c + 1;
  }
  return NULL;
}

int main() {
  pthread_t t1, t2;

  pthread_create(&t1, NULL, proc, (void *) (long) 0);
  pthread_create(&t2, NULL, proc, (void *) (long) 1);

  pthread_join(t1, NULL);
  pthread_join(t2, NULL);

  printf("Visitantes finales: %d\n", visitantes);
  return 0;
}
