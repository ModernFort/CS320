#include "grep.h"

//Maximum length a line of a file can be, equivalent to the max page size on most systems.
#define MAX_LINE_LEN 4096

int match_text(FILE* fp, const char* str_to_match){
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

int match_pattern(FILE* fp, const char* pattern_to_match){

}