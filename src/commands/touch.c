#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

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
int touch(char *filepath, char *opts) {
  if (opts == NULL || strcmp(opts, "") == 0) {
    return default(char *filepath);
  }
  if (strcmp(opts, "-a") == 0) {
    return access_only(filepath);
  }
  if (strcmp(opts, "-m") == 0) {
    return mod_only(filepath);
  } else {
    fprintf(stderr, "touch: invalid option -- '%s'\n", opts);
  }
}

int default(char *filepath) {
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
  } else {
    // file exists, just update access and modify times
    if ((result = utimensat(AT_FDCWD, filepath, NULL, 0)) != 0) {
      perror("Utimensat");
      return result;
    }
  }
  return 0;
}

int access_only(char *filepath) {
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
  } else {
    // file exists, just update access time
    struct timespec acc, mod;
    acc.tv_nsec = UTIME_NOW;
    mod.tv_nsec = UTIME_OMIT;
    struct timespec times[] = {acc, mod};

    if ((result = utimensat(AT_FDCWD, filepath, times, 0)) != 0) {
      perror("Utimensat");
      return result;
    }
  }
  return 0;
}

int mod_only(char *filepath) {
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
  } else {
    // file exists, just update access time
    struct timespec acc, mod;
    acc.tv_nsec = UTIME_OMIT;
    mod.tv_nsec = UTIME_NOW;
    struct timespec times[] = {acc, mod};

    if ((result = utimensat(AT_FDCWD, filepath, times, 0)) != 0) {
      perror("Utimensat");
      return result;
    }
  }
  return 0;

}
