#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>


int main(void) {
  int fd[2];
  pid_t pid;
  pipe(fd);
  pid = fork();
  if (pid == -1) {
    perror("Error en el fork.\n");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    printf("Soy el hijo\n");
    close(fd[1]);
    char c;
    while (read(fd[0], &c, 1) > 0) {
      write(1, &c, 1);
    }
    close(fd[0]);
  } else {
    close(fd[0]);
    char *msg = "Hola mundo.\n";
    write(fd[1], msg, strlen(msg));
    printf("Soy el padre\n");
    close(fd[1]);
  }

  return 0;
}
