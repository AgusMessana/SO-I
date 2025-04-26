// Crea un programa que ejecute una cadena de dos comandos usando pipes. Ejemplo: ls -l | grep "archivo".
// El proceso padre debe hacer un fork() y crear un pipe.
// El padre debe crear un proceso hijo y usar execvp() para ejecutar el comando ls -l.
// El padre debe crear otro proceso hijo y usar execvp() para ejecutar el comando grep.
// Conecta el pipe entre los procesos para que la salida del primer proceso (ls) sea la entrada del segundo (grep).

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
#define MAX_ARGS 10
#define MAX_LEN 1024

int main() {
  char line[MAX_LEN];
  printf("Ingrese el comando:\n");
  fgets(line, MAX_LEN, stdin);
  line[strcspn(line, "\n")] = '\0';

  char *cmd1 = strtok(line, "|");
  char *cmd2 = strtok(NULL, "|");
  if (cmd2 == NULL) {
    printf("Error. Se debe proporcionar un comando después de '|'.\n");
    exit(1);
  }
  while (*cmd1 == ' ') {
    cmd1++;
  }
  while (*cmd2 == ' ') {
    cmd2++;
  }

  char *args1[MAX_ARGS];
  char *args2[MAX_ARGS];
  char *token = strtok(cmd1, " ");
  int i = 0;
  while (token != NULL && i < MAX_ARGS) {
    args1[i++] = token;
    token = strtok(NULL, " ");
  }
  args1[i] = NULL;

  i = 0;
  token = strtok(cmd2, " ");
  while (token != NULL && i < MAX_ARGS) {
    args2[i++] = token;
    token = strtok(NULL, " ");
  }
  args2[i] = NULL;

  int fd[2];
  pipe(fd);
  pid_t pid1 = fork();
  if (pid1 < 0) {
    perror("fork");
    exit(1);
  }
  if (pid1 == 0) {
    close(fd[0]);
    dup2(fd[1], STDOUT_FILENO);
    close(fd[1]);

    execvp(args1[0], args1);
    perror("execvp");
    exit(1);
  } else {
    pid_t pid2 = fork();
    if (pid2 < 0) {
      perror("fork");
      exit(1);
    }
    if (pid2 == 0) {
      close(fd[1]);
      dup2(fd[0], STDIN_FILENO);
      close(fd[0]);

      execvp(args2[0], args2);
      perror("execvp");
      exit(1);
    } else {
      close(fd[0]);
      close(fd[1]);
      wait(NULL);
      wait(NULL);
    }
  }

  return 0;
}
