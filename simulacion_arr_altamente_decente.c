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
int flag[2] = {0, 0}; // 0 -> sin intencion de entrar a RC, 1 -> con intencion
//int turno = 0;  // 0: te toca ir.

void* molinete(void* proceso) {
    for (int x = 0; x < visitantes_moli / 2; ++x) {
        // int otro = 1 - proceso;

        // LOCK
        flag[0] = 1; //molinete 1 intención de entrar a RC
        while (flag[1] == 1) {  // prioridad=otro :)
            ;                   // PAUSA.
        }

        contador++;  // -> region critica.

        // UNLOCK
        flag[0] = 0;  // ya terminé

        printf("[%d] Ha ingresado otro más (%p) \n", contador, proceso);
    }

    // return NULL;
}

void* molinete2(void* proceso) {
    for (int x = 0; x < visitantes_moli / 2; ++x) {
        // int otro = 1 - proceso;

        // LOCK
        flag[1] = 1;
        while (flag[0] == 1) {  // prioridad=otro :)
            ;                 // PAUSA.
        }

        contador++;  // -> region critica.

        // UNLOCK
        flag[1] = 1;

        printf("[%d] Ha ingresado otro más (%p) \n", contador, proceso);
    }
}

int main() {
    pthread_t moli_1, moli_2;

    pthread_create(&moli_1, NULL, molinete, NULL);
    pthread_create(&moli_2, NULL, molinete2, NULL);

    pthread_join(moli_1, NULL);
    pthread_join(moli_2, NULL);

    printf("[VISITANTES TOTAL] %d \t", contador);

    return 0;
}


//deadlock  proceso bloqueado sin poder avanzar