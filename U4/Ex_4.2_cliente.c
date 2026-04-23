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
    perror("Error en la ejecución de socket.\n");
    exit(EXIT_FAILURE);
  }
  struct sockaddr_un direccion_cliente;
  direccion_cliente.sun_family = AF_UNIX;
  strcpy(direccion_cliente.sun_path, "/tmp/socket_cliente");
  unlink("/tmp/socket_cliente");
  int resultado_bind = bind(mi_socket, (struct sockaddr *) &direccion_cliente,
                            sizeof(direccion_cliente));
  if (resultado_bind < 0) {
    perror("Error en la ejecución de bind.\n");
    exit(EXIT_FAILURE);
  }

  char buffer[1024];
  strcpy(buffer, "Hola servidor, soy el cliente!\n");
  struct sockaddr_un direccion_servidor;
  direccion_servidor.sun_family = AF_UNIX;
  strcpy(direccion_servidor.sun_path, "/tmp/socket_servidor");
  socklen_t tamano_direccion_servidor = sizeof(direccion_servidor);
  sendto(mi_socket, buffer, strlen(buffer), 0,
         (struct sockaddr *) &direccion_servidor, tamano_direccion_servidor);
  int bytes_recibidos = recvfrom(mi_socket, buffer, 1024, 0,
                                 (struct sockaddr *) &direccion_servidor,
                                 &tamano_direccion_servidor);
  buffer[bytes_recibidos] = '\0';
  printf("Cliente: recibí el siguiente mensaje del servidor: %s", buffer);

  return 0;
}
