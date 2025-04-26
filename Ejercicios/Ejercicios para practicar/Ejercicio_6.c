// Crea un programa donde el proceso padre ejecute el comando ls -l y redirija su salida al proceso hijo.
// El hijo debe leer la salida del comando ls -l desde un pipe y contar cuántas líneas de salida tiene.
// El hijo debe imprimir cuántas líneas ha recibido.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#define LONGI_MAX 1024

int main() {
  int fd[2];
  pipe(fd);
  char *args[] = { "ls", "-l", NULL };
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(1);
  }

  if (pid == 0) {
    close(fd[1]);
    char buffer[LONGI_MAX];
    int leido, lineas = 0;

    while ((leido = read(fd[0], buffer, sizeof(buffer))) > 0) {
      for (int i = 0; i < leido; i++) {
        if (buffer[i] == '\n') {
          lineas++;
        }
      }
    }
    close(fd[0]);
    printf("Hijo: recibí %d lineas de salida.\n", lineas);
  } else {
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);

    execvp(args[0], args);
    perror("execvp");
    exit(1);
  }

  return 0;
}
