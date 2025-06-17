#include <stdio.h>
#include <mpi.h>
#include <stdlib.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);
    
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    
    // Valor inicial de cada proceso (podría ser cualquier valor, aquí usamos el rank)
    int my_value = rank;
    int total_sum = my_value; // Inicializamos la suma con el valor propio
    
    // Buffer para recibir valores
    int received_value;
    
    // Identificamos los vecinos en el anillo
    int next = (rank + 1) % size;
    int prev = (rank - 1 + size) % size;
    
    // Enviamos nuestro valor al siguiente proceso
    MPI_Send(&my_value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    
    // Realizamos N-1 iteraciones para recibir todos los valores
    for (int i = 0; i < size - 1; i++) {
        MPI_Recv(&received_value, 1, MPI_INT, prev, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        
        // Acumulamos el valor recibido
        total_sum += received_value;
        
        // Reenviamos el valor al siguiente proceso
        MPI_Send(&received_value, 1, MPI_INT, next, 0, MPI_COMM_WORLD);
    }
    
    // Todos los procesos ahora tienen la suma total
    printf("Proceso %d: La suma total es %d\n", rank, total_sum);
    
    MPI_Finalize();
    return 0;
}