/**
 * 1. Registrar un handler para capturar o manejar las señales SIGINT (CTRL+C) o SIGQUIT (CTRL + \)
 * 2. Si se genera la señal SIGQUIT (utilizando el comando kill o con CTRL+\), el handler simplemente imprime el mensaje.
 * 3. Si se genera la señal SIGINT (utilizando el comando kill o con CTRL+C) por primera vez, se captura la señal mostrando un mensaje en pantalla y se restablece la acción de la señal con el comportamiento por defecto (con SIG_DFL).
 * 4. Si se genera la señal SIGINT por segunda vez, realiza la finalización del programa.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void handler_SIGQUIT(int sig) {
  printf("\nSe ejecutó SIGQUIT. Terminamos el programa.\n");
  exit(0);
}

void handler_SIGINT(int sig) {
  printf
      ("\nSe ejecutó SIGINT por primera vez. Volvemos al comportamiento por defecto.\n");
  signal(SIGINT, SIG_DFL);
}


int main(void) {
  printf("Mi PID es %d.\n", getpid());
  signal(SIGQUIT, handler_SIGQUIT);
  signal(SIGINT, handler_SIGINT);

  while (1) {
    sleep(1);
  }

  return 0;
}
