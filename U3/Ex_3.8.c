/**
 * El Parent debe enviarle una señal al Child y esperar a que el Child termine. El Child al recibir la señal, debe imprimir en pantalla que la señal fue recibida y terminar.
 */

#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

void handler(int sig) {
  printf("Hijo: Recibí la señal. Finalizo mi ejecución.\n");
  exit(0);
}

int main(void) {
  pid_t pid = fork();
  if (pid < 0) {
    printf("Error en la ejecución de fork.\n");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    signal(SIGCHLD, handler);
    printf("Hijo: Hola! Soy el hijo.\n");
    while (1) {
      sleep(1);                 // Espera hasta que el padre envíe la señal.
    }
  } else {
    printf
        ("Padre: Hola! Soy el padre. Envío una señal a mi hijo y espero que termine.\n");
    sleep(1);
    kill(pid, SIGCHLD);
    wait(NULL);
  }

  return 0;
}
