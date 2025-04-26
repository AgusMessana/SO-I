//! Caso 1
// if (A[i] == 42)
// encontrado = true;

//! Caso 2
// encontrado = encontrado || (A[i] == 42);

// Si hay un solo hilo, no hay diferencia ya que ambos ponen enconrado en true cuando encuentran el número 42.
// Si hay múltiples hilos accediendo a A y a encontrado al mismo tiempo, si hay diferencia:
// - en el primer caso, el acceso y la escritura en encontrado sólo pasa cuando A[i] = 42. Entonces, al haber menos escrituras, hay menos probabilidades de tener problemas de concurrencia.
// - en el segundo caso, siempre se lee y se escribe encontrado aunque pase que A[i] != 42. Eso puede producir más conflictos de escritura concurrente entre hilos, lo que lleva a race conditions si no hay protección.