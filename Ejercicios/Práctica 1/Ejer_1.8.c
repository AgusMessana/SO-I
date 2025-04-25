#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

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

// Al correr el programa se genera un bucle infinito. Desglosemos paso a paso
// i) Al principio, denom = 0
// ii) Se ejecuta 1 / denom, que genera un SIGFPE
// iii) El manejador se ejecuta y poone denom = 1
// iv) La ejecución no continúa donde se cortó. La instrucción que causó el error se repite. Y como el procesador cargó denom = 0, sigue siendo división por 0. Así, arroja SIGFPE, vuelve al handler y pone denom = 1 tarde. Y así infinitamente
// Esto sucede pues las instrucciones de máquina que hacen la división se generan antes de que se evalúe el valor actualizado de denom. El CPU ya estaba usando 0 cuando se generó la excepción.
