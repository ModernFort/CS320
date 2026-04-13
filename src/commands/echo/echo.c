#include "echo.h"

int validate_params(char **params, int num_params) {
  for (int i = 0; i < num_params - 1; i++) {
    // validate option parameters
    if (strcmp(params[i], "-n") != 0 &&
        strcmp(params[i], "-e") != 0 &&
        strcmp(params[i], "-E") != 0 &&
        strcmp(params[i], "-en") != 0 &&
        strcmp(params[i], "-ne") != 0 &&
        strcmp(params[i], "-En") != 0 &&
        strcmp(params[i], "-nE") != 0 &&
        strcmp(params[i], "-eE") != 0 &&
        strcmp(params[i], "-Ee") != 0 &&
        strcmp(params[i], "-neE") != 0 &&
        strcmp(params[i], "-nEe") != 0 &&
        strcmp(params[i], "-enE") != 0 &&
        strcmp(params[i], "-eEn") != 0 &&
        strcmp(params[i], "-Ene") != 0 &&
        strcmp(params[i], "-Een") != 0) {
      fprintf(stderr, "echo: invalid option -- '%s'\n", params[i]);
      return -1;
    }
  }
  return 0;
}
int echo(char **params, int num_params) {
  if (validate_params(params, num_params) == -1) {
    // invalid parameter
    return -1;
  }
}
