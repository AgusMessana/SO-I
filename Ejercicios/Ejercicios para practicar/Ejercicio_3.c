// Crea un programa que ejecute un comando en el hijo utilizando exec(). Por ejemplo, puedes usar el comando ls o cat.
// El padre debe hacer un fork() para crear un proceso hijo.
// En el proceso hijo, usa execvp() para ejecutar un comando como ls -l.
// El padre debe esperar a que el hijo termine usando wait().

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#define MAX_ARGS 10
#define MAX_LEN 100

int main() {
  char line[MAX_LEN];
  char *args[MAX_ARGS];
  int i = 0;

  printf("Ingresa el comando.\n");
  fgets(line, MAX_LEN, stdin);
  line[strcspn(line, "\n")] = '\0';

  char *token = strtok(line, " ");
  while (token != NULL && i < MAX_ARGS - 1) {
    args[i++] = token;
    token = strtok(NULL, " ");
  }
  args[i] = NULL;

  pid_t pid = fork();
  if (pid < 0) {
    perror("fork");
    exit(1);
  }
  if (pid == 0) {
    execvp(args[0], args);
    perror("execvp");
    exit(1);
  } else {
    wait(NULL);
    printf("El hijo termino.\n");
  }

  return 0;
}
