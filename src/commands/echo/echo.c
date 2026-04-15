#include "echo.h"

int echo(char **params, int num_params) {
  for (int i = 0; i < num_params; i++) {
    if (i != 0) {
      printf(" ");
    }
    printf("%s", params[i]);
  }
  printf("\n");
  fflush(stdout);
}
