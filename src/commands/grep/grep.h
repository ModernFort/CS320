#ifndef GREP_H
#define GREP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "grep_util/grep_state.h"
#include "grep_functions/grep_matching.h"
#include "grep_util/grep_util.h"

//Search a file for an extended regex and print lines that match to stdout
int match_pattern(FILE* fp, const char* pattern_to_match);

grep_state get_state(int paramc, char** params);

//Match text from all files according to the state of the command
void match_files_txt(grep_state *state);

//TODO: Implement match functionality for things other than file pointers, like piped output through dynamically created files (EG through cat)
#endif