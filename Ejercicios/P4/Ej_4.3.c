#include <float.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 10
void funcion_1() {
  double min = DBL_MAX;
  double *a = (double *) malloc(N * sizeof(double));
  if (a == NULL) {
    printf("Error: no hay memoria suficiente.\n");
    exit(EXIT_FAILURE);
  }
  double tiempo_inicio = omp_get_wtime();
#pragma omp parallel shared(min)
  {
#pragma omp for
    for (int i = 0; i < N; i++) {
#pragma omp critical
      {
        if (a[i] < min) {
          min = a[i];
        }
      }
    }
  }
  double tiempo_fin = omp_get_wtime();
  double tiempo_total = tiempo_fin - tiempo_inicio;

  printf("El valor más chico obtenido con critical es %lf en %lf segundos.\n",
         min, tiempo_total);

  free(a);
}

void funcion_2() {
  double minimo = DBL_MAX;
  double *a = (double *) malloc(N * sizeof(double));
  if (a == NULL) {
    printf("Error: no hay memoria suficiente.\n");
    exit(EXIT_FAILURE);
  }
  double tiempo_inicio = omp_get_wtime();
#pragma omp parallel for reduction(min : minimo)
  for (int i = 0; i < N; i++) {
    if (a[i] < minimo) {
      minimo = a[i];
    }
  }
  double tiempo_fin = omp_get_wtime();
  double tiempo_total = tiempo_fin - tiempo_inicio;

  printf("El valor más chico obtenido con reduction es %lf en %lf segundos.\n",
         minimo, tiempo_total);

  free(a);
}

int main(void) {
  funcion_1();
  funcion_2();

  return 0;
}
