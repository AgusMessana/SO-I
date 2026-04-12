/**
 * Crear dos procesos que se comuniquen a través de un pipe. Después del fork cada proceso cierra los File Descriptors que no necesita. El Child hereda un conjunto duplicado de descriptores de archivo que se refieren a la misma canalización.
 * Luego, el Parent escribe una cadena de caracteres en el pipe, y el Child lee esta cadena de a un byte a la vez e imprime lo que leyó en la salida estándar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void) {
  int fd[2];
  int p = pipe(fd);
  if (p < 0) {
    printf("Error en la ejecución de pipe.\n");
    exit(EXIT_FAILURE);
  }
  pid_t pid = fork();
  if (pid < 0) {
    printf("Error en la ejecución de fork.\n");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    close(fd[1]);
    char buffer[1024];
    ssize_t bytes = read(fd[0], buffer, sizeof(buffer));
    write(1, buffer, bytes);
    close(fd[0]);
  } else {
    close(fd[0]);
    char mensaje[] = "Hola mundo.\n";
    write(fd[1], mensaje, sizeof(mensaje));
    close(fd[1]);
    wait(NULL);
  }

  return 0;
}
