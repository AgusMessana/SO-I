// free() NO es una llamada al sistema. Esta función forma parte de la librería estándar de C. Esta librería se ejecuta en el espacio de ususario del proceso.
// Cuando se llama a malloc() o realloc(), la librería de C no siempre solicita memoria directamente al kernel, sino que suele mantener un gran área de memoria (llama heap) que ha sido asignada previamente por el kernel.
