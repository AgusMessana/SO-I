#include <pthread.h>
#include <semaphore.h>

struct barrier {
  int n;                        // total de hilos esperados
  int count;                    // cuántos llegaron
  pthread_mutex_t lock;
  pthread_cond_t cond;
};

void barrier_init(struct barrier *b, int n) {
  b->n = n;
  b->count = 0;
  pthread_mutex_init(&b->lock, NULL);
  pthread_cond_init(&b->cond, NULL);
}

void barrier_wait(struct barrier *b) {
  pthread_mutex_lock(&b->lock);
  b->count++;
  if (b->count == b->n) {
    b->count = 0;
    pthread_cond_broadcast(&b->cond);   // liberar a todos
  } else {
    pthread_cond_wait(&b->cond, &b->lock);
  }
  pthread_mutex_unlock(&b->lock);
}
