#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
  pid_t pid;
  int t = atoi(argv[2]);
  while(1) {
    pid = fork();
    if(pid == 0) {
      printf("Soy el hijo.\n");
      execl(argv[1], argv[1], NULL);
    } else {
      printf("Soy el padre.\n");
      wait(NULL);
      sleep(t);
    }
  }
  
  return 0;
}
