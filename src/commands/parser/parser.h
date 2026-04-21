#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <stdlib.h>
#include <linux/limits.h>
#include <errno.h>
#include <string.h>
#include "../touch/touch.h"
#include "../ls/ls.h"
#include "../manpage/manpage.h"
#include "../echo/echo.h"
#include "../rawMode/raw.h"
#include "../grep/grep.h"

//Delimiters to use for tokenizing user input, exclusively tokenizes on whitespace.
#define TOKEN_DELIMS " \t\n"
//Max length of a user command, including the path max plus a 64-byte buffer for the command itself.
#define COMMAND_MAX PATH_MAX + 64
//Maximum amount of parameters that can be used for a command. Including flags, files, etc.
#define MAX_PARAMS 64
//Optional debug flag to enable debugging output
#define DEBUG_FLAG "-d"

/**
  * method returns 1 or 0 to indicate whether or not the debug flag should be set
  *
  * @param argc - the number of arguments passed to main
  * @param flag - the value of argv[1]
  *
  * @returns 1 or 0 if the debug flag has or has not been specified
  * @warning DEPRECATED
  */
//int set_debug(int argc, char *flag);

/**
  * method prompts for input from user and stores in command_buf
  *
  * @param command_buf - a pointer to the storage location for the received input
  *
  * @returns 0, 1, or the negative of an errno to indicate whether the method reached the 
  * end of input, succeeded, or encountered an error, respectively.
  * @warning DEPRECATED
  */
//int get_input(char *command_buf);


/**
  * method parse command_buf to obtain first token, stored in cmd_token
  *
  * @param command_buf - a pointer to the string containing user input
  * @param cmd_token - a pointer to the storage location for the first command token
  *
  * @returns 1 or 0 to indicate success or failure
  */
void parse_command(char *command_buf, char **cmd_token);

/**
  * @brief takes in context struct and runs it's command
  *
  * @param c context of terminal 
  */
void parser_main(Context *c);

int grep_main(int argc, char** argv);

#endif