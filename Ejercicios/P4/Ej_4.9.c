#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char **argv) {
  int my_id, num_procs;

  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  int origen, destino;
  destino = (my_id + 1) % num_procs;
  if (my_id == 0) {
    origen = num_procs - 1;
  } else {
    origen = my_id - 1;
  }

  int suma_total = my_id;
  int a_enviar = my_id;
  int recibido;
  for (int i = 0; i < num_procs - 1; i++) {
    MPI_Send(&a_enviar, 1, MPI_INT, destino, 0, MPI_COMM_WORLD);
    MPI_Recv(&recibido, 1, MPI_INT, origen, 0, MPI_COMM_WORLD,
             MPI_STATUS_IGNORE);
    suma_total += recibido;
    a_enviar = recibido;
  }

  printf("[Proceso %d]: la suma total de los id's es %d.\n", my_id, suma_total);

  MPI_Finalize();
  return 0;
}
