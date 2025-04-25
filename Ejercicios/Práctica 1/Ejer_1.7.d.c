#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#define MAX_LINE 1024
#define MAX_CMDS 16
#define MAX_ARGS 64

void parse_command(char *line, char **argv) {
  int i = 0;
  char *token = strtok(line, " ");
  while (token != NULL && i < MAX_ARGS - 1) {
    argv[i++] = token;
    token = strtok(NULL, " ");
  }
  argv[i] = NULL;
}

int main() {
  char line[MAX_LINE];
  while (1) {
    printf("$ ");
    if (fgets(line, sizeof(line), stdin) == NULL) {
      break;
    }
    line[strcspn(line, "\n")] = '\0';

    char *cmds[MAX_CMDS];
    int n = 0;
    char *token = strtok(line, "|");
    while (token != NULL && n < MAX_CMDS) {
      while (*token == ' ') {
        token++;
      }
      cmds[n++] = token;
      token = strtok(NULL, "|");
    }

    int pipes[MAX_CMDS - 1][2];
    for (int i = 0; i < n - 1; i++) {
      if (pipe(pipes[i]) == -1) {
        perror("pipe");
        exit(1);
      }
    }

    for (int i = 0; i < n; i++) {
      pid_t pid = fork();
      if (pid == 0) {
        if (i > 0) {
          dup2(pipes[i - 1][0], STDIN_FILENO);
        }
        if (i < n - 1) {
          dup2(pipes[i][1], STDOUT_FILENO);
        }
        for (int j = 0; j < n - 1; j++) {
          close(pipes[j][0]);
          close(pipes[j][1]);
        }

        char *argv[MAX_ARGS];
        parse_command(cmds[i], argv);
        execvp(argv[0], argv);
        perror("execvp");
        exit(1);
      }
    }
    for (int i = 0; i < n - 1; i++) {
      close(pipes[i][0]);
      close(pipes[i][1]);
    }
    for (int i = 0; i < n; i++) {
      wait(NULL);
    }
  }

  return 0;
}
