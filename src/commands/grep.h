#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#ifndef GREP_H
#define GREP_H

//Enum ensuring types of matches remain mutually exclusive, so multiple can't be
//selected at once.
typedef enum {
    BASIC_REGEX,
    EXTENDED_REGEX,
    PLAIN_TEXT
} match_type;

//Struct that stores information about the current state of the grep command, like the type
//of match, selected flags, match count, etc.
typedef struct {
    //Can be either extended/basic regex or plain text, but not multiple
    match_type mode;

    //Boolean flags representing output options of grep
    int ignore_case;
    int invert_match;
    int suppress_errs;
    int only_matching;
    int count_only;
    int files_with_matches;
    int files_without_matches;

    //Pattern/file arguments passed at runtime alongside their counts
    char** patterns;
    int pattern_count;
    char** file_paths;
    int file_count;

    //If max count flag is selected, this stores the maximum matches that can be counted before stopping
    int max_count;

} grep_state;

//Checks if an individual flag is present in the array of valid flags.
//Returns 1 if it exists, 0 if not.
int flag_valid(const char* param);

//Takes an array of strings that represent the parameters the user used when using grep. Returns 0 if invalid params are
//given, 1 if all params are valid. The amount of params must also be passed (in similar fashion to main) so the param
//array can be accurately iterated over.
int validate_params(int paramc, const char** params);

//Takes an array of strings representing arguments and flags the user passed when calling grep, and creates a new
//grep state struct based on these args. Validates that flags contain arguments if required, and throws errors if not.
grep_state init_state(int argc, const char** args);

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