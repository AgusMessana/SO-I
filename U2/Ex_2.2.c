#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void) {
  pid_t pid = fork();
  if (pid < 0) {
    perror("Error en la ejecución de fork.\n");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    printf("Hola! Soy el hijo! Mi PID es %d y el PID de mi padre es %d.\n",
           getpid(), getppid());
  } else {
    printf("Hola! Soy el padre! Mi PID es %d y el PID de mi hijo es %d.\n",
           getpid(), pid);
    wait(NULL);
    exit(0);
  }

  return 0;
}
