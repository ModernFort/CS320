#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <errno.h>

int main(int argc, char *argv[]) {
  char command[PATH_MAX];

  while (1) {
    // read input
    printf("Enter a command: ");
    fflush(stdout);
    int result = scanf("%s", command);
    if (result == EOF) {
      // EOF received
      if (ferror(stdin) != 0) {
        // read error
        perror("Scanf");
        exit(errno);
      }
      // True EOF
      printf("End of input received, exiting.\n");
      exit(0);
    } else if (result < 1) {
      printf("Matching failed.\n");
    } else {
      printf("Command %s received.\n", command);
      // add your commands as else if (condition) to trigger appropriate libraries
    }
  }
}
