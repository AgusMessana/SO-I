#include <sys/epoll.h>
#include <sys/resource.h>
#include <netinet/in.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define PORT 8080
#define MAX_EVENTS 10

// Definimos instancia global para que todos los procesos la vean.
int epfd;
int listen_sd;

void *funcion_thread(void *arg) {
  int epoll_fd = *(int *) arg;
  struct epoll_event events[MAX_EVENTS];

  while (1) {
    // Esperar un evento
    int n_events = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    for (int i = 0; i < n_events; i++) {
      // Ejecutar el handler de pedido del cliente
      int event_fd = events[i].data.fd;

      // Opción A: Es un evento en el socket principal (Alguien nuevo llama)
      // Hay que hacer un accept() para atenderlo.

      // Opción B: Es un evento en un socket de un cliente viejo (Nos mandan datos)
      // Hay que hacer read() o write().
    }
  }

  return NULL;
}

int main(int argc, char **argv) {
  int nproc = sysconf(_SC_NPROCESSORS_ONLN);
  pthread_t th[nproc];

  // Crear socket de escucha
  int server_fd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in server_addr;
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr));
  listen(server_fd, SOMAXCONN);

  // Crear una instancia GLOBAL de epoll
  int epoll_fd = epoll_create1(0);

  // Agregar el socket a la instancia de epoll
  struct epoll_event event;
  event.events = EPOLLIN;
  event.data.fd = server_fd;
  epoll_ctl(epoll_fd, EPOLL_CTL_ADD, server_fd, &event);

  // Crear threads
  for (int i = 0; i < nproc; i++) {
    pthread_create(&th[i], NULL, funcion_thread, &epoll_fd);
  }

  for (int i = 0; i < nproc; i++) {
    pthread_join(th[i], NULL);
  }

  close(server_fd);
  close(epoll_fd);
  return 0;
}
