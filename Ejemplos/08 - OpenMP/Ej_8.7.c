#include <stdio.h>
#include <omp.h>
#define N_VISITANTES 10000

int visitantes = 0;

void molinete(void) {
  for (int i = 0; i < N_VISITANTES; i++) {
#pragma omp critical
    {
      visitantes++;
    }
  }
}

int main() {
  int sum = 0;
  int val = 1;
  int num_threads = 2;
#pragma omp parallel num_threads(num_threads)
  {
    molinete();
  }

  printf("visitantes = %d\n", visitantes);

  return 0;
}
