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

#pragma omp task
  qsort(a, p);
#pragma omp task
  qsort(a + p + 1, n - p - 1);
#pragma omp taskwait
}

int main(void) {
  int a[10];
#pragma omp parallel single
  {
    qsort(a, 10);
  }

  return 0;
}

/**
 * Este caso es mucho mejor que los otros dos ya que no tenemos que crear enorme cantidad de hilos (como con la primera versión), ni nos vemos en la obligación de realizar los trabajos de manera secuencial (como en la segunda versión). Además, todos los núcleos pueden trabajar, no se limita a sólo 2.
 */
