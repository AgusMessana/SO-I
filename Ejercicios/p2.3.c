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


int main(){
    pthread_t t0, t1;
    pthread_create(&t0, NULL, foo, NULL);  
    pthread_create(&t1, NULL, bar, NULL);
    pthread_join(t0, NULL);
    pthread_join(t1, NULL);
    assert (a || b);
    return 0;
}

//no falla :)