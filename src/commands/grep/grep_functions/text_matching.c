#include "text_matching.h"

int match_text(FILE* fp, const char* str_to_match, grep_state current_state){
    //Create a buffer for fgets to store the line of the file
    char buf[MAX_LINE_LEN];

    int found_match = 0;

    //Iterate over each line of the file until EOF is reached
    while(fgets(buf, MAX_LINE_LEN, fp) != NULL){
        int len = strlen(buf);
        //If the text is contained, print the entire line (grep's default functionality)
        if(contains_text(buf, str_to_match)){
            found_match = 1;
            printf("%s", buf);
            //If the line in the file somehow didn't contain a newline, manually add one.
            if(strlen(buf) != 0 && buf[len - 1] != '\n') putchar('\n');
        }
    }

    return found_match;
}

int contains_text(char* line, const char* str_to_match){
    //Use strstr to check the line for the substring, returning 0 if the string
    //is not found, in which case strstr returns NULL.
    return strstr(line, str_to_match) != NULL;
}