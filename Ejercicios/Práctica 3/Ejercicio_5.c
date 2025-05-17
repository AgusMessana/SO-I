#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define NUM_CLIENTES 10
#define SILLAS 5

pthread_mutex_t lock;
sem_t cliente_listo;
sem_t barbero_listo;
sem_t cortar;
sem_t corte_listo;
sem_t pagar;
sem_t recibir_pago;
sem_t pago_confirmado;

int clientes_en_espera = 0;

void *me_cortan(void *arg) {
  int id_cliente = (int) (long) arg;

  printf("Cliente %d: espero a que el barbero me llame\n", id_cliente);
  sem_wait(&barbero_listo);

  printf("Cliente %d: me atienden\n", id_cliente);
  sem_post(&cortar);

  sem_wait(&corte_listo);
  printf("Cliente %d: el corte terminó\n", id_cliente);

  return NULL;
}

void *cortando(void *arg) {
  sem_wait(&cortar);
  printf("Barbero: comienzo a cortar el pelo...\n");
  sleep(rand() % 3 + 1);
  printf("Barbero: terminé de cortar\n");

  sem_post(&corte_listo);

  return NULL;
}

void *pagando(void *arg) {
  int id_cliente = (int) (long) arg;
  printf("Cliente %d: hago el pago\n", id_cliente);
  sem_post(&pagar);
  sem_wait(&pago_confirmado);
  printf("Cliente %d: pago confirmado. Me voy\n", id_cliente);

  return NULL;
}

void *me_pagan(void *arg) {
  sem_wait(&pagar);
  printf("Barbero: recibí el pago\n");
  sem_post(&pago_confirmado);

  return NULL;
}

void *cliente(void *arg) {
  int id = (int) (long) arg;

  printf("Cliente %d: llego a la barbería\n", id);

  pthread_mutex_lock(&lock);
  if (clientes_en_espera == SILLAS) {
    printf("Cliente %d: no hay sillas disponibles. Me voy\n", id);
    pthread_mutex_unlock(&lock);
    return NULL;
  }
  clientes_en_espera++;
  printf("Cliente %d: me siento a esperar (%d esperando)\n", id,
         clientes_en_espera);
  pthread_mutex_unlock(&lock);

  sem_post(&cliente_listo);     // Aviso que llegué

  pthread_t t_corte, t_pago;

  // me_cortan + pagando
  pthread_create(&t_corte, NULL, me_cortan, (void *) (long) id);
  pthread_create(&t_pago, NULL, pagando, (void *) (long) id);

  pthread_join(t_corte, NULL);
  pthread_join(t_pago, NULL);

  return NULL;
}

void *barbero(void *arg) {
  while (1) {
    sem_wait(&cliente_listo);   // Espero a que haya un cliente

    pthread_mutex_lock(&lock);
    clientes_en_espera--;
    printf("Barbero: llamo a un cliente (%d esperando)\n", clientes_en_espera);
    pthread_mutex_unlock(&lock);

    sem_post(&barbero_listo);   // Llamo al cliente

    pthread_t t_corte, t_pago;
    pthread_create(&t_corte, NULL, cortando, NULL);
    pthread_create(&t_pago, NULL, me_pagan, NULL);

    pthread_join(t_corte, NULL);
    pthread_join(t_pago, NULL);
  }

  return NULL;
}

int main() {
  srand(time(NULL));
  pthread_t th_barbero;
  pthread_t th_clientes[NUM_CLIENTES];

  // Inicializar mutex y semáforos
  pthread_mutex_init(&lock, NULL);
  sem_init(&cliente_listo, 0, 0);
  sem_init(&barbero_listo, 0, 0);
  sem_init(&cortar, 0, 0);
  sem_init(&corte_listo, 0, 0);
  sem_init(&pagar, 0, 0);
  sem_init(&recibir_pago, 0, 0);
  sem_init(&pago_confirmado, 0, 0);

  pthread_create(&th_barbero, NULL, barbero, NULL);

  for (int i = 0; i < NUM_CLIENTES; i++) {
    pthread_create(&th_clientes[i], NULL, cliente, (void *) (long) i);
    usleep(500000);             // medio segundo entre clientes
  }

  for (int i = 0; i < NUM_CLIENTES; i++) {
    pthread_join(th_clientes[i], NULL);
  }

  printf("Todos los clientes fueron atendidos\n");
  return 0;
}
