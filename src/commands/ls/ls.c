#include "ls.h"

int ls(char *path) {
  if (path == NULL) {
    path = ".";
  }
  struct dirent *entry;
  DIR *dp = opendir(path);

  if (dp == NULL) {
    perror("opendir");
    return -1;
  }

  while ((entry = readdir(dp)) != NULL) {
      printf("%s\n", entry->d_name);
  }

  closedir(dp);
  return 0;
}
