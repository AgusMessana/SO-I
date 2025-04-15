#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>  //POSIX. incluir en gcc -pthread / -lpthread (w/MACROS)
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/select.h>
#include <sys/shm.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t mutex_v1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_v2 = PTHREAD_MUTEX_INITIALIZER;

void* funcion(void *arg){
    pthread_mutex_lock(&mutex_v1);
    printf("T1 - M1\n");
    sleep(1);
    printf("T1 - esperando M2\n");

    if(pthread_mutex_trylock(&mutex_v2) < 0){
        pthread_mutex_unlock(&mutex_v1);
    }


    // -- region critica --

    printf("T1 \n");

    pthread_mutex_unlock(&mutex_v1);
    pthread_mutex_unlock(&mutex_v2);
}

void* funcion2(void* arg) {
    pthread_mutex_lock(&mutex_v2);
    printf("T2 - M2\n");
    sleep(1);
    printf("T2 - esperando M1\n");
    pthread_mutex_lock(&mutex_v1);

    // -- region critica --

    printf("T2 \n");

    pthread_mutex_unlock(&mutex_v2);
    pthread_mutex_unlock(&mutex_v1);
}

int main(){
    pthread_t hilo1,hilo2;

    pthread_create(&hilo1,NULL,funcion,NULL);
    pthread_create(&hilo2, NULL, funcion2, NULL);

    pthread_join(hilo1, NULL);
    pthread_join(hilo2, NULL);

    return 0;
}

//se arregla con trylock