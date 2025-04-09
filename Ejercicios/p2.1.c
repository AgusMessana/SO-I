// Pr´actica 2
// Multiprocesador y Exclusi´on M´utua
// Nota: Si desea limitar los procesadores en los que puede correr un proceso, puede usar el comando
// taskset.

// Ej. 1. Suponga que queremos detectar si un array A contiene el entero 42 iterando por el mismo, y si es
// as´ı, prender la bandera encontrado. ¿Hay diferencia entre los dos fragmentos siguientes? ¿En qu´e casos?

// if (A[i] == 42)
// encontrado = true;



//encontrado = encontrado || (A[i] == 42);


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
#include <pthread.h>

int found(int ar[], int len){
    int encontrado=0; //no
    for (int x = 0; x<len; x++)
    {
        if(ar[x]==42){
            encontrado=1;
        }
    }
    return encontrado;
}

int main(){

    int array[]={1,3,5,7,42};
    int array2[]={1,3,5,7,43};

    int encontrado=found(array,5);

    if(encontrado){
     
        printf("ENCONTRAMOS AL 42 en a1");
    }

    int encontrado2=found(array2,5);

    if(encontrado2){    
        printf("ENCONTRAMOS AL 42 en a2");
    }

    return 0;

}
