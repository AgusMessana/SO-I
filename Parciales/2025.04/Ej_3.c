#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int value = 0;
int hello = 0;
int test_and_set(int *value) {
  int old_value = *value;
  *value = 1;
  return old_value;
}

void print_hello() {
  while (test_and_set(&value));
  hello += 1;
  printf("Child thread: %d\n", hello);
  value = 0;
  pthread_exit(0);
}

void main() {
  pthread_t thread1, thread2;
  pthread_create(&thread1, NULL, &print_hello, NULL);
  pthread_create(&thread2, NULL, &print_hello, NULL);
  while (test_and_set(&value));
  printf("Parent thread: %d\n", hello);
  value = 0;
  return;
}

/**
a)
Estado Inicial: value = 0, hello = 0.
* Paso 1: main crea thread1 y thread2. Cambio de contexto.
  Variables: value = 0, hello = 0.

* Paso 2: thread2 entra a correr. Ejecuta el while y gana el candado. Incrementa hello. Cambio de contexto.
  Estado: thread2 está dentro de la sección crítica.
  Variables: value = 1 (bloqueado por thread2), hello = 1.

* Paso 3: thread1 corre. Intenta entrar al while, pero el candado está ocupado. Se queda "bloqueado" (haciendo espera activa). Cambio de contexto.
  Estado: thread2 en sección crítica (pausado), thread1 "bloqueado" en el while.
  Variables: value = 1, hello = 1.

* Paso 4: main reanuda. Intenta entrar a su propio while, pero el candado sigue ocupado por thread2. También se queda "bloqueado". Cambio de contexto.
  Estado: thread2 en sección crítica (pausado), thread1 y main "bloqueados" en el while.
  Variables: value = 1, hello = 1.

* Paso 5: thread2 retoma. Hace su printf, ejecuta value = 0 (liberando el candado) y termina (pthread_exit).
  Estado: thread2 muere. thread1 y main siguen listos para salir del while, pero pausados.
  Variables: value = 0 (¡candado libre!), hello = 1.

* Paso 6: main corre. Como value es 0, gana el candado saliendo del while. Hace su printf, luego ejecuta value = 0 (liberando el candado nuevamente). No termina.
  Estado: main pausado después de soltar el candado. thread1 listo para correr.
  Variables: value = 0 (candado libre), hello = 1.

* Paso 7: thread1 corre. Gana el candado saliendo de su while. Incrementa hello, hace su printf, libera el candado (value = 0) y termina.
  Estado Final: Todo finalizado.
  Variables: value = 0, hello = 2.

b)
Para cada hilo, en los momentos clave descritos en los pasos anteriores, la función devolvió lo siguiente:

* Para thread2 (Paso 2): Devolvió 0. (Ganó el acceso a la sección crítica a la primera).
* Para thread1 (Paso 3): Devolvió 1. (Múltiples veces mientras consumía su quantum de CPU, manteniéndolo en el while).
* Para main (Paso 4): Devolvió 1. (Múltiples veces también).
* Para main (Paso 6): Devolvió 0. (Cuando el scheduler le dio turno, vio el candado liberado por thread2 y entró).
* Para thread1 (Paso 7): Devolvió 0. (Cuando le tocó de nuevo, vio el candado liberado por main y entró).

c)
Child thread: 1
Parent thread: 1
Child thread: 2

d)
El mayor problema de usar este while(test_and_set(&value)) es lo que se conoce como espera activa (o busy waiting).
Cuando un hilo (como thread1 o main en los pasos 3 y 4) no puede adquirir el candado, no se va a dormir. En su lugar, se queda iterando a toda velocidad en el while, evaluando incesantemente la condición y desperdiciando ciclos completos de procesador sin realizar ningún trabajo útil real.
 */