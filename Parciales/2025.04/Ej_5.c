/**
 * Región crítica: es un segmento de código dentro de un proceso o hilo donde se accede a recursos compartidos, como leer o escribir en una variable global, modificar un archivo en disco, o acceder a estructuras en el Heap.
 * Race condition: es un error o comportamiento anómalo que se produce cuando varios procesos o hilos acceden a la región crítica al mismo tiempo y manipulan los datos de forma concurrente. El problema es que el resultado final del programa se vuelve completamente impredecible porque depende del orden de la ejecución de las instrucciones.
 * Exclusión mutua: es una propiedad o regla de sincronización que garantiza que, si un hilo está ejecutando instrucciones dentro de su región crítica, ningún otro hilo puede entrar a ejecutar esa misma región crítica. Es la herramienta que usamos para poner orden.

 * La relación entre estos tres conceptos es una de causa-efecto y problema-solución. Se puede resumir en una sola oración:
 Una Race Condition ocurre cuando una Región Crítica no está protegida por un mecanismo que garantice Exclusión Mutua.
 */