#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
  int sv[2];
  int mi_socket = socketpair(AF_UNIX, SOCK_STREAM, 0, sv);
  if (mi_socket < 0) {
    perror("Error en la ejecución de socketpair.\n");
  }

  char *mensaje_padre = "Padre: mensaje para enviar por socket a mi hijo.\n";
  char *mensaje_hijo = "Hijo: mensaje para enviar por socket a mi padre.\n";
  char buffer[1024];
  pid_t pid = fork();
  if (pid < 0) {
    perror("Error en la ejecución de fork.\n");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    close(sv[0]);
    write(sv[1], mensaje_hijo, strlen(mensaje_hijo));
    int bytes_leidos_hijo = read(sv[1], buffer, 1024);
    buffer[bytes_leidos_hijo] = '\0';
    printf("Hola! Soy el hijo! Recibí: %s\n", buffer);
    close(sv[1]);
  } else {
    close(sv[1]);
    int bytes_leidos_padre = read(sv[0], buffer, 1024);
    buffer[bytes_leidos_padre] = '\0';
    write(sv[0], mensaje_padre, strlen(mensaje_padre));
    printf("Hola! Soy el padre! Recibí: %s\n", buffer);
    close(sv[0]);
    wait(NULL);
  }

  return 0;
}
