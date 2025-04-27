// Completa este programa para agregar un STDIN (fd 0) a epoll, y que imprima un mensaje cada vez que haya algo para leer del teclado.

#include <sys/epoll.h>
#include <unistd.h>
#include <stdio.h>

#define MAX_EVENTS 5

int main() {
  int epoll_fd = epoll_create1(0);
  if (epoll_fd == -1) {
    perror("epoll_create1");
    return 1;
  }

  struct epoll_event ev, events[MAX_EVENTS];

  // 1. Configurar 'ev' para que escuche EPOLLIN en STDIN (fd = 0)
  ev.events = EPOLLIN;
  ev.data.fd = 0;

  // 2. Agregar STDIN al epoll (epoll_ctl)
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, 0, &ev) == -1) {
    perror("epoll_ctl");
    return 1;
  }

  while (1) {
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (nfds == -1) {
      perror("epoll_wait");
      return 1;
    }

    for (int i = 0; i < nfds; i++) {
      if (events[i].events & EPOLLIN) {
        printf("¡Algo escrito en stdin!\n");
        char buf[128];
        read(0, buf, sizeof(buf));      // Leer para limpiar
      }
    }
  }
}
