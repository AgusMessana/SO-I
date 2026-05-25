#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
  int num_procs, my_id;
  int n = 10;
  int array[n];
  for (int i = 1; i <= n; i++) {
    array[i - 1] = i;
  }

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  int base = n / num_procs;
  int inicio = my_id * base;
  int elementos_locales = base;

  if (my_id == num_procs - 1) {
    int resto = n % num_procs;
    elementos_locales += resto;
  } else {
    int inicio = my_id * base;
  }

  int suma_local = 0;
  for (int i = inicio; i < inicio + elementos_locales; i++) {
    suma_local += array[i];
  }

  int suma_total = 0;
  MPI_Reduce(&suma_local, &suma_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

  if (my_id == 0) {
    printf("La suma de los valores del array da como resultado: %d.\n",
           suma_total);
  }

  MPI_Finalize();
  return 0;
}
