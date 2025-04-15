// Ej. 3. ¿Puede fallar la siguiente aserci´on? ¿Bajo qu´e condiciones? Explique. Si puede fallar, arregle el
// programa.

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
#include <assert.h>

int x = 0, y = 0, a = 0, b = 0;
pthread_mutex_t mutex_v = PTHREAD_MUTEX_INITIALIZER;

void * foo(void *arg){
    x = 1;
    a = y;
    return NULL;
}
void * bar(void *arg){
    y = 1;
    b = x;
    return NULL;
}

void* molinete(void * arg){
    //lock
    pthread_mutex_lock(&mutex_v);

    //región critica:
    
    // unlock
    pthread_mutex_unlock(&mutex_v);
}


int main(){
    pthread_t t0, t1;
    pthread_create(&t0, NULL, molinete, NULL);  
    pthread_create(&t1, NULL, molinete, NULL);
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    pthread_mutex_destroy(&mutex_v);
    assert (a || b);
    return 0;
}

// Si puede fallar, ya que se puede producir RACE CONDITION

/**
 *  Vemos que pasa:
 *  x=1;
 * a=y;
 * 
 * y=1;
 * b=x;
 *
 * entonces si pasa CC: (foo): x=1 -> a=0;
 *                      (bar): y=1->b=0
 * 
 *  y entonces assert(a||b) fallará.
 * 
 */


// Solución: usar barriers, pthread_barrier_t, mutex, o atomic para sincronizar accesos.
