#include "grep.h"

//Maximum length a line of a file can be, equivalent to the max page size on most systems.
#define MAX_LINE_LEN 4096

int match_pattern(FILE* fp, const char* pattern_to_match){

}

//Call the helper to initialize flag state from user args. The helper handles errors
//With argument entry/parsing
grep_flags_state get_state(int paramc, char** params){
    return init_state(paramc, params);
}


void match_files_txt(grep_flags_state *flags){
}