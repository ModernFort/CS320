#ifndef TEXT_MATCHING_H
#define TEXT_MATCHING_H

#include <stdio.h>
#include <stdlib.h>
#include "../grep_util/grep_state.h"
#include "../grep_util/grep_util.h"

//Maximum length a line of a file can be, equivalent to the max page size on most systems.
#define MAX_LINE_LEN 4096

//Check match for a single line of a file according to grep's settings. Iterates over
//all patterns stored in the state struct and returns 1 if a match is found, 0 otherwise.
int line_has_match(const char* line, const grep_state* state);

//Simple util function that takes a line from a file and uses a sliding window to determine if the line
//Contains the given string. returns 1 if the string is contained, 0 if not (for easy boolean checks)
int contains_text(char* line, const char* str_to_match);

#endif