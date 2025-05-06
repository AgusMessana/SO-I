#include <stdio.h>
#include <omp.h>
#define NTHREADS 5

int main() {
#pragma omp parallel sections num_threads(NTHREADS)
  {
#pragma omp section
    {
      printf("Thread id %d (section 1)\n", omp_get_thread_num());
    }
#pragma omp section
    {
      printf("Thread id %d (section 2)\n", omp_get_thread_num());
    }
#pragma omp section
    {
      printf("Thread id %d (section 3)\n", omp_get_thread_num());
    }
#pragma omp section
    {
      printf("Thread id %d (section 4)\n", omp_get_thread_num());
    }
#pragma omp section
    {
      printf("Thread id %d (section 5)\n", omp_get_thread_num());
    }
#pragma omp section
    {
      printf("Thread id %d (section 6)\n", omp_get_thread_num());
    }
  }

  return 0;
}
