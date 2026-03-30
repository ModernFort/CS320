#ifndef TEXT_MATCHING_H
#define TEXT_MATCHING_H

#include <stdio.h>
#include <stdlib.h>

//Simple util function that takes a line from a file and uses a sliding window to determine if the line
//Contains the given string. returns 1 if the string is contained, 0 if not (for easy boolean checks)
int contains_text(char* line, const char* str_to_match);

#endif