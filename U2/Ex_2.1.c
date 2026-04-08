#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>


int main(void) {
  pid_t pid;
  pid = fork();
  if (pid == -1) {
    exit(EXIT_FAILURE);
  }
  if (pid == 0) {
    printf("Hola! Soy el hijo!\n");
    execl("hola_mundo.out", "hola_mundo.out", NULL);
    exit(0);
  } else {
    printf("El hijo tiene pid %d.\n", pid);
    wait(0);
  }

  return 0;
}
