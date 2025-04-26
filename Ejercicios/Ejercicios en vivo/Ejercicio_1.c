/*
Escriba un programa con las siguientes caracteristicas: 

Lea un número N por teclado y luego imprima de forma aleatoria los números desde el 1 a N en grupos de 3.

El programa debe usar al menos dos procesos y algún mecanismo de comunicación entre ellos para lograr la tarea.
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <sys/un.h>
#include <time.h>

int *mezclar(int n) {

  int *lista = malloc(sizeof(int) * n);

  for (int x = 0; x < n; x++) {
    lista[x] = x + 1;
  }

  for (int x = 0; x < n; x++) {
    int i = rand() % n;         //nro random hasta n
    int temporal = lista[x];
    lista[x] = lista[i];
    lista[i] = temporal;
  }

  return lista;
}


int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Uso: %s N,\n", argv[0]);
    exit(1);
  }

  srand(time(NULL));
  int n = atoi(argv[1]);

  int fd[2];
  pipe(fd);
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(1);
  }
  if (pid == 0) {
    // Hijo: lee grupos de 3 y escribe
    close(fd[1]);
    int buffer[3];
    int leidos;
    while ((leidos = read(fd[0], buffer, sizeof(buffer))) > 0) {
      int cantidad = leidos / sizeof(int);
      for (int i = 0; i < cantidad; i++) {
        printf("%d ", buffer[i]);
      }
      printf("\n");
      sleep(1);
    }
    close(fd[0]);
  } else {
    // Padre: genera y envía la lista
    close(fd[0]);
    int *lista = mezclar(n);
    int i = 0;
    while (i < n) {
      int buffer[3];
      int j;
      for (j = 0; j < 3 && i < n; j++, i++) {
        buffer[j] = lista[i];
      }
      write(fd[1], buffer, j * sizeof(int));
    }
    close(fd[1]);
    wait(NULL);
    free(lista);
  }

  return 0;
}
