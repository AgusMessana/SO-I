// Completa el siguiente código para que un servidor TCP reciba una conexión entrante y le envíe un mensaje de bienvenida.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

int main() {
  int server_fd, new_socket;
  struct sockaddr_in address;
  socklen_t addrlen = sizeof(address);

  // Crea el socket
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  // Configura la dirección
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);

  // 1. Asignar dirección al socket (bind)
  if (bind(server_fd, (struct sockaddr *) &address, addrlen) == -1) {
    perror("bind");
    exit(EXIT_FAILURE);
  }
  // 2. Escuchar conexiones entrantes (listen)
  if (listen(server_fd, 5) == -1) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
  // 3. Aceptar una conexión (accept)
  new_socket = accept(server_fd, (struct sockaddr *) &address, &addrlen);
  if (new_socket == -1) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
  // 4. Enviar un mensaje al cliente
  char *mensaje = "Bienvenido.\n";
  write(new_socket, mensaje, strlen(mensaje));

  // 5. Cerrar el socket
  close(new_socket);
  close(server_fd);

  return 0;
}
