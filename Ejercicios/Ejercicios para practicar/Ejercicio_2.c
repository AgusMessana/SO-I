// Crea un programa donde el proceso padre cree un pipe y luego haga un fork().
// El padre debe escribir una cadena de texto en el pipe (por ejemplo: "Hola, hijo").
// El hijo debe leer la cadena desde el pipe y mostrarla en pantalla.
// Utiliza el close() en los extremos del pipe que no sean utilizados por cada proceso.

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define LONGI_CAD 20

int main() {
  int fd[2];
  pipe(fd);
  pid_t pid = fork();

  if (pid < 0) {
    perror("fork");
    exit(1);
  }

  if (pid == 0) {
    close(fd[1]);
    char buffer[LONGI_CAD];
    int leido = read(fd[0], buffer, sizeof(buffer) - 1);
    if (leido > 0) {
      buffer[leido] = '\0';
      printf("Hijo recibió: %s\n", buffer);
    } else {
      perror("read");
    }
    close(fd[0]);
  } else {
    close(fd[0]);
    const char *mensaje = "Hola, hijo";
    write(fd[1], mensaje, strlen(mensaje));
    close(fd[1]);
    wait(NULL);
  }

  return 0;
}
