#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
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
    // Veamos si hay redirección
    char *redir = strchr(line, '>');    // Busca el carácter > en la linea
    char *filename = NULL;
    if (redir != NULL) {
      *redir = '\0';            // Cortamos la linea antes del >
      redir++;                  // Mover al nombre del archivo
      while (*redir == ' ') {
        redir++;                // Sacamos los espacios
      }
      filename = redir;
      filename[strcspn(filename, "\n")] = '\0';
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
      if (filename != NULL) {
        int fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
        if (fd < 0) {
          perror("open");
          exit(1);
        }
        dup2(fd, STDOUT_FILENO);        // Hacemos que stdout apunte a fd. STDOUT_FILENO es 1 (salida estándar)
        //Así cualquier printf() o puts() va al archivo y no a la consola.
        close(fd);
      }
      execvp(args[0], args);
      perror("execvp");
      exit(1);
    } else if (pid > 0) {
      waitpid(pid, NULL, 0);
    } else {
      perror("fotk");
    }
  }

  return 0;
}
