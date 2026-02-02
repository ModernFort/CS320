#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int touch(char *filepath, int opts) {
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
  } else if (opts == 0) {
    // touch or touch -am or touch -ma
    if ((result = utimensat(AT_FDCWD, filepath, NULL, 0)) != 0) {
      perror("Utimensat");
      return result;
    }
  } else {
    struct timespec acc;
    struct timespec mod;

    if (opts == 1) {
      // touch -a
      acc.tv_nsec = UTIME_NOW;
      mod.tv_nsec = UTIME_OMIT;
    } else {
      // touch -m
      acc.tv_nsec = UTIME_OMIT;
      mod.tv_nsec = UTIME_NOW;
    }

    struct timespec times[] = {acc, mod};

    if ((result = utimensat(AT_FDCWD, filepath, times, 0)) != 0) {
      perror("Utimensat");
      return result;
    }
  }
  return 0;
}
