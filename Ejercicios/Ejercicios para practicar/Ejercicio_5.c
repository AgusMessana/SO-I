// Escribe un programa donde el padre cree un hijo con fork().
// El hijo debe ejecutar una tarea (por ejemplo, imprimir un mensaje) y luego usar pause() para esperar una señal del padre.
// El padre debe enviar una señal SIGUSR1 al hijo después de un segundo.
// Al recibir la señal, el hijo debe imprimir otro mensaje.
// El padre debe esperar a que el hijo termine usando wait().

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>

void sigusr1_handler(int sig) {
  printf("Hijo (PID %d): recibí SIGUSR1.\n", getpid());
}

int main() {
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(1);
  }
  if (pid == 0) {
    printf("Hola, soy el hijo. Mi pid es %d.\n", getpid());
    signal(SIGUSR1, sigusr1_handler);
    pause();
    exit(0);
  } else {
    sleep(1);
    kill(pid, SIGUSR1);
    wait(NULL);
  }

  return 0;
}
