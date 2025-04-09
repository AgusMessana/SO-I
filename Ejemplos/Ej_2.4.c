#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

void handler(int signal) {
    printf("Recibí SIGTSTP. Finalizando...\n");
    exit(1);
}

int main() {
    signal(SIGTSTP, handler);
    
    printf("PID del proceso: %d\n", getpid());
    printf("Esperando SIGTSTP. Probar desde otra terminal con kill -TSTP %d.\n", getpid());

    while(1) {
        pause();
    }
    //Este while es para que el programa no termine y espere una señal. En este caso SIGTSTP
    
    return 0;
}