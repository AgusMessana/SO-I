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
    printf("Me voy a detener con SIGSTOP...\n");

    raise(SIGSTOP);

    printf("¡Proceso reanudado!\n");
    return 0;
}

//El proceso se deteniene con SIGSTOP pero no se elimina.
//Al escribir "fg" el prceso se reanuda.