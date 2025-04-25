// Luego del fork, el padre envía una señal SIGUSR1 al hijo y entra en un loop infinito. El hijo espera en
// un loop infinito. Ambos (padre e hijo) cuando reciven una señal SIGUSR1 responden lo mismo. (El padre
// al hijo y el hijo al padre). ¿Qué pasa si cambiamos los loops infinitos por pause()?
// Usar las funciones signal() y sigaction().

#define _POSIX_C_SOURCE 200809L //para que struct sigaction sa no tire error
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>


pid_t pid2;

void handler(int signal) {
    printf("Recibí SIGUSR1 (PID: %d)\n", getpid());
    kill(pid2, SIGUSR1);
}

int main() {
    struct sigaction sa;
    sa.sa_handler = handler; //que hacer cuando llega la señal
    sigemptyset(&sa.sa_mask); //no se bloquea ninguna otra señal mientras se ejecuta el handler.
    sa.sa_flags = 0; //sin banderas especiales
    //la función sigaction() establece cómo el proceso va a reaccionar cuando reciba una señal específica.
    

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(1);
    }
    
    if(pid == 0){
        pid2 = getppid();
        //getppid() devuelve el PID del padre
        printf("Soy el hijo. Mi PID es: %d. Esperando señal...\n", getpid());
    }
    else{
        pid2 = pid;
        printf("Soy el padre. Mi PID es: %d. Enviando SIGUSR1 al hijo %d.\n", getpid(), pid);
        kill(pid, SIGUSR1);
        //La función kill() envía una señal a un proceso.
        //pid: el ID del proceso al que querés mandarle la señal.
        //sig: la señal que querés enviar 
    }

    while(1){
        pause();
    }

    return 0;
}