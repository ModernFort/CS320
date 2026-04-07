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
    
    lowered[len] = '\0';
    return lowered;
}

size_t read_next_line(FILE* fp, char** line, size_t* cap){
    //Call getline with the external pointers to get the amount of bytes read
    //to the line buffer, returning -1 if EOF
    size_t read = getline(line, cap, fp);
    if(read == -1) return -1;

    //Strip the newline at the end of the line and replace with null terminator
    //to turn the line into a string
    if(read > 0 && (*line)[read-1] == '\n' ) {
        (*line)[read-1] = '\0';
    }

    return read;
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