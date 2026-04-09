#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main(void) {
  pid_t pid1 = fork();
  pid_t pid2 = fork();
  printf("pid1: %d pid2: %d\n", pid1, pid2);
  
  return 0;
}

/*
P (padre original)
|-> H2 (hijo de P. Segunda ejecución de fork())
|
|-> H1 (hijo de P. Primera ejecución de fork())
    |-> H3 (hijo de H1. Segunda ejecución de fork())

Cuando imprimen, se ve lo siguiente:
Proceso     pid1          pid2
P           PID H1        PID H2
H1          0             PID H3
H2          PID H1        0       (H2 hereda la memoria de P, por eso ve pid1)
H3          0             0       (H3 hereda la memoria de H1, por eso pid1 = 0)
*/