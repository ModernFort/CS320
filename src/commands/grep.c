#include "grep.h"

//All uppercase ascii chars are 32 less than their lowercase equivalants, so I define this macro
//for easy conversion.
#define TO_LOWER(x) (x + 32)

const char** get_valid_flags(){
    static const char *flags[] = {
        //Pattern matching flags
        "-E", "--extended-regexp", //Flags to indicate extended regex matching
        "-F", "--fixed-strings", //Indicates plain text matching
        "-G", "--basic-regexp", //Basic regex matching
        
        //Matching control flags
        "-e", "--regexp=", //Searches for multiple patterns
        "-f", "--file=", //Matches a specific file
        "-i", "--ignore-case", //Ignore case of pattern/input data
        "-v", "--invert-match", //Select only non-matching lines

        //Output control flags
        "-c", "--count", //Print a count of matching lines rather than normal output
        "-L", "--files-without-match", //Print the name of input files without matching text
        "-l", "--files-with-matches", //Print files that do contain matching text
        "-m", "--max-count=", //Stop reading a file after --max-count= n matching lines
        "-o", "--only-matching", //Print only the matching parts of a line seperate from the actual line
        "-s", "--no-messages" //Suppress error messages about unreadable/nonexistent files 
    };

    return(flags);
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

int contains_text(char* line, const char* str_to_match){
    //Use strstr to check the line for the substring, returning 0 if the string
    //is not found, in which case strstr returns NULL.
    return strstr(line, str_to_match) != NULL;
}