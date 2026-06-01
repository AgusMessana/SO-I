#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

struct cv_node {
  sem_t sem;
  struct cv_node *next;
};

struct cond_sem {
  sem_t mtx_internal;
  struct cv_node *head;
  struct cv_node *tail;
};

void cond_sem_init(struct cond_sem *c) {
  sem_init(&c->mtx_internal, 0, 1);
  c->head = NULL;
  c->tail = NULL;
}

void cond_sem_wait(struct cond_sem *c, pthread_mutex_t * user_mtx) {
  struct cv_node *node = malloc(sizeof(struct cv_node));
  if (node == NULL) {

    return;
  }

  sem_init(&node->sem, 0, 0);
  node->next = NULL;

  sem_wait(&c->mtx_internal);
  if (c->head == NULL) {
    c->head = node;
    c->tail = node;
  } else {
    c->tail->next = node;
    c->tail = node;
  }
  sem_post(&c->mtx_internal);

  pthread_mutex_unlock(user_mtx);

  sem_wait(&node->sem);

  sem_destroy(&node->sem);
  free(node);
  pthread_mutex_lock(user_mtx);
}

void cond_sem_signal(struct cond_sem *c) {
  sem_wait(&c->mtx_internal);

  if (c->head != NULL) {
    struct cv_node *woken_node = c->head;
    c->head = c->head->next;
    if (c->head == NULL) {
      c->tail = NULL;
    }

    sem_post(&woken_node->sem);
  }

  sem_post(&c->mtx_internal);
}

void cond_sem_broadcast(struct cond_sem *c) {
  sem_wait(&c->mtx_internal);

  struct cv_node *curr = c->head;
  while (curr != NULL) {
    sem_post(&curr->sem);
    curr = curr->next;
  }

  c->head = NULL;
  c->tail = NULL;

  sem_post(&c->mtx_internal);
}

void cond_sem_destroy(struct cond_sem *c) {
  sem_destroy(&c->mtx_internal);
}

/**
 * Problemas de la implementación:
 * Usar un solo semáforo compartido para simular una variable de condición rompe todo por dos razones:

 * 1. Señales que no se pierden: En una variable de condición real, si tiro un signal y no hay nadie esperando, la señal se tiene que perder. Con el semáforo de la práctica, el sem_post te deja un crédito. El próximo hilo que haga wait va a pasar de largo con una señal vieja del pasado en vez de bloquearse

 * 2. El bache de tiempo: Soltar el mutex y dormir al hilo es un paso único y atómico. Con semáforos hay que hacer primero unlock del mutex y recién después el wait del semáforo. En ese microsegundo del medio, si el sistema corta el hilo y entra otro que toma el mutex y mete un signal, la señal se pierde porque el hilo viejo todavía no se durmió en el semáforo, dandonos un Deadlock
 
 * La solución en nuestro codigo a esto fue: Armamos una cola explícita en el heap donde cada hilo tiene su propio semáforo privado arrancando en 0. Así, el signal o broadcast solo despierta a los que ya están en la lista. Además, usando malloc evitamos que el broadcast rompa los punteros si un hilo se despierta muy rápido y libera su memoria mientras el bucle sigue recorriendo a los demás
 */
