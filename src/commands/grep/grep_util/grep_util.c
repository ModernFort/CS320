#include "grep_util.h"

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
        fprintf(stderr, "Could not open file: %s\n", user_file);
        exit(EXIT_FAILURE);
    }

    return fp;
}