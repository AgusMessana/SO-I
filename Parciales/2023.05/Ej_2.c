/**
 * Un deadlock es una situación de bloqueo permanente donde un conjunto de procesos o hilos se quedan congelados infinitamente. Esto pasa porque cada proceso tiene asignado un recurso que otro necesita, y a la vez está esperando un recurso que está en poder de otro. Como nadie quiere soltar lo que tiene hasta conseguir lo que le falta, nadie avanza nunca.
 * a) La existencia de región crítica con acceso mutuamente exclusivo: El recurso no se puede compartir (Ej: una impresora, o se la doy a uno o se la doy a otro).
 * c) Hold and wait: Un proceso tiene un recurso agarrado en la mano y, sin soltarlo, se pone a esperar a que le den otro.
 * d) No preemption: Nadie le puede arrancar el recurso de las manos a un proceso por la fuerza; lo tiene que soltar él de forma voluntaria.
 * e) Espera circular: Se forma una ronda. El hilo A espera a B, B espera a C, y C espera a A.

 * 1. Eliminar la Espera Circular (e) - La más recomendada:
 Para romper el círculo, lo que se hace es imponer un orden estricto. Si tu programa tiene dos mutex (Mutex 1 y Mutex 2), obligás por convención de código a que todos los hilos del mundo siempre pidan primero el Mutex 1 y recién después el Mutex 2. Si todos van en la misma dirección, es matemáticamente imposible que se forme un ciclo, porque nadie va a tener el 2 esperando el 1.
 * 2. Eliminar Hold and Wait (c):
 Podés obligar a los hilos a pedir todos los recursos que necesitan juntos al principio, de un solo golpe. Si no están todos disponibles, el hilo no agarra ninguno. Otra variante es: si un hilo ya tiene un Mutex agarrado, y necesita otro pero está ocupado, la regla es que suelte el que ya tiene, se vaya a dormir, y vuelva a intentar agarrar los dos más tarde.
 */
