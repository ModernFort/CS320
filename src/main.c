#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <errno.h>
#include <string.h>
#include "commands/touch/touch.h"
#include "commands/ls/ls.h"
#include "commands/manpage/manpage.h"
#include "commands/grep/grep.h"
#include "commands/rawMode/raw.h" 

int main() {
  enterRaw();

  Context *c = initContext();

  printf("Now entering custom terminal. Normal terminal shortcuts may not work. enter ':q' to exit\r\n");

  procInputs(c);

  return 0;
}
