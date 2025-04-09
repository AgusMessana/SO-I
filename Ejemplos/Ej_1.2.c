#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid = fork();
    if(pid == 0){
        printf("Soy el hijo. Mi PID es: %d\n", getpid());
    }
    else{
        printf("Soy el padre. Mi PID es: %d\n", getpid());
        printf("El PID del hijo es: %d\n", pid);
    }

    return 0;
}