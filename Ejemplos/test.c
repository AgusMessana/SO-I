#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    printf("El PID del proceso es: %d\n", getpid());
    return 0;
}