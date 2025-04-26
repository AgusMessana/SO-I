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


#define BYTE_SIZE 4
#define BACKING_FILE "/shMemEx"
#define ACCESS_PERMS 0644


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

void wait_for_clients(int lsock)
{
	int csock;
	/* Esperamos una conexión, no nos interesa de donde viene */
	csock = accept(lsock, NULL, NULL);
		if (csock < 0)
			quit("accept");


	int pid = fork();
	if(pid!=0){
	

		/* Atendemos al cliente */		
		handle_conn(csock);
		exit(0);
		
	}
	else{
		/* Volvemos a esperar conexiones */
		close(csock);
		wait(NULL);
		wait_for_clients(lsock);
	
		
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
	int lsock;
	lsock = mk_lsock();
	prepare_men();
	wait_for_clients(lsock);
	munmap(memptr, BYTE_SIZE);
  	close(fd);
 	unlink(BACKING_FILE);
	return 0;	
}
