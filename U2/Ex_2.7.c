/**
 * Ejemplo:
 * Hacer una Shell, la misma debe poder leer comandos (con o sin argumentos ingresados por teclado y ejecutarlo. Una vez finalizado el programa que se ejecutó, debe volver al prompt y permitir el ingreso de otro programa. Se debe también proporcionar el comportamiento de poder salir de la shell.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>

int main(void) {
  char buffer[1024];
  while (1) {
    // Lee el comando que introduce el ususario.
    printf("$ ");
    fflush(stdout);             // Se asegura de que el prompt se imprima antes de esperar input.
    fgets(buffer, 1024, stdin);
    buffer[strlen(buffer) - 1] = '\0';

    // Si el comando es exit termina la ejecución.
    if (strcmp(buffer, "exit") == 0) {
      break;
    }
    // Divide el comando en argumentos.
    char *args[1024];
    int i = 0;
    args[i] = strtok(buffer, " ");
    while (args[i] != NULL) {
      i++;
      args[i] = strtok(NULL, " ");
    }

    // Si se introdujo un enter, se espera otro comando.
    if (args[0] == NULL) {
      continue;
    }
    
    // Crea al hijo que ejecutará los comandos pasados
    pid_t pid = fork();
    if (pid < 0) {
      perror("Error en la ejecución de fork.\n");
      exit(EXIT_FAILURE);
    } else if (pid == 0) {
      execvp(args[0], args);
      perror("Error en la ejecución de execvp.\n");
      exit(EXIT_FAILURE);
    } else {
      wait(NULL);
    }
  }

  return 0;
}
