#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

int main(int argc, char *argv[]) {
  printf("Enter a command: ");
  fflush(stdout);
  char command[PATH_MAX];
  int result = scanf("%s", command);
  if (result == EOF) {
    if (ferror(stdin) != 0) {
      perror("Scanf");
      exit(errno);
    }
    printf("End of input received, exiting.\n");
    exit(0);
  } else if (result < 1) {
    printf("Matching failed.\n");
  }
  printf("Hello World!\n");
  exit(0);
}
