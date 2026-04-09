/**
 * Ejemplo:
 * Escribir un programa en C que toma como entrada la dirección a un binario, un tiempo en segundos, y ejecuta el comando cada los segundo especificados.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    perror("Cantidad incorrecta de argumentos.\n");
    exit(EXIT_FAILURE);
  }
  int seg = atoi(argv[2]);
  while (1) {
    pid_t pid = fork();
    if (pid < 0) {
      perror("Error en la ejecución de fork.\n");
      exit(EXIT_FAILURE);
    } else if (pid == 0) {
      printf("Soy el hijo. Ejecuto el programa pasado.\n");
      execl(argv[1], argv[1], NULL);
      perror("Error en execl");
      exit(EXIT_FAILURE);
    } else {
      printf("Soy el padre. Espero a que el hijo ejecute.\n");
      sleep(seg);
      wait(NULL);
    }
  }

  return 0;
}
