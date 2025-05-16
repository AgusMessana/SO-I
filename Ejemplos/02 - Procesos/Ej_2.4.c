#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main() {
    int fd;
    char buff[1024];
    ssize_t bytesRead;

    fd = open("texto.txt", O_RDONLY);
    if(fd < 0) {
        printf("Error al abrir el archivo.\n");
        close(fd);
        return 1;
    }

    bytesRead = read(fd, buff, sizeof(buff));
    if(bytesRead < 0) {
        printf("Error al leer el archivo.\n");
        close(fd);
        return 1;
    }

    buff[bytesRead] = '\0';
    printf("Leído (%ld bytes):\n%s\n",bytesRead, buff);

    close(fd);
    return 0;
}