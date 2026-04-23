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
    perror("Error en la ejecución de ");
  }
  struct sockaddr_un direccion_servidor;
  direccion_servidor.sun_family = AF_UNIX;
  strcpy(direccion_servidor.sun_path, "/tmp/socket_stream_servidor");
  unlink("/tmp/socket_stream_servidor");
  int resultado_bind = bind(mi_socket, (struct sockaddr *) &direccion_servidor,
                            sizeof(direccion_servidor));
  if (resultado_bind < 0) {
    perror("Error en la ejecución de bind.\n");
    exit(EXIT_FAILURE);
  }

  int resultado_listen = listen(mi_socket, 5);
  if (resultado_listen < 0) {
    perror("Error en la ejecución de listen.\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    char buffer[1024];
    struct sockaddr_un direccion_cliente;
    socklen_t tamano_direccion_cliente = sizeof(direccion_cliente);
    int socket_cliente =
        accept(mi_socket, (struct sockaddr *) &direccion_cliente,
               &tamano_direccion_cliente);
    if (socket_cliente < 0) {
      perror("Error en la ejecución de accept.\n");
      continue;
    }

    int bytes_recibidos = recv(socket_cliente, buffer, 1024, 0);
    buffer[bytes_recibidos] = '\0';
    printf("Servidor Stream: recibí el siguiente mensaje: %s\n", buffer);

    send(socket_cliente, buffer, bytes_recibidos, 0);
    close(socket_cliente);
  }

  return 0;
}
