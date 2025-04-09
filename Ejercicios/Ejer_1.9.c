#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void INThandler(int);

int main(void) {
    signal(SIGINT, INThandler);
    while (1) {
        sleep(10);
    }
}

void INThandler(int sig) {
    char c;
    signal(SIGINT, SIG_IGN);
    //Ignoramos la SIGINT para que al estar dentro del handler no podamos
    //presionar Ctrl+c y entrar a otro handler.

    printf("\nOUCH, did you hit Ctrl-C?\n"
    "Do you really want to quit? [y/n] ");
    c = getchar();
    if (c == 'y' || c == 'Y') {
        exit(0);
    }
    else {
        signal(SIGINT, INThandler);
        //Como antes ignoramos los Ctrl+c, debemos activar de nuevo el
        //handler. Sino, no funcionarán más los Ctrl+c
    }
    getchar();
}