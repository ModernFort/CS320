#include <stdio.h>
#include "commands/rawMode/raw.h" 

int main() {
  enterRaw();

  Context *c = initContext();

  printf("Now entering custom terminal. Normal terminal shortcuts may not work. enter ':q' to exit\r\n");

  procInputs(c);

  return 0;
}