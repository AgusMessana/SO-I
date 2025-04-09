#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <sys/un.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/select.h>
#include <sys/mman.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>


void funcion(char *msg){
    char *ptr;
    int c;
    setbuf(stdout,NULL); //no buff no seas molesto

    for(ptr=msg; (c=*ptr++)!=0;){
        putc(c,stdout);
    }

}

int main(){

    pid_t pid=fork();

    if(pid==0){
            funcion("yo escribi desde child\n");

    }else{
        funcion("yo escribi desde parent\n");

    }

    return 0;
}



/* si lo ejecutamos muchas veces se empiezan a corromper las cosas OMG :O [RACE CONDITION PELEA EXTREMA CON LINKIN PARK DE FONDO]*/