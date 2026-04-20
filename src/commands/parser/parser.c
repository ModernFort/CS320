#include "parser.h"

// int debug_mode = 0;

// ------------------ DEPRECATED ------------------- //
// int set_debug(int argc, char *flag) {
//   if (argc > 1) {
//     if (strcmp(flag, DEBUG_FLAG) == 0) {
//       printf("Debugger enabled!\n");
//       debug_mode = 1;
//       return 1;
//     } else {
//       fprintf(stderr, "Error: flag \"%s\" not recognized\n"
//               "Run \"./main -d\" to enable debugger output\n", flag);
//       exit(EXIT_FAILURE);
//     }
//   }
//   debug_mode = 0;
//   return 0;
// }

// also deprecated, but keeping here so I don't delete other peoples work
// int get_input(char *command_buf) {
//   // read input
//   printf("Enter a command: ");
//   fflush(stdout);
//   if (fgets(command_buf, COMMAND_MAX, stdin) == NULL) {
//     // input read failed
//     if (feof(stdin)) {
//       // EOF received
//       printf("End of input received, exiting.\n");
//       return 0;
//     }
//     perror("fgets");
//     return -errno;
//   } else {
//     return 1;
//   }
// }

// could be depreceted as well, but keeping for now
void parse_command(char *command_buf, char **cmd_token) {
  *cmd_token = strtok(command_buf, TOKEN_DELIMS);
  // if (!(*cmd_token)) {
  //   // parse failed
  //   if (debug_mode) {
  //     printf("DEBUGGER: No command entered. Continuing...\n");
  //   }
  // } else if (debug_mode) {
  //   printf("DEBUGGER: command received '%s'\n", *cmd_token);
  // }
}

void parser_main(Context *context) {
  char command_buf[COMMAND_MAX] = {0};
  strncpy(command_buf, context->cmd, COMMAND_MAX - 1);


  char *cmd_token = NULL;
  parse_command(command_buf, &cmd_token);
  if (cmd_token == NULL) {
    // parse failed
    return;
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
  // if(debug_mode) {
  //   printf("DEBUGGER: Command %s entered ", cmd_token);
  //   if (param_idx > 0) {
  //     printf("with params: ");
  //     for (int i = 0; i < param_idx; i++) {
  //       printf("%s ", cmd_params[i]);
  //     }
  //     printf("\n");
  //   }
  // }
  int paramsc = param_idx;
  
  //printf("params: %d\n", paramsc);
  printf("\n");
  if (strcmp(cmd_token, "grep") == 0) {
    //Call grep and pass params to process the command, validate input within grep and continue if grep returns an error. Print the error in main.
    //Some pseudocode as an example:
    
    // int grep_result = grep(params);
    // if(grep_result != 0){
    //   print_error(grep_result);
    // }
    return;
  }
  if (strcmp(cmd_token, "touch") == 0) {
    if (paramsc == 0) {
      // no path provided
      printf("ERROR: You must specify a filepath.\n");
    } else {
      touch(cmd_params, paramsc);
    }
    return;
  }
  if (strcmp(cmd_token, "ls") == 0) {
    if (paramsc == 0) {
      ls(NULL);
    } else {
      ls(cmd_params[0]);
    }
    return;
  }
  if (strcmp(cmd_token, "man") == 0) {
    // if there is a value to search the manpage for
    // request_manpage(query)
    // if there isnt a prompt to search the manpage for
    // request_manpage(NULL)
  }
  if (strcmp(cmd_token, "echo") == 0) {
    echo(cmd_params, paramsc);
    return;
  }

  if (strcmp(cmd_token, "history") == 0) {
    history(cmd_params, paramsc, context->hist);
    return;
  }

  printf("command not found: %s\n", cmd_token);
}
//exit(0);
