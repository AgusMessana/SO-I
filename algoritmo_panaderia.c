//lampor

#include <arpa/inet.h>
#include <assert.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>  //POSIX. incluir en gcc -pthread
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

/*
el algoritmo de peterson falla porque asuimos consistenci secuencial , cada CPU tiene un cache  y en un algun momento difieren - no estan sincro entonces no son visibles al otro procesador

puede suceder que checkeo la cond del while, antes de que se asigne la misma pra q sea True

--barreras de memoria(ayuda a evitar lo anterior) mfence

-O3:lo + rapido
*/

//-----------------------------------------------


//pthread_mutex  lock n unlock bloquea por hardwre: NO HAY BUSY WAIT
//un mismo mutex me sirve para cda molino :) ES GLOBAL
// spin: bloqueo x bree periodo de untiempo




//------------------


// indent-rainbow,ainbow brackets,rainbow tags

#define visitantes_moli 40

pthread_mutex_t mutex_v = PTHREAD_MUTEX_INITIALIZER;

int contador = 0;

void* molinete(void* proceso) {
    for (int x = 0; x < visitantes_moli / 2; ++x) {

        // LOCK
        pthread_mutex_lock(&mutex_v);
        
        contador++;  // -> region critica.
        printf("[%d] m1 - Ha ingresado otro más (%p) \n", contador, proceso);

        // UNLOCK
        pthread_mutex_unlock(&mutex_v);

        
    }

    // return NULL;
}

//SI PONGO LAS COSAS AFUERAS - LO SECUENCIALIZO (lock n unlock afuera del for)
void* molinete2(void* proceso) {
    for (int x = 0; x < visitantes_moli / 2; ++x) {
        // LOCK
        pthread_mutex_lock(&mutex_v);

        contador++;  // -> region critica.
        printf("[%d] m2 - Ha ingresado otro más (%p) \n", contador, proceso);

        // UNLOCK
        pthread_mutex_unlock(&mutex_v);

        
    }

    // return NULL;
}

int main() {
    pthread_t moli_1, moli_2;

    pthread_create(&moli_1, NULL, molinete, NULL);
    pthread_create(&moli_2, NULL, molinete2, NULL);

    pthread_join(moli_1, NULL);
    pthread_join(moli_2, NULL);

    pthread_mutex_destroy(&mutex_v);

    printf("[VISITANTES TOTAL] %d \t", contador);


    return 0;
}

//region critric->>>>> lo mas small posibbleeeeeeee
