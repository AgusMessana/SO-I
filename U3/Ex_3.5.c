/**
 * Generar la señal SIGTSTP cuya acción por defecto es detener la ejecución. Luego, crear un handler que tome la señal, imprima un mensaje y termine.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handler(int sig) {
  printf("Capturé SIGTSTP. Termino el programa.\n");
  exit(0);
}

int main(void) {
  signal(SIGTSTP, handler);
  printf("Ejecuto la señal SIGTSTP.\n");
  int sig = raise(SIGTSTP);
  if(sig != 0) {
    printf("Error en la ejecución de raise.\n");
    exit(EXIT_FAILURE);
  }

  return 0;
}
