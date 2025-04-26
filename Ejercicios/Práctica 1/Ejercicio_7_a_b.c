#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_LINE 1024
#define MAX_ARGS 64

int main() {
  char line[MAX_LINE];
  while (1) {
    printf("$ ");
    if (fgets(line, sizeof(line), stdin) == NULL) {
      break;
    }

    line[strcspn(line, "\n")] = '\0';
    char *args[MAX_ARGS];
    int i = 0;
    char *token = strtok(line, " ");
    while (token != NULL && i < MAX_ARGS - 1) {
      args[i++] = token;
      token = strtok(NULL, " ");
    }
    args[i] = NULL;
    if (args[0] == NULL) {
      continue;
    }

    pid_t pid = fork();
    if (pid == 0) {
      execvp(args[0], args);    // execvp toma el progrma a ejecutar y y los argumentos del programa
      perror("execvp");
      exit(1);
    } else if (pid > 0) {
      waitpid(pid, NULL, 0);
    } else {
      perror("fork");
    }
  }

  return 0;
}
