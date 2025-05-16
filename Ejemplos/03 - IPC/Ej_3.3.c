#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main() {
    printf("PID del proceso: %d\n", getpid());
    printf("Me voy a detener con SIGSTOP... Usar kill -CONT %d en otra terminal para reanudar.\n", getpid());

    raise(SIGSTOP);

    printf("¡Proceso reanudado con SIGCONT!\n");
    //SIGCONT se escribe desde otra terminal para que el proceso comienze de nuevo.
    //Es como el fg, sólo que SIGCONT va en otra terminal.
    for(int i = 0; i <=5; i++) {
        printf("%d\n", i);
        sleep(1);
    }
    printf("Proceso finalizado.\n");

    fflush(stdout);
    sleep(1);
    //Estas últimas dos lineas son para que aparezca el prompt sin apretar ctrl+c
    return 0;
}