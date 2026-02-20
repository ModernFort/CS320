#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <errno.h>
#include <string.h>

//Delimiters to use for tokenizing user input, exclusively tokenizes on whitespace.
#define TOKEN_DELIMS " \t\n"
//Max length of a user command, including the path max plus a 64-byte buffer for the command itself.
#define COMMAND_MAX PATH_MAX + 64
//Maximum amount of parameters that can be used for a command. Including flags, files, etc.
#define MAX_PARAMS 64
//Optional debug flag to enable debugging output
#define DEBUG_FLAG "-d"

int main(int argc, char *argv[]) {
  char command_buf[COMMAND_MAX];

  //Check for any flags and enable debug mode if the debug flag is recognized.
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
    if (fgets(command_buf, COMMAND_MAX, stdin) == NULL) {
      if (feof(stdin)) {
          printf("End of input received, exiting.\n");
          exit(0);
      } else {
          perror("fgets");
          exit(EXIT_FAILURE);
      }
    } else {
      char* cmd_token = strtok(command_buf, TOKEN_DELIMS);
      if(!cmd_token) {
        if(debug_mode) printf("DEBUGGER: No command entered. Continuing...\n");
        continue;
      }

      //Find all parameters of the user command (separated by whitespace) and put them in a null-terminated array.
      char* cmd_params[MAX_PARAMS];
      int param_idx = 0;
      char* param = strtok(NULL, TOKEN_DELIMS);
      while(param != NULL && param_idx < MAX_PARAMS){
        cmd_params[param_idx] = param;
        param = strtok(NULL, TOKEN_DELIMS);
        param_idx++;
      }
      if(param_idx < MAX_PARAMS) cmd_params[param_idx] = NULL;

      if(debug_mode) {
        printf("DEBUGGER: Command %s entered ", cmd_token);
        if(param_idx > 0){
          printf("with params: ");
          for(int i = 0; i < param_idx; i++){
            printf("%s ", cmd_params[i]);
          }
          printf("\n");
        }
      }
      int paramsc = param_idx;
      
      printf("params: %d\n", paramsc);
      if(strcmp(cmd_token, "grep") == 0){
        //Call grep and pass params to process the command, validate input within grep and continue if grep returns an error. Print the error in main.
        //Some pseudocode as an example:
        
        // int grep_result = grep(params);
        // if(grep_result != 0){
        //   print_error(grep_result);
        // }
        // continue;

      }
      // add your commands as else if (condition) to trigger appropriate libraries
    }
  }
}
