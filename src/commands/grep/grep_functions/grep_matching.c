#include "grep_matching.h"

int contains_text(char* line, const char* str_to_match){
    //Use strstr to check the line for the substring, returning 0 if the string
    //is not found, in which case strstr returns NULL.
    return strstr(line, str_to_match) != NULL;
}