// Al usar la versión del jardín ornamental sin exclusión mutua, si compilamos con optimizaciones (-O2 o -O3), el compilador asume que no hay race condition entre hilos pues no usamos exclusión mutua.
// Al suceder eso, el compilador puede optimizar el acceso a contador, cachear el valor en un registro y reordenar las instrucciones.
// Esto puede traer como consecuencia varios problemas. Por ejemplo, un thread lee contador en un registro r1. Otro thread incrementa el verdadero contador en memoria. El primer thread incrementa su copia vieja en r1 y la vuelve a guardar, pisando el valor correcto. El resultado es la pérdida de incrementos.
// Si no hay mutex, y con optimizaciones, en el assembler generado se puede ver algo como:
// mov eax, contador
// add eax, 1
// mov contador, eax
// El compilador puede no recargar contador cada vez, sino trabajar con una copia local.

#include <stdio.h>
#include <pthread.h>

#define N 1000000

int contador = 0;

void *incrementar(void *arg) {
    for (int i = 0; i < N; i++) {
        contador++;
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;

    pthread_create(&t1, NULL, incrementar, NULL);
    pthread_create(&t2, NULL, incrementar, NULL);

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    printf("Contador final: %d\n", contador);

    return 0;
}
