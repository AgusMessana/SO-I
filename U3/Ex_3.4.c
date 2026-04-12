/**
 * Mejorar el programa del ejemplo 3 para continuar con la ejecución del proceso detenido emitiendo SIGCONT desde otra terminal con el comando:
 kill -CONT <PID>
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler(int sig) {
  printf("Received signal SIGCONT.\n");
}

int main(void) {
  printf
      ("Voy a ejecutar la señal SIGSTOP y luego voy a reanudarme. Lo haré a través del comando kill -CONT desde otra terminal.\nMi PID es %d.\n",
       getpid());
  signal(SIGCONT, handler);
  int sig = raise(SIGSTOP);
  if (sig != 0) {
    printf("Error en la ejecución de raise.\n");
    exit(EXIT_FAILURE);
  }

  printf("Me reanudé. Termino.\n");

  return 0;
}
