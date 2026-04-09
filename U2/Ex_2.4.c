#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(void) {
  char buffer[1024];
  int fd = open("/mnt/i/SO-I/U2/texto.txt", O_RDONLY);
  if (fd < 0) {
    perror("Error en la ejecución de open.\n");
    exit(EXIT_FAILURE);
  }
  ssize_t bytes_leidos = read(fd, buffer, 1024);
  if (bytes_leidos < 0) {
    perror("Error en la ejecución de read.\n");
    exit(EXIT_FAILURE);
  }
  write(1, buffer, bytes_leidos);
  printf("\n");
  close(fd);
  return 0;
}
