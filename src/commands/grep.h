#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#ifndef GREP_H
#define GREP_H

//Returns an array of the valid flags that can be used with grep, to be used for param validation.
//Includes both the long and short versions of each flag.
const char** get_valid_flags();

//Takes an array of strings that represent the parameters the user used when using grep. Returns 0 if invalid params are
//given, 1 if all params are valid.
int validate_params(const char** params);

//Takes an array of strings representing arguments the user used when calling grep and parses them, setting relevant
//global flags/variables like files and matching strings to be used in text/pattern matching later.
int parse_args(const char** args);

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