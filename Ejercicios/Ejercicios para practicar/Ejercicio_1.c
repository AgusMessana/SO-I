// Escribe un programa que cree un proceso hijo utilizando fork().
// El padre debe enviar una señal SIGUSR1 al hijo.
// El hijo debe capturar esa señal y, al recibirla, imprimir un mensaje en la consola (por ejemplo: "¡He recibido SIGUSR1!").
// Después de capturar la señal, el hijo debe enviar una señal SIGUSR2 al padre, y el padre debe capturarla e imprimir un mensaje (por ejemplo: "El hijo envió SIGUSR2").
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void sigusr1_handler(int sig) {
  printf("Hijo (PID %d): he recibido SIGUSR1.\n", getpid());
  kill(getppid(), SIGUSR2);     // Enviar SIGUSR2 al padre
}

void sigusr2_handler(int sig) {
  printf("Padre (PID %d): el hijo envío SIGUSR2.\n", getpid());
}

int main() {
  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(1);
  }

  if (pid == 0) {
    signal(SIGUSR1, sigusr1_handler);
    pause();                    // Espera a recibir SIGUSR1
  } else {
    signal(SIGUSR2, sigusr2_handler);
    sleep(1);                   // Da tiemo al hijo a instalar su handler
    kill(pid, SIGUSR1);         // Envía señal al hijo
    pause();                    // Espera a recibir SIGUSR2
    wait(NULL);                 // Espera al hijo
  }

  return 0;
}
