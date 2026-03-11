#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <math.h>
#include <sys/types.h>

int denom = 0;

void handler() {
  printf("Outch.\n");
  denom = 1;
}

int main(void) {
  int r;
  signal(SIGFPE, handler);
  r = 1 / denom;
  printf("r %d\n", r);
  return 0;
}
