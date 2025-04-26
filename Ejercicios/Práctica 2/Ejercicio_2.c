//! ¿Cuales de los siguientes componentes del estado de un programa se comparten entre los threads de un proceso?

//* Valores de registro
// No. Cada thread tiene sus propios registros, porque cada uno puede estar ejecutando distintas instrucciones.

//* Heap
// Sí. El heap (memoria dinámica con malloc(), realloc(), etc.) es compartido entre los threads.

//* Variables globales
// Sí. Las variables globales son compartidas entre todos los threads.

//* Stack
// No. Cada thread tiene su propio stack privado apra variables locales y control de ejecución.