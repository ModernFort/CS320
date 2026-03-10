#include "grep.h"

//All uppercase ascii chars are 32 less than their lowercase equivalants, so I define this macro
//for easy conversion.
#define TO_LOWER(x) (x + 32)

//Maximum length a line of a file can be, equivalent to the max page size on most systems.
#define MAX_LINE_LEN 4096

//A comprehensive array of flags that my grep clone will recognize. Contains short and long
//versions of each flag.
static const char *VALID_FLAGS[] = {
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

//Compute the length of the valid flags array one time and store as a static global.
//Find the length by dividing the size of the array by the size of an entry to get total 
//amount of entries.
static const int VALID_FLAG_COUNT = 
    sizeof(VALID_FLAGS) / sizeof(VALID_FLAGS[0]);

int flag_valid(const char* param){
    //Iterate over the array of valid flags, if a match is found return 1.
    for(int i = 0; i < VALID_FLAG_COUNT; i++) {
        if(strcmp(param, VALID_FLAGS[i]) == 0) return 1;
    }
    //If the entire array has been searched and no match was found, return 0.
    return 0;
}

int validate_params(int paramc, const char** params){
    //Iterate over each param
    for(int i = 0; i < paramc; i++){
        //Find the current param and look for a dash at the start indicating it is a flag.
        //If it is, check to see if the flag is valid. If it isn't return 0.
        char* curr = params[i];
        if(curr[0] == '-'){
            if(!flag_valid(curr)) {
                fprintf(stderr, "Error: Parameter \"%s\" is not a valid flag!\n", curr);
                return 0;
            }
        }
    }

    //All detected flags were valid, return success.
    return 1;
}

grep_state init_state(int grep_argc, const char** args){
    if(grep_argc == 0 || grep_argc == 1){
        fprintf(stderr, "Usage: grep <OPTIONS>... PATTERNS [FILES]...\n");
        exit(EXIT_FAILURE);
    }
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
        fprintf(stderr, "Could not open file: %s\n", user_file);
        exit(EXIT_FAILURE);
    }

    return fp;
}

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

int contains_text(char* line, const char* str_to_match){
    //Use strstr to check the line for the substring, returning 0 if the string
    //is not found, in which case strstr returns NULL.
    return strstr(line, str_to_match) != NULL;
}