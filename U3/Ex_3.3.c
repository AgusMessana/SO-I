/**
 * Utilizar la función raise() para generar una señal SIGSTOP. Luego, reanudar el proceso detenido con el comando fg.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(void) {
  printf("Voy a ejecutar la señal SIGSTOP y luego voy a reanudarme.\n");
  int sig = raise(SIGSTOP);
  if (sig != 0) {
    printf("Error en la ejecución de raise.\n");
    exit(EXIT_FAILURE);
  }

  printf("Me reanudé. Termino.\n");

  return 0;
}
