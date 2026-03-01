#ifndef TOUCH_H
#define TOUCH_H

/**
  * method implements basic functionality of bash command 'touch'
  * Supported options are -a, -m, and any combination of the two
  *
  * @param params - a pointer to the parameters
  * @param num_params - the number of parameters passed to the function
  * 
  * @returns an int indicating the success (0) or failure (-1) of the method
  */
int touch(char **params, int num_params);

#endif
