#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <errno.h>
#include <string.h>
#include "commands/touch.h"

//Delimiters to use for tokenizing user input, exclusively tokenizes on whitespace.
#define TOKEN_DELIMS " \t\n"
//Max length of a user command, including the path max plus a 64-byte buffer for the command itself.
#define COMMAND_MAX PATH_MAX + 64
//Maximum amount of parameters that can be used for a command. Including flags, files, etc.
#define MAX_PARAMS 64
//Optional debug flag to enable debugging output
#define DEBUG_FLAG "-d"

int debug_mode = 0;
/**
  * method returns 1 or 0 to indicate whether or not the debug flag should be set
  *
  * @param argc - the number of arguments passed to main
  * @param flag - the value of argv[1]
  *
  * @returns 1 or 0 if the debug flag has or has not been specified
  */
int set_debug(int argc, char *flag) {
  if (argc > 1) {
    if (strcmp(flag, DEBUG_FLAG) == 0) {
      printf("Debugger enabled!\n");
      debug_mode = 1;
      return 1;
    } else {
      fprintf(stderr, "Error: flag \"%s\" not recognized\n"
              "Run \"./main -d\" to enable debugger output\n", flag);
      exit(EXIT_FAILURE);
    }
  }
  debug_mode = 0;
  return 0;
}

/**
  * method prompts for input from user and stores in command_buf
  *
  * @param command_buf - a pointer to the storage location for the received input
  *
  * @returns 0, 1, or the negative of an errno to indicate whether the method reached the 
  * end of input, succeeded, or encountered an error, respectively.
  */
int get_input(char *command_buf) {
  // read input
  printf("Enter a command: ");
  fflush(stdout);
  if (fgets(command_buf, COMMAND_MAX, stdin) == NULL) {
    // input read failed
    if (feof(stdin)) {
      // EOF received
      printf("End of input received, exiting.\n");
      return 0;
    }
    perror("fgets");
    return -errno;
  } else {
    return 1;
  }
}

/**
  * method parse command_buf to obtain first token, stored in cmd_token
  *
  * @param command_buf - a pointer to the string containing user input
  * @param cmd_token - a pointer to the storage location for the first command token
  *
  * @returns 1 or 0 to indicate success or failure
  */
void parse_command(char *command_buf, char **cmd_token) {
  *cmd_token = strtok(command_buf, TOKEN_DELIMS);
  if (!(*cmd_token)) {
    // parse failed
    if (debug_mode) {
      printf("DEBUGGER: No command entered. Continuing...\n");
    }
  } else if (debug_mode) {
    printf("DEBUGGER: command received '%s'\n", *cmd_token);
  }
}

int main(int argc, char **argv) {
  char command_buf[COMMAND_MAX];

  //Check for any flags and enable debug mode if the debug flag is recognized.
  set_debug(argc, argv[1]);

  while (1) {
    int result = get_input(command_buf);
    if (result <= 0) {
      exit(-result);
    }

    char *cmd_token = NULL;
    parse_command(command_buf, &cmd_token);
    if (cmd_token == NULL) {
      if (debug_mode) {
        printf("DEBUGGER: cmd_token is null\n");
      }
      // parse failed
      continue;
    }

    //Find all parameters of the user command (separated by whitespace) and put them in a null-terminated array.
    char* cmd_params[MAX_PARAMS];
    int param_idx = 0;
    char* param = strtok(NULL, TOKEN_DELIMS);
    while (param != NULL && param_idx < MAX_PARAMS) {
      cmd_params[param_idx] = param;
      param = strtok(NULL, TOKEN_DELIMS);
      param_idx++;
    }
    if (param_idx < MAX_PARAMS) {
      cmd_params[param_idx] = NULL;
    }

    if(debug_mode) {
      printf("DEBUGGER: Command %s entered ", cmd_token);
      if (param_idx > 0) {
        printf("with params: ");
        for (int i = 0; i < param_idx; i++) {
          printf("%s ", cmd_params[i]);
        }
        printf("\n");
      }
    }
    int paramsc = param_idx;
    
    printf("params: %d\n", paramsc);
    if (strcmp(cmd_token, "grep") == 0) {
      //Call grep and pass params to process the command, validate input within grep and continue if grep returns an error. Print the error in main.
      //Some pseudocode as an example:
      
      // int grep_result = grep(params);
      // if(grep_result != 0){
      //   print_error(grep_result);
      // }
      // continue;

    }
    if (strcmp(cmd_token, "touch") == 0) {
      if (paramsc == 0) {
        // no path provided
        printf("ERROR: You must specify a filepath.\n");
      } else {
        touch(cmd_params, paramsc);
      }
    }
    // add your commands as else if (condition) to trigger appropriate libraries
  }
  exit(0);
}
