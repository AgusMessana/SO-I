// Completa este programa para que el proceso hijo envíe su PID al padre mediante un pipe, y el padre lo imprima.

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main() {
  int pipefd[2];
  pid_t pid;

  pipe(pipefd);

  pid = fork();

  if (pid == 0) {
    // Hijo
    close(pipefd[0]);
    // Completar: mandar su propio PID al padre
    pid_t mi_pid = getpid();
    if (write(pipefd[1], &mi_pid, sizeof(mi_pid)) == -1) {
      perror("write");
      return 1;
    }
    close(pipefd[1]);

  } else {
    // Padre
    close(pipefd[1]);
    pid_t pid_hijo;
    // Completar: recibir el PID del hijo y mostrarlo
    if (read(pipefd[0], &pid_hijo, sizeof(pid_hijo)) == -1) {
      perror("read");
      return 1;
    }
    printf("Soy el padre (PID: %d). PID del hijo: %d\n", getpid(), pid_hijo);

    close(pipefd[0]);
  }

  return 0;
}
