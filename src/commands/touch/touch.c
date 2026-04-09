#include "touch.h"

#define _POSIX_C_SOURCE 200809L

/**
  * Method returns 0 or 1 to indicate whether an array of strings
  * contains a given string.
  *
  * @param arr - the array to search
  * @param size - the size of the array
  * @param target - the string for which to search in the array
  *
  * @returns 1 if the string is in the array, 0 otherwise
  */
int contains_string(char **arr, int size, const char *target) {
  for (int i = 0; i < size; i++) {
    if (strcmp(arr[i], target) == 0) {
      // Found
      return 1;
    }
  }
  // Not found
  return 0;
}

int standard(char *filepath) {
  // file exists, update access and modify times
  int result;
  if ((result = utimensat(AT_FDCWD, filepath, NULL, 0)) != 0) {
    perror("Utimensat");
    return result;
  }
  return 0;
}

int access_only(char *filepath) {
  struct timespec acc, mod;
  acc.tv_nsec = UTIME_NOW;
  mod.tv_nsec = UTIME_OMIT;
  struct timespec times[] = {acc, mod};

  int result;
  if ((result = utimensat(AT_FDCWD, filepath, times, 0)) != 0) {
    perror("Utimensat");
    return result;
  }
  return 0;
}

int mod_only(char *filepath) {
  struct timespec acc, mod;
  acc.tv_nsec = UTIME_OMIT;
  mod.tv_nsec = UTIME_NOW;
  struct timespec times[] = {acc, mod};

  int result;
  if ((result = utimensat(AT_FDCWD, filepath, times, 0)) != 0) {
    perror("Utimensat");
    return result;
  }
  return 0;
}

/**
  * method implements basic functionality of bash command 'touch'
  * Supported options are -a, -m, and any combination of the two
  *
  * @param params - a pointer to the parameters
  * @param num_params - the number of parameters passed to the function
  * 
  * @returns an int indicating the success (0) or failure (-1) of the method
  */
int touch(char **params, int num_params) {
  for (int i = 0; i < num_params - 1; i++) {
    // validate option parameters
    if (strcmp(params[i], "-a") != 0 &&
        strcmp(params[i], "-m") != 0 &&
        strcmp(params[i], "-ma") != 0 &&
        strcmp(params[i], "-am") != 0) {
      fprintf(stderr, "touch: invalid option -- '%s'\n", params[i]);
      return -1;
    }
  }
  int result;
  if ((result = access(params[num_params - 1], F_OK)) < 0) {
    if (errno == ENOENT) {
      // requested file does not exist, ok to create
      if ((result = creat(params[num_params - 1], S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH)) < 0) {
        // file creation failed
        perror("Create");
        return result;
      }
      return 0;
    }
    printf("%d\n", errno);
    perror("Access");
    return result;
  }
  if (contains_string(params, num_params - 1, "-am") ||
      contains_string(params, num_params - 1, "-ma") ||
      (contains_string(params, num_params - 1, "-a") &&
      contains_string(params, num_params - 1, "-m"))) {
    // option -ma, -am, or -a and -m
    return standard(params[num_params - 1]);
  } else if (contains_string(params, num_params - 1, "-a")) {
    return access_only(params[num_params - 1]);
  } else {
    return mod_only(params[num_params - 1]);
  }
}
