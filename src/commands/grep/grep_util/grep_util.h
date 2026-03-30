#ifndef GREP_UTIL_H
#define GREP_UTIL_H

#include <stdio.h>
#include <stdlib.h>

//All uppercase ascii chars are 32 less than their lowercase equivalants, so I define this macro
//for easy conversion.
#define TO_LOWER(x) (x + 32)

//Takes a string as an argument and returns the version with all lowercase, used if ignoring case when checking match.
char* lower_line(const char* line);

//Helper util that wraps getline, calling getline with an external file, line buffer, and max bytes
//and handling stripping the newline and replacing with a null terminator to convert the line
//to a null-terminated string.
size_t read_next_line(FILE* fp, char** line, size_t* cap);

//Open the file desired by the user, returning the open file pointer
FILE* open_file(char* user_file);


#endif