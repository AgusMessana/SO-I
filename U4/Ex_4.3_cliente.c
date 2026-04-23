#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>

int main(void) {
  int mi_socket = socket(AF_UNIX, SOCK_STREAM, 0);
  if (mi_socket < 0) {
    perror("Error en la ejecución de socket.\n");
    exit(EXIT_FAILURE);
  }

  char buffer[1024];
  strcpy(buffer, "Hola servidor, soy el cliente!\n");
  struct sockaddr_un direccion_servidor;
  direccion_servidor.sun_family = AF_UNIX;
  strcpy(direccion_servidor.sun_path, "/tmp/socket_stream_servidor");
  socklen_t tamano_direccion_servidor = sizeof(direccion_servidor);

  int resultado_connect =
      connect(mi_socket, (struct sockaddr *) &direccion_servidor,
              sizeof(direccion_servidor));
  if (resultado_connect < 0) {
    perror("Error en la ejecución de connect.\n");
    exit(EXIT_FAILURE);
  }

  send(mi_socket, buffer, strlen(buffer), 0);
  int bytes_recibidos = recv(mi_socket, buffer, 1024, 0);
  buffer[bytes_recibidos] = '\0';
  printf("Cliente: recibí el siguiente mensaje del servidor: %s", buffer);

  close(mi_socket);
  return 0;
}
