// Escriba utilizando OpenMP un algoritmo que calcule la suma de un arreglo de N = 5×10^8 doubles. Compare la performance con la implementación secuencial usando distintos números de hilos. Compare también con una versión paralela que usa un mutex para proteger la variable que lleva la suma.

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define ARRAY_SIZE 500000000

void suma_secuencial(double *arr, size_t n) {
  double sum = 0.0;

  double start = omp_get_wtime();

  for (size_t i = 0; i < n; i++) {
    sum += arr[i];
  }

  double end = omp_get_wtime();

  printf("Suma secuencial: %f\tTiempo: %.5fs\n", sum, end - start);
}

void suma_reduction(double *arr, size_t n) {
  double sum = 0.0;

  double start = omp_get_wtime();

#pragma omp parallel for reduction(+:sum)
  for (size_t i = 0; i < n; i++) {
    sum += arr[i];
  }

  double end = omp_get_wtime();

  printf("Suma con reduction: %f\tTiempo: %.5fs\n", sum, end - start);
}

void suma_critical(double *arr, size_t n) {
  double sum = 0.0;

  double start = omp_get_wtime();

#pragma omp parallel for
  for (size_t i = 0; i < n; i++) {
#pragma omp critical
    sum += arr[i];
  }

  double end = omp_get_wtime();

  printf("Suma con critical: %f\tTiempo: %.5fs\n", sum, end - start);
}

int main() {
  double *arr = malloc(sizeof(double) * ARRAY_SIZE);
  if (!arr) {
    fprintf(stderr, "Error: malloc falló\n");
    return 1;
  }

  printf("Inicializando arreglo...\n");
#pragma omp parallel for
  for (size_t i = 0; i < ARRAY_SIZE; i++) {
    arr[i] = i;
  }

  suma_secuencial(arr, ARRAY_SIZE);
  suma_reduction(arr, ARRAY_SIZE);
  suma_critical(arr, ARRAY_SIZE);

  free(arr);
  return 0;
}
