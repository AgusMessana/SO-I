#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  printf("Hola mundo.\n");
  printf("El pid del programa hola_mundo.c es %d.\n", getpid());
  
  return 0;
}
