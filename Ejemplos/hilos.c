//procesos livianos - threads - hilos

// solo se duplica el stack - cada uno tiene su propio stack , luego todo es =
// memoria compartida (CHAU PIPES n SOCKETS n fork ; te re cabio gil)


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
#include <pthread.h> //POSIX.

//pthread crea un subproce3so:hun hilo , que correra una f. que le pasemos  (si lo sacamos no se convierte en zombie jsjsjs)
//!gcc -pthread  o no anda loco
//pthread_join similar a wait c:'()

void* function(void* v) {
    printf("thread v: %d\n", *(int*) v);

    pthread_exit(0);
    // return NULL;
}


int main(){ 

    pthread_t id;
    int v = 2;

    printf("Main running\n - creating thread! \n");

    pthread_create(&id, NULL, function, &v);

    // sleep(1);
    int* ptr;  //quiero ver el estado del exit de function

    pthread_join(id,(void**)&ptr)                                                                           ; //puntero del void puntero ¡? chargpt explain.
    //que hago digo digo digo (what am I doing, I say I say I say)

    //si solo ll quiero esperar y n omanejar salida:     pthread_join(id); //puntero del void puntero ¡? chargpt explain.

    printf("valor salida: %p \n\n",ptr);

    printf("Main running... chau\n");

    return 0;
} //cancelar - no vamos a trabajar con esto.