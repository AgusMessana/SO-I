#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define N 1000
#define W 4
#define ITERS 10000

struct barrier {
  pthread_mutex_t mutex;
  pthread_cond_t cond;
  int contador;
  int n;
  int ciclo;
};

void barrier_init(struct barrier *b, int n) {
  pthread_mutex_init(&b->mutex, NULL);
  pthread_cond_init(&b->cond, NULL);
  b->contador = 0;
  b->n = n;
  b->ciclo = 0;
}

void barrier_wait(struct barrier *b) {
  pthread_mutex_lock(&b->mutex);
  int mi_ciclo = b->ciclo;
  b->contador += 1;
  if (b->contador == b->n) {
    b->ciclo += 1;
    b->contador = 0;
    pthread_cond_broadcast(&b->cond);
  } else {
    while (mi_ciclo == b->ciclo) {
      pthread_cond_wait(&b->cond, &b->mutex);
    }
  }

  pthread_mutex_unlock(&b->mutex);
}

float arr1[N];
float arr2[N];
struct barrier mi_barrera;

static inline int min(int x, int y) {
  return x < y ? x : y;
}

void calor(float *in, int lo, int hi, float *out) {
  int i;
  for (i = lo; i < hi; i++) {
    int m = in[i];
    int l = i > 0 ? in[i - 1] : m;
    int r = i < N - 1 ? in[i + 1] : m;
    out[i] = m + (l - m) / 1000.0 + (r - m) / 1000.0;
  }
}

/* Dado un array de [n], devuelve el punto de corte [i] de los [m] totales. */
static inline int cut(int n, int i, int m) {
  return i * (n / m) + min(i, n % m);
}

void *thr(void *arg) {
  int id = arg - (void *) 0;
  int lo = cut(N, id, W);
  int hi = cut(N, id + 1, W);
  int i;

  for (i = 0; i < ITERS; i++) {
    calor(arr1, lo, hi, arr2);
    barrier_wait(&mi_barrera);
    calor(arr2, lo, hi, arr1);
    barrier_wait(&mi_barrera);
  }
}

int main() {
  pthread_t w[W];
  float f;
  int i;

  /*
   * No cambiamos la semilla, por lo que este programa
   * debería ser determinista
   */
  barrier_init(&mi_barrera, W);
  for (i = 0; i < N; i++)
    arr1[i] = rand() / 10000.0;

  for (i = 0; i < W; i++)
    pthread_create(&w[i], NULL, thr, i + (void *) 0);

  for (i = 0; i < W; i++)
    pthread_join(w[i], NULL);

  f = 0;
  for (i = 0; i < N; i++)
    f += arr1[i];
  f /= N;

  /* Imprimimos temperatura promedio, siempre debería dar igual */
  printf("Temp promedio: %f\n", f);
  return 0;
}
