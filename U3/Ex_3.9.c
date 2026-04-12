/**
 * El Parent debe enviarle una señal al Child y esperar a que el Child termine. El Child al recibir la señal, debe imprimir en pantalla que la señal fue recibida y terminar. Se debe usar sigaction().
 */

#define _POSIX_C_SOURCE 200809L // Para que sigaction esté disponible.
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
    struct sigaction sa;
    sa.sa_handler = handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(SIGCHLD, &sa, NULL);

    printf("Hijo: Hola! Soy el hijo.\n");
    while (1) {
      sleep(1);
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
