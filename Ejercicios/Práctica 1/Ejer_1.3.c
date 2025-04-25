#include <stdlib.h>
#include <stdio.h>

int main() {
  while (1) {
    puts("y");
  }
  return 0;
}

// Cuando se ejecuta yes | ./installer, yes genera datos constantemente. Si el otro estremo del pipe (./intsaller) no consume los datos suficientemente rápido, el proceso yes se bloquea cuando el buffer del pipe se llena.
// Si installer lee sin parar, yes está cosntantemente escribiendo, y usa casi un 100% de la CPU.
