// Escriba utilizando OpenMP una función que verifique si un entero es primo (buscando divisores entre 2 y √N). Su solución debería andar igual o más rápido que una versión secuencial que “corta” apenas encuentra un divisor. Escriba su función para tomar un long, i.e. un entero de 64 bits, y asegúrese de probarla con números grandes (incluyendo primos, semiprimos, y pares).

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int primo_secuencial(long int n) {
  if (n < 2) {
    return 0;
  }
  if (n == 2 || n == 3) {
    return 1;
  }
  if (n % 2 == 0) {
    return 0;
  }

  int raiz = (int) sqrt(n);
  for (int i = 3; i <= raiz; i += 2) {
    if (n % i == 0) {
      return 0;
    }
  }

  return 1;
}

int primo_paralelo(long int n) {
  if (n < 2) {
    return 0;
  }
  if (n == 2 || n == 3) {
    return 1;
  }
  if (n % 2 == 0) {
    return 0;
  }

  int raiz = (int) sqrt(n);
  volatile int es_primo = 1;

#pragma omp parallel for
  for (int i = 3; i <= raiz; i += 2) {
    if (!es_primo) {
      continue;
    }
    if (n % i == 0) {
      es_primo = 0;
    }
  }

  return es_primo;
}

int main() {
  long int numeros[] = { 17, 18, 19, 20, 999999937, 999999939 };
  int count = sizeof(numeros) / sizeof(numeros[0]);

  for (int i = 0; i < count; i++) {
    double start_seq = omp_get_wtime();
    int es_primo_seq = primo_secuencial(numeros[i]);
    double end_seq = omp_get_wtime();

    double start_par = omp_get_wtime();
    int es_primo_par = primo_paralelo(numeros[i]);
    double end_par = omp_get_wtime();

    printf("Número %ld:\n", numeros[i]);
    printf("  Secuencial: %s, Tiempo: %.6f s\n",
           es_primo_seq ? "primo" : "no primo", end_seq - start_seq);
    printf("  Paralelo:   %s, Tiempo: %.6f s\n\n",
           es_primo_par ? "primo" : "no primo", end_par - start_par);
  }

  return 0;
}
