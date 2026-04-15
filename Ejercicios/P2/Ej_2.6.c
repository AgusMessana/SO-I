/**
 * Considere el problema del jardín ornamental en un sistema con un único procesador.
 */

 // Aclaración: al haber dos hilos pero un solo procesador, se van inercalando. El SO le da un tiempo de CPU al hilo 0, lo pausa, luego un tiempo de CPU al hilo 1, lo pausa, y así sucesivamente. Esto se denomina concurrencia.

 /**
  * a) ¿Sigue habiendo un problema de concurrencia? Justifique.
  * Sí. Aunque haya un solo procesador, la condición de carrera sigue existiendo. El SO puede decidir suspender un hilo en una operación no atómica como por ejemplo visitante++.
  * Es decir, el hilo 0 lee visitantes = 10. Luego el SO lo pausa y le da el turno al hilo 1. El hilo 1 lee visitantes = 10, lo aumenta en 1 y guarda visitantes = 11. Luego vuelve al hilo 0. Éste sigue leyendo visitantes = 10. Lo aumenta en 1 y guarda visitantes = 11. De esa forma se pierde un visitante, porque debería ser visitantes = 12.
  */

/**
 * b) Si implementa el algoritmo de Peterson, ¿son necesarias las barreras de memoria?
 * Para el caso de un único procesador no son necesarias las barreras de memoria.
 * Como no hay otro procesador mirando la memoria al mismo tiempo, el hilo que entra después de un cambio de contexto siempre verá los datos tal como el hilo anterior los dejó, porque comparten el mismo pipeline y caché. Por eso, el mfence (que asegura visibilidad entre procesadores) no tiene utilidad.
 */

/**
 * c) Si el incremento se hace con la instrucci´on incl de x86, ¿hay problema? Puede aprovechar lasiguiente función:
 */
static inline void incl(int *p) {
asm("incl %0": "+m"(*p): :"memory");
}
/**
 * Al usar dicha instrucción el problema desaparece pues incl es una instrucción única de Assembler. Como el SO sólo hace cambios de conexto entre instrucciones (nunca a mitad de una), la definición se vuelve atómica.
 */

/**
 * d) ¿Qué pasa con la implementación con incl al tener más de un procesador?
 * Con multiprocesador volvemos a tener problemas porque dos procesadores pueden ejecutar incl físicamente al mismo tiempo sobre la misma memoria.
 */

/**
 * e) Repita el experimento con esta versión de incl:
 */
static inline void incl(int *p) {
  asm("lock; incl %0" : "+m"(*p) : : "memory");
}
/**
 * Para este caso no tendremos problemas en multiprocesador ya que el prefijo lock asegura que el incremento sea atómico, bloqueando el bus de memoria para que ningún otro hilo interfiera mientras se realiza la operación.
 */