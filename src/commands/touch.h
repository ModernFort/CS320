#ifndef TOUCH_H
#define TOUCH_H

/**
  * method implements all functionality of bash command 'touch'
  *
  * @param filepath - a pointer to the path to touch
  * @param opts - an int, acting as an enum to specify options, where:
  *   - 0: no options or -am
  *   - 1: -a
  *   - 2: -m
  * 
  * @returns an int indicating the success or failure of the method
  */
int touch(char *filepath, unsigned short opts);

#endif
