/**
 * Compartir memoria entre un padre y un hijo.
 */

#define _POSIX_C_SOURCE 200809L // Para que ftruncate esté disponible.
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(void) {
  // Creamos el objeto de memoria compartida.
  int fd = shm_open("/mi_memoria", O_CREAT | O_RDWR, 0666);
  if (fd < 0) {
    perror("Error en la ejecución de shm_open.\n");
    exit(EXIT_FAILURE);
  }

  // Definimos el tamaño de lo que queremos compartir.
  int tam = ftruncate(fd, sizeof(int)); // sizeof(int) porque queremos compartir un entero. Puede ser otro
  if (tam < 0) {
    perror("Error en la ejecución de ftruncate.\n");
    exit(EXIT_FAILURE);
  }

  // Mapeamos la memoria
  int *ptr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  /**
   * NULL → el SO elige la dirección
   * sizeof(int) → tamaño a mapear
   * PROT_READ | PROT_WRITE → permisos de lectura y escritura
   * MAP_SHARED → los cambios son visibles para otros procesos
   * fd → el file descriptor del objeto de memoria compartida
   * 0 → offset, empezamos desde el principio
   */
  if (ptr == MAP_FAILED) {
    perror("Error en la ejecución de mmap.\n");
    exit(EXIT_FAILURE);
  }

  pid_t pid = fork();
  if (pid < 0) {
    printf("Error en la ejecución de fork.\n");
    exit(EXIT_FAILURE);
  } else if (pid == 0) {
    sleep(1);                   // Para que el padre llegue a escribir antes de que el hijo lea.
    printf("Hola! Soy el hijo. El número que escribió el padre es %d.\n",
           *ptr);
  } else {
    printf("Hola! Soy el padre. Escribo un número en ptr.\n");
    *ptr = 45;
    wait(NULL);
    munmap(ptr, sizeof(int));   // Desmapeamos la memoria.
    shm_unlink("/mi_memoria");  // Eliminamos el objeto de memoria compartida.
  }

  return 0;
}
