#include "text_matching.h"

char* match_next_line(FILE* fp, const char* str_to_match, int ignore_case){
    //Create a buffer for fgets to store the line of the file
    char buf[MAX_LINE_LEN];

    //Match the next line of the file pointer, unless EOF
    if(fgets(buf, MAX_LINE_LEN, fp) != NULL){
        int len = strlen(buf);
        //If the text is contained, store the line and return it
        if(contains_text(buf, str_to_match)){
            char* match_line = buf;
        }
    }

    return buf;
}



int contains_text(char* line, const char* str_to_match){
    //Use strstr to check the line for the substring, returning 0 if the string
    //is not found, in which case strstr returns NULL.
    return strstr(line, str_to_match) != NULL;
}