/**
 * Hacer un programa que haga una división por 0.
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int num = 1, den = 0, res;
  res = num / den;
  printf("El resultado es %d.\n", res);

  return 0;
}
