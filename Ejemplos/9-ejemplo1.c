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


// Entrada/Salida a un archivo!
// ● lseek(x): mueve el cabezal a la posición x
// ● read(): retorna el valor que se encuentre en la posición que se encuentre el cabezal
// ● write(v): escribe el valor v en la posición que se encuentre el cabezal
// Implementar:
// ● seek_write(x,v): escribe el valor v en la posición x
// ● seek_read(x): retorna el valor que se encuentre en la posición x


char seek_read(int fd,off_t x){
    char c;
    lseek(fd,x,SEEK_SET);    //x : absoluto
    int n=read(fd,&c,1);

    if(n<0){
        perror("fallo read");
        exit(EXIT_FAILURE);
    }

    return c;
}

void seek_write(int fd,off_t x, const char c){
    lseek(fd,x,SEEK_SET);    //x : absoluto
    int n=write(fd,&c,1);

    if(n<0){
        perror("fallo write");
        exit(EXIT_FAILURE);
    }
    return;
}

//no son atomicas ni deterministas: no podemos saber quién llega primero ni cuando termina (cuando habra cambio de contexto).

int main(){

    char path[]="/home/alumno/Descargas/p/ORG/basura.txt";

    int pos=3;

    int fd=open(path,O_RDWR);

    pid_t pid=fork();


    if(pid==0){
        char c=seek_read(fd,pos);
        printf("leimos: %c en child \n",c);

        seek_write(fd,pos,'+');

    }else{

        char c=seek_read(fd,pos);
        printf("leimos: %c en parent:\n",c);

        seek_write(fd,pos,'_');

    }

    close(fd);

    return 0;
}




/* lo q pasa lseek pos 13 y luego CAMBIO DE CONTEXTO (procesos - termina el time del proceso1 , comienza proceso2) entonces se ejecuta el cambio entre lseek y read , el child se para en pos 13 tmb , el child sigue hasta read y entonces como read lee y mueve el cabezal -> ahora estamos en pos 14. Luego, CAMBIO CONTEXTO -  seguira el maldito padre y leera la pos 14*/

//cambio d contexto: how to stop - lo q trata so D: !!