#define _POSIX_C_SOURCE 200809L
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
  struct sigaction sa;
  sa.sa_handler = sigusr1_handler;
  sigemptyset(&sa.sa_mask);
  sa.sa_flags = 0;

  if (sigaction(SIGUSR1, &sa, NULL) == -1) {
    perror("sigaction");
    exit(1);
  }

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
