#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>

void handler(int signal) {
    printf("¡Error! No se puede dividir por cero.\n");
    exit(1);
}

int main() {
    signal(SIGFPE, handler);
    //SIGFPE es la señal del error de dividir por cero.
    int cero = 0;
    int num = 3;

    printf("Dividiremos %d entre %d.\n", num, cero);

    printf("Resultado = %d", num/cero);
    return 0;
}