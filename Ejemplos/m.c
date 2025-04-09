// 🔍 Nota importante: los hijos comparten el pipe, así que quien lea primero lo consume (no es como una copia del mensaje para cada uno).

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    int fd[2];
    pipe(fd);

    char mensaje[] = "Hola hijos!";
    pid_t pid1, pid2;

    pid1 = fork();
    if (pid1 == 0) {
        // Primer hijo
        close(fd[1]);
        char buffer[100];
        int n = read(fd[0], buffer, sizeof(buffer));
        if (n > 0) {
            printf("[Hijo 1] Recibió: %s\n", buffer);
        }
        close(fd[0]);
        exit(0);
    }

    pid2 = fork();
    if (pid2 == 0) {
        // Segundo hijo
        close(fd[1]);
        char buffer[100];
        int n = read(fd[0], buffer, sizeof(buffer));
        if (n > 0) {
            printf("[Hijo 2] Recibió: %s\n", buffer);
        }
        close(fd[0]);
        exit(0);
    }

    // Padre
    close(fd[0]);
    write(fd[1], mensaje, strlen(mensaje) + 1); // escribir incluyendo '\0'
    close(fd[1]);

    wait(NULL);
    wait(NULL);
    printf("[Padre] Terminaron los hijos.\n");
    return 0;
}