#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

pid_t pid_hijo;

void handler_pong(int sig) {
  printf("Proceso %d: Recibí la señal SIGUSR1\n", getpid());
  sleep(1);
  if (pid_hijo == 0) {
    printf("Hijo: devolviendo al padre...\n");
    kill(getppid(), SIGUSR1);
  } else {
    printf("Padre: devolviendo al hijo...\n");
    kill(pid_hijo, SIGUSR1);
  }
}

int main() {
  struct sigaction sa;
  sa.sa_handler = handler_pong;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = SA_RESTART;

  sigaction(SIGUSR1, &sa, NULL);
  pid_hijo = fork();
  if (pid_hijo == -1) {
    perror("Error en el fork.\n");
    exit(EXIT_FAILURE);
  } else if (pid_hijo == 0) {
    printf("Hijo (PID %d) esperando el saque del padre...\n", getpid());
    while (1) {
      pause();
    }
  } else {
    sleep(1);
    printf("Padre (PID %d) haciendo saque inical al hijo %d...\n", getpid(),
           pid_hijo);
    kill(pid_hijo, SIGUSR1);
    while (1) {
      pause();
    }
  }

  return 0;
}
