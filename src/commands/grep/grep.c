#include "grep.h"

//Maximum length a line of a file can be, equivalent to the max page size on most systems.
#define MAX_LINE_LEN 4096

int match_pattern(FILE* fp, const char* pattern_to_match){

}

//Call the helper to initialize flag state from user args. The helper handles errors
//With argument entry/parsing
grep_state get_state(int paramc, char** params){
    return init_state(paramc, params);
}


void match_files_txt(grep_state *state){
    //Unpack file/pattern counts and arrays
    char** files = state->file_info.file_paths;
    int file_count = state->file_info.file_count;

    FILE* current_fp;

    for(int i = 0; i < file_count; i++){
        //Open the file and match it using the helper
        current_fp = open_file(files[i]);
        match_file_txt(state, current_fp, files[i]);
    }
}

int main(int argc, char** argv){
    char** grep_args = argv+1;
    grep_state state = get_state(argc-1, grep_args);
    match_files_txt(&state);
    return 0;
}