#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void) {
  char buffer[1024];
  int fd;
  ssize_t bytes_leidos;

  fd = open("texto.txt", O_RDONLY);
  if (fd == -1) {
    perror("Error al abrir el archivo.\n");
    exit(EXIT_FAILURE);
  }

  bytes_leidos = read(fd, buffer, sizeof(buffer));
  if (bytes_leidos == -1) {
    perror("Error al leer.\n");
    exit(EXIT_FAILURE);
  }

  printf("Leímos %ld bytes:\n%.*s\n", bytes_leidos, (int) bytes_leidos,
         buffer);
  // .*s indica lo siguiente:
  // . : vamos a indicar la precisión.
  // * : indicamos que el número vendrá como argumento antes del string.
  // s : indica una cadena de caracteres (string).
  // Esto lo hacemos pues read no pone el '/0' al final.

  close(fd);

  return 0;
}
