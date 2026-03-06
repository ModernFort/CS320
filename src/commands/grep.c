#include "grep.h"

//All uppercase ascii chars are 32 less than their lowercase equivalants, so I define this macro
//for easy conversion.
#define TO_LOWER(x) (x + 32)

char** get_valid_flags(){

}

int validate_params(const char** params){

}

int parse_args(const char** args){

}

char* lower_line(const char* line){
    //Find the length of the line and allocate a new char*
    //with this length to store the lowercase version
    int len = strlen(line);
    char* lowered = malloc((len + 1) * sizeof(char));

    //Loop thru the line. If a char is uppercase, convert to lower
    //and add it. Otherwise, add it as is.
    for(int i = 0; i < len; i++){
        char c = line[i];
        if(isupper(c)){
            lowered[i] = TO_LOWER(c);
        } else {
            lowered[i] = c;
        }
    }
    
    return lowered;
}

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

int contains_text(const char* line, const char* str_to_match){
    //If the line itself is smaller in length than the string to be checked,
    //return 0 automatically.
    int line_len = strlen(line);
    int match_len = strlen(str_to_match);

    if(line_len < match_len){
        return 0;
    }

    int start = 0;
    int end = match_len;

    while(end <= line_len){
        for(int ch = 0; ch < match_len; ch++){
            int matching = 0;
            //Check the line from the current window start + current character of the window and compare it to
            //The equivalent char in the string to match. Continue if the chars don't match. 
            if(line[start + ch] != str_to_match[ch]){
                continue;
            } else {
                matching++;
                //If the end of the window has been reached and all chars matched, return success.
                if(matching == match_len) return 1;
            }
        }
        //Slide the window by incrementing both start and end, and check again.
        start++;
        end++;
    }
    return 0;
}