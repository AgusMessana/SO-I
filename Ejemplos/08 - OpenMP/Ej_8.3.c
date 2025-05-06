#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <pthread.h>

#include <omp.h>

int main() {
  int a[100000];
#pragma omp parallel
  {
#pragma omp for                 // ordered hace que se ejecute de manera secuencial / nowait especifica que los threads pueden seguir sin esperar al resto, es decir, cuando termina su parte del for puede seguir ejecutando codigo que viene despues
    for (int i = 0; i < 100000; i++) {
      a[i] = 2 * i;
      printf("%d\n", a[i]);
    }
  }

  return 0;
}
