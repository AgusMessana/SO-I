#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    const char* prog = "test.out";
    printf("PID: %d\n", getpid());

    execl(prog, prog, NULL);

    return 0;
}