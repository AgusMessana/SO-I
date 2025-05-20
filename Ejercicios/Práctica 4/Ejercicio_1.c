// Para calentar motores, adapte a OpenMP su solución del jardín ornamental usando el Algoritmo de la Panadería de Lamport.

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <unistd.h>
#define ITERACIONES 20
#define MAX_THREADS 128

volatile int contador = 0;
volatile int choosing[MAX_THREADS] = { 0 };
volatile int number[MAX_THREADS] = { 0 };

int max_number(int n) {
  int max = 0;
  for (int i = 0; i < n; i++) {
    if (number[i] > max) {
      max = number[i];
    }
  }

  return max;
}

void molinete(int id, int N) {
  for (int i = 0; i < ITERACIONES; i++) {
    choosing[id] = 1;
    number[id] = 1 + max_number(N);
    choosing[id] = 0;

    for (int j = 0; j < N; j++) {
      if (j == id) {
        continue;
      }

      while (choosing[j]);
      while (number[j] != 0 && (number[j] < number[id] || (number[j] == number[id] && j < id)));
    }

    contador++;
    printf("[%d] Hilo %d entra a la sección crítica\n", contador, id);
    usleep(10000);
    number[id] = 0;
  }

  return;
}

int main() {
  int N;
#pragma omp parallel
  {
#pragma omp single
    N = omp_get_num_threads();
  }

#pragma omp parallel
  {
    int id = omp_get_thread_num();
    molinete(id, N);
  }

  printf("Valor final del contador: %d\n", contador);
  return 0;
}
