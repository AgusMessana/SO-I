#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

pid_t target_pid;
void sigusr1_handler(int sig) {
  printf("PID %d recibió SIGUSR1\n", getpid());
  sleep(1);
  kell(target_pid, SIGUSR1);
}

int main() {
  signal(SIGUSR1, sigusr1_handler);

  pid_t child_pid = fork();
  if (child_pid < 0) {
    perror("fork");
    exit(1);
  }

  if (child_pid == 0) {
    target_pid = getppid();
    printf("Hijo listo. PID = %d, padre = %d\n", getpid(), target_pid);
  } else {
    target_pid = child_pid;
    printf("Padre listo. PID = %d, hijo = %d\n", getpid(), target_pid);
    kill(child_pid, SIGUSR1);
  }

  while (1) {
    pause();
  }
  return 0;
}
