#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

pthread_barrier_t barrier;

#define NTHREADS 20



void* barrera(void* i){
    printf("Soy el thread %d, me pongo a esperar...\n",*(int*)i);
    pthread_barrier_wait(&barrier);
}


int main(){
    pthread_t pthreads[NTHREADS];
    pthread_barrier_init(&barrier,NULL, 5);
    for(int i = 0; i < NTHREADS; i++){
        pthread_create(&pthreads[i],NULL,barrera, (void *)&i);
    }
    for(int i = 0; i < NTHREADS; i++){
        pthread_join(pthreads[i],NULL);
    }

    pthread_barrier_destroy(&barrier);

    return 0;
}