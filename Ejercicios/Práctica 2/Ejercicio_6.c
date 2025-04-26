//! Consideradndo el problema del jardín ornamental con un único procesador
//* ¿Sigue habiendo un problema?
// No, no hay problemas de race condition grave.
// Como hay un único procesador, no puede ejecutarse más de un thread a la vez. Cuando un thread se está ejecutando, el otro necesariamente está pausado.
// Sin embargo, si los hilos se interrumpen en un mal momento (durante un contador++ por ejemplo) puede quedar inconsistencia si no se protege bien.

//* Si implementa el algoritmo de Peterson, ¿son necesarias las barreras de memoria?
// Sí, son necesarias. Aunque haya un solo procesador, el compilador puede reordenar instrucciones para optimizar. Sin barreras de memoria, el CPU puede cambiar el orden de flag[i] = 1 o de turno = otro, y romper la correcta sincronización.

//* Si el incremento se hace con la instrucción incl de x86, ¿hay problema?
static inline void incl(int *p) {
asm("incl %0": "+m"(*p): :"memory");
}

// Sí, puede haber problemas. La instrucción incl sola en x86 no es atómica entre hilos: no bloquea el bus de memoria, y si dos hilos hacen incl podría leerse un valor viejo y sobrescribir.
// Esyo sucede especialmente si hay preemption (el sistema operativo interrumpe y cambia de hilo en medio de incl).

//* ¿Qué pasa con la implementación con incl al tener más de un procesador?
// El problema se agrava más aún. Cada procesador puede tener su propio caché de memoria. Esto puede ocasionar que un procesador pueda ver un valor de contador diferente al que ve otro. Sin lock, dos incl podrían no verse mutuamente y pisarse.

//* ¿Qué pasa si usamos esta versión de incl?
static inline void incl(int *p) {
asm("lock; incl %0": "+m"(*p): :"memory");
}

// Esta versión es correcta. Con el prefijo lock logramos que se bloquee el bus de memoria (o coordinamos los caché), garantizamos que el acceso a *p es atómico respecto a otros procesadores e incluímos barreras de memoria para que no se reordenen accesos.
