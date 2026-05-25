#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

// Función para fusionar (merge) dos mitades ya ordenadas
void merge(int a[], int temp[], int izq, int medio, int der) {
  int i = izq;                  // Índice para la mitad izquierda
  int j = medio + 1;            // Índice para la mitad derecha
  int k = izq;                  // Índice para el arreglo temporal

  // Comparamos y copiamos el menor al temporal
  while (i <= medio && j <= der) {
    if (a[i] <= a[j]) {
      temp[k++] = a[i++];
    } else {
      temp[k++] = a[j++];
    }
  }

  // Copiamos los elementos sobrantes (si quedaron)
  while (i <= medio) {
    temp[k++] = a[i++];
  }
  while (j <= der) {
    temp[k++] = a[j++];
  }

  // Volcamos el resultado ordenado de vuelta al arreglo original
  for (i = izq; i <= der; i++) {
    a[i] = temp[i];
  }
}

// Función principal recursiva (Top-down)
void mergesort_secuencial(int a[], int temp[], int izq, int der) {
  if (izq < der) {
    int medio = izq + (der - izq) / 2;

    mergesort_secuencial(a, temp, izq, medio);  // Ordena mitad izquierda
    mergesort_secuencial(a, temp, medio + 1, der);      // Ordena mitad derecha

    merge(a, temp, izq, medio, der);    // Fusiona ambas mitades
  }
}

void mergesort_bottomup(int a[], int temp[], int n) {
  for (int tam = 1; tam < n; tam *= 2) {
#pragma omp parallel for
    for (int izq = 0; izq < n; izq += tam * 2) {
      int medio = izq + tam - 1;
      if (medio > n - 1) {
        medio = n - 1;
      }
      int der = medio + tam;
      if (der > n - 1) {
        der = n - 1;
      }
      merge(a, temp, izq, medio, der);
    }
  }
}

int main(void) {
  int n = 500000000;            // 500 millones de elementos

  printf("Pidiendo memoria (esto puede tardar unos segundos)...\n");
  int *a_secuencial = (int *) malloc(n * sizeof(int));
  int *a_paralelo = (int *) malloc(n * sizeof(int));
  int *temp = (int *) malloc(n * sizeof(int));

  // Verificamos que la computadora nos haya dado la memoria
  if (a_secuencial == NULL || a_paralelo == NULL || temp == NULL) {
    printf("Error: No hay suficiente memoria RAM.\n");
    return 1;
  }

  printf("Llenando arreglos con numeros aleatorios...\n");
  for (int i = 0; i < n; i++) {
    int num = rand() % 10000;
    a_secuencial[i] = num;
    a_paralelo[i] = num;        // Copia exacta para comparar justamente
  }

  // --- PRUEBA SECUENCIAL ---
  printf("Ejecutando version secuencial...\n");
  double inicio_sec = omp_get_wtime();

  mergesort_secuencial(a_secuencial, temp, 0, n - 1);

  double fin_sec = omp_get_wtime();
  printf("Tiempo secuencial: %f segundos\n", fin_sec - inicio_sec);

  // --- PRUEBA PARALELA ---
  printf("Ejecutando version paralela bottom-up...\n");
  double inicio_par = omp_get_wtime();

  mergesort_bottomup(a_paralelo, temp, n);

  double fin_par = omp_get_wtime();
  printf("Tiempo paralelo: %f segundos\n", fin_par - inicio_par);

  // --- LIMPIEZA ---
  free(a_secuencial);
  free(a_paralelo);
  free(temp);

  return 0;
}
