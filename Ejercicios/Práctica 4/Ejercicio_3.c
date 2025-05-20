// Escriba utilizando OpenMP un algoritmo que dado un arreglo de N = 5×10^8 enteros busque el mínimo. Compare la performance con la implementación secuencial con distinto número de hilos.

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#define N 500000000

int minimo_secuencial(int *arr, size_t n) {
  int minimo = arr[0];
  for (int i = 1; i < n; i++) {
    if (arr[i] < minimo) {
      minimo = arr[i];
    }
  }

  return minimo;
}

int minimo_reduction(int *arr, size_t n) {
  int minimo = arr[0];
#pragma omp parallel for reduction(min : minimo)
  for (size_t i = 1; i < n; i++) {
    if (arr[i] < minimo) {
      minimo = arr[i];
    }
  }

  return minimo;
}

int main() {
  int *arr = malloc(sizeof(int) * N);
#pragma omp parallel for
  for (size_t i = 0; i < N; i++) {
    arr[i] = rand();
  }

  double t1 = omp_get_wtime();
  int min_sec = minimo_secuencial(arr, N);
  double t2 = omp_get_wtime();
  printf("Min (secuencial): %d\tTiempo: %.4fs\n", min_sec, t2 - t1);

  double t3 = omp_get_wtime();
  int min_par = minimo_reduction(arr, N);
  double t4 = omp_get_wtime();
  printf("Min (paralelo): %d\tTiempo: %.4fs\n", min_par, t4 - t3);
  
  free(arr);
  return 0;
}
