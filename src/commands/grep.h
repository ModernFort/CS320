#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#ifndef GREP_H
#define GREP_H

//Open the file desired by the user, returning the open file pointer
FILE* open_file(char* user_file);

//Search a file for text and print lines that match to stdout
int match_text(FILE* fp, const char* str_to_match);

//Search a file for an extended regex and print lines that match to stdout
int match_pattern(FILE* fp, const char* pattern_to_match);

//TODO: Implement match functionality for things other than file pointers, like piped output through dynamically created files (EG through cat)
#endif