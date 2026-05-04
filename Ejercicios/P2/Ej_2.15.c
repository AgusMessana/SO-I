#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>

// --- ESTRUCTURA DE DATOS ---
struct Nodo {
  char clave[256];
  char valor[1024];
  struct Nodo *siguiente;
};

struct Nodo *cabeza_lista = NULL;
pthread_mutex_t candado_lista = PTHREAD_MUTEX_INITIALIZER;

// --- FUNCIÓN DEL HILO CLIENTE ---
void *atender_cliente(void *arg) {
  int socket_cliente = *(int *) arg;
  free(arg);                    // Liberamos el puntero que nos pasó el main

  char buffer[2048];
  char comando[16], clave[256], valor[1024];

  // Bucle para mantener la conexión viva y procesar varios pedidos
  while (1) {
    int bytes_leidos = recv(socket_cliente, buffer, sizeof(buffer) - 1, 0);
    if (bytes_leidos <= 0) {
      break;                    // El cliente se desconectó o hubo un error
    }
    buffer[bytes_leidos] = '\0';        // Aseguramos que sea un string válido

    // Parseamos el comando. 
    // %15s lee hasta el primer espacio.
    // %255s lee hasta el segundo espacio.
    // %1023[^\n\r] lee todo lo que queda hasta el salto de línea.
    int campos_leidos =
        sscanf(buffer, "%15s %255s %1023[^\n\r]", comando, clave, valor);

    if (campos_leidos >= 1) {
      if (strcmp(comando, "PUT") == 0 && campos_leidos >= 3) {
        // LÓGICA PUT [cite: 267]
        pthread_mutex_lock(&candado_lista);
        struct Nodo *actual = cabeza_lista;
        int encontrado = 0;

        // Buscamos si ya existe para pisarlo
        while (actual != NULL) {
          if (strcmp(actual->clave, clave) == 0) {
            strcpy(actual->valor, valor);
            encontrado = 1;
            break;
          }
          actual = actual->siguiente;
        }

        // Si no existía, creamos un nodo nuevo al principio de la lista
        if (!encontrado) {
          struct Nodo *nuevo = malloc(sizeof(struct Nodo));
          strcpy(nuevo->clave, clave);
          strcpy(nuevo->valor, valor);
          nuevo->siguiente = cabeza_lista;
          cabeza_lista = nuevo;
        }
        pthread_mutex_unlock(&candado_lista);

        send(socket_cliente, "OK\n", 3, 0);

      } else if (strcmp(comando, "GET") == 0 && campos_leidos >= 2) {
        // LÓGICA GET [cite: 270]
        pthread_mutex_lock(&candado_lista);
        struct Nodo *actual = cabeza_lista;
        int encontrado = 0;
        char respuesta[1050];

        while (actual != NULL) {
          if (strcmp(actual->clave, clave) == 0) {
            // Armamos la respuesta "OK valor\n"
            snprintf(respuesta, sizeof(respuesta), "OK %s\n", actual->valor);
            send(socket_cliente, respuesta, strlen(respuesta), 0);
            encontrado = 1;
            break;
          }
          actual = actual->siguiente;
        }
        pthread_mutex_unlock(&candado_lista);

        if (!encontrado) {
          send(socket_cliente, "NOTFOUND\n", 9, 0);
        }

      } else if (strcmp(comando, "DEL") == 0 && campos_leidos >= 2) {
        pthread_mutex_lock(&candado_lista);
        struct Nodo *actual = cabeza_lista;
        struct Nodo *anterior = NULL;

        while (actual != NULL) {
          if (strcmp(actual->clave, clave) == 0) {
            if (anterior == NULL) {
              cabeza_lista = actual->siguiente; // Borramos el primero
            } else {
              anterior->siguiente = actual->siguiente;  // Borramos en el medio/final
            }
            free(actual);
            break;
          }
          anterior = actual;
          actual = actual->siguiente;
        }
        pthread_mutex_unlock(&candado_lista);

        send(socket_cliente, "OK\n", 3, 0);

      } else {
        // CUALQUIER OTRA COSA (o comandos incompletos) [cite: 272]
        send(socket_cliente, "EINVAL\n", 7, 0);
      }
    }
  }

  close(socket_cliente);
  return NULL;
}

// --- MAIN (SERVIDOR TCP) ---
int main(void) {
  int socket_servidor = socket(AF_INET, SOCK_STREAM, 0);
  if (socket_servidor < 0) {
    perror("Error creando el socket");
    exit(EXIT_FAILURE);
  }
  // Evita el error "Address already in use"
  int opt = 1;
  setsockopt(socket_servidor, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in direccion_servidor;
  direccion_servidor.sin_family = AF_INET;
  direccion_servidor.sin_addr.s_addr = INADDR_ANY;
  direccion_servidor.sin_port = htons(3942);    // 

  if (bind
      (socket_servidor, (struct sockaddr *) &direccion_servidor,
       sizeof(direccion_servidor)) < 0) {
    perror("Error en bind");
    exit(EXIT_FAILURE);
  }

  if (listen(socket_servidor, 10) < 0) {
    perror("Error en listen");
    exit(EXIT_FAILURE);
  }

  printf("Servidor Mini Memcached corriendo en el puerto 3942...\n");

  while (1) {
    struct sockaddr_in direccion_cliente;
    socklen_t tamano_cliente = sizeof(direccion_cliente);

    int socket_cliente =
        accept(socket_servidor, (struct sockaddr *) &direccion_cliente,
               &tamano_cliente);
    if (socket_cliente < 0) {
      perror("Error en accept");
      continue;
    }

    int *socket_hilo = malloc(sizeof(int));
    *socket_hilo = socket_cliente;

    pthread_t hilo_cliente;
    pthread_create(&hilo_cliente, NULL, atender_cliente, socket_hilo);
    pthread_detach(hilo_cliente);       // Para que libere memoria al terminar sin hacer join
  }

  close(socket_servidor);
  return 0;
}
