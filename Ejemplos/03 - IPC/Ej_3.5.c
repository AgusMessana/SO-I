#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

int main() {
    signal(SIGTSTP, SIG_IGN);
    //Esta línea ignora la señal SIGTSTP mediante el comando SIG_IGN

    printf("PID del proceso: %d\n", getpid());
    printf("Ignorando SIGTSTP... Probar apretar Ctrl+Z.\n");

    raise(SIGTSTP);

    printf("Sigo ignorando SIGTSTP.\n");
    sleep(3);

    printf("Finalizando...\n");
    return 0;
}