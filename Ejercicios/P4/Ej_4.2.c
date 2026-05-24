#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 500000000

void funcion_1() {
  double suma = 0.0;
  double *a = (double *) malloc(N * sizeof(double));
  if (a == NULL) {
    printf("Error: no hay memoria suficiente.\n");
    exit(EXIT_FAILURE);
  }
  double tiempo_inicio = omp_get_wtime();
#pragma omp parallel for reduction(+: suma)
  for (int i = 0; i < N; i++) {
    a[i] = 2.0;
    suma = suma + a[i];
  }
  double tiempo_fin = omp_get_wtime();
  double tiempo_total = tiempo_fin - tiempo_inicio;
  printf("Resultado final obtenido en %f segundos con reduction: %.2f.\n",
         tiempo_total, suma);

  free(a);
}

void funcion_2() {
  double suma = 0.0;
  double *a = (double *) malloc(N * sizeof(double));
  if (a == NULL) {
    printf("Error: no hay memoria suficiente.\n");
    exit(EXIT_FAILURE);
  }
  double tiempo_inicio = omp_get_wtime();
#pragma omp parallel shared (suma)
  {
#pragma omp for
    for (int i = 0; i < N; i++) {
      a[i] = 2.0;
#pragma omp atomic
      suma = suma + a[i];
    }
  }
  double tiempo_fin = omp_get_wtime();
  double tiempo_total = tiempo_fin - tiempo_inicio;
  printf("Resultado final obtenido en %f segundos con atomic: %.2f.\n",
         tiempo_total, suma);

  free(a);
}

int main(void) {
  funcion_1();
  funcion_2();

  return 0;
}
