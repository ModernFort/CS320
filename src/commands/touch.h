#ifndef TOUCH_H
#define TOUCH_H

/**
  * method implements basic functionality of bash command 'touch'
  * Supported options are -a and -m
  *
  * @param filepath - a pointer to the path to touch
  * @param opts - a pointer to a string containing the options specified by the user;
  *   if NULL or empty, behavior matches -am
  * 
  * @returns an int indicating the success or failure of the method
  */
int touch(char *filepath, char *opts);

#endif
