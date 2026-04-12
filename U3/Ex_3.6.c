/**
 * Primero registrar la señal SIGTSTP para ignorarla a través de SIG_IGN, y luego generar la señal SIGTSTP. Cuando se genere la señal SIGTSTP, está será ignorada.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int main(void) {
  signal(SIGTSTP, SIG_IGN);
  printf("Vamos a ejecutar la señal SIGTSTP. Debe ser ignorada.\n");
  int sig = raise(SIGTSTP);
  if (sig != 0) {
    printf("Error en la ejecución de raise.\n");
    exit(EXIT_FAILURE);
  }
  printf("Se ignoró correctamente la señal SIGTSTP.\n");

  return 0;
}
