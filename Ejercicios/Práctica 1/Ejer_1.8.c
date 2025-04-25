#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int denom = 0;

void handler(int s) { 
    printf("ouch!\n"); 
    denom = 1; 
}

int main() {
    int r;
    signal(SIGFPE, handler);
    r = 1 / denom;
    printf("r = %d\n", r);
    return 0;
}

// Al correr el programa se imprime "ouch!" infinitas veces.
// Esto pasa porque:
/* i) Se detecta la división por cero en  r = 1 / denom y se entra al handler
ii) Se imprime "ouch!" y denom = 1.
iii) Después de ejecutar el handler, el programa intenta ejecutar la misma
instrucción que causó el SIGFPE, o sea, r = 1 / denom
iv) Como en ese paso denom = 0, vuelve a entrar al handler
v) Así sucede hasta que se interrumpa el programa
*/