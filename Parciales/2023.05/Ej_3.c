#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>          // ¡Librería clave!

// 1. Declaramos los semáforos globales
sem_t sem_a1_listo;
sem_t sem_b1_listo;

void *Thread_A(void *arg) {
  // a1; (Código que no nos interesa)
  printf("A termina a1\n");

  sem_post(&sem_a1_listo);      // A avisa: "Dejo ficha de a1"
  sem_wait(&sem_b1_listo);      // A espera la ficha de b1

  // a2; 
  printf("A ejecuta a2\n");
  pthread_exit(NULL);
}

void *Thread_B(void *arg) {
  // b1;
  printf("B termina b1\n");

  sem_post(&sem_b1_listo);      // B avisa: "Dejo ficha de b1"
  sem_wait(&sem_a1_listo);      // B espera la ficha de a1

  // b2;
  printf("B ejecuta b2\n");
  pthread_exit(NULL);
}

int main() {
  pthread_t th_a, th_b;

  // 2. Inicializamos los semáforos. 
  // El segundo parámetro en 0 significa que se comparten entre hilos.
  // El tercer parámetro es la cantidad de fichas iniciales (0).
  sem_init(&sem_a1_listo, 0, 0);
  sem_init(&sem_b1_listo, 0, 0);

  pthread_create(&th_a, NULL, Thread_A, NULL);
  pthread_create(&th_b, NULL, Thread_B, NULL);

  pthread_join(th_a, NULL);
  pthread_join(th_b, NULL);

  // 3. Buenas prácticas: destruir los semáforos al terminar
  sem_destroy(&sem_a1_listo);
  sem_destroy(&sem_b1_listo);

  return 0;
}
