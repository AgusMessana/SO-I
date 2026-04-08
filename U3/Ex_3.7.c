#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

void handler() {
  printf("Recibí la señal.\n");
}

int main(void) {
  signal(SIGUSR1, handler);
  pid_t pid = fork();
  if (pid == -1) {
    perror("Error en el fork.\n");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    printf("Hijo esperando...\n");
    pause();
    exit(0);
  } else {
    sleep(1);
    printf("Padre...\n");
    kill(pid, SIGUSR1);
    wait(0);
    printf("El hijo a terminado\n");
  }
  return 0;
}
