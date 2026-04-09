#include "grep_txt_matching.h"

int contains_text(char* line, const char* str_to_match){
    //Use strstr to check the line for the substring, returning 0 if the string
    //is not found, in which case strstr returns NULL.
    return strstr(line, str_to_match) != NULL;
}

void match_file_txt(grep_state* state, FILE* fp, char* path){
    //Initialize a line buffer and cap for the getline wrapper
    char* line = NULL;
    size_t cap = 0;

    //Initialize a variable to represent the amount of matching/nonmatching files depending on settings
    int file_matching = 0;

    //Read each line of the file using the getline wrapper, terminating the loop on EOF.
    while(read_next_line(fp, &line, &cap) != -1){

        //Call the helper to see if the line matches any of the patterns. If invert match is selected,
        //flip the flag to indicate if non-matching or matching.
        int matched = match_line_txt(state, line);
        if(state->invert_match) matched = !matched;
        //If the line isn't selectible, continue
        if(!matched) continue;
        
        file_matching++;
        update_totals(state, path, file_matching);

        //If count only or path printing of files with/without matches are not selected, print
        //the line and continue.
        if(is_printable(state)) {
            printf("%s", line);
            continue;
        }

        //If max count is selected and has been reached, print and exit
        if(file_matching == state->max_count){
            printf("Max count of matches for file %s reached\n", path);
            printf("Maximum allowed matches: %ld\n", state->max_count);
            free(line);
            return;
        }
    }
    
    //If no matches were found and files without matches are desired, store the path and update the count
    if(!file_matching && state->files_without_matches) 
        state->totals._files_without_matches[state->totals._files_without_matches_count++] = path;

    free(line);
}

int match_line_txt(grep_state* state, const char* line){

    char *lower_line_buf = NULL;
    //Convert the line to lowercase if ignore_case is selected
    if(state->ignore_case){
        lower_line_buf = lower_line(line);
        line = lower_line_buf;
    }

    //Store the array of patterns and the amount of patterns
    char** patterns = state->pattern_info.patterns;
    int patternc = state->pattern_info.pattern_count;

    //Iterate over each pattern
    for(int i = 0; i < patternc; i++){
        const char* curr_pattern = patterns[i];
        char *lower_pattern_buf = NULL;

        //Convert the pattern to lowercase if ignore_case is selected
        if(state->ignore_case){
            lower_pattern_buf = lower_line(curr_pattern);
            curr_pattern = lower_pattern_buf;
        }

        //If the pattern is contained in the line, free both buffers and return true
        if(contains_text((char*)line, curr_pattern)){
            free(lower_pattern_buf);
            free(lower_line_buf);
            return 1;
        }

        free(lower_pattern_buf);
    }
    
    //The pattern was not contained, return 0.
    free(lower_line_buf);
    return 0;
}

void update_totals(grep_state* state, char* path, int file_matching){
    //If this is the first match found, store the path of the file in the files with matches array
    if(file_matching == 1)
        state->totals._files_with_matches[state->totals._files_with_matches_count++] = path;

    //Update the total count of matches
    state->totals.total_matches++;
}

int is_printable(grep_state *state){
    return(
        !state->count_only &&
        !state->files_with_matches &&
        !state->files_without_matches
    );
}