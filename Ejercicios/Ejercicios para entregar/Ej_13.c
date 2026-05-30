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
  struct cv_node node;
  sem_init(&node.sem, 0, 0);
  node.next = NULL;

  sem_wait(&c->mtx_internal);
  if (c->head == NULL) {
    c->head = &node;
    c->tail = &node;
  } else {
    c->tail->next = &node;
    c->tail = &node;
  }

  sem_post(&c->mtx_internal);
  pthread_mutex_unlock(user_mtx);

  /**
   * El hilo se bloquea acá, ya que el semáforo local arranca en 0.
   * Al quedarse pausado, la función no retorna y la variable local 'node' sigue viva en el stack, permitiendo que el hilo que hace el 'signal' pueda acceder a ella de forma segura a través de los punteros.
   */
  sem_wait(&node.sem);

  sem_destroy(&node.sem);
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
