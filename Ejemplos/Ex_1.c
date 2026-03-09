#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>


int main(void) {
  pid_t pid;
  pid = fork();
  if(pid == -1) {
    exit(EXIT_FAILURE);
  }
  if(pid == 0) {
    printf("Hola! Soy el hijo!\n");
    execl("hola.o", "hola.o", NULL);
    exit(0);
  } else {
    printf("El hijo tiene pid %d.\n", pid);
    printf("Mi pid es %d.\n", getpid());
    wait(0);
  }

  return 0;
}