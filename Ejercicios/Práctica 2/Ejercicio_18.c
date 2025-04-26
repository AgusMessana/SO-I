#include <pthread.h>
#include <sys/syscall.h>
#include <unistd.h>

typedef struct {
  pthread_mutex_t lock_interno; // Un mutex común de pthreads para la protección
  pid_t owner;                  // Quién tiene el lock
  int recursion_count;          // Cuántas veces lo bloqueó el mismo thread
} mutex_recursivo_t;

// Función para inicializar
void mutex_recursivo_init(mutex_recursivo_t * m) {
  pthread_mutex_init(&m->lock_interno, NULL);
  m->owner = -1;                // nadie lo tiene
  m->recursion_count = 0;
}

// Función para lockear
void mutex_recursivo_lock(mutex_recursivo_t * m) {
  pid_t tid = syscall(SYS_gettid);      // Obtener id
  pthread_mutex_lock(&m->lock_interno);

  if (m->owner == tid) {
    m->recursion_count++;
  } else {
    // Si no lo tengo, espero hasta tenerlo
    while (m->owner != -1) {
      pthread_mutex_unlock(&m->lock_interno);   // Suelto para no bloquear otros
      sched_yield();            // Cedo CPU
      pthread_mutex_lock(&m->lock_interno);     // Vuelvo a intentar
    }
    m->owner = tid;
    m->recursion_count = 1;
  }

  pthread_mutex_unlock(&m->lock_interno);
}

// Función para unlockear
void mutex_recursivo_unlock(mutex_recursivo_t * m) {
  pid_t tid = syscall(SYS_gettid);      // Obtener id
  pthread_mutex_lock(&m->lock_interno);

  if (m->owner != tid) {
    printf("Error: unlock de mutex que no poseo\n");
    pthread_mutex_unlock(&m->lock_interno);
    return;
  }

  m->recursion_count--;

  if (m->recursion_count == 0) {
    m->owner = -1;              // El mutex queda libre
  }

  pthread_mutex_unlock(&m->lock_interno);
}
