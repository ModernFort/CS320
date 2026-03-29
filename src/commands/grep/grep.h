#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#ifndef GREP_H
#define GREP_H

//Checks if an individual flag is present in the array of valid flags.
//Returns 1 if it exists, 0 if not.
int flag_valid(const char* param);

//Takes an array of strings that represent the parameters the user used when using grep. Returns 0 if invalid params are
//given, 1 if all params are valid. The amount of params must also be passed (in similar fashion to main) so the param
//array can be accurately iterated over.
int validate_params(int paramc, char** params);

//Takes an array of strings representing arguments and flags the user passed when calling grep (not including the grep
//command itself) and creates a new grep state struct based on these args. Validates that flags contain arguments if 
//required, and throws errors if not.
grep_state init_state(int grep_argc, char** args);

//Takes a string as an argument and returns the version with all lowercase, used if ignoring case when checking match.
char* lower_line(const char* line);

//Open the file desired by the user, returning the open file pointer
FILE* open_file(char* user_file);

//Search a file for text and print lines that match to stdout
int match_text(FILE* fp, const char* str_to_match);

//Search a file for an extended regex and print lines that match to stdout
int match_pattern(FILE* fp, const char* pattern_to_match);

//Simple util function that takes a line from a file and uses a sliding window to determine if the line
//Contains the given string. returns 1 if the string is contained, 0 if not (for easy boolean checks)
int contains_text(char* line, const char* str_to_match);

//TODO: Implement match functionality for things other than file pointers, like piped output through dynamically created files (EG through cat)
#endif