#include <stdio.h>
#include <stdlib.h>

int main(void) {
  int A[] = { 1, 2, 23, 54, -2, 7, 42 };
  int encontrado_if = 0, encontrado_noif = 0;
  for (int i = 0; i < sizeof(A); i++) {
    if (A[i] == 42) {
      encontrado_if = 1;
    }
    encontrado_noif = encontrado_noif || (A[i] == 42);
  }

  printf("Con if: %d.\n", encontrado_if);
  printf("Sin if: %d.\n", encontrado_noif);

  return 0;
}

/**
 * Con un solo hilo los dos fragmentos son equivalentes.
 * En concurrencia aparece una diferencia:
 * Con el fragmento condicional, una vez que encontrado_if se hace true, nunca vuelve a ser false. Otro thread no puede deshacer ese resultado.
 * En el segundo caso puede haber race condition. Sucede lo siguiente:
 * 1. Thread 1 encuentra el 42 y pone encontrado = true. Al mismo tiempo, Thread 2 está en otra posición donde no hay un 42.
 * 2. Thread 2 lee el valor viejo de encontrado (que todavía era false en su caché), calcula false || false y sobreescribe el true que acaba de poner Thread 1 con un false.
 * Resultado: El programa dice que no encontró el 42, aunque estaba ahí.
 * Por ende, el primer caso es más seguro.
 */
