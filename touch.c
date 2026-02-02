#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>

int touch(char *filepath) {
  int result;
  if ((result = utimensat(AT_FDCWD, filepath, NULL, 0)) != 0) {
    perror("Error:");
    return result;
  }
  return 0;
}
