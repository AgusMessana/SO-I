// Completa el siguiente código para implementar un servidor que usa epoll para manejar múltiples conexiones en modo no bloqueante.

#include <sys/epoll.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#define MAX_EVENTS 10
#define SOCKET_PATH "/mnt/d/SO-I/Ejercicios/Ejercicios para practicar/mysocket"

int main() {
  int epoll_fd = epoll_create1(0);
  if (epoll_fd == -1) {
    perror("epoll_create1");
    return 1;
  }
  // Completa el código aquí para agregar un socket (o descriptor de archivo)
  // a epoll y manejar los eventos.
  int sock_fd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    perror("socket");
    return 1;
  }

  struct sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

  if (bind(sock_fd, (struct sockaddr *) &addr, sizeof(addr)) == -1) {
    perror("bind");
    return 1;
  }

  if (listen(sock_fd, 5) == -1) {
    perror("listen");
    return 1;
  }

  struct epoll_event ev;
  ev.events = EPOLLIN | EPOLLONESHOT;
  ev.data.fd = sock_fd;

  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sock_fd, &ev) == -1) {
    perror("epoll_ctl");
    return 1;
  }

  struct epoll_event events[MAX_EVENTS];
  while (1) {
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      perror("epoll_wait");
      return 1;
    }

    for (int i = 0; i < nfds; i++) {
      if (events[i].events & EPOLLIN) {
        // Si el socket está listo para leer, aceptamos la conexión
        int client_fd = accept(sock_fd, NULL, NULL);
        if (client_fd == -1) {
          perror("accept");
          continue;
        }
        // Aquí manejaríamos la lectura del cliente
        char buffer[128];
        ssize_t n = read(client_fd, buffer, sizeof(buffer));
        if (n > 0) {
          buffer[n] = '\0';
          printf("Datos recibidos: %s\n", buffer);
        }
        // Cerrar el socket del cliente
        close(client_fd);

        // Re-registrar el socket en epoll si usamos EPOLLONESHOT
        ev.events = EPOLLIN | EPOLLONESHOT;     // Lo registramos de nuevo
        ev.data.fd = sock_fd;
        if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, sock_fd, &ev) == -1) {
          perror("epoll_ctl mod");
          return 1;
        }
      }
    }
  }

  close(epoll_fd);
  close(sock_fd);
  return 0;
}
