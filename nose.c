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
#include <sys/epoll.h>

/*  1 cpu
    solución race-condition:    software: peterson y lamport
                                api->mutex

    n cpu
    peterson NO sirve.
    mutex es ideal ;) [PTHREAD SAFE]

    Utilizaremos directamente epoll ;)

    kernel queue



*/

// creemos instancioa d epoll en el kernel

int main(){
    int fde= epoll_create1(0);

    struct epoll_event{
        uint32_t events; //moniteramo epollin
        epoll_data_t data;
    };

    struct epoll_data_t{
        union epoll_data{
            void *ptr;
            int fd;
            uint32_t u32;
            uint64_t u64;
        };
    };

}

//epoll_ctp  -> agrega cosas al epoll que monitoriamos , la cola del kernel q nos delvovlio poll_crate como 1er argumento . como 2do arg, debemos especificar la op que queremos hacer.
//fd1=cola, fd2=queremos monitorear, dir de struct
//como queremos seguir con las conexiones, le pasamos ev.events para esperar eventso (creo q es ev.events)
// el programa lo queh ace (incompleto) es acpetar la conexion y agregar fd a la cola de los eventos
// si era una conesion aceptada anteiormente handler ((si la conexion no es de escucha))
// en vez del child, creamos otra poll diferente

//oinstacia poll - creamos polls - c/u monitorean a cada cosa de la estructura. lo llenamos con las conexiones que tengamos
// como es share , we need to be careful w/ race condition?
//no  es proceso safe , es thread safe ya que el epoll pued edespertar al hijo aún si NO lo quiero usar.

//bandera EPOLLONESHOT (!) -> elimina de la cola al fd.

//epoll_ctl(...., EPOLL_CTL_MOVE) -> lo ponemos en la Qeue de nuevo ;'P pa q lo sacaste mai friend

//EPOLLET -> apenas hay un cambio , me notifcara
//EPOLLEVEL -> works when nivel cambia. Se despierta solo cuando hay un cambio (DEFECTO. NO EXISTE)

//+ info : epoll_Ctl


// problema q solucuoina onesht: si tengo mas de 1 tarea, + de 1 hilo monitoriando un fd - -- cuando viene un evento (algo paraleer) el poll elige alguno d elasd 3 y lo despierte para avisarle , luego llegan mas datos de la conexioines. si no la saque de la lista de interes, podir adespertar a otro hilo para eso

//epoll : debo usarlo con multiples threads. ya q soluciona la parte de que se bloque al esperar conexiones

//poll wait, las q necito, son var locales de cada thread

///PSD CDE:

/*

int fde;
*   int main(){
        fde=epoll_create1(0);
        epoll_ctl(fde,EPOLL_CTL_ADD;lsock,&ev);
        for(i,i<;i++){
             pthread_Create(trread[i],NULL,w_f_client,&fde);
        }        
        
        //w_F_Client es una f:wait for client , recibe como arguemtno el fd de epoll.
        
        
        pthread_join(thrad[i])
        
        
        }
*
*
*/