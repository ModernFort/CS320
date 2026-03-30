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

    char** patterns = state->pattern_info.patterns;
    int pattern_count = state->pattern_info.pattern_count;

    FILE* current_fp;

    for(int i = 0; i < file_count; i++){
        //Open the file and initialize a variable to store the cap for the getline wrapper
        //and the line itself
        current_fp = open_file(files[i]);
        char *line = NULL;
        size_t cap = 0;

        //Read each line of the file until EOF is returned
        while(read_next_line(current_fp, &line, &cap) != -1){
            //Iterate over the patterns of the array and compare each to each
            //line of the file
            for(int j = 0; j < pattern_count; j++){
                if(contains_text(line, patterns[j])){
                    printf("%s", line);
                    break;
                }
            }
        }

        free(line);
        fclose(current_fp);
    }

}