#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

int denom = 0;
void handler(int s) {
  printf("ouch!\n");
  denom = 1;
}
int main() {
  int r;
  signal(SIGFPE, handler);
  r = 1 / denom;
  printf("r = %d\n", r);
  return 0;
}

/**
 * Al ejecutar el programa, este entra en un bucle infinito, imprimiendo el mensaje "ouch!" en la pantalla de forma ininterrumpida hasta que el proceso es finalizado manualmente.
 * A diferencia de las señales enviadas por software, cuando el sistema operativo entrega una señal causada por una excepción de hardware, el comportamiento tras ejecutar el handler es intentar re-ejecutar la misma instrucción que causó la falla.
 * Aunque el handler modifica la variable global denom asignándole el valor 1, la CPU vuelve exactamente al punto donde ocurrió el error (la línea r = 1 / denom;). 
 * Debido a cómo se maneja el estado de los registros y el flujo de ejecución en una excepción de hardware, el programa re-intenta la división fallida.
 */