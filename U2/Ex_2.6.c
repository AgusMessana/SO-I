/**
 * Ejemplo:
 * Proceso que crea otro con fork, el proceso parent espera a que finalice el proceso child.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void) {
  pid_t pid = fork();
  if (pid < 0) {
    perror("Error en la ejecución de fork.\n");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    printf("Soy el hijo. Mi padre espera hasta que termine para finalizar.\n");
  } else {
    printf("Soy el padre. Espero a que mi hijo termine para finalizar.\n");
    wait(NULL);
  }

  return 0;
}
