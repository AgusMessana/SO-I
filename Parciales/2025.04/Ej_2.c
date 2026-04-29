#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void) {
  int i;
  int status;
  int fd[2];
  pipe(fd);
  for (i = 0; i < 3; i++) {
    pid_t pid = fork();
    if (pid == 0) {
      close(fd[0]);
      char buff[5] = "HOLA\n";
      int k = 0;
      k = write(fd[1], &buff, 5);
      if (k > 0)
        printf("Soy el hijo %d y salude a mi padre %d\n", getpid(), getppid());
    } else {
      close(fd[1]);
      char buff2[5];
      int r;
      r = read(fd[0], buff2, 5);
      if (r > 0)
        printf("Soy el padre %d, mi hijo %d me dijo %s", getpid(), pid, buff2);
      wait(&status);
    }
  }

  return 0;
}

/**
 * a) El programa debería crear 3 hijos, donde cada uno saluda a su padre, y el padre dice que sus hijos lo saludaron. Se crean 8 procesos. 1 padre original y 7 hijos. Esto porque al no haber un exit() al final de la ejecución del hijo, este también entra al for y ejecuta fork(), creando más hijos.
 * b) No todos los hijos saludaron a su padre ni todos los padrs fueron saludados. Esto porque en la primera ejecución se cierran los extremos de lectura y escritura. Al heredar los hijos los extremos cerrados, ya no pueden ni leer ni escribir.
 * c) Al agregar el exit(0) en la línea 18 se soluciona el problema de la creación de los hijos de más. Se crea un padre y 3 hijos.
  La salida sería:
  Soy el hijo 9261 y salude a mi padre 9260
  Soy el padre 9260, mi hijo 9261 me dijo HOLA
  No se soluciona el problema de las impresiones porque los hijos siguen heredando los extremos cerrados ya que el pipe se crea fuera del for.
 */
