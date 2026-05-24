#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

void funcion_1(long long n) {
  if (n == 0 || n == 1) {
    printf("El número %lld no es primo.\n", n);
    return;
  }
  int es_primo = 1;

  double tiempo_inicio = omp_get_wtime();

#pragma omp parallel for
  for (long long i = 2; i <= (long long) sqrt(n); i++) {
    if (es_primo == 1 && n % i == 0) {
      es_primo = 0;
    }
  }

  double tiempo_fin = omp_get_wtime();
  double tiempo_total = tiempo_fin - tiempo_inicio;

  if (es_primo == 1) {
    printf("El número %lld es primo.\n", n);
  } else {
    printf("El número %lld no es primo.\n", n);
  }
  printf("Se comprobó en %lf segundos con la versión paralela.\n",
         tiempo_total);
}

void funcion_2(long long n) {
  if (n == 0 || n == 1) {
    printf("El número %lld no es primo.\n", n);
    return;
  }
  int es_primo = 1;

  double tiempo_inicio = omp_get_wtime();

  for (long long i = 2; i <= (long long) sqrt(n); i++) {
    if (n % i == 0) {
      es_primo = 0;
      break;
    }
  }

  double tiempo_fin = omp_get_wtime();
  double tiempo_total = tiempo_fin - tiempo_inicio;

  if (es_primo == 1) {
    printf("El número %lld es primo.\n", n);
  } else {
    printf("El número %lld no es primo.\n", n);
  }
  printf("Se comprobó en %lf segundos con la versión secuencial.\n",
         tiempo_total);
}

int main(void) {
  long long num;
  printf("Ingrese un entero no negativo.\n");
  scanf("%lld", &num);
  funcion_1(num);
  funcion_2(num);

  return 0;
}
