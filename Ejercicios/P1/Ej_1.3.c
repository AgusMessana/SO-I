int main() {
  while(1) {
    printf("yes\n");
  }

  return 0;
}

/**
 * Si se ejecuta solo
 * El proceso intentará usar el 100% de un núcleo de CPU, ya que estará ejecutando instrucciones y llamadas al sistema de escritura lo más rápido posible sin interrupciones voluntarias.
 */

 /**
  * En un pipeline (yes | ./installer)
  * El uso de CPU de yes baja drásticamente y pasa a depender totalmente de la velocidad del consumidor (./installer). Esto ocurre porque los pipes tienen un tamaño fijo en bytes.
  * El proceso yes pasa la mayor parte del tiempo bloqueado por I/O, por lo que su consumo de CPU es mínimo, adaptándose al ritmo de procesamiento del segundo programa.
  */