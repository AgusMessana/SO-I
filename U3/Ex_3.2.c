/**
 * Modificar el código del ejemplo 1 para manejar esa señal en particular usando la llamada al sistema signal().
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void handler(int sig) {
  printf("No se puede dividir por 0.\n");
  exit(EXIT_FAILURE);           // Sin el exit se repite el mensaje infinitamente.
}

int main(void) {
  int num = 1, den = 0, res;
  signal(SIGFPE, handler);
  res = num / den;
  printf("El resultado es %d.\n", res);

  return 0;
}
