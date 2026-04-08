#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
  pid_t pid1 = getpid();
  printf("El pid del programa antes de ejecutar execl es %d.\n", pid1);
  execl("/mnt/i/SO-I/U2/hola_mundo.out", "/mnt/i/SO-I/U2/hola_mundo.out", NULL);

  return 0;
}
