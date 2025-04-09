#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    if(pid < 0) {
        perror("Error en el fork.\n");
        return 1;
    }

    if(pid == 0) {
        printf("Hola, soy el hijo. Mi PID es: %d. Estoy trabajando...\n", getpid());
        sleep(2);
        printf("Terminé.\n");
        exit(0);
    }
    else {
        printf("Hola, soy el padre. Mi PID es %d. Espero a mi hijo. Su PID es %d.\n", getpid(), pid);
        int status;
        waitpid(pid, &status, 0);
        //waitpid espera a que el proceso con PID igual a pid termine.
        //El resultado (el estado de salida del hijo) se guarda en status.
        //El 0 significa que se queda ahí hasta que el hijo termine.
        if (WIFEXITED(status)) {
            //WIFEXITED verifica si el hijo terminó normalmente (con exit() o return)
            printf("El hijo terminó con código %d.\n", WEXITSTATUS(status));
            //WEXITSTATUS devuelve el status con el que terminó el hijo
        }
        else {
            printf("El hijo terminó anormalmente.\n");
        }
    }
    return 0;
}