#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void) {
  char buffer[100];
  while (1) {
    printf("Mi shell >");
    fgets(buffer, 100, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    // strcspn : Busca en un string la primera aparición de cualquiera de los caracteres que se le pase y devuelve el índice donde encontró ese carácter.
    // Lo usamos porque fgets captura el enter ("\n"). Para eliminarlo, buscamos el índice del "\n" con la función y lo reemplazamos por un '\0'.

    char *args[64];
    int i = 0;
    // La primera vez usando strtok le pasamos el buffer de donde leer la cadena.
    args[i] = strtok(buffer, " ");
    // Mientras strtok siga encontrando palabras, las guardamos.
    // El último elemento del arreglo es NULL. Lo vamosa necesitar para exec.
    while (args[i] != NULL && i < 63) {
      i++;
      args[i] = strtok(NULL, " ");
    }

    if (args[0] == NULL) {
      continue;                 // Volvemos al principio porque no escribimos nada.
    }
    // Comprobamos si el usuario quiere salir.
    if (strcmp(args[0], "exit") == 0) {
      printf("Cerrando terminal.\n");
      break;
    }
    // Testeamos el troceo
    printf("Ejecutando: %s\n", args[0]);

    pid_t pid = fork();
    if (pid == -1) {
      perror("Error en el fork.\n");
      // No ponemos exit así el usuario puede intentar nuevamente.
    } else if (pid == 0) {
      if (execvp(args[0], args) == -1) {
        //execvp busca el path sin tener que pasarlo
        perror("Comando no encontrado.\n");
        exit(EXIT_FAILURE);
      }
    } else {
      wait(NULL);
    }

  }

  return 0;
}
