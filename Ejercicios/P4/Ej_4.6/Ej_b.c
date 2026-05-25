#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

/* Particion de Lomuto, tomando el primer elemento como pivote */
int particionar(int a[], int n) {
  int i, j = 0;
  int p = a[0];
  swap(&a[0], &a[n - 1]);
  for (i = 0; i < n - 1; i++) {
    if (a[i] <= p)
      swap(&a[i], &a[j++]);
  }
  swap(&a[j], &a[n - 1]);
  return j;
}

void qsort(int a[], int n) {
  if (n < 2)
    return;
  int p = particionar(a, n);
#pragma omp parallel sections
  {
#pragma omp section
    qsort(a, p);
#pragma omp section
    qsort(a + p + 1, n - p - 1);
  }
}

/**
 * Con esta opción, habrá una mejora respecto al caso de los hilos, pero no será una gran diferencia respecto a resolver el problema secuencialmente.
 * Por defecto, OpenMP desactiva el paralelismo anidado. Básicamente, en la primera ejecución de qsort se crarán dos equipos de trabajo: A y B. En la llamada recursiva, cuando se quiera crear un subequipo de, por ejemplo, A, no lo hará porque ya está el equipo A creado. Luego, se resolvera de forma secuencial.
 * Si el pivote cae justo a la mitad, sí podríamos ver una gran mejora porque el tiempo se reduce a la mitad. Sin embargo, como tenemos sólo 2 núcleos trabajando, los demás no harán nada. Por ende, no estaríamos aprovechando al máximo la capacidad de la computadora.
 */
