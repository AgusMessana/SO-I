#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

#define N 8

int main(int argc, char **argv) {
  int my_id, num_procs;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  int filas_locales = N / num_procs;
  int A[N][N];
  int v[N];
  int x[N];

  // Variables locales de cada proceso.
  int A_local[filas_locales][N];
  int x_local[filas_locales];

  // El proceso 0 inicializa la amtriz y el vector.
  if (my_id == 0) {
    for (int i = 0; i < N; i++) {
      v[i] = 1;                 // Llenamos con 1's para probar.
      for (int j = 0; j < N; j++) {
        A[i][j] = i + j;        // Datos de prueba.
      }
    }
  }

  // El proceso 0 copia el vector 'v' entero a todos los procesos.
  MPI_Bcast(v, N, MPI_INT, 0, MPI_COMM_WORLD);

  // El proceso 0 reparte la matriz 'A' a cada procso. Cada uno recibe 'filas_locales * N' números y los guarda en su A_local.
  MPI_Scatter(A, filas_locales * N, MPI_INT, A_local, filas_locales * N,
              MPI_INT, 0, MPI_COMM_WORLD);

  // Realizamos los cálculos
  for (int i = 0; i < filas_locales; i++) {
    x_local[i] = 0;
    for (int j = 0; j < N; j++) {
      x_local[i] += A_local[i][j] * v[j];
    }
  }

  // El proceso 0 recolecta los 'x_local' de todos y los pega formando el x final.
  MPI_Gather(x_local, filas_locales, MPI_INT, x, filas_locales, MPI_INT, 0,
             MPI_COMM_WORLD);

  // El proceso 0 imprime el resultado.
  if (my_id == 0) {
    printf("Resultado del vector x:\n");
    for (int i = 0; i < N; i++) {
      printf("%d ", x[i]);
    }
    printf("\n");
  }

  MPI_Finalize();
  return 0;
}
