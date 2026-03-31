#ifndef GREP_MATCHING_H
#define GREP_MATCHING_H

#include <stdio.h>
#include <stdlib.h>
#include "../grep_util/grep_state.h"
#include "../grep_util/grep_util.h"

//Maximum length a line of a file can be, equivalent to the max page size on most systems.
#define MAX_LINE_LEN 4096

//Simple util function that takes a line from a file and uses a sliding window to determine if the line
//Contains the given string. returns 1 if the string is contained, 0 if not (for easy boolean checks)
int contains_text(char* line, const char* str_to_match);

//Process/match an entire file, modifying and using state settings and an open file pointer
//to do so. Include the path of the file so it can be added to the array of files with/without
//matches as appropriate
void match_file_txt(grep_state* state, FILE* fp, char* path);

//Use grep settings to process and match a file line according to state settings, returning the
//value of getline to show EOF has occured
int match_line_txt(grep_state* state, const char* line);

//Helper that updates the running totals with paths/counts based on the inverted state
void update_totals(grep_state* state, const char* path, int file_matching);

//Helper that indicates if a line can be printed, which sometimes isn't wanted due to state settings
int is_printable(grep_state *state);


#endif