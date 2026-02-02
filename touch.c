#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int touch(char *filepath) {
  int result;
  if ((result = access(filepath, F_OK)) < 0) {
    if (errno == ENOENT) {
      // requested file does not exist, ok to create
      if ((result = creat(filepath, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) < 0) {
        // file creation failed
        perror("Create");
        return result;
      }
    } else {
      printf("%d\n", errno);
      perror("Access");
      return result;
    }
  } else if ((result = utimensat(AT_FDCWD, filepath, NULL, 0)) != 0) {
    perror("Utimensat");
    return result;
  }
  return 0;
}
