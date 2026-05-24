#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_HILOS 100           // Cantidad de hilos (puede aumentarse)
#define ITERACIONES 20          // Cantidad de veces que cada hilo entra a la región crítica

volatile int contador = 0;
volatile int choosing[MAX_HILOS] = { 0 };       // indica si un hilo está eligiendo número
volatile int number[MAX_HILOS] = { 0 }; // número asignado a cada hilo (su "ticket")

int main(void) {
  int id;
#pragma omp parallel shared(choosing, number, contador) private (id)
  {
    id = omp_get_thread_num();
    int num_hilos = omp_get_num_threads();
    for (int iter = 0; iter < ITERACIONES; iter++) {
      // Paso 1: sacar número
      choosing[id] = 1;

      // Calculamos el máximo ticket actual
      int max = 0;
      for (int i = 0; i < num_hilos; i++) {
        if (number[i] > max) {
          max = number[i];
        }
      }
      number[id] = 1 + max;
      choosing[id] = 0;

      // Paso 2: esperar turno
      for (int j = 0; j < num_hilos; j++) {
        if (j == id) {
          continue;
        }
        while (choosing[j]);    // Esperar a que el otro hilo elija su numero.
        while (number[j] != 0 && (number[j] < number[id] || (number[j] == number[id] && j < id)));      // Esperar turno
      }

      // Paso 3: región crítica
      contador++;
      printf("[%d] Hilo %d entra a la sección crítica.\n", contador, id);
      usleep(10000);

      // Paso 4: salir
      number[id] = 0;
    }
  }

  printf("Valor final del contador: %d.\n", contador);

  return 0;
}
