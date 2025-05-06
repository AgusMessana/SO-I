#include <stdio.h>
#include <omp.h>

int main() {
#pragma omp parallel
  {
    int id = omp_get_thread_num();
    int nt = omp_get_num_threads();

    printf("Thread id %d, num threads: %d\n", id, nt);
  }

  return 0;
}
