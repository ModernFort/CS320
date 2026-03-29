#ifndef GREP_H
#define GREP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "grep_util/grep_state.h"
#include "grep_functions/text_matching.h"

//Search a file for text and print lines that match to stdout
int match_text(FILE* fp, const char* str_to_match);

//Search a file for an extended regex and print lines that match to stdout
int match_pattern(FILE* fp, const char* pattern_to_match);

//TODO: Implement match functionality for things other than file pointers, like piped output through dynamically created files (EG through cat)
#endif