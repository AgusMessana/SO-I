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

// indent-rainbow,ainbow brackets,rainbow tags

#define visitantes_moli 40

int contador = 0;
int turno = 0;  // 0: te toca ir.

void* molinete(void* proceso) {
    for (int x = 0; x < visitantes_moli / 2; x++) {
       

        //int otro = 1 - proceso;

        // LOCK
        turno=2;
        while (turno==2) {  // prioridad=otro :)
            ;                           // PAUSA.
        }


        contador++;  // -> region critica.

        // UNLOCK
        turno=2; //me re-asigno el turno

        printf("[%d] Ha ingresado otro más (%p) \n", contador, proceso);
    }

    // return NULL;
}

void* molinete2(void* proceso) {
    for (int x = 0; x < visitantes_moli / 2; x++) {
        // int otro = 1 - proceso;

        // LOCK
        turno = 1;
        while (turno == 1) {  // prioridad=otro :)
            ;                 // PAUSA.
        }

        contador++;  // -> region critica.

        // UNLOCK
        turno=1;

        printf("[%d] Ha ingresado otro más (%p) \n", contador, proceso);
    }
}

    int main() {
        pthread_t moli_1, moli_2;

        pthread_create(&moli_1, NULL, molinete, NULL);
        pthread_create(&moli_2, NULL, molinete2, NULL);

        pthread_join(moli_1, NULL);
        pthread_join(moli_2, NULL);

        printf("[VISITANTES TOTAL] %d", contador);

        return 0;
    }

    /* suponete que lo vimos,

        lee la linea nro  30 , lee la var contador (0) y [CC] lee (c=0), +1 , c=1 [CC] le intenta sumar 1 + guardarlo (contador=1) LO PISA! entonces ni se entero el thread1 del thread2 [RACE CONDITION]


        ASIGNACION (r=1) NO ES ATÓMICA
    */

/*

    el programa no termina pq el thread1 termina pero el thread2 se quedo bloqueado, esperando turno :(  - con linea nro 66, se arregla OK

    PERO NO SE ARRELGA BIEN DEL TODO
    el último, presentará problema - hará rebote (37-40-39)
    PING-PONG , estamos secuenciando  para q una persona pase, debe pasar otra !??
*/