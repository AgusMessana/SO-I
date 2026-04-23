#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>

int main(void) {
  int mi_socket = socket(AF_UNIX, SOCK_DGRAM, 0);
  if (mi_socket < 0) {
    perror("Error en la ejecución de ");
  }
  struct sockaddr_un direccion_servidor;
  direccion_servidor.sun_family = AF_UNIX;
  strcpy(direccion_servidor.sun_path, "/tmp/socket_servidor");
  unlink("/tmp/socket_servidor");
  int resultado_bind = bind(mi_socket, (struct sockaddr *) &direccion_servidor,
                            sizeof(direccion_servidor));
  if (resultado_bind < 0) {
    perror("Error en la ejecución de bind.\n");
    exit(EXIT_FAILURE);
  }

  while (1) {
    char buffer[1024];
    struct sockaddr_un direccion_cliente;
    socklen_t tamano_direccion_cliente = sizeof(direccion_cliente);
    int bytes_recibidos = recvfrom(mi_socket, buffer, 1024, 0,
                                   (struct sockaddr *) &direccion_cliente,
                                   &tamano_direccion_cliente);
    buffer[bytes_recibidos] = '\0';
    printf("Servidor: recibí el siguiente mensaje del cliente: %s\n", buffer);
    sendto(mi_socket, buffer, bytes_recibidos, 0,
           (struct sockaddr *) &direccion_cliente, tamano_direccion_cliente);
  }

  return 0;
}
