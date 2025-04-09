#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if(argc != 3) {
        fprintf(stderr, "Cantidad incorrecta de argumentos.\n"); 
        //se usa fpritf para que se escriba a cualquier archivo o stream, no solo a stdout
        return 1;
    }

    const char* prog = argv[1];
    int seg = atoi(argv[2]);
    if(seg < 0) {
        fprintf(stderr, "El tiempo debe ser positivo.\n");
        return 1;
    }

    while(1) {
        pid_t pid = fork();
        if(pid < 0) {
            perror("Error en fork.\n");
            //perror imprime un mensaje de error relacionado con la última llamada al sistema que falló
            return 1;
        }

        if(pid == 0) {
            execl(prog, prog, NULL);
            perror("Error al ejecutar el binario.\n");
            exit(1);
        }
        else {
            wait(NULL);
            sleep(seg);
        }
    }
    return 0;
}