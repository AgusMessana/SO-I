#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000
int A[N][N], B[N][N], C[N][N];

void mult_sec(int A[N][N], int B[N][N], int C[N][N]) {
  int i, j, k;
  double tiempo_inicio = omp_get_wtime();
  for (i = 0; i < N; i++)
    for (j = 0; j < N; j++)
      for (k = 0; k < N; k++)
        C[i][j] += A[i][k] * B[k][j];
  double tiempo_fin = omp_get_wtime();
  double tiempo_total = tiempo_fin - tiempo_inicio;
  printf("El calculo se hizo en %lf segundos con la versión secuencial.\n",
         tiempo_total);
}

void mult_par(int A[N][N], int B[N][N], int C[N][N]) {
  double tiempo_inicio = omp_get_wtime();
#pragma omp parallel for
  for (int i = 0; i < N; i++)
    for (int j = 0; j < N; j++)
      for (int k = 0; k < N; k++)
        C[i][j] += A[i][k] * B[k][j];
  double tiempo_fin = omp_get_wtime();
  double tiempo_total = tiempo_fin - tiempo_inicio;
  printf("El calculo se hizo en %lf segundos con la versión paralela.\n",
         tiempo_total);
}

int main() {
  int i, j;
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      A[i][j] = random() % 1000;
      B[i][j] = random() % 1000;
    }
  }
  mult_sec(A, B, C);
  mult_par(A, B, C);
  return 0;
}

/**
 * a) Relación aproximada de los tiempos al cambiar el tamaño
 * Teóricamente, como el algoritmo tiene tres bucles anidados que iteran N veces, la cantidad de operaciones crece a un ritmo de N^3. Al duplicar el tamaño (de 500 a 1000), el tiempo debería multiplicarse por 8 (ya que 2^3 = 8). Sin embargo, en la práctica el tiempo aumenta un poco más (casi por 10). Esto ocurre porque al procesar matrices más grandes, los datos dejan de entrar en la memoria caché ultrarrápida del procesador, generando un "cuello de botella" al tener que ir a buscar los números a la memoria RAM, que es mucho más lenta.
 * b) Mejora del rendimiento cambiando el orden de los índices
 * Sí, el rendimiento se puede mejorar drásticamente cambiando el orden original (i-j-k) por el orden (i-k-j). En lenguaje C, las matrices se almacenan en memoria fila por fila de forma contigua. Al dejar la variable j (las columnas) en el bucle más interno, nos aseguramos de que el procesador lea los datos moviéndose de forma secuencial sobre la misma fila. Esto maximiza el uso de la memoria caché y evita los costosos saltos en la memoria RAM.
 * c) Computar la multiplicación de A x B^T
 * Sí, si utilizamos el algoritmo original (i-j-k), computar A x B^T mejora muchísimo el rendimiento. Al transponer la matriz B, lo que antes eran columnas dispersas en la memoria ahora se guardan como filas continuas. De esta manera, cuando la variable k avanza rápidamente en el bucle interno leyendo B^T[j][k], el procesador encuentra todos los valores ordenados de forma consecutiva. Esto elimina los saltos de memoria que ocurrían al leer las columnas de la matriz original, logrando que el programa se ejecute a su máxima velocidad.
 */
