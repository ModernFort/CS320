#ifndef TEXT_MATCHING_H
#define TEXT_MATCHING_H

#include <stdio.h>
#include <stdlib.h>
#include "../grep_util/grep_state.h"

//Maximum length a line of a file can be, equivalent to the max page size on most systems.
#define MAX_LINE_LEN 4096

//Search a file for text and print lines that match to stdout
int match_text(FILE* fp, const char* str_to_match, int ignore_case);

//Simple util function that takes a line from a file and uses a sliding window to determine if the line
//Contains the given string. returns 1 if the string is contained, 0 if not (for easy boolean checks)
int contains_text(char* line, const char* str_to_match);

#endif