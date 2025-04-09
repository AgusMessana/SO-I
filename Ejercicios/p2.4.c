// j. 4. ¿Puede fallar la siguiente aserci´on (wr y rd corren en un thread cada uno)? Explique. Si puede
// fallar, arregle el programa.

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

volatile int x = 0;
volatile int y = 0; //share var for each thread :)

void * wr(void *arg) {
    x = 123;
    y = 1;
}

void * rd(void *arg) {
    while (!y);
    assert(x == 123);
    }

    int main(){
        pthread_t t0, t1;
        pthread_create(&t0, NULL, wr, NULL);  
        pthread_create(&t1, NULL, rd, NULL);
        pthread_join(t0, NULL);
        pthread_join(t1, NULL);
        assert (x || y);
        return 0;
    }


    //no falla . no sé que hacemos - chatgpt contá.