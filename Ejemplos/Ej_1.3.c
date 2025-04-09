#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid1 = fork();
    pid_t pid2 = fork();
    printf("pid1: %d pid2: %d!\n", pid1, pid2);
    return 0;
}

/* 
P0: pid1 > 0, pid2 > 0      ← padre original
├── P1: pid1 == 0, pid2 > 0
│   └── P4: pid1 == 0, pid2 == 0
└── P2: pid1 > 0, pid2 == 0
 */