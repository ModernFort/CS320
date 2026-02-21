#include <grep.h>

//Open the desired file and error check, returning the file pointer on success.
FILE* open_file(char* user_file){
    FILE* fp;
    fp = fopen(user_file, "r");
    if(fp == NULL){
        fprintf(stderr, "Could not open file: %s", user_file);
        exit(EXIT_FAILURE);
    }

    return fp;
}

int match_text(FILE* fp, const char* str_to_match){
    
}

int match_pattern(FILE* fp, const char* pattern_to_match){

}