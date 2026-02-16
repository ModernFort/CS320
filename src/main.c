#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <errno.h>
#include <string.h>

//Delimiters to use for tokenizing user input, exclusively tokenizes on whitespace.
#define TOKEN_DELIMS " \t\n"
//Max length of a user command, including the path max plus a 64-byte buffer for the command itself.
#define COMMAND_MAX PATH_MAX + 64
//Optional debug flag to enable debugging output
#define DEBUG_FLAG "-d"

int main(int argc, char *argv[]) {
  char command_buf[COMMAND_MAX];

  int debug_mode = 0;
  if(argc > 1){
    if(strcmp(argv[1], DEBUG_FLAG) == 0){
      debug_mode = 1;
      printf("Debugger enabled!\n");
    } else {
      fprintf(stderr, "Error: flag \"%s\" not recognized\nRun \"./main -d\" to enable debugger output\n", argv[1]);
      exit(EXIT_FAILURE);
    }
  }

  while (1) {
    // read input
    printf("Enter a command: ");
    fflush(stdout);
    int result = scanf("%s", command_buf);
    if (result == EOF) {
      // EOF received
      if (ferror(stdin) != 0) {
        // read error
        perror("Scanf");
        exit(errno);
      }

      char* cmd_token = strtok(command_buf, TOKEN_DELIMS);
      if(!cmd_token) continue;
      

      // True EOF
      printf("End of input received, exiting.\n");
      exit(0);
    } else if (result < 1) {
      printf("Matching failed.\n");
    } else {
      printf("Command %s received.\n", command_buf);
      // add your commands as else if (condition) to trigger appropriate libraries
    }
  }
}
