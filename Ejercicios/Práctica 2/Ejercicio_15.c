// Ej. 15 (Servidor de Turnos). Esta vez vamos a usar threads...
// a) Adapte su implementación de la práctica anterior del servidor de turnos para atender concurrentemente a todas las conexiones abiertas levantando un thread nuevo por cada conexión. Nota: todos los clientes deberán poder hacer pedidos sin esperar a otros, y siempre debe poder conectarse un nuevo cliente. Esta vez, se debe garantizar que dos pedidos nunca reciben el mismo entero.
// b) Implemente una solución con select/epoll.
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
int* memptr;
int fd;

void quit(char *s) {
	perror(s);
	abort();
}

int fd_readline(int fd, char *buf) {
	int rc, i = 0;

	while((rc = read(fd, buf + i, 1)) > 0) {
		if(buf[i] == '\n') {
			break;
		}
		i++;
	}

	if (rc < 0) {
		return rc;
	}

	buf[i] = 0;
	return i;
}

void handle_conn(int csock) {
	char buf[200];
	int rc;

	while (1) {
		/* Atendemos pedidos, uno por linea */
		rc = fd_readline(csock, buf);
		if(rc < 0) {
			quit("read... raro");
		}

		if(rc == 0) {
			/* linea vacia, se cerró la conexión */
			close(csock);
			return;
		}

		if(!strcmp(buf, "NUEVO")) {
			char reply[20];
			pthread_mutex_lock(&mutex_v);
			int U = *memptr;
			sprintf(reply, "%d\n", U);
			*memptr = U + 1;
			pthread_mutex_unlock(&mutex_v);
			write(csock, reply, strlen(reply));
		} else if (!strcmp(buf, "CHAU")) {
			write(csock, "Chau!, cierro la conexión\n", 26);
			close(csock);
			return;
		}
	}
}

void* wait_for_clients(void* arg) {
	struct epoll_event ev, events[MAX_EVENTS];
	
	while (1) {
		int nfds = epoll_wait(fde, events, MAX_EVENTS, -1);
		if (nfds == -1) {
			perror("epoll_wait");
			exit(EXIT_FAILURE);
		}

		for (int n = 0; n < nfds; ++n) {
			if (events[n].data.fd == lsock) {
				int csock = accept(lsock, NULL, NULL);
				if (csock < 0) {
					quit("accept");
					exit(EXIT_FAILURE);
				}
				ev.events = EPOLLIN | EPOLLONESHOT;
				ev.data.fd = csock;
				if (epoll_ctl(fde, EPOLL_CTL_ADD, csock, &ev) == -1) {
					perror("epoll_ctl: conn_sock");
					exit(EXIT_FAILURE);
				}
			} else {
				handle_conn(events[n].data.fd);
				ev.events = EPOLLIN | EPOLLONESHOT;
				ev.data.fd = events[n].data.fd;
				if (epoll_ctl(fde, EPOLL_CTL_MOD, events[n].data.fd, &ev) == -1) {
					perror("epoll_ctl: rearm");
					exit(EXIT_FAILURE);
				}
			}
		}
	}
	return NULL;
}

int mk_lsock() {
	struct sockaddr_in sa;
	int sockfd, rc, yes = 1;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		quit("socket");
	}
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof yes) == -1) { 
		quit("setsockopt");
	}

	sa.sin_family = AF_INET;
	sa.sin_port = htons(4040);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);

	rc = bind(sockfd, (struct sockaddr *)&sa, sizeof sa);
	if (rc < 0) {
		quit("bind");
	}

	rc = listen(sockfd, 10);
	if (rc < 0) {
		quit("listen");
	}

	return sockfd;
}

void prepare_men(){
	fd = shm_open(BACKING_FILE, O_RDWR | O_CREAT, ACCESS_PERMS);
	if(fd < 0) {
		quit("No pude abrir el segmento compartido...");
	}

  	ftruncate(fd, BYTE_SIZE);
	memptr = mmap(0, BYTE_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	if(memptr == MAP_FAILED) {
		quit("mmap");
	}

	*memptr = 0;

	fprintf(stderr, "Dirección de memoria compartida: %p [0..%d]\n", memptr, BYTE_SIZE - 1);
	fprintf(stderr, "backing file: /dev/shm%s\n", BACKING_FILE);
}

int main() { 
    fde = epoll_create1(0);
    if(fde == -1) {
		perror("epoll_create1");
        exit(EXIT_FAILURE);
    }
	
	struct  epoll_event ev;
	lsock = mk_lsock();

    ev.events = EPOLLIN;
    ev.data.fd = lsock;
    if (epoll_ctl(fde, EPOLL_CTL_ADD, lsock, &ev) == -1) {
        perror("epoll_ctl: mk_lsock");
        exit(EXIT_FAILURE);
    }
    
	prepare_men();
	pthread_t threads[MAX_EVENTS];
    for(int i = 0; i < MAX_EVENTS; i++) {
        pthread_create(&threads[i], NULL, wait_for_clients, NULL);
    }

	for(int i = 0; i < MAX_EVENTS; i++) {
		pthread_join(threads[i], NULL);
	}

  	close(fd);
	close(lsock);
	close(fde);
	
	return 0;	
}