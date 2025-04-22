// Ej. 15 (Servidor de Turnos). Esta vez vamos a usar threads...
// a) Adapte su implementaci´on de la pr´actica anterior del servidor de turnos para atender concurrentemente
// a todas las conexiones abiertas levantando un thread nuevo por cada conexi´on. Nota: todos los
// clientes deber´an poder hacer pedidos sin esperar a otros, y siempre debe poder conectarse un nuevo
// cliente. Esta vez, se debe garantizar que dos pedidos nunca reciben el mismo entero.
// b) Implemente una soluci´on con select/epoll.
// c) Compare la performance de ambas soluciones

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/epoll.h>
#include <pthread.h>

#define BYTE_SIZE 4
#define BACKING_FILE "/shMemEx"
#define ACCESS_PERMS 0644
#define MAX_EVENTS 10

pthread_mutex_t mutex_v = PTHREAD_MUTEX_INITIALIZER;

int fde;

int lsock;
/*
struct epoll_event{
    uint32_t events; //moniteramo epollin
    epoll_data_t data;
} ev;

struct epoll_data_t{
    union epoll_data{
        void *ptr;
        int fd;
        uint32_t u32;
        uint64_t u64;
    } data;
};*/

/*
 * Para probar, usar netcat. Ej:
 *
 *      $ nc localhost 4040
 *      NUEVO
 *      0
 *      NUEVO
 *      1
 *      CHAU
 */

void quit(char *s)
{
	perror(s);
	abort();
}



int* memptr;
int U = 0;
int fd;

int fd_readline(int fd, char *buf)
{
	int rc;
	int i = 0;

	/*
	 * Leemos de a un caracter (no muy eficiente...) hasta
	 * completar una línea.
	 */
	while ((rc = read(fd, buf + i, 1)) > 0) {
		if (buf[i] == '\n')
			break;
		i++;
	}

	if (rc < 0)
		return rc;

	buf[i] = 0;
	return i;
}

void handle_conn(int csock)
{
	char buf[200];
	int rc;

	while (1) {
		/* Atendemos pedidos, uno por linea */
		rc = fd_readline(csock, buf);
		if (rc < 0)
			quit("read... raro");

		if (rc == 0) {
			/* linea vacia, se cerró la conexión */
			close(csock);
			return;
		}

		if (!strcmp(buf, "NUEVO")) {
			char reply[20];
			U = *memptr;
			sprintf(reply, "%d\n", U);
			U++;
			*memptr = U;
			write(csock, reply, strlen(reply));
		} else if (!strcmp(buf, "CHAU")) {
			write(csock, "Chau!, cierro la conexión\n", 26);
			close(csock);
			return;
		}
	}
}

void wait_for_clients(pthread_t lsock)
{
	int csock;
	struct epoll_event ev,  events[MAX_EVENTS];

    int nfds= epoll_wait(fde,events,MAX_EVENTS,-1);
    if(nfds==-1){
        perror("epoll_wait");
        exit(EXIT_FAILURE);
    }
   // int n=MAX_EVENTS;
    for(int n=0;n<nfds;++n){
        if(events[n].data.fd==fd){
            csock = accept(fd, NULL, NULL);
            if (csock < 0)
                quit("accept");
                exit(EXIT_FAILURE);
            }

         //   setnonblocking(csock);
            ev.events = EPOLLIN | EPOLLONESHOT;
            ev.data.fd = csock;
            if (epoll_ctl(fde, EPOLL_CTL_ADD, csock,&ev) == -1) {
                perror("epoll_ctl: conn_sock");
                exit(EXIT_FAILURE);
            }else{
               
                handle_conn(events[n].data.fd);
                ev.events = EPOLLIN | EPOLLONESHOT;
                ev.data.fd =events[n].data.fd;
                if (epoll_ctl(fde, EPOLL_CTL_MOD, events[n].data.fd,&ev) == -1) {
                    perror("epoll_ctl: conn_sock");
                    exit(EXIT_FAILURE);
            }}
        }
	

}

/* Crea un socket de escucha en puerto 4040 TCP */
int mk_lsock()
{
	struct sockaddr_in sa;
	int lsock;
	int rc;
	int yes = 1;

	/* Crear socket */
	lsock = socket(AF_INET, SOCK_STREAM, 0);
	if (lsock < 0)
		quit("socket");

	/* Setear opción reuseaddr... normalmente no es necesario */
	if (setsockopt(lsock, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == 1)
		quit("setsockopt");

	sa.sin_family = AF_INET;
	sa.sin_port = htons(4040);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	/* Bindear al puerto 4040 TCP, en todas las direcciones disponibles */
	rc = bind(lsock, (struct sockaddr *)&sa, sizeof sa);
	if (rc < 0)
		quit("bind");

	/* Setear en modo escucha */
	rc = listen(lsock, 10);
	if (rc < 0)
		quit("listen");

	return lsock;
}

/*Crea un segmento de memoria compartida*/
void prepare_men(){
	fd = shm_open(BACKING_FILE,      
                    O_RDWR | O_CREAT, /* read/write, crear si es necesario */
                    ACCESS_PERMS);     /* permisos (0644) */
	if (fd < 0) quit("No pude abrir el segmento compartido...");

  	ftruncate(fd, BYTE_SIZE); /* get the bytes */

	memptr = (int *)mmap(0,      
                        BYTE_SIZE,   /* cuantos bytes */
                        PROT_READ | PROT_WRITE, 
                        MAP_SHARED, 
                        fd,        
                        0);         /* offset: empieza en el primer byte */
	if ((int *) -1  == memptr) quit("No pude tener el segmento...");
	U = *memptr;
	*memptr = 0; /*Inicializo a 0*/

	fprintf(stderr, "Dirección de memoria compartida: %p [0..%d]\n", memptr, BYTE_SIZE - 1);
	fprintf(stderr, "backing file:       /dev/shm%s\n", BACKING_FILE );
	return;

}

int main()


{ 
     fde=epoll_create1(0);
    if (fde == -1) {
        perror("epoll_create1");
        exit(EXIT_FAILURE);
    }

 struct  epoll_event ev;


    

 

	lsock = mk_lsock();
    
    ev.events = EPOLLIN;
    ev.data.fd = mk_lsock;
    if (epoll_ctl(fde, EPOLL_CTL_ADD, &mk_lsock, &ev) == -1) {
        perror("epoll_ctl: mk_lsock");
        exit(EXIT_FAILURE);
    }
    
    

	prepare_men();

    for(int i=0;i<MAX_EVENTS;i++){
        pthread_t pthread;
        pthread_create(&pthread,NULL,(void *)wait_for_clients,&fde);
    }

	//munmap(memptr, BYTE_SIZE);
  	close(fd);
 //	unlink(BACKING_FILE);
	return 0;	
}