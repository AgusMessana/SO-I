#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define N 1000
#define ITERS 1000
#define W 4                     // número de hilos

float arr1[N], arr2[N];
struct barrier barr;

static inline int cut(int n, int i, int m) {
  return i * (n / m) + (i < n % m ? i : n % m);
}

void calor(float *arr, int lo, int hi, float *arr2) {
  for (int i = lo; i < hi; i++) {
    float m = arr[i];
    float l = (i > 0) ? arr[i - 1] : m;
    float r = (i < N - 1) ? arr[i + 1] : m;
    arr2[i] = m + (l - m) / 1000.0 + (r - m) / 1000.0;
  }
}

void *thr(void *arg) {
  int id = (int) (size_t) arg;
  int lo = cut(N, id, W), hi = cut(N, id + 1, W);

  for (int i = 0; i < ITERS; i++) {
    calor(arr1, lo, hi, arr2);
    barrier_wait(&barr);        // sincroniza después de primer paso

    calor(arr2, lo, hi, arr1);
    barrier_wait(&barr);        // sincroniza después de segundo paso
  }

  return NULL;
}

int main() {
  pthread_t tids[W];
  barrier_init(&barr, W);

  for (int i = 0; i < W; i++) {
    pthread_create(&tids[i], NULL, thr, (void *) (size_t) i);
  }

  for (int i = 0; i < W; i++) {
    pthread_join(tids[i], NULL);
  }

  return 0;
}
